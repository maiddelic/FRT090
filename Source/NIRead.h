#pragma once
#include "ThreadBuffer.h"
#include <NIDAQmx.h>
#include <stdexcept>

struct NIFrame;

/**
 * Communicate with IR sensor through NI card. This class reads values, one for
 * each quadrant, and sends them to a buffer to be retreived for furhter
 * processing. 
 */
ref class NIRead {
public:
	/**
	 * Constructor. Sets up reading from 4 analog input channel and
	 * configures sampling. Sample rate is currently hardcoded.
	 * Ordering of the input channels (i.e. which quadrant corresponds to
	 * which sample) is also hardcoded.
	 */
	NIRead(ThreadBuffer<NIFrame> ^buf);

	/**
	 * Stops sampling and deallocates resources. The ThreadBuffer object supplied
	 * to the constructor is not deleted, as this could cause issues in other
	 * parts of the code where it is read from.
	 */
	~NIRead();

	/**
	 * Stops reading from the NI-card. This ends the reading thread and
	 * deallocates all resources, including the NIRead object itself.
	 * Using the object in any way after a call to stop() is thus illegal.
	 */
	void stop();

private:
	/* Configure channels and sampling for NI card */
	void init();
	/* run method of thread */
	void run();

	bool doRun;
	ThreadBuffer<NIFrame> ^buf;
	TaskHandle readTask;

	System::Threading::Thread ^readThread;

	/*
	Quadrant numbering (channels):
	-------------
	|  0  |  1  |
	-------------
	|  2  |  3  |
	-------------

	No pinout is specified on the InGaAs sensor, but assuming that it is
	mounted with the cable facing away from the opening, the numbering
	corresponds to the output cables as follows:

		0 - brown
		1 - purple
		2 - white
		3 - green

	Any changes in this input - channel mapping should be done in the
	definitions below, to avoid changes in the rest of the code. Note also
	That the current channel names are dependent on how the IR sensor is
	connected to the NI card.
	*/

	const static char *chan0 = "Dev1/ai11",
					  *chan1 = "Dev1/ai3",
					  *chan2 = "Dev1/ai10",
					  *chan3 = "Dev1/ai2";


};

/**
 * Nicer way to represent a frame than to send around double[4] or (double *) 
 * everywhere.
 */
struct NIFrame {
	double values[4];
};
