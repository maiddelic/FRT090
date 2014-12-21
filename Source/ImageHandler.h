#pragma once
#include "BufferedCamCapture.h"
#include "ThreadBuffer.h"
#include "ServoHandler.h"


/**
 * Class for image processing. ImageHandler grabs images from a camera feed,
 * and uses these to calculate a reference signal for the telescope.
 *
 * Objects to track are found by identifying contours in a threshold image.
 * It is assumed that tracking is done against the sky, so that the background
 * is uniform, and thus removable by such a thresholding.
 *
 * The calculated reference signal is passed on to a ServoHandler, which then
 * controls the telescope.
 */
ref class ImageHandler
{
public:
	/**
	 * Contructor. Sets up reading from the camera feed.
	 * 
	 * @param camID
	 *		ID of the camera to be used
	 * @param bufsize
	 *		Size of the internal buffer used to store images before processing.
	 * @param servoHandler
	 *		Reference to a ServoHandler object, which is to receive its
	 *		reference signal from this ImageHandler.
	 */ 
	ImageHandler(int camID, int bufsize, ServoHandler ^servoHandler);

	/**
	 *	Deconstructor. Ends the camera feed.
	 */
	~ImageHandler();

	/**
	 * Halts the image processing when the current image is done. When the
	 * thread stops, its resources are automatically deallocated.
	 */
	void stop();

	/**
	 * Begin sending reference signals based on image analysis to ServoHandler.
	 */
	void startTracking();

	/**
	 * Stop sending reference signals to ServoHandler.
	 */
	void stopTracking();

private:

	/* Functions */
	bool isTracking(); 	// isTracking checks whether the program is set on tracking object or not
	void runProcess();	// runProcess is a thread which runs the image analysis
	

	/* Constants */
	const static double		MAX_VEL = 0.5;			//highest velocity the telescope can move with
	const static double		INTENS_THRESH = 200;	//calculate by median instensity instead?
	const static double		MIN_AREA = 40;			//area size threshold

	/* Variables */
	BufferedCamCapture^			bufcam;				//object reference to the BufferedCamCapture object 
	ServoHandler				^servoHandler;		//object reference to the ServoHandler object
	System::Threading::Thread^	imageProcessThread;	//object refernce to the the thread which runs runProcess
	
	bool	runThread,								//shows if imageProcessThread is running 
			tracking;								//shows if the program is set on tracking
	
	int		frameWidth,								//the width of the frame
			frameHeight;							//the height of the frame

	System::Threading::Mutex ^trackingMx;			//the mutex used for the tracking variable

	

};

