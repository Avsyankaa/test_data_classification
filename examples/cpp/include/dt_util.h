#ifndef DT_UTIL_H
	#define DT_UTIL_H

#ifndef ToInt 
#	define ToInt(x)  (static_cast<int>(x))
#endif
#ifndef ToSizeT
	#define ToSizeT(x) (static_cast<size_t>(x))
#endif
#ifndef ToPtrDiffT
	#define ToPtrDiffT(x) (static_cast<ptrdiff_t>(x))
#endif
#ifndef ToWCharT
	#define ToWCharT(x) (static_cast<wchar_t>(x))
#endif
#ifndef ToUShort
	#define ToUShort(x) (static_cast<unsigned short>(x))
#endif


#ifndef makeBool
#	define makeBool(x)	((x)? true : false)
#endif

#ifndef _WIN32

#ifndef _stricmp
#	define _stricmp strcasecmp
#endif

#define __int64 long long int

inline int MulDiv(int a, int b, int c)
{   __int64 prod = a;
    prod = prod * b;
    if (c != 0)
        prod = prod / c;
    return (int) prod;
}

#define min(a,b) ((a) > (b)? (b) : (a))

#define _snprintf snprintf


#if !defined(BYTE)
    #define BYTE unsigned char
#endif

#if !defined(WORD)
    #define WORD unsigned short
#endif

#if !defined(DWORD)
	// Under Linux, unsigned long is 64-bits, which is not correct for DWORD
	#define DWORD unsigned int
#endif

#ifndef MAKEWORD
#define MAKEWORD(lo,hi)     ((WORD)(((BYTE)(lo)) | ((WORD)((BYTE)(hi))) << 8))
#endif

#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif



#endif

inline ptrdiff_t _Min(ptrdiff_t a, ptrdiff_t b)
{	if (a > b)
    return b;
else
    return a;
}

//
//   These constructors and copy() functions can use memset and memove
//   because the structs are guaranteed not to have virtual functions
//   and because the pointers in them are supposed to be copied
//   as pointers (i.e., not by reallocating the string).
//

// Copy two flat structures, both of which have a structSize member,
// when the structSizes may be different because of program version differences.
// Size to copy is the minimum of the two structSizes
template <class T>
void copyStruct(T& dest, const T& source, int defaultSize = sizeof(T))
{   ptrdiff_t destSize = (dest.structSize ? dest.structSize : defaultSize);
	ptrdiff_t sourceSize = (source.structSize? source.structSize : defaultSize);
	ptrdiff_t toMove = _Min(destSize, sourceSize);
    ptrdiff_t saveStructSize = dest.structSize;
    memmove(&dest, &source, toMove);
    dest.structSize = (int) saveStructSize;
}

// Clear a flat structure with a structSize member.
template <class T>
void clearStruct(T& item, int defaultSize = sizeof(T))
{   int size = (int) (item.structSize ? item.structSize : defaultSize);
	memset(&item, 0, size);
	item.structSize = size;
}

// Initialize a flat structure with a structSize member
template <class T>
void initStruct(T& item)
{	memset(&item, 0, sizeof(T));
	item.structSize = sizeof(T);
}


#endif

