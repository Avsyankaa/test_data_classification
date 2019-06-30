#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <string.h>
#include <fstream>
using namespace std;
#include <dtconfig.h>
#include <dtsearch.h>
#include <dtsviewr.h>

#if defined(_WIN64)
    #if defined(_DEBUG) && defined(DTSEARCHINTERNAL)
        #pragma comment(lib, "dtengine64d.lib")
    #else
        #pragma comment(lib, "dtengine64.lib")
    #endif        
#else
    #if defined(_DEBUG) && defined(DTSEARCHINTERNAL)
        // dtSearch Corp. internal only
        #pragma comment(lib, "dten600d.lib")
    #else
        #pragma comment(lib, "dten600.lib")
    #endif

#endif

