#ifndef EXCEPTIONS
#define EXCEPTIONS

/* This is an old class, from the previous implementation. It is included in
   Servo.h, but never used anywhere in the new code. */

#include <exception>
#include <sstream>
#include <string>

class CouldNotFindHomeException
{
public:
	CouldNotFindHomeException()
	{
	}
};

#endif
