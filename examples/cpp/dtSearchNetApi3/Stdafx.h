// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <windows.h>
#include <vcclr.h>

#include <dtsearch.h>
#include <dtsfc.h>
#include <dstring.h>
#include <dthread.h>

#ifdef debugLog
#	undef debugLog
#endif
#ifdef gfLoggingEnabled
#	undef gfLoggingEnabled
#endif

void debugLog(const char *s);
void debugLog(const char *s, const char *s2);
void debugLog(const char *s, const char *s2, const char *s3);
void debugLog(const char *s, long v);
void debugLog(const char *s, long a, long b);
#define LogSeparator "=============================================================++"
extern int gfLoggingEnabled;

#include "Constants.h"
#include "Utils.h"
