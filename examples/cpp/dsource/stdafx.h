#ifdef __UNIX__
	// Enable large files support
	#define	LARGEFILES
    #define _LARGEFILE_SOURCE
    #define _LARGEFILE64_SOURCE
    #define _FILE_OFFSET_BITS 64
    #define __USE_LARGEFILE64
    #define __USE_FILE_OFFSET64

	#include <unistd.h>
#endif
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
	#include <afx.h>
#endif
#include <fstream>
#include <iostream>
#ifdef _WIN32
	#include <dtsfclib.h>
	#include <stringtools.h>
#else
#	include <dtsearch.h>
#	include <dtsviewr.h>
#	include <dtsfc.h>
#	include <xmlmaker.h>
#	include <settings.h>
#endif

#include <settings.h>

