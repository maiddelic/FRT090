#pragma once

#include <windows.h>
#include <stdio.h>
#include "Servo.h"
#include "TransferFunction.h"
#include <vector> 
#include <omp.h>

/**
 * Directions in which the telescope can move. Use instead of e.g. bools when
 * determining which motor to call. It is currently only used in test
 * functions, and can probably be removed when these are not needed.
 */ 
enum Direction {
	VERTICAL,
	HORIZONTAL
};


/**
 * Class for controlling servos, given a reference signal (in degrees/second).
 */ 
ref class ServoHandler{
public:

	/** 
	 * Constructor. Internally sets up contact with the servos, as well as
	 * setting parameters for the feed-forward transfer functions.
	 *
	 * The limits of movement for the telescope are also hardcoded here.
	 * Currently, the limit along the x-axis has been disabled (movement is
	 * allowed in the range [-180, 180] degrees), but it should be narrowed
	 * down when testing is finished.
	 */

	ServoHandler();

	/** 
	* Desructor for ServoHandler, turns off the motors and deallocates all
	* internal resources. 
	*/
	~ServoHandler();

	/**
	 * Finalizer for ServoHandler. Simply calls the destructor.
	 */
	!ServoHandler();

	/**
	* Turns on the motors, both the X- and the Y-motor and returns whether the
	* motors are on or not.
	*
	* @return		vector with two elements, with the first corresponding to
	*				the x-motor, and the second to the y-motor. NOTE: This
	*				could be changed from std::vector to std::pair. It would
	*				be more consistent with the rest of the code.
	*/
	std::vector<bool> startMotors();


	/**
	* Sets the allowed range of movement for the telescope. Bounds are given in
	* degrees, with 0 being the home position of the telescope.
	*
	* @param	left	sets maximum value for the telescope to move to the left
	* @param	right	sets maximum value for the telescope to move to the right
	*/
	void ServoHandler::setHorizontalGuards(double left, double right);

	/**
	* Sets the lower bound for elevation of the telescope. This can be used to
	* ensure that the telescope is always high enough to have a clear sky, as
	* this is required for tracking.
	*
	* As the horizon is currently set by pointing the telescope at the point
	* to be used as lower bound, the tracking position elevation is updated to be
	* 15 degrees above the horizon, to prevent a tracking position which is
	* impossible to reach.
	*
	* An unnecessary restriction at present, which has not been fixed due to lack
	* of time, is that the telescope cannot be moved below the currently set
	* horizon. As the horizon can only be set to the current elevation of the
	* telescope, this means that the horizont cannot be lowered without restarting
	* the program.
	*/
	void ServoHandler::setHorizon();

	/**
	 * Checks whether the telescope is outside of its allowed range of motion.
	 *
	 * @return	true if x-motor position is not within the allowed interval.
	 */
	bool ServoHandler::isMotorXAtMax();
	
	/**
	 * Checks whether the telescope's elevation is at or below the level set as
	 * horizon.
	 *
	 * @return	true if elevation (y-motor position) is at or below the specified
	 *			horizon.
	 */
	bool ServoHandler::isMotorYAtHorizon();

	/** 
	* Moves the the telescope from the home find position to the opening in the
	* dome. As the dome is currently not integrated in the system, the tracking
	* position is hardcoded to (0, 90) degrees. This places the telescope
	* horizontally, facing about 90 degrees left of the entrance to the dome.
	*
	* NOTE: The elevation of the tracking position is changed by calls to
	*       setHorizon().
	*
	* @returns void
	*/
	void moveTelescopesToBirdTrackingInitialPosition();

	
	/**
	* Sets the velocity of both motors to zero, and clears any earlier reference
	* to force an immediate halt. Blocking call.
	*/
	void pauseMotors();

	/**
	 * Set the reference signal for x- and y controllers. Blocking call.
	 *
	 * The telescope mount can move at speeds up to 13 degrees/second,
	 * according to the data sheet.
	 *
	 * @param xref
     *			the reference angular velocity along the x-axis, in degrees/second

	 * @param yref
	 *			the reference angular velocity alogn the y-axis, in degrees/second
	 */ 
	void setReference(double xref, double yref);



	/** 
	* Stops the thread running the ServoHandler
	*
	*/
	void stop();

	/* TESTING METHODS */
	
	/** 
	* Creates a step that is used to send to the motors as a reference signal
	*
	* @param stepTime	the time at which the step starts
	* @param totalTime	the total amount of time at which the step is run
	* @param intialAmp	the initial amplitude of the step, usually 0
	* @param finalAmp	the final amplitude of the step, usually 1
	* @returns			a vector of the total step
	*/
	std::vector<double> step(int stepTime,int totalTime, double initialAmp, double finalAmp);

	
	void runTFTest(Direction dir);




private:
	/** 
	*Runs the thread running the ServoHandler, when stop is called this method calls the destructor
	*/
	void run();

	/* Filters the given input signal through the transfer function, and then
	   sets the transfer function output as the angular velocity of the 
	   x-motor. This call is not thread safe, and thus has to be private to avoid
	   interference with setReference. It is currently only called from
	   runProcess(), and it is probably best if it stays that way.

	   moveXaxis4 does the same thing but with a different transfer function.
	   This method only exists for testing purposes, and is an attempt at making
	   a slightly slower transfer function to get a longer overshoot in the
	   feed-forward design.
	 */
	std::pair<double, double> moveXaxis(double insignal);
	std::pair<double, double> moveXaxis4(double insignal);

	/* As moveXaxis, but for the other motor. */
	std::pair<double, double> moveYaxis(double insignal);
	std::pair<double, double> moveYaxis4(double insignal);

	/* Periodicity of the thread, in ms. This is how often the reference is
	   sampled and an updated signal is sent to the motors. */
	const static int PERIOD = 20;
	/* The speed used when automatically moving the telescope. Currently used
	   when moving from home position to tracking position. */
	const static int DEFAULT_SPEED = 10;
											

	Servo ^servo;						// Object to communicate with the motors
	TF ^tfX, ^tfY,^tfX4, ^tfY4;			// creates transfer functions for the X- and the Y-motor
	bool	servoRun;

	/* Bounds on telescope movement */
	double	maxXLeft,
			maxXRight,
			horizon;

	/* These coordinates define the default position of the telescope when
	   tracking. After initializing the motors, the telescope will be moved
	   to this point. It is also returned here if a safety bound is reached. */
	int		trackPosX,
			trackPosY;

	/* Current reference signal. Should always be accessed in a thread safe way */
	double	xref,
			yref;

	System::Threading::Thread ^servoThread;
	System::Threading::Mutex ^refMx;			//lock for reference signal
};
