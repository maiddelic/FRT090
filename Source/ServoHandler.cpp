#include "ServoHandler.h"
#include <iostream>
#include <fstream>

using namespace System::Threading;

/* See ServoHandler.h for documentation */

ServoHandler::ServoHandler() {
	//remove two of these after testing
	tfX = gcnew TF();
	tfY = gcnew TF();
	tfX4 = gcnew TF();
	tfY4 = gcnew TF();
	
	tfX->updateVariables(1.0, -0.8, 0.16, 6.0708, -10.1908, 4.4762);
	tfY->updateVariables(1.0, -0.8, 0.16, 2.9268, -4.2113, 1.6422);
	tfX4->updateVariables(1.0, -0.4, 0.04, 3.2, -4.2982, 1.7397);
	tfY4->updateVariables(1.0, -0.4, 0.04, 1.7486, -1.6580, 0.5505);

	servo = gcnew Servo();

	trackPosX = 0;
	trackPosY = 90;

	xref = 0;
	yref = 0;

	//TESTING ONLY, these do not stop the telescope in time
	horizon = 100;
	setHorizontalGuards(-180, 180); 
	//set some default guards here (e.g. trackPosX +/- 20, horizon = trackPosY +15)

	refMx = gcnew Mutex();

	servoThread = gcnew Thread(gcnew ThreadStart(this, &ServoHandler::run));


}

ServoHandler::~ServoHandler(){
	delete refMx;
	servo->motorsOff();
	delete servo;
	delete tfX;
	delete tfY;
}

ServoHandler::!ServoHandler(){
	this->~ServoHandler();
}

//gör det här till std::pair när allt annat för motorerna är det?
std::vector<bool> ServoHandler::startMotors() {
	std::vector<bool> areMotorsOn;
	servo->initializeAndMotorsOn();
	areMotorsOn.push_back(servo->isMotorOn(MotorX)); //Check if motor X is on
	areMotorsOn.push_back(servo->isMotorOn(MotorY)); //Check if motor Y is on

	if (areMotorsOn.at(0) && areMotorsOn.at(1)) {
		servoRun = true;
		std::cout << "servoThread->isAlive = " << servoThread->IsAlive << std::endl;
		servoThread->Start();
		std::cout << "Starting servo thread" << std::endl;
	}

	return areMotorsOn;
}

void ServoHandler::setHorizontalGuards(double left, double right){
	maxXRight = right;
	maxXLeft = left;
}

void ServoHandler::setHorizon(){
	horizon = servo->getFinalPosition(MotorY);

	std::cout << "horizon set to " << horizon << std::endl;

	// y direction is negative in hardware, so horizon - 15 is 15 degrees
	// higher than previously
	trackPosY = horizon - 15;
}


std::pair<double, double> ServoHandler::moveXaxis(double insignal){
	std::pair<double, double> writer;
	double filteredX = tfX->filterSignal(insignal);
	writer.first = filteredX;
	servo->setVelocity(MotorX, filteredX);
	writer.second = servo->getPosition(MotorX);
	return writer;
}


std::pair<double, double>  ServoHandler::moveYaxis(double insignal){
	std::pair<double, double> writer;
	double filteredY = tfY->filterSignal(insignal);
	writer.first = filteredY;
	servo->setVelocity(MotorY, filteredY);
	writer.second = servo->getPosition(MotorY);
	return writer;
}


std::pair<double, double> ServoHandler::moveXaxis4(double insignal){
	std::pair<double, double> writer;
	double filteredX = tfX4->filterSignal(insignal);
	writer.first = filteredX;
	servo->setVelocity(MotorX, filteredX);
	writer.second = servo->getPosition(MotorX);
	return writer;
}


std::pair<double, double>  ServoHandler::moveYaxis4(double insignal){
	std::pair<double, double> writer;
	double filteredY = tfY4->filterSignal(insignal);
	writer.first = filteredY;
	servo->setVelocity(MotorY, filteredY);
	writer.second = servo->getPosition(MotorY);
	return writer;
}

bool ServoHandler::isMotorXAtMax(){
	return (servo->getFinalPosition(MotorX) >= maxXRight) ||
		   (servo->getFinalPosition(MotorX) <= maxXLeft);
}

bool ServoHandler::isMotorYAtHorizon(){
	return servo->getFinalPosition(MotorY) >= horizon;
}


void ServoHandler::moveTelescopesToBirdTrackingInitialPosition(){
	std::pair<double, double> printer;
	std::cout << "Y Start pos: "<< servo->getFinalPosition(MotorY) << printer.first << std::endl;
	std::cout << "X Start pos: "<< servo->getFinalPosition(MotorX) << printer.first << std::endl;
	
	int		xDir,
			yDir;

	int		currentX = servo->getFinalPosition(MotorX);
	int		currentY = servo->getFinalPosition(MotorY);

	// determine direction of movement based on whether current position is
	// past the default tracking position or not.
	xDir = currentX >= trackPosX ? -1 : 1;
	yDir = currentY >= trackPosY ? -1 : 1;

	while(trackPosY != static_cast<int>(servo->getFinalPosition(MotorY))) {
		printer = moveYaxis(yDir * DEFAULT_SPEED);
	}
	pauseMotors();

	while(trackPosX != static_cast<int>(servo->getFinalPosition(MotorX))) {
		printer = moveXaxis(xDir * DEFAULT_SPEED);
	}
	pauseMotors();

	std::cout << "Telescope in tracking position" << std::endl;
	std::cout << "trackPosX = " << trackPosX << "     x = " << servo->getFinalPosition(MotorX) << std::endl;
	std::cout << "trackPosY = " << trackPosY << "     y = " << servo->getFinalPosition(MotorY) << std::endl;

}


