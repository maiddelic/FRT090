#include "stdafx.h"
#include "Servo.h"
#include <math.h>
#include "utils.h"
#include <iostream>

using namespace ClServo;

Servo::Servo(void)
{
	domeMx = gcnew System::Threading::Mutex();
	servos = gcnew Servos();
	servos->Init;
	domeThreadRunning = false;
	domeThreadStoped = true;

#ifdef USE_NON_BLOCKING
	servoMx = gcnew System::Threading::Mutex();
	servoThreadRunning = false;
	servoThreadStoped = false;
#endif
}

Servo::~Servo()
{
	this->!Servo();
}

Servo::!Servo()
{

	domeTrackingOff();
	servos->Close();
}

bool Servo::isMotorOn(Motor motor){
	return servos->MotorOn[motor];
}



bool Servo::motorsOn(double motorX, double motorY)
{
	servos->MotorOn[1] = true;
	servos->MotorOn[2] = true;
	if (servos->MotorOn[1] && servos->MotorOn[2])
	{
		servos->Synch[MotorX] = motorX;
		servos->Synch[MotorY] = motorY;

#ifdef USE_NON_BLOCKING
		startNonBlocking();
#endif
		return true;
	}
	else
	{
		return false;
	}
}

//why is this bool and not void?
bool Servo::motorsOn()
{
	servos->MotorOn[1] = true;
	servos->MotorOn[2] = true;
	return true;
	//return servos->MotorOn[1] && servos->MotorOn[2];
	// does this just mean return true?
}

bool Servo::initializeAndMotorsOn()
{
	motorInitializePosition(MotorX, true);
	motorInitializePosition(MotorY, true);
	//motorXInitialisePosition(true);
	//motorYInitialisePosition(true);

	if (servos->MotorOn[1] && servos->MotorOn[2])
	{
#ifdef USE_NON_BLOCKING
		startNonBlocking();
#endif
		return true;
	}
	else
	{
		return false;
	}
}

void Servo::motorsOff()
{
#ifdef USE_NON_BLOCKING
	stopNonBlocking();
#endif

	servos->MotorOn[1] = false;
	servos->MotorOn[2] = false;
	domeTrackingOff();
	if (dome)
	{
		dome->AbortSlew();
		dome->Connected = false;
	}
}

void Servo::domeTrackingOn()
{
	if (!dome)
	{
		dome = gcnew ASCOMDome::DomeClass();
		dome->Connected = true;
	}
	updateDomeTracking();
	if (domeThreadRunning == false)
	{
		domeThreadStoped = false;
		domeThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &Servo::domeUpdate));
		domeThread->Start();
	}
}

void Servo::domeTrackingOff()
{
	domeThreadStoped = true;
	while (domeThreadRunning)
	{
		System::Threading::Thread::Sleep(20);
	}
}

#ifdef USE_NON_BLOCKING

void Servo::startNonBlocking()
{
	if (!servoThreadRunning)
	{
		servoThreadStoped = false;
		servoThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &Servo::servoRun));
		servoThread->Start();
	}
}

void Servo::stopNonBlocking()
{
	servoThreadStoped = true;
	while (servoThreadRunning)
	{
		System::Threading::Thread::Sleep(20);
	}
}

#endif

bool Servo::motorInitializePosition(Motor motor, bool manual) {
	if(manual) {
		servos->MotorOn[motor] = false;

	} else {
		if (!motorsOn())
		{
			return false;
		}
		servos->VelSpeed[1] = 3;							    // set speed
		double posError = 0;							// create double for pos error
		while(abs(posError) < 0.1)						// while error is less than
		{
			posError = servos->PositionError[motor,false];	// updates posError each loop
			System::Threading::Thread::Sleep(10);
			if (servos->ErrorRaised[motor])
			{
				servos->ErrorClear(motor);
			}
		}

		servos->MotorOn[motor] = false;
		motorsOn();
		servos->VelSpeed[motor] = 0;

		servos->Synch[motor] = DIST_TO_LIMIT_ONE;			// tells the MotorX its position, absolute position, nbr of counts
		//servos->MoveRelative[motor] = -DIST_TO_LIMIT_ONE;	// move back to zero, the exact middle between edges
		if (servos->ErrorRaised[motor])
		{
			servos->ErrorClear(motor);
		}

		if(motor == MotorX) {
			moveTo(motor, 90);
		} else if (motor == MotorY) {
			moveTo(motor, 32);
		}
	}

	servos->HomeFindIndexPos(motor);
	bool found = servos->HomeFound[motor];

	if(!found) {
		return false;
	}

	// magic numbers from old code
	if(motor == MotorX) {
		servos->Synch[motor] = 82.35;
	} else if (motor == MotorY) {
		servos->Synch[motor] = 42 + 90;
	}

	return true;
}


void Servo::setVelocity(Motor motor,double vel)
{
#ifndef USE_NON_BLOCKING
		servos->VelSpeed[motor] = vel;
#else
	if (motor == MotorX)
	{
		servoMx->WaitOne();
		activeUsage = true;
		motorXVelocity = vel;
		servoMx->ReleaseMutex();
	}
	else if(motor = MotorY)
	{
		servoMx->WaitOne();
		activeUsage = true;
		motorYVelocity = vel;
		servoMx->ReleaseMutex();
	}
#endif
}

void Servo::moveTo(Motor motor,double degree)
{
	if(!(servos->MotorOn[1] && servos->MotorOn[2]))
	{
		return;
	}
	double dir, speed = 5;
	double relativeMove = degree-getPosition(motor);
	if (relativeMove < 0)
	{
		dir = -1;
	}
	else if (relativeMove > 0)
	{
		dir = 1;
	}

	setVelocity(motor, speed * dir);	
	while(dir * (degree - getFinalPosition(motor)) >= 0)
	{
		System::Threading::Thread::Sleep(10);
	}
	setVelocity(motor, 0);
}

double Servo::getPositionError(Motor motor)
{
#ifndef USE_NON_BLOCKING
	return servos->PositionError[motor,false];
#else
	if (motor == MotorX)
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret = motorXPosError;
		servoMx->ReleaseMutex();
		return ret;
	}
	else
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret = motorYPosError;
		servoMx->ReleaseMutex();
		return ret;
	}
#endif
}

double Servo::getPosition(Motor motor)
{
#ifndef USE_NON_BLOCKING
	return servos->ServoPosition[motor,false];
#else
	if (motor == MotorX)
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret = motorXPos;
		servoMx->ReleaseMutex();
		return ret;
	}
	else
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret = motorYPos;
		servoMx->ReleaseMutex();
		return ret;
	}
#endif
}

double Servo::getFinalPosition(Motor motor)
{
#ifndef USE_NON_BLOCKING
	return getPosition(motor) + getPositionError(motor);
#else
	if (motor == MotorX)
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret = motorXPos + motorXPosError;
		servoMx->ReleaseMutex();
		return ret;
	}
	else
	{
		servoMx->WaitOne();
		activeUsage = true;
		double ret =  motorYPos + motorYPosError;
		servoMx->ReleaseMutex();
		return ret;
	}
#endif
}

void Servo::updateDomeTracking()
{
	domeMx->WaitOne();
	lastMotorXAngle = getPosition(MotorX);
	lastMotorYAngle = getPosition(MotorY);
	domeMx->ReleaseMutex();
}

#ifdef USE_NON_BLOCKING
void Servo::servoRun()
{
	TimeCounter timer;
	double inactiveTime = 0;
	double motorXPos, motorYPos;
	double motorXPosError, motorYPosError;
	double motorXVelocity, motorYVelocity;

	motorXPos = s->ServoPosition[MotorX,false];
	motorYPos = s->ServoPosition[MotorY,false];
	motorXPosError = s->PositionError[MotorX,false];
	motorYPosError = s->PositionError[MotorY,false];

	servoThreadRunning = true;
	while (!servoThreadStoped)
	{
		inactiveTime += timer.getDeltaTime();
		servoMx->WaitOne();
		this->motorXPos = motorXPos;
		this->motorYPos = motorYPos;
		this->motorXPosError = motorXPosError;
		this->motorYPosError = motorYPosError;
		motorXVelocity = this->motorXVelocity;
		motorYVelocity = this->motorYVelocity;
		if (activeUsage)
		{
			inactiveTime = 0;
		}
		activeUsage = false;
		servoMx->ReleaseMutex();

		if (inactiveTime > 0.5)
		{
			motorXVelocity = 0;
			motorYVelocity = 0;
			servoThread->Sleep(50);
		}

		motorXPos = s->ServoPosition[MotorX,false];
		motorYPos = s->ServoPosition[MotorY,false];
		motorXPosError = s->PositionError[MotorX,false];
		motorYPosError = s->PositionError[MotorY,false];
		s->VelSpeed[MotorX] = motorXVelocity;
		s->VelSpeed[MotorY] = motorYVelocity;

		if (domeThreadRunning)
		{
			domeMx->WaitOne();
			lastMotorXAngle = motorXPos;
			lastMotorYAngle = motorYPos;
			domeMx->ReleaseMutex();
		}
	}
	servoThreadRunning = false;
}
#endif

using namespace System;

void Servo::domeUpdate()
{
	domeThreadRunning = true;
	while (!domeThreadStoped)
	{
		domeMx->WaitOne();
		double angleButtom = (-lastMotorXAngle) / 180 * 3.14159265;
		double angleTop = (90 - lastMotorYAngle) / 180 * 3.14159265;
		domeMx->ReleaseMutex();
		double x1 = MOUNT_RADIUS * sin(angleButtom);
		double y1 = OFF_CENTER_Y + MOUNT_RADIUS * -cos(angleButtom);
		double xDir = cos(angleButtom);
		double yDir = sin(angleButtom);

		double b = (xDir * x1 + yDir * y1);
		double c = (domeRadius * domeRadius - x1 * x1 - y1 * y1);

		double cutRadius = sqrt(b * b + c);
		double p = b;

		double xDirBeta = cos(angleTop);
		double yDirBeta = sin(angleTop);

		b = (xDirBeta * p + yDirBeta * OFF_CENTER_Z);
		c = (cutRadius * cutRadius - p * p - OFF_CENTER_Z * OFF_CENTER_Z);

		double t = -b + sqrt(b * b + c);
		double beamT = t;

		double xHit = x1 + xDir * xDirBeta * t;
		double yHit = y1 + yDir * xDirBeta * t;

		double angleDome = -atan2(yHit, xHit) / 3.14159265 * 180;
		if (angleDome < 0)
		{
			angleDome += 360;
		}
		std::cout << angleButtom << ", " << angleTop << ", \t" << xHit << ", " << yHit << " \t" <<  angleDome << "\n";
		dome->SyncToAzimuth(angleDome);
	}
	domeThreadRunning = false;
}