// Handle platform-dependent macros
//

#ifdef DTCONFIG_H
	#undef ios_binary
	#undef ios_nocreate
	#undef ios_in
	#undef IOS_OPEN_MODE
	#undef cSysSlash
	#undef sSysSlash
#endif

#define DTCONFIG_H

#ifndef __UNIX__
#ifdef __clang__
#   define __UNIX__
#endif
#endif

// Handle iostreams differences
#define ios_binary ios::binary
#if defined(__UNIX__)
#   define ios_nocreate ((ios::openmode) 0)
#   define ios_in  ios::in
#   define IOS_OPEN_MODE ios::openmode
#elif defined(_FSTREAM_)
#   define ios_nocreate 0
#   define ios_in  ios::in
#   define IOS_OPEN_MODE int
#else
#   define ios_nocreate ios::nocreate
#   define ios_in  (ios::in | ios_nocreate)
#   define IOS_OPEN_MODE int
#endif

#ifdef __UNIX__
#   define cSysSlash    '/'
#   define sSysSlash    "/"
#else
#   define cSysSlash    '\\'
#   define sSysSlash    "\\"
#endif

#ifdef __UNIX__
#   define WCHAR_IS_32_BITS
#else
#	define WCHAR_IS_16_BITS
#endif

#if defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64) || defined(_WIN64)
#	ifndef _X64
#		define _X64
#	endif
#endif

#ifdef _X64
#	define SIZE_T_IS_64_BITS
#	ifdef _WIN32
#		define LONG_IS_32_BITS
#	else
		// Under 64-bits Linux, a long is 64-bits, not 32-bits
#		define LONG_IS_64_BITS
#	endif
#else
#	define SIZE_T_IS_32_BITS
#	define LONG_IS_32_BITS
#endif


// Macros that can be set up more than once
#ifndef FAIL
#   define FAIL    (-1)
#endif

#ifndef SUCCESS
#   define SUCCESS  0
#endif

#if !defined(_T) && !defined(UNICODE)
#   define _T(x) x
#   define TCHAR char
#endif

#ifndef debugLog
#   define debugLog  //
#   define gfLoggingEnabled 0
#endif
#ifndef LogFunction
#	define LogFunction //
#endif

#if !defined(_MSC_VER) || (_MSC_VER < 1300)
#ifdef _X64
#	define DWORD_PTR unsigned __int64
#else
#	define DWORD_PTR DWORD
#endif
#endif

#if (_MSC_VER >= 1400)
	#define HAVE_SAFE_SPRINTF
	#define HAVE_SAFE_GMTIME64
	#define HAVE_SAFE_LOCALTIME64
	#define HAVE_SAFE_I64TOA
	#define HAVE_SAFE_LTOA
	#define HAVE_SAFE_ITOA
	#define HAVE_SAFE_STRUPR
	#define HAVE_SAFE_STRLWR
	#define HAVE_SAFE_GMTIME
	#define HAVE_SAFE_GETENV
	#define HAVE_SAFE_FOPEN
#endif

#if defined(VS2005)
	#undef VS2005
#endif
#if defined(VS2003)
	#undef VS2003
#endif
#if defined(__cplusplus_cli) && (_MSC_VER >= 1400)
	#define VS2005
#elif (_MSC_VER >= 1300) && defined(_INC_VCCLR)
	#define VS2003
#endif

#ifndef DTCONFIG_LITTLE_ENDIAN
#define DTCONFIG_LITTLE_ENDIAN
#endif
#ifndef _MSC_VER
	#ifndef _stricmp
		#define _stricmp strcasecmp
	#endif
	#define _strncmp strncmp
	#define _strnicmp strnicmp
	#define _wcsnicmp wcsnicmp
	#define _wcsicmp wcsicmp
	#define _strlwr strlwr
	#define _strupr strupr
	#define _strdup strdup
	#define _itoa itoa
	#define _ltoa ltoa
	#define _putenv putenv
	#define _dup dup
	#define _close close
	#define _getpid getpid
	#define _snprintf snprintf
#endif

#if defined(_WIN32) 
#	if (defined(__cplusplus_winrt))
#		undef HAVE_SAFE_GETENV
#		define _WINRT

#	else
#		define  HAVE_WINAPI
#	endif
#endif
