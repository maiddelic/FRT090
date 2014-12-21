#ifndef UTILS_H
#define UTILS_H

/* Below are some random functions, used in different parts of the old code.
   Most of them are not used, but BufferedCamCapture has a dependency on
   this. If they are to be used somewhere, it is probably better to place
   them there.
*/
#include <Windows.h>
#include <opencv2\opencv.hpp>
#include <math.h>

double goTowards(double current, double goal, double speed);

double toRadian(double degree);
double toDegree(double radian);

int roundToInt(double in);

/* This method should not be used. cv::Rect should be used instead of CvRect */
bool cvRectEquals(CvRect r1, CvRect r2);

bool waitForKeyPress(int ms);

uint64 getU64Filetime();
std::string u64timeToDateString(uint64 time);
std::string u64timeToMsTimeString(uint64 time);

class TimeCounter
{
public:
	TimeCounter();

	void reset();

	double getDeltaTime();

	SYSTEMTIME getCurrentTime();

	static void systemTimeToDate(SYSTEMTIME& time, short& year, short& month, short& day, short& hour, short& min, short& sec, short& msec);

private:
	unsigned long long lastTime;
};

class LowPassFilter
{
public:
	LowPassFilter(double tau);
	
	void reset();
	void reset(double input);

	double filter(double input, double dt);

	double getValue();

private:
	double value, tau;
};

template <typename T>
ref struct AutoPtr
{
    AutoPtr() : m_ptr(0) {}
    AutoPtr(T* ptr) : m_ptr(ptr) {}
    AutoPtr(AutoPtr<T>% right) : m_ptr(right.Release()) {}

    ~AutoPtr()
    {
        delete m_ptr;
        m_ptr = 0;
    }
    !AutoPtr()
    {
        //ASSERT(0 == m_ptr);
        delete m_ptr;
    }
    T* operator->()
    {
        //ASSERT(0 != m_ptr);
        return m_ptr;
    }

	T* operator=(T* in)
	{
		reset(in);
		return m_ptr;
	}

    T* get()
    {
        return m_ptr;
    }
    T* release()
    {
        T* released = m_ptr;
        m_ptr = 0;
        return released;
    }

    void reset()
    {
        Reset(0);
    }

    void reset(T* ptr)
    {
        if (ptr != m_ptr)
        {
            delete m_ptr;
            m_ptr = ptr;
        }
    }

private:
    T* m_ptr;
};

#endif