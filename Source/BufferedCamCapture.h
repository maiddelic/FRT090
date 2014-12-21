#pragma once
#include <string>
#include <opencv2\opencv.hpp>
#include <exception>
#include <Windows.h>

struct BTImage;
class RecordRequest;

/* This class can be used to get images from cameras working with
OpenCV. The images are stored in a ring buffer, and they can upon
request be retrieved or written to file.

Whether images from the camera should be copied to the ring
buffer is controlled with the methods containing "capture".

Whether images should be written to file is controlled by the
methods containing "record".
Recording typically happens slower than image capture. Hence,
continuous recrding is typically not possible. If a frame is overwritten
by a new file while it is being recorded to file, the recording is
aborted.

When an image is retrieved with one of the get-methods, only
a pointer to a BTImage is given, to avoid unnecessary copying.
The image could be modified by the caller, but it is advised not
to. The data could also be modified by the BufferedCamCapture
object if the frame is overwritten by a new frame. To detect this
situation, call isValidTimeIndex() when you are done using a frame.

*/

ref class BufferedCamCapture
{
public:
	// Get video stream from camera
	// bufSize = number of frames in ring buffer
	BufferedCamCapture(int camId, int bufSize);
	// Get video stream from file
	BufferedCamCapture(const std::string& sourceFile, int bufSize);
	~BufferedCamCapture();
	!BufferedCamCapture();

	// Set the camera name. It is used when naming the folder
	// the images are recorded to.
	void setName(const std::string& str);

	// To capture the entire image, pass a null pointer
	// Only the ROI (region of interest) is copied to the
	// buffer during capture
	void setROI(cv::Rect *roi);
	// Set whether images should be converted to gray before
	// being copied to the buffer
	void setConvertToGray(bool doConvert);
	// Make sure capturing and recording are not running when this method is called
	// If this method is not called after changing ROI or convert-to-gray-setting,
	// the capturing will be slower until the buffer is refilled, due to reallocation
	// of memory.
	void reInitBuffer();

	// Controls whether images should be copied from the camera to the ring buffer.
	void startCapture();
	void stopCapture();
	bool isCapturing();

	// Controls whether images from the ring buffer should be copied to file
	void startRecord();
	void startRecord(int recordInterval);
	void startRecord(RecordRequest& req);
	void stopRecord();
	void stopRecord(RecordRequest& req);

	/**
	 * Returns the width of frames retreived from the video stream.
	 */ 
	int getFrameWidth();

	/**
	 * Returns the height of frames retreived from the video stream.
	 */ 
	int getFrameHeight();

	void getNewestFrame(BTImage** frame);
	void getFrameByIndex(int index, BTImage** frame);
	// Get the newest frame. If no frame has been added to the
	// buffer since the previous call to a blocking get, this
	// call blocks until a new frame is added to the buffer.
	void getNewestFrameBlocking(BTImage** frame, int *frameLag);
	// Get the frame that is one frame newer than the one returned
	// by the previous call to a blocking get.
	void getNextFrameBlocking(BTImage** frame, int *frameLag);
	// Get the frame that is n frames newer than the one returned
	// by the previous call to a blocking get.
	// Convenient for getting every nth frame.
	void getNextFrameBlocking(BTImage** frame, int *frameLag, int n);
	void resetNextFrameCounter();
	// Check whether the frame corresponding to a time index is
	// still valid in the buffer.
	bool isValidTimeIndex(int index);

	// For debugging etc
	static void test();

	// It's kind of ugly that this is public, but it lets you
	// query and set the different properties of the capture
	// from outside BufferedCamCapture, which may be very convenient.
	cv::VideoCapture *capture;

private:
	// Called by all constructors.
	void init(int bufSize);

	std::string* name;

	// The following variables may only be accessed when
	// bufferMon is entered.
	System::Object^ bufferMon;
	int bufSize;
	BTImage *imBuffer;
	int newestFrame, oldestFrame;
	int lastBlockedGetFrame;

	// The following variables may only be accessed when
	// captureMon is entered.
	System::Threading::Thread^ captureThread;
	bool doCapture;
	cv::Rect* roi;
	bool convertToGray;
	System::Object^ captureMon;
	void captureRun();

	// Do a deep copy of a frame, considering the settings for ROI 
	// and whether it should be converted to gray.
	void copyConvertFrame(const cv::Mat& src, cv::Mat& dst);

	// The following variables may only be accessed when
	// recordMon is entered.
	int firstFrameToRecord;
	int nextFrameToRecord;
	int lastFrameToRecord;
	int recordInterval;
	System::Threading::Thread^ recordThread;
	bool doRecord;
	System::Object^ recordMon;
	void recordRun();


};

class BufferedCamCaptureException: public std::exception
{
public:
	BufferedCamCaptureException(std::string message)
	{
		this->message = message;
	}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
private:
	std::string message;
};

class InvalidFrameNumberException: public std::exception
{
public:
	InvalidFrameNumberException(int requestedFrame, int oldestFrame, int newestFrame)
	{
		std::stringstream ss;
		ss << "Requested frame: " << requestedFrame
			<< ". Valid range from " << oldestFrame
			<< " to " << newestFrame << ".";
		this->message = ss.str();
	}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
private:
	std::string message;
};


struct BTImage
{
	// Image - OpenCV style
	cv::Mat mat;
	// Incremented by one for every new image in the buffer
	int timeIndex;
	// Time stamp in units of 100 ns
	uint64 time;
};

class RecordRequest
{
	uint64 key;
	int timeIndex;
	uint64 time;
	int recordInterval;
public:
	RecordRequest(int timeIndex, uint64 key)
	{
		this->timeIndex = timeIndex;
		this->key = key;
		recordInterval = 1;
	}

	int getTimeIndex()
	{
		return timeIndex;
	}

	uint64 getKey()
	{
		return key;
	}

	void setRecordInterval(int interval)
	{
		recordInterval = interval;
	}
	int getRecordInterval()
	{
		return recordInterval;
	}
};


