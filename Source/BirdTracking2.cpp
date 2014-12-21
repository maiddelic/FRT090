/*	Main class. Starts up everything.
 */
#include "stdafx.h"
#include "ThreadBuffer.h"
#include "NIRead.h"
#include <iostream>
#include <exception>
#include <opencv2\opencv.hpp>
#include "ImageHandler.h"
#include "GUITest.h"

using namespace std;
using namespace System::Threading;

#define FILE_PATH ("C:\\Users\\lumbo\\Dropbox\\frt090\\data\\Motoridentification\\Testing.csv")

int main() {
	ServoHandler ^servoHandler = gcnew ServoHandler();
	ImageHandler ^imHandler = gcnew ImageHandler(0, 50, servoHandler);

	GUITest ^gui = gcnew GUITest(servoHandler, imHandler);
	gui->ShowDialog();

	int stop;
	std::cin >> stop;
	imHandler->stop();
	servoHandler->stop();
	return 0;
}