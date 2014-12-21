#include "stdafx.h"
#include "utils.h"

double goTowards(double current, double goal, double speed)
{
	if (abs(current - goal) <= speed)
	{
		current = goal;
	}
	else if (current > goal)
	{
		current -= speed;
	}
	else
	{
		current += speed;
	}
	return current;
}

double toRadian(double degree)
{
	return degree / 180 * 3.14159265;
}

double toDegree(double radian)
{
	return radian / 3.14159265 * 180;
}

int roundToInt(double in)
{
	return static_cast<int>(in + 0.5);
}

bool cvRectEquals(CvRect r1, CvRect r2)
{
	bool ret = r1.x == r2.x;
	ret = ret && r1.y == r2.y;
	ret = ret && r1.width == r2.width;
	ret = ret && r1.height == r2.height;
	return ret;
}

bool waitForKeyPress(int ms)
{
    int c = cvWaitKey(ms);
    return c != -1;
}

uint64 getU64Filetime()
{
	FILETIME filetimeUTC, filetimeLocal;
	GetSystemTimeAsFileTime(&filetimeUTC);
	FileTimeToLocalFileTime(&filetimeUTC, &filetimeLocal);
	ULARGE_INTEGER tmp;
	tmp.LowPart = filetimeLocal.dwLowDateTime;
	tmp.HighPart = filetimeLocal.dwHighDateTime;
	return tmp.QuadPart;
}

std::string u64timeToDateString(uint64 time)
{
	ULARGE_INTEGER uli;
	uli.QuadPart = time;
	FILETIME ft;
	ft.dwLowDateTime = uli.LowPart;
	ft.dwHighDateTime = uli.HighPart;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	char str[128];
	char* weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	sprintf_s(str, sizeof(str), "%04d-%02d-%02d_%s_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, weekDays[st.wDayOfWeek], st.wHour, st.wMinute, st.wSecond);
	return std::string(str);
}

std::string u64timeToMsTimeString(uint64 time)
{
	ULARGE_INTEGER uli;
	uli.QuadPart = time;
	FILETIME ft;
	ft.dwLowDateTime = uli.LowPart;
	ft.dwHighDateTime = uli.HighPart;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	char str[128];
	sprintf_s(str, sizeof(str), "%02d-%02d-%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return std::string(str);
}


TimeCounter::TimeCounter()
{
	reset();
}

void TimeCounter::reset()
{
	FILETIME timeNow;
	GetSystemTimeAsFileTime(&timeNow);
	lastTime = (static_cast<unsigned long long>(timeNow.dwHighDateTime) << 32) + timeNow.dwLowDateTime;
}

double TimeCounter::getDeltaTime()
{
	FILETIME  timeNow;
	GetSystemTimeAsFileTime(&timeNow);
	unsigned long long now = (static_cast<unsigned long long>(timeNow.dwHighDateTime) << 32) + timeNow.dwLowDateTime;
	double delta = (now - lastTime) / 10000000.0;
	lastTime = now;

	return delta;
}

SYSTEMTIME TimeCounter::getCurrentTime()
{
	SYSTEMTIME timeNow;
	GetLocalTime(&timeNow);
	return timeNow;
}

void TimeCounter::systemTimeToDate(SYSTEMTIME& time, short& year, short& month, short& day, short& hour, short& min, short& sec, short& msec)
{
	year = time.wYear;
	month = time.wMonth;
	day = time.wDay;
	hour = time.wHour;
	min = time.wMinute;
	sec = time.wSecond;
	msec = time.wMilliseconds;
}

LowPassFilter::LowPassFilter(double tau)
{
	this->tau = tau;
	value = 0;
}

void LowPassFilter::reset()
{
	reset(0);
}

void LowPassFilter::reset(double input)
{
	value = input;
}

double LowPassFilter::filter(double input, double dt)
{
	value = (value + (exp(dt/ tau) - 1) * input) / exp(dt / tau);		// exponential filter
	return value;
}

double LowPassFilter::getValue()
{
	return value;
}

