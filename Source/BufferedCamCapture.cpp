#include "BufferedCamCapture.h"
#include "utils.h"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace System::Threading;

// Place to store the recordings
#define RECORD_DIR "C:\\Users\\Lumbo\\Documents\\Data\\"

BufferedCamCapture::BufferedCamCapture(int camId, int bufSize)
{
	capture = new VideoCapture(camId);
	if (!capture->isOpened()) {
		throw BufferedCamCaptureException("Couldn't open cv::VideoCapture with camId");
	}

	// Temp
	//capture->set(CV_CAP_PROP_FRAME_WIDTH, 2048);
	//capture->set(CV_CAP_PROP_FRAME_HEIGHT, 1088);
	//capture->set(CV_CAP_PROP_FRAME_WIDTH, 1024);
	//capture->set(CV_CAP_PROP_FRAME_HEIGHT, 768);

	// Default name
	std::stringstream ss;
	ss << "Camera" << camId;
	name = new std::string(ss.str());

	init(bufSize);
}


BufferedCamCapture::BufferedCamCapture(const std::string& sourceFile, int bufSize)
{
	capture = new VideoCapture(sourceFile);
	if (!capture->isOpened()) {
		throw BufferedCamCaptureException("Couldn't open cv::VideoCapture with sourceFile");
	}

	// Default name
	name = new std::string(sourceFile);

	init(bufSize);
}


void BufferedCamCapture::init(int bufSize)
{
	roi = nullptr;
	convertToGray = false;

	bufferMon = gcnew System::Object();
	this->bufSize = bufSize;
	imBuffer = new BTImage[bufSize];

	doCapture = false;
	captureMon = gcnew System::Object();
	captureThread = gcnew Thread(gcnew ThreadStart(this, &BufferedCamCapture::captureRun));
	captureThread->Start();

	doRecord = false;
	recordMon = gcnew System::Object();
	recordThread = gcnew Thread(gcnew ThreadStart(this, &BufferedCamCapture::recordRun));
	recordThread->Start();

	reInitBuffer();

	std::cout << "Created BufferedCamCapture with name: " << name->c_str() << " at " << u64timeToDateString(getU64Filetime()) << std::endl;
}


BufferedCamCapture::~BufferedCamCapture()
{
	this->!BufferedCamCapture();
}


BufferedCamCapture::!BufferedCamCapture()
{

	captureThread->Abort();
	recordThread->Abort();
	captureThread->Join();
	recordThread->Join();

	delete name;
	delete capture;
	delete[] imBuffer;
}

void BufferedCamCapture::setName(const std::string& str)
{
	delete name;
	name = new std::string(str);
	//std::cout << "Name set to " << name->c_str() << std::endl;
	//printf("Name set to %s\n", name);
}

void BufferedCamCapture::setROI(Rect *roi)
{
	Monitor::Enter(captureMon);
		if (this->roi) {
			delete this->roi;
		}
		if (roi) {
			this->roi = new Rect(*roi);
		} else {
			this->roi = nullptr;
		}
	Monitor::Exit(captureMon);
}

void BufferedCamCapture::setConvertToGray(bool doConvert)
{
	Monitor::Enter(captureMon);
		convertToGray = doConvert;
	Monitor::Exit(captureMon);
}

