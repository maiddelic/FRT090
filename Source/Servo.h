#pragma once
#include "Exceptions.h"

//#define USE_NON_BLOCKING


/* This class is used for direct communication with the hardware, both the
   telescope mount and the dome. It is carried over from the group who worked
   with this project before us, and was completely undocumented. Anything
   which is not called from ServoHandler is untested, and little effort has
   been made to make the code more readable, though some comments have been
   added. However, it contains a lot of useful functionality. It is also the
   closest thing we have to a documentation of the ClServo class.

   We have not tried controlling the dome, but we have been told that the
   existing implementation does not work, though it seems unclear why.

   Everything used if USE_NON_BLOCKING is defined seems to be an for threading
   of the motor control. This has been implemented elsewhere in the current 
   system, and these parts should thus be obsolete.

   /Group B, Control Project 2014
 */

#ifndef MOTOR
#define MOTOR
enum Motor
{
	MotorX = 1,
	MotorY = 2
};
#endif

ref class Servo
{
public:
	static const double DIST_TO_LIMIT_ONE = 174.54;
	static const double MOUNT_RADIUS = 0.55,
						OFF_CENTER_Y = 0.4,
						OFF_CENTER_Z = 0.34,
						domeRadius = 1.5;

	Servo(void);		// constructor
	~Servo();			// destructor
	!Servo();

	/* Returns true if the motor is on in the ClServo class */
	bool isMotorOn(Motor motor);

	/* Starts the motors using ClServo. Seems to always return true. */
	bool motorsOn(double motorX, double motorY);

	/* Finds the home position of the telescope. It has to be moved manually.
	   Returns true if home position was found for both motors */
	bool initializeAndMotorsOn();

	/* Turns off motors in ClServo and stops the dome */
	void motorsOff();

	void domeTrackingOn();

	void domeTrackingOff();

	/* Set angular velocity (degrees/second) for the given motor. */
	void setVelocity(Motor motor,double vel);

	void moveTo(Motor motor,double degree);

	/* Returns the position error */
	double getPositionError(Motor motor);
	/* Returns the position, relative to the home position */
	double getPosition(Motor motor);
	/* Returns position + positionError */
	double getFinalPosition(Motor motor);

	void updateDomeTracking();

private:
	/* ClServo is the class which communicates directly with the telescope mount
	   hardware. It has no known documentation, the only reference is its use in
	   this class. */
	ClServo::Servos ^servos;

#ifdef USE_NON_BLOCKING
	System::Threading::Thread^ servoThread;
	System::Threading::Mutex^ servoMx;
	bool servoThreadRunning, servoThreadStoped;
	bool activeUsage;
	double motorXPos, motorYPos;
	double motorXPosError, motorYPosError;
	double motorXVelocity, motorYVelocity;
#endif

	ASCOMDome::DomeClass^ dome;
	System::Threading::Thread^ domeThread;
	System::Threading::Mutex^ domeMx;
	bool domeThreadRunning, domeThreadStoped;
	double lastMotorXAngle, lastMotorYAngle;

	bool motorsOn();

	/* Find the home position for the specified motor. Has only been tested with
	   with manual = true, and this was the only thing used in the old code as
	   well. */
	bool motorInitializePosition(Motor motor, bool manual);

#ifdef USE_NON_BLOCKING
	void startNonBlocking();

	void stopNonBlocking();

	void servoRun();
#endif

	void domeUpdate();
};

