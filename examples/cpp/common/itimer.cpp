#include <stdafx.h>
#include <itimer.h>

CSimpleTimer::CSimpleTimer() :
    elapsed(0), recalc(1)
{
}

void CSimpleTimer::start()
{   startTime = clock();
}

void CSimpleTimer::stop()
{   stopTime = clock();
    elapsed += (stopTime - startTime);
    recalc = 1;
}

void CSimpleTimer::click()
{   stopTime = clock();
    elapsed = (stopTime - startTime);
    recalc = 1;
}

void CSimpleTimer::reset()
{   elapsed = 0;
    recalc = 1;
}

#ifndef CLK_TCK
#   define CLK_TCK CLOCKS_PER_SEC
#endif


void CSimpleTimer::calc()
{   if (!recalc)
        return;

	__int64 x = 1000;
	x *= elapsed;
	x /= CLK_TCK;
    int v = (int) x;
    millisec = (v % 1000L);
    seconds = v/1000L;
    recalc = 0;
}

int CSimpleTimer::time()
{   calc();
    if (millisec >= 500L)
        return seconds + 1;
    else
        return seconds;
}

void CSimpleTimer::deltaImage(char *dest, int destSize, int decimals)
{   calc();
    int sec = seconds % 60;
    seconds /= 60;
    int min = seconds % 60;
    seconds /= 60;
    int hours = seconds;
    char ms[64];

#if defined(HAVE_SAFE_SPRINTF)
	_snprintf_s(ms, sizeof ms, _TRUNCATE, "%3.3d", millisec);
    if (decimals < sizeof ms)
        ms[decimals] = 0;
    if (decimals)
        _snprintf_s(dest, destSize, _TRUNCATE, "%3d:%2.2d:%2.2d.%s", hours, min, sec, ms);
    else
        _snprintf_s(dest, destSize, _TRUNCATE, "%3d:%2.2d:%2.2d", hours, min, sec);
#else
	_snprintf(ms, sizeof ms, "%3.3d", millisec);
    if (decimals < sizeof ms)
        ms[decimals] = 0;
    if (decimals)
        _snprintf(dest, destSize, "%3d:%2.2d:%2.2d.%s", hours, min, sec, ms);
    else
        _snprintf(dest, destSize, "%3d:%2.2d:%2.2d", hours, min, sec);
#endif
}