void BufferedCamCapture::reInitBuffer()
{
	newestFrame = -1;
	oldestFrame = 0;
	lastBlockedGetFrame = -1;

	firstFrameToRecord = 0;
	nextFrameToRecord = 0;
	lastFrameToRecord = 0;
	recordInterval = 1;

	// Fill the cv::Mat members of the buffer with data of the same 
	// dimensions as will be returned by copyConvertFrame() during capture.

	// Since I haven't managed to do this by extracting the propoerties only,
	// This is done by getting a frame, painting it with some default image,
	// And copying it into all the buffer elements.
	
	// Get an image
	Mat captIm, cvtdIm;
	capture->retrieve(captIm);
	copyConvertFrame(captIm, cvtdIm);

	// Give it some distinct look so one notices if they are used erroneously.
	putText(cvtdIm, "NON_INIT", Point(5,25), FONT_HERSHEY_SIMPLEX, 1.0, Scalar_<int>(255), 3);
	putText(cvtdIm, "NON_INIT", Point(5,50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar_<int>(0), 3);

	/*// Debug
	namedWindow("BufferInitFrame", CV_WINDOW_NORMAL);
	imshow("BufferInitFrame", cvtdIm);*/

	// Copy image to all buffer images
	for (int i = 0; i < bufSize; i++) {
		cvtdIm.copyTo(imBuffer[i].mat);
	}
}

void BufferedCamCapture::startCapture()
{
	printf("Starting capture\n");
	Monitor::Enter(captureMon);
		doCapture = true;
		Monitor::PulseAll(captureMon);
	Monitor::Exit(captureMon);

	// Wait for first image to be retrieved before returning
	Monitor::Enter(bufferMon);
		while(newestFrame < 0) {
			Monitor::Wait(bufferMon);
		}
	Monitor::Exit(bufferMon);
}

void BufferedCamCapture::stopCapture()
{
	Monitor::Enter(captureMon);
		doCapture = false;
	Monitor::Exit(captureMon);
}

bool BufferedCamCapture::isCapturing()
{
	bool result;
	Monitor::Enter(captureMon);
		result = doCapture;
	Monitor::Exit(captureMon);
	return result;
}

void BufferedCamCapture::startRecord()
{
	startRecord(1);
}

void BufferedCamCapture::startRecord(int recordInterval)
{
	Monitor::Enter(bufferMon);
		int index = newestFrame;
	Monitor::Exit(bufferMon);
	RecordRequest req(index, 0);
	req.setRecordInterval(recordInterval);
	startRecord(req);
}

void BufferedCamCapture::startRecord(RecordRequest& req)
{
	Monitor::Enter(recordMon);
		if (!doRecord) {
			doRecord = true;
			nextFrameToRecord = req.getTimeIndex();
			firstFrameToRecord = nextFrameToRecord;
			lastFrameToRecord = MAXINT;
			recordInterval = req.getRecordInterval();
			printf("Commanded record start\n");
			Monitor::PulseAll(recordMon);
		} else {
			printf("Tried to command recording while already recording.\n");
		}
	Monitor::Exit(recordMon);
}

void BufferedCamCapture::stopRecord()
{
	Monitor::Enter(bufferMon);
		int index = newestFrame;
	Monitor::Exit(bufferMon);
	stopRecord(RecordRequest(index, 0));
}

void BufferedCamCapture::stopRecord(RecordRequest& req)
{
	Monitor::Enter(recordMon);
		if (lastFrameToRecord == MAXINT) {
			lastFrameToRecord = req.getTimeIndex();
			printf("Commanded stopped recording.\n");
		} else {
			printf("Commanded stopped recording, that was already commanded.\n");
		}
	Monitor::Exit(recordMon);
}


int BufferedCamCapture::getFrameWidth() {
	double val = this->capture->get(CV_CAP_PROP_FRAME_WIDTH);
	return static_cast<int>(val);
}

int BufferedCamCapture::getFrameHeight() {
	double val = this->capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	return static_cast<int>(val);
}


void BufferedCamCapture::getNewestFrame(BTImage** frame)
{
	Monitor::Enter(bufferMon);
		*frame = &imBuffer[newestFrame%bufSize];
	Monitor::Exit(bufferMon);
}

void BufferedCamCapture::getFrameByIndex(int index, BTImage** frame)
{
	Monitor::Enter(bufferMon);
		if (index < oldestFrame || index > newestFrame) {
			Monitor::Exit(bufferMon);
			throw InvalidFrameNumberException(index, oldestFrame, newestFrame);
		}
		*frame = &imBuffer[index%bufSize];
	Monitor::Exit(bufferMon);
}

void BufferedCamCapture::getNextFrameBlocking(BTImage** frame, int *frameLag)
{
	getNextFrameBlocking(frame, frameLag, 1);
}

void BufferedCamCapture::getNextFrameBlocking(BTImage** frame, int *frameLag, int interval)
{
	Monitor::Enter(bufferMon);
		while (lastBlockedGetFrame + interval > newestFrame) {
			Monitor::Wait(bufferMon);
		}
		lastBlockedGetFrame += interval;
		*frameLag = newestFrame - lastBlockedGetFrame;
		if (lastBlockedGetFrame < oldestFrame) {
			Monitor::Exit(bufferMon);
			throw InvalidFrameNumberException(lastBlockedGetFrame, oldestFrame, newestFrame);
		}
		*frame = &imBuffer[lastBlockedGetFrame%bufSize];
	Monitor::Exit(bufferMon);
}

void BufferedCamCapture::getNewestFrameBlocking(BTImage** frame, int *frameLag)
{
	int lag = 0;
	Monitor::Enter(bufferMon);
		while (lastBlockedGetFrame + 1 > newestFrame) {
			// Set lag = -1 if a new frame is not available immediately
			lag = -1;
			Monitor::Wait(bufferMon);
		}
		if (lag != -1) {
			lag = newestFrame - (lastBlockedGetFrame + 1);
		}
		lastBlockedGetFrame = newestFrame;
		*frame = &imBuffer[lastBlockedGetFrame%bufSize];
		*frameLag = lag;
	Monitor::Exit(bufferMon);
}

void BufferedCamCapture::resetNextFrameCounter()
{
	Monitor::Enter(bufferMon);
		lastBlockedGetFrame = newestFrame;
	Monitor::Exit(bufferMon);
}

bool BufferedCamCapture::isValidTimeIndex(int index)
{
	bool result;
	Monitor::Enter(bufferMon);
		result = index <= newestFrame && index >= oldestFrame;
	Monitor::Exit(bufferMon);
	return result;
}



void BufferedCamCapture::copyConvertFrame(const cv::Mat& src, cv::Mat& dst)
{
	Monitor::Enter(captureMon);
		if (roi) {
			Mat cropped = src(*roi);
			if (convertToGray) {
				cvtColor(cropped, dst, CV_BGR2GRAY);
				/*// Faster way to convert to gray if src is already almost gray
				int fromTo[] = {1, 0};
				mixChannels(&cropped, 1, &dst, 1, fromTo, 1);*/
			} else {
				cropped.copyTo(dst);
			}
		} else {
			if (convertToGray) {
				cvtColor(src, dst, CV_BGR2GRAY);
				/*int fromTo[] = {1, 0};
				mixChannels(&src, 1, &dst, 1, fromTo, 1);*/
			} else {
				src.copyTo(dst);
			}
		}
	Monitor::Exit(captureMon);
}

void BufferedCamCapture::captureRun()
{
	printf("Starting capture thread\n");
	while (true) {
		Monitor::Enter(captureMon);
			while (!doCapture) {
				Monitor::Wait(captureMon);
			}
			//printf("Capture thread released from wait\n");
		Monitor::Exit(captureMon);

		/*Monitor::Enter(recordMon);
			int localNextFrameToRecord = nextFrameToRecord;
		Monitor::Exit(recordMon);*/

		//printf("Capture thread getting space in buffer\n");
		// Get storage space from buffer
		Monitor::Enter(bufferMon);
			int i = newestFrame + 1;
			if (i == oldestFrame + bufSize) {
				oldestFrame++;
			}
			/* //This warning message is obsolete, since the record thread checks whether the buffer was 
			//overwritten during file recording, after the recording is finished.
			if (doRecord && oldestFrame > localNextFrameToRecord) {
				printf("Warning: Overwriting frame %d while recording frame %d\n", i-bufSize, localNextFrameToRecord);
			}*/
			BTImage& im = imBuffer[i%bufSize];
		Monitor::Exit(bufferMon);

		//printf("Capture thread retrieving image\n");
		// Wait for image
		Mat tmpIm;
		capture->retrieve(tmpIm);
		//im.mat = tmpIm.clone();
		copyConvertFrame(tmpIm, im.mat);
		im.time = getU64Filetime();
		im.timeIndex = i;

		// Update buffer
		//printf("Capture thread updating buffer\n");
		Monitor::Enter(bufferMon);
			newestFrame++;
			Monitor::PulseAll(bufferMon);
		Monitor::Exit(bufferMon);
	}
}

void BufferedCamCapture::recordRun()
{
	bool videoInitialized = false;

	int localFirstFrameToRecord;
	int localNextFrameToRecord;
	int localLastFrameToRecord;
	int localRecordInterval;

	bool lastFrameUpdated;

	std::string dirName;
	std::ofstream infoFile;

	Monitor::Enter(recordMon);
	while (true) {
			if (doRecord && nextFrameToRecord > lastFrameToRecord) {
				Monitor::Exit(recordMon);
					// Release monitor while finishing recording to avoid blocking other threads
					// Close video file?
					infoFile.close();
					printf("Finished recording\n");
				Monitor::Enter(recordMon);
				doRecord = false;
			}
			while (!doRecord) {
				videoInitialized = false;
				Monitor::Wait(recordMon);
			}

			localFirstFrameToRecord = firstFrameToRecord;
			localNextFrameToRecord = nextFrameToRecord;
			localLastFrameToRecord = lastFrameToRecord;
			lastFrameUpdated = false;
			localRecordInterval = recordInterval;
		Monitor::Exit(recordMon);

		if (!videoInitialized) {
			// Initialize video/directory
			std::stringstream ss;
			string dateStr = u64timeToDateString(imBuffer[localFirstFrameToRecord%bufSize].time);
			ss << RECORD_DIR << "Video_" << dateStr.c_str() << "_" << name->c_str() << "\\" ;
			dirName = ss.str();
			if (!CreateDirectoryA(dirName.c_str(), nullptr)) {
				std::cout << "Failed creating directory " << dirName.c_str() << std::endl;
				doRecord = false;
				continue;
			}
			std::cout << "Created directory " << dirName.c_str() << std::endl;
			ss << "info.txt";
			infoFile.open(ss.str().c_str());
			infoFile << "File name           Time\n";			
			videoInitialized = true;
		}


		BTImage* imp;
		Monitor::Enter(bufferMon);
			while (localNextFrameToRecord > newestFrame) {
				// Wait for frame to become available
				Monitor::Wait(bufferMon);
			}
			// Check if the buffer is being overwritten by new frames
			if (localNextFrameToRecord < oldestFrame) {
				// Finish recording
				printf("Buffer full. Stopping recording.\n");
				localLastFrameToRecord = localNextFrameToRecord - localRecordInterval;
				lastFrameUpdated = true;
			}
			imp = &imBuffer[localNextFrameToRecord%bufSize];
		Monitor::Exit(bufferMon);

		// Write to file (if the recording wasn't stopped because the buffer is full).
		//printf("localFirstFrameToRecord = %d, localNextFrameToRecord = %d, localLastFrameToRecord = %d, next-first = %d\n", localFirstFrameToRecord, localNextFrameToRecord, localLastFrameToRecord, localNextFrameToRecord - localFirstFrameToRecord);
		char fileName[MAX_PATH];
		char filePath[MAX_PATH];
		if (localNextFrameToRecord <= localLastFrameToRecord) {
			//printf("Creating file name, MAX_PATH = %d\n", MAX_PATH);
			sprintf_s(fileName, sizeof(fileName), "im%08d.bmp", localNextFrameToRecord - localFirstFrameToRecord);
			sprintf_s(filePath, sizeof(filePath), "%s%s", dirName.c_str(), fileName);
			//printf("Writing file %s\n", filePath);
			imwrite(filePath, imp->mat);
		}

		// Check if the buffer was overwritten while it was being written to file
		// First store the time of the frame
		uint64 frameTime = imp->time;
		Monitor::Enter(bufferMon);
			int localOldestFrame = oldestFrame;
		Monitor::Exit(bufferMon);
		if (localOldestFrame > localNextFrameToRecord) {
			// The buffer was overwritten while it was being recorded to file
			// Delete the last image
			printf("The frame was overwritten while being recorded to file. Deleting the last file.\n");
			localLastFrameToRecord = localNextFrameToRecord - localRecordInterval;
			lastFrameUpdated = true;
			DeleteFileA(filePath);
		} else {
			// The image was recorded to file properly
			// Write a line in the info file as too
			infoFile << fileName << "      " << u64timeToMsTimeString(imp->time).c_str() << "\n";
		}


		Monitor::Enter(recordMon);
			if (lastFrameUpdated) {
				// The buffer is full. Override external requests for when recording should be stopped.
				lastFrameToRecord = localLastFrameToRecord;
			}
			nextFrameToRecord += recordInterval;
	}
	Monitor::Exit(recordMon);
}

void BufferedCamCapture::test()
{
		BufferedCamCapture bcc(1, 600);
	
	namedWindow("TestWindow", WINDOW_NORMAL);

	System::Threading::Thread::Sleep(500);

	bcc.startCapture();

	BTImage* im;
	int lag;

	for (int i = 0; true; i++) {

		if (i % 50 == 25) {
			bcc.startRecord();
		}
		if (i % 50 == 0) {
			bcc.stopRecord();
		}
		printf("i = %d\n", i);

		//printf("Getting frame\n");
		bcc.getNextFrameBlocking(&im, &lag, 1);
		bcc.getNewestFrame(&im);
		if (lag) {
			printf("Lag: %d\n", lag);
		}
		//printf("Showing image with time index %d\n", im->timeIndex);
		imshow("TestWindow", im->mat);
		cvWaitKey(1);
	}
	printf("Exiting MoonTracker::start()\n");

}
