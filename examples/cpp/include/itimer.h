#ifndef ITIMER_H
    #define ITIMER_H

#include <time.h>

//
//   Timer class that does not use floating point
//
#ifdef _WIN32_WCE
typedef unsigned long clock_t;
#endif

class CSimpleTimer {
    public:
        CSimpleTimer();
        void start();
        void stop();
        void reset();
        int time();
        void click();
        void deltaImage(char *dest, int destSize, int decimals = 0);
        const char *deltaImage(int decimals = 0) {
			deltaImage(image, sizeof image, decimals);
			return image;
			}
    protected:
		char image[32];
        char recalc;
        clock_t startTime;
        clock_t stopTime;
        int elapsed;
        int millisec;
        int seconds;
        void calc();
    };
#endif
