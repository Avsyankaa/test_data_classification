#ifndef DTSFCLIB_H

#ifndef _MT
#error The dtsfclib library is compiled with the multithreaded version of the runtime library
#endif


#ifndef USE_DTSEARCH_NAMESPACE
#define USE_DTSEARCH_NAMESPACE
#endif

#include <dtsearch.h>
#include <dtsfc.h>
#include <stringtools.h>
#include <xmlmaker.h>
#include <settings.h>
#if !defined(_UNICODE) && (_MSC_VER >= 1500)
	#error dtsfclib projects must be compiled for the Unicode character set with Visual C++ 
#endif

#ifdef _X64
	#define LIB_PLATFORM "x64"
#else
	#define LIB_PLATFORM "win32"
#endif
#ifdef _AFXDLL
	#define LIB_USEOFMFC "Dynamic"
#else
	#define LIB_USEOFMFC "Static"
#endif

#ifdef _DEBUG
	#define LIB_DBG	"Debug"
#else
	#define LIB_DBG "Release"
#endif


#if _MSC_VER >= 1900
	#define LIB_PLATFORM_TOOLSET "v140"
	#define DTAPPLIB_NAME "dtsfclib-" LIB_PLATFORM_TOOLSET "-" LIB_PLATFORM "-" LIB_USEOFMFC "-" LIB_DBG ".lib"
	#pragma message("Linking with " DTAPPLIB_NAME)
	#pragma comment(lib, DTAPPLIB_NAME)
#elif _MSC_VER >= 1700
	#define LIB_PLATFORM_TOOLSET "v110_xp"
	#define DTAPPLIB_NAME "dtsfclib-" LIB_PLATFORM_TOOLSET "-" LIB_PLATFORM "-" LIB_USEOFMFC "-" LIB_DBG ".lib"
	#pragma message("Linking with " DTAPPLIB_NAME)
	#pragma comment(lib, DTAPPLIB_NAME)
#elif _MSC_VER >= 1600
	#ifdef _WIN64
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfc64id_vc10.lib")
				#pragma message("Using lib dtsfc64id_vc10.lib")
			#else
				#pragma comment(lib, "dtsfc64d_vc10.lib")
				#pragma message("Using lib dtsfc64d_vc10.lib")
			#endif
		#else
			#ifdef _AFXDLL
				#pragma comment(lib, "dtsfc64_vc10.lib")
				#pragma message("Using lib dtsfc64_vc10.lib")
			#else
				#pragma comment(lib, "dtsfc64_s_vc10.lib")
				#pragma message("Using lib dtsfc64_s_vc10.lib")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfcwid_vc10.lib")
				#pragma message("Using lib dtsfcwid_vc10.lib")
			#else 
				#pragma comment(lib, "dtsfcwd_vc10.lib")
				#pragma message("Using lib dtsfcwd_vc10.lib")
			#endif
		#else
			#ifdef _AFXDLL
				#pragma comment(lib, "dtsfcw_vc10.lib")
				#pragma message("Using lib dtsfcw_vc10.lib")
			#else
				#pragma comment(lib, "dtsfcw_s_vc10.lib")
				#pragma message("Using lib dtsfcw_s_vc10.lib")
			#endif					
		#endif
	#endif
#elif _MSC_VER >= 1500
	
	#ifdef _WIN64
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfc64id_vc9.lib")
				#pragma message("Using lib dtsfc64id_vc9.lib")
			#else
				#pragma comment(lib, "dtsfc64d_vc9.lib")
				#pragma message("Using lib dtsfc64d_vc9.lib")
			#endif
		#else
			#ifdef _AFXDLL
				#pragma comment(lib, "dtsfc64_vc9.lib")
				#pragma message("Using lib dtsfc64_vc9.lib")
			#else
				#pragma comment(lib, "dtsfc64_s_vc9.lib")
				#pragma message("Using lib dtsfc64_s_vc9.lib")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfcwid_vc9.lib")
				#pragma message("Using lib dtsfcwid_vc9.lib")
			#else 
				#pragma comment(lib, "dtsfcwd_vc9.lib")
				#pragma message("Using lib dtsfcwd_vc9.lib")
			#endif
		#else
			#ifdef _AFXDLL
				#pragma comment(lib, "dtsfcw_vc9.lib")
				#pragma message("Using lib dtsfcw_vc9.lib")
			#else
				#pragma comment(lib, "dtsfcw_s_vc9.lib")
				#pragma message("Using lib dtsfcw_s_vc9.lib")
			#endif					
		#endif
	#endif
#elif _MSC_VER >= 1400
	#ifdef _WIN64
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfc64id_vc8.lib")
				#pragma message("Using lib dtsfc64id_vc8.lib")
			#else
				#pragma comment(lib, "dtsfc64d_vc8.lib")
				#pragma message("Using lib dtsfc64d_vc8.lib")
			#endif
		#else
			#ifdef _AFXDLL
				#pragma comment(lib, "dtsfc64_vc8.lib")
				#pragma message("Using lib dtsfc64_vc8.lib")
			#else
				#pragma comment(lib, "dtsfc64_s_vc8.lib");
				#pragma message("Using lib dtsfc64_s_vc8.lib")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#ifdef DTSEARCHINTERNAL
				#pragma comment(lib, "dtsfcwid_vc8.lib")
				#pragma message("Using lib dtsfcwid_vc8.lib")
			#elif defined(_UNICODE)
				#pragma comment(lib, "dtsfcwd_vc8.lib")
				#pragma message("Using lib dtsfcwd_vc8.lib")
			#else
				#pragma comment(lib, "dtsfcd_vc8.lib")
				#pragma message("Using lib dtsfcd_vc8.lib")
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _AFXDLL
					#pragma comment(lib, "dtsfcw_vc8.lib")
					#pragma message("Using lib dtsfcw_vc8.lib")
				#else
					#pragma comment(lib, "dtsfcw_s_vc8.lib")
					#pragma message("Using lib dtsfcw_s_vc8.lib")
				#endif					
			#else
				#ifdef _AFXDLL
					#pragma comment(lib, "dtsfc_vc8.lib")
					#pragma message("Using lib dtsfc_vc8.lib")
				#else
					#pragma comment(lib, "dtsfcw_s_vc8.lib")
					#pragma message("Using lib dtsfc_s_vc8.lib")
				#endif					
				
			#endif
		#endif
	#endif
#elif _MSC_VER >= 1300
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "dtsfcwd_vc7.lib")
		#else
			#pragma comment(lib, "dtsfcd_vc7.lib")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "dtsfcw_vc7.lib")
		#else
			#pragma comment(lib, "dtsfc_vc7.lib")
		#endif
	#endif
#ifndef _DLL
#error The dtsfclib library is compiled with the multithreaded DLL version of the runtime library
#endif
#endif



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

#endif