void ServoHandler::pauseMotors(){
	refMx->WaitOne();
	
	tfX->flush();
	xref = 0;
	
	tfY->flush();
	yref = 0;

	refMx->ReleaseMutex();
}

void ServoHandler::setReference(double xref, double yref) {
	refMx->WaitOne();
	this->xref = xref;
	this->yref = yref;
	refMx->ReleaseMutex();
}

void ServoHandler::stop(){
	servoRun = false;
	std::cout << "servoRun = false " << std::endl;
}


void ServoHandler::run(){
	double	xref_current,
			yref_current;
			
	double	startTimer,
			endTimer;

	int		diff;

	std::cout << "ServoHandler::run(): starting" << std::endl;
	while (servoRun) {
		startTimer = omp_get_wtime();

		//make local copy of reference to ensure thread safe behaviour
		refMx->WaitOne();
			xref_current = xref;
			yref_current = yref;
		refMx->ReleaseMutex();


		if(!isMotorXAtMax()){
			this->moveXaxis(xref_current);

		} else {
			servo->setVelocity(MotorX, 0);
			moveTelescopesToBirdTrackingInitialPosition();
		}

		if(!isMotorYAtHorizon()){
			this->moveYaxis(yref_current);
		} else {
			servo->setVelocity(MotorY, 0);
			moveTelescopesToBirdTrackingInitialPosition();
		}
		endTimer = omp_get_wtime();

		// Calculate sleep time to maintain correct periodicity
		diff = PERIOD - (int)(endTimer - startTimer)*1000;
		if (diff > 0)
			Sleep(diff);
	}
	
	std::cout << "ServoHandler::run(): aborting" << std::endl;
	servoThread->Abort();
	servoThread->Join();
	std::cout << "ServoHandler::run(): aborted" << std::endl;
}






/* TESTING FUNCTIONS */


void ServoHandler::runTFTest(Direction dir){
	//Endast till för att testa olika stegsvar..... Till rapporten
	std::pair<double, double> printer;
	std::ofstream writer;
	std::ofstream writer4;
	std::ofstream direct;
	if(dir == VERTICAL) {
			writer.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\Y-4-samp2-stepsize1.csv");
			writer4.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\Y-4-samp4-stepsize1.csv");
			direct.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\Y-4-direct-stepsize1.csv");
		}else{
			writer.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\X-4-samp2-stepsize1.csv");
			writer4.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\X-4-samp4-stepsize1.csv");
			direct.open("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Verification\\X-4-direct-stepsize1.csv");
		}
	
	std::vector<double> Vec1 = step(50, 200, 0.0, (1)*1.0);
	std::vector<double> Vec2 = step(10, 200, 0.0, (-1)*1.0);
	/*--- Step for sampling 2 ---*/
	for(auto i = 0; i < Vec1.size(); i++) {
		if(dir == VERTICAL) {
			printer = moveYaxis(Vec1.at(i));
		}else{
			printer = moveXaxis(Vec1.at(i));
		}
		writer << Vec1.at(i) << ", " << printer.first << ", " << printer.second << std::endl; 
	}

	for(auto i = 0; i < Vec2.size(); i++) {
		if(dir == VERTICAL) {
			printer = moveYaxis(Vec2.at(i));
		}else{
			printer = moveXaxis(Vec2.at(i));
		}
		writer <<Vec2.at(i) << ", " << printer.first << ", " << printer.second << std::endl; 
	}

	this->pauseMotors();	
	writer.close();

	/*--- Step for sampling 4 ---*/
	for(auto i = 0; i < Vec1.size(); i++) {
		if(dir == VERTICAL) {
			printer = moveYaxis4(Vec1.at(i));
		}else{
			printer = moveXaxis4(Vec1.at(i));
		}
		writer4 << Vec1.at(i) << ", " << printer.first << ", " << printer.second << std::endl; 
	}

	for(auto i = 0; i < Vec2.size(); i++) {
		if(dir == VERTICAL) {
			printer = moveYaxis4(Vec2.at(i));
		}else{
			printer = moveXaxis4(Vec2.at(i));
		}
		writer4 << Vec2.at(i) << ", " << printer.first << ", " << printer.second << std::endl; 
	}
	this->pauseMotors();
	writer4.close();
	
	/*--- Step direct to motors---*/
	double returned;
	for(auto i = 0; i < Vec1.size(); i++) {
		if(dir == VERTICAL) {
			servo->setVelocity(MotorY, Vec1.at(i));
			returned = servo->getFinalPosition(MotorY);
		}else{
			servo->setVelocity(MotorX, Vec1.at(i));
			returned = servo->getFinalPosition(MotorX);
		}
		direct << Vec1.at(i) << ", " <<  returned << std::endl; 
	}
	this->pauseMotors();

	for(auto i = 0; i < Vec2.size(); i++) {
		if(dir == VERTICAL) {
			servo->setVelocity(MotorY, Vec2.at(i));
			returned = servo->getFinalPosition(MotorY);
		}else{
			servo->setVelocity(MotorX, Vec2.at(i));
			returned = servo->getFinalPosition(MotorX);
		}
		direct << Vec1.at(i) << ", " <<  returned << std::endl; 
	}
	this->pauseMotors();
}

std::vector<double> ServoHandler::step(int stepTime,int totalTime, double initialAmp, double finalAmp) {
	std::vector<double> signal;

	for(int i = 0; i < totalTime; i++) {
		if(i < stepTime) {
			signal.push_back(initialAmp);
		} else {
			signal.push_back(finalAmp);
		} 
	}
	return signal;
}
