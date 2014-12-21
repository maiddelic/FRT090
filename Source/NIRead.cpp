#include "NIRead.h"

// if DEBUG_NI is defined, some output to the terminal is performed
#ifdef DEBUG_NI
#include <iostream>
#endif

using namespace std;
using namespace System::Threading;

NIRead::NIRead(ThreadBuffer<NIFrame> ^buf) {
	doRun = true;
	this->buf = buf;
	init();

	readThread = gcnew Thread(gcnew ThreadStart(this, &NIRead::run));
	readThread->Start();
}

NIRead::~NIRead() {
	DAQmxStopTask(readTask);
	DAQmxClearTask(readTask);

	readThread->Abort();
	readThread->Join();
}

void NIRead::stop() {
	doRun = false;
}

void NIRead::init() {
	const uInt32 errStrSize = 2048;
	char errStr[errStrSize];
	int32 error = 0;

	readTask = 0;

	pin_ptr<TaskHandle> pinReadTask = &readTask;	//c++/cli has weird pointers, get real one here.
	error = DAQmxCreateTask("AIVoltageRead", (TaskHandle*) pinReadTask);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

	/* Input channel set up */
	error = DAQmxCreateAIVoltageChan(readTask,
									 chan0,
									 "",
									 DAQmx_Val_RSE,
									 -10,
									 0,
									 DAQmx_Val_Volts,
									 NULL);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

		error = DAQmxCreateAIVoltageChan(readTask,
									 chan1,
									 "",
									 DAQmx_Val_RSE,
									 -10,
									 0,
									 DAQmx_Val_Volts,
									 NULL);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

		error = DAQmxCreateAIVoltageChan(readTask,
									 chan2,
									 "",
									 DAQmx_Val_RSE,
									 -10,
									 0,
									 DAQmx_Val_Volts,
									 NULL);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

		error = DAQmxCreateAIVoltageChan(readTask,
									 chan3,
									 "",
									 DAQmx_Val_RSE,
									 -10,
									 0,
									 DAQmx_Val_Volts,
									 NULL);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

	/* Configure sampling rate */
	error = DAQmxCfgSampClkTiming(readTask,
								  NULL, //timer source, null means NI card internal
								  2,	//read 2 samples/s
								  DAQmx_Val_Rising,
								  DAQmx_Val_ContSamps,
								  10);	//internal buffer size
	if(DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

	/* Begin sampling */
	error = DAQmxStartTask(readTask);
	if (DAQmxFailed(error)) {
		DAQmxGetErrorString(error, errStr, errStrSize);
		throw runtime_error((const char*) errStr);
	}

}

void NIRead::run() {
	const uInt32 errStrSize = 2048;
	char errStr[errStrSize];
	int32 error = 0;

	double values[4];
	int32 samplesRead = 0;


	while(doRun) {
		error = DAQmxReadAnalogF64(readTask,
								   1,	//read 1 samp/channel
								   -1,  //blocking read
								   DAQmx_Val_GroupByChannel,
								   values,
								   4,
								   &samplesRead,
								   NULL);
		if (DAQmxFailed(error)) {
			DAQmxGetErrorString(error, errStr, errStrSize);
			throw runtime_error((const char*) errStr);
		}

		NIFrame frame;
		for (int i = 0; i < 4; i++) {
			frame.values[i] = values[i];

#ifdef DEBUG_NI
			printf("v[%d] = %1.3lf\t", i, values[i]);

		}
		printf("\n");

#else
		}
#endif

		buf->put(frame);
	}


	this->~NIRead();
}
