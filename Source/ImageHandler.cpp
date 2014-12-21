#include "ImageHandler.h"
#include <opencv2\opencv.hpp>

using namespace System::Threading;
using namespace cv;


ImageHandler::ImageHandler(int camID, int bufsize, ServoHandler ^servoHandler)
{

	this -> servoHandler = servoHandler;
	
	/* Create bufferedCamCapture */
	bufcam = gcnew BufferedCamCapture(camID, bufsize);
	bufcam->setConvertToGray(true);
	frameWidth = bufcam->getFrameWidth();
	frameHeight = bufcam->getFrameHeight();
	/* ---- set any other properties here ---- */

	bufcam->startCapture(); // start pushing images to buffer

	trackingMx = gcnew System::Threading::Mutex();

	/* Start thread */
	imageProcessThread = gcnew Thread(gcnew ThreadStart(this, &ImageHandler::runProcess));
	runThread = true;
	tracking = false;
	imageProcessThread->Start(); 



}

ImageHandler::~ImageHandler() {
	imageProcessThread->Abort();
	imageProcessThread->Join();
	
	delete bufcam;
}


void ImageHandler::stop() {
	
	runThread = false;
}

void ImageHandler::startTracking() {
	trackingMx->WaitOne();

	tracking = true;

	trackingMx->ReleaseMutex();
}

void ImageHandler::stopTracking() {
	trackingMx->WaitOne();

	tracking = false;

	trackingMx->ReleaseMutex();

	servoHandler->pauseMotors();
	// maybe return to default tracking position?
}

bool ImageHandler::isTracking() {
	trackingMx->WaitOne();

	bool result = tracking;

	trackingMx->ReleaseMutex();

	return result;
}

void ImageHandler::runProcess() {

	BTImage *image;		//defined in BufferedCamCapture.h
	
	Mat thresholdIm, drawIm;

	double  vx,
			vy;


	std::vector<vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	std::vector<Point> largestContour;
	Point2f massCenter;
	Point2f imgCenter(frameWidth/2, frameHeight/2);

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);

	bool triggered = false;

	while(runThread) {
		bufcam->getNewestFrame(&image);

		/* Create image to draw on. As drawing changes the image, this will prevent
		   drawn object from disturbing the image analysis if the image is analyzed
		   twice due to delays in the camera/camera buffer. As the input is single
		   channel grayscale (CV_8UC1), a conversion to a 3-channel BGR image is
		   needed in order to draw with colors.*/
		drawIm = image->mat.clone();
		cvtColor(drawIm, drawIm, CV_GRAY2BGR);


		/* Get threshold image. Use THRESH_BINARY_INV, as we are looking for a
		   dark object against a bright background */
		threshold(image->mat, thresholdIm, INTENS_THRESH, 255, THRESH_BINARY_INV);
		
		/* Blur image to remove noise */
		blur(thresholdIm, thresholdIm, Size(10, 10));

		/* Find contours */
		findContours(thresholdIm.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		/* Calculate contour area. Right now the last contour in the vector is
		   always used, but it does not always seem to be the largest. Perhaps
		   it does not matter, as we are only interested in one bird at a time,
		   but there is no guarantee that tracking works when several objects
		   are in the image simultaneously. */
		if (contours.size() > 0 &&
			contourArea(contours.at(contours.size() - 1)) >= MIN_AREA) {
			
			/* Calculate centers of mass */
			Moments m = moments(contours.at(contours.size() - 1));
			massCenter.x = m.m10/m.m00;
			massCenter.y = m.m01/m.m00;
			drawContours(drawIm, contours, contours.size() - 1, Scalar(0,0,255)); 
			triggered = true;

			std::cout << "-----------------------------------" << std::endl;
			std::cout << "x = " << massCenter.x << "\t y = " << massCenter.y << std::endl;
			std::cout << "center = (" << imgCenter.x << ", " << imgCenter.y  << ")" << std::endl;

			double vx = (imgCenter.x - massCenter.x) / (frameWidth / 2) * MAX_VEL;
			double vy = -1 * (imgCenter.y - massCenter.y) / (frameWidth / 2) * MAX_VEL;
			std::cout << std::endl << "Control signal:" << std::endl;
			std::cout << "x = " << vx << "\t y = " << vy << std::endl;
			std::cout << "-----------------------------------" << std::endl;


		} else {
			massCenter = imgCenter;
			triggered = false;
		}

		// Show in window. Push to GUI when implemented.
		circle(drawIm, massCenter, 4, Scalar(0,255,0), 2, 8, 0);
		imshow( "Contours", drawIm );

		/* Input is not needed, but if OpenCV is not told to wait, it does not
		   take time to draw the images specified by imshow. Perhaps this can
		   be done in a better way, but as the images should be printed in the
		   GUI, this is only temporary anyway. */
		cvWaitKey(3);

		/* Calculate control signal based on distance between center of mass
		   and center of image. Output will be on the interval [0, 1] * MAX_VEL
		   */
		vx = (massCenter.x - imgCenter.x) / (frameWidth / 2) * MAX_VEL;
		vy = (massCenter.y - imgCenter.y) / (frameHeight / 2) * MAX_VEL;

		if(isTracking())
			servoHandler->setReference(vx, vy);

	}
	this->~ImageHandler();
}
