#pragma once

/**
 * Thread for generating control signals to be set as inpu in Servo objects.
 * Should receive reference signal from a buffer.
 *
 * It is possible that this has to be a managed class (i.e. declared ref class)
 * to be able to contain a reference to Servo.
 */
class Controller
{
public:

	/**
	 * Constructor. Should take reference signal buffer as argument when such
	 * a class exists.
	 */
	Controller(void);

	/**
	 * Destructor.
	 */
	~Controller(void);

};

