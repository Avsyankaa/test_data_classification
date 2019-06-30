//
//   Utility string classes for dtSearch Engine samples
//
#pragma once
#ifndef DSTRING_H
    #define DSTRING_H

#include <string.h>
#include <dt_util.h>
#ifndef DARRAY_H
    #include <darray.h>
#endif

#ifdef _MAC
// Including <fstream> causes a warning on OSX if min is defined
	#ifdef min
		#undef min
	#endif
#endif

#pragma warning(push)
#pragma warning(disable:4706) // assignment within conditional expression

//
// DString uses the istream and ostream classes from the standard library.
// These includes and defines ensure that _std_istream and _std_ostream can be
// used to refer to istream and ostream, whether or not "using namespace std"
// has preceeded this header, and whether or not streams support has been included
//
#ifndef NO_STREAMS
	#if !defined(_FSTREAM_) && !defined(_INC_FSTREAM) && !defined(_FSTREAM_H)
	#	include <fstream>
	#endif
	#if defined(_INC_FSTREAM)
	#	define _std_istream istream
	#	define _std_ostream ostream
	#else
	#	define _std_istream std::istream
	#	define _std_ostream std::ostream
	#endif
#endif

#if defined(_WIN32) && !defined(_T)
	#include <tchar.h>
#endif


#include <dtconfig.h>


#ifndef _countof
#define _countof(x)  (sizeof x/sizeof x[0])
#endif

#ifdef __AFX_H__
    #ifndef MfcAvailable
        #define MfcAvailable 
    #endif
#endif


#if defined(MfcAvailable) || defined(_OLEAUTO_H_) 
	#define BSTR_DEFINED
#endif

#ifdef __UNIX__
#   define cSysSlash    '/'
#   define sSysSlash    "/"
#else
#   define cSysSlash    '\\'
#   define sSysSlash    "\\"
#endif

#ifdef __UNIX__
    #define NeedStringFunctions
    #define true 1
    #define false 0
#endif

#   define cUnixSlash   '/'

#if !defined(_WIN32) && !defined(_MAC)
    #define TCHAR char
    #define _T(x) x
    #include <wchar.h>

#endif




#define IllegalFilenameChars "\\/?*:\"<>|"

#define IllegalWindowsPathChars "/?*:\"<>|"


#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

#if defined(__UNIX__) && !defined(DTSEARCH_H)
#error dtsearch.h must be included before dstring.h
#endif

#ifndef FAIL
#   define FAIL -1
#endif
#ifndef SUCCESS
#   define SUCCESS 0
#endif



#ifdef NeedStringFunctions
	int wcsnicmp(const wchar_t *a, const wchar_t *b, ptrdiff_t len);
	int wcsicmp(const wchar_t *a, const wchar_t *b);
    char * strlwr(char *s);
    char *strupr(char *s);
    char *ultoa(unsigned long i, char *dest, unsigned int radix);
    char *_i64toa(__int64 val, char *buf, int base);
    char *itoa(int i, char *dest, int radix);
    char *ltoa(long i, char *dest, int radix);
    int stricmp(const char *a, const char *b);
    int strnicmp(const char *a, const char *b, ptrdiff_t len);
#endif



bool I32_to_A(int v, char *buf, int bufSize, int radix);
bool U32_to_A(unsigned int v, char *buf, int bufSize, int radix);
bool I64_to_A(__int64 v, char *buf, int bufSize, int radix);

bool strFormatNumI32(int v, char *dest, ptrdiff_t maxDigits, char padChar, int radix);
bool strFormatNumU32(unsigned int v, char *dest, ptrdiff_t maxDigits, char padChar, int radix);


bool strFormatNum64(__int64 v, char *dest, ptrdiff_t maxDigits, char padChar, int radix);

inline void strClear(char *dest) {
	*dest = '\0';
}

inline void strClear(wchar_t *dest) {
	*dest = '\0';
}

#if !defined(NoInlineStringFuncs)

inline bool _IsSpace(char c) {
	if (c > 32)
		return false;
	return (c == ' ') ||
		   ((c >= 9) && (c <= 13));
}

inline bool _IsSpace(unsigned char c) {
	if (c > 32)
		return false;
	return (c == ' ') ||
		   ((c >= 9) && (c <= 13));
	}

inline bool _IsSpace(wchar_t c) {
	if (c > 32)
		return false;
	return (c == ' ') ||
		   ((c >= 9) && (c <= 13));
}

inline bool strIsBlank(const char *p)
{   if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while(*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
        }
    return true;
}

inline bool strIsBlank(const wchar_t *p)
{   if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while(*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
        }
    return true;
}

inline bool strIsBlank(const wchar_t *p, ptrdiff_t nChars)
{	if (nChars <= 0)
		return true;
		
    if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while(*p && (nChars-- > 0)) {
        if (!_IsSpace(*p))
            return false;
        p++;
        }
    return true;
}

inline bool strIsBlank(const char *p, ptrdiff_t nChars)
{	if (nChars <= 0)
		return true;
		
    if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while(*p && (nChars-- > 0)) {
        if (!_IsSpace(*p))
            return false;
        p++;
        }
    return true;
}


inline bool IsUnicodeSpace(wchar_t ch) {
	if (ch == 0xa0)
		return true;
	if ((ch >= 0x2000) && (ch <= 0x200a))
		return true;
	if (ch == 0x205F)
		return true;
	if (ch == 0x3000)
		return true;
	return false;
}

void wcsmove(wchar_t *dest, const wchar_t *source, ptrdiff_t cch);


#else


bool _IsSpace(char c);

bool _IsSpace(unsigned char c) ;

bool _IsSpace(wchar_t c) ;

bool strIsBlank(const char *p);
bool strIsBlank(const wchar_t *p);
void wcsmove(wchar_t *dest, const wchar_t *source, long cch);

#endif


char *stristr(const char *a, const char *b);
const char *strrstr(const char *a, const char *b);

int UnicodeToAnsi(char *aDest, const wchar_t *source, ptrdiff_t maxLen);
int AnsiToUnicode(wchar_t *dest, const char *aSource, ptrdiff_t maxLen);

inline char AsciiToUpper(char ch)
{	if ((ch >= 'a') && (ch <= 'z'))
		return ch - ('a' - 'A');
	else
		return ch;
}

inline char AsciiToLower(char ch)
{	if ((ch >= 'A') && (ch <= 'Z'))
		return ch + ('a' - 'A');
	else
		return ch;
}

inline wchar_t AsciiToUpper(wchar_t ch)
{	if ((ch >= 'a') && (ch <= 'z'))
		return ch - ('a' - 'A');
	else
		return ch;
}

inline wchar_t AsciiToLower(wchar_t ch)
{	if ((ch >= 'A') && (ch <= 'Z'))
		return ch + ('a' - 'A');
	else
		return ch;
}

inline void Ascii_strlwr(char *s)
{	char ch;
	while((ch = *s)) {
		*s = AsciiToLower(ch);
		s++;
		}
}


inline void Ascii_strupr(char *s)
{	char ch;
	while((ch = *s)) {
		*s = AsciiToUpper(ch);
		s++;
		}
}

inline int Ascii_stricmp(const char *a, const char *b)
{	char ca = *a, cb = *b;
    while (ca && cb) {
        int v = AsciiToUpper(ca) - AsciiToUpper(cb);
        if (v != 0)
            return v;
        a++;
        b++;
        ca = *a;
        cb = *b;
    }
    if (a && ca)
        return 1;
    else if (b && cb)
        return -1;
    else
        return 0;
}

inline int Ascii_strnicmp(const char *a, const char *b, ptrdiff_t len)
{
	char ca = *a, cb = *b;
    while (ca && cb && (len > 0)) {
        int v = AsciiToUpper(ca) - AsciiToUpper(cb);
        if (v != 0)
            return v;
        a++;
        b++;
        ca = *a;
        cb = *b;
		len--;
    }
    if (len == 0)
        return 0;
    else if (a && ca)
        return 1;
    else if (b && cb)
        return -1;
    else
        return 0;
}

inline int Ascii_memicmp(const char *a, const char *b, ptrdiff_t len)
{
    while (len > 0) {
        int v = AsciiToUpper(*a) - AsciiToUpper(*b);
        if (v != 0)
            return v;
        a++;
        b++;
		len--;
		}
    return 0;
}


inline bool Ascii_IsDigit(wchar_t c) {
		 return ((c >= '0') && (c <= '9')); 	
	}
	
inline bool Ascii_IsAlpha(wchar_t c) {
    return ((c >= 'A') && (c <= 'Z')) ||
           ((c >= 'a') && (c <= 'z')) ||
           (c < 0);
    }

inline bool Ascii_IsAlphaW(wchar_t c) {
    return ((c >= 'A') && (c <= 'Z')) ||
           ((c >= 'a') && (c <= 'z')) ||
           (c < 0);
    }

inline bool Ascii_IsAlnum(wchar_t c) {
    return ((c >= 'A') && (c <= 'Z')) ||
           ((c >= 'a') && (c <= 'z')) ||
           ((c >= '0') && (c <= '9')) ||
           (c < 0);
    }

inline bool Ascii_IsXDigit(wchar_t c) {
    return Ascii_IsDigit(c) ||
            ((c >= 'a') && (c <= 'f')) ||
            ((c >= 'A') && (c <= 'F'));
}

inline bool Ascii_IsPunct(wchar_t c) {
	return (c > 32) && (c < 127) && !Ascii_IsAlnum(c);
	}

inline bool Ascii_IsSpace(wchar_t c) {
	if (c > 32)
		return false;
	return (c == ' ') ||
		   ((c >= 9) && (c <= 13));
}

inline ptrdiff_t sstrlen(const char *s) {
	if (s)
		return (strlen(s));
	else
		return 0;
}

inline int swcslen(const wchar_t *s) {
	if (s)
		return ((int) wcslen(s));
	else
		return 0;
}



inline bool strIsLowChars(const unsigned char *s, ptrdiff_t len = -1)
{   if (len < 0)
        len = (int) strlen((const char *)s);
    for (int i = 0; i < len; ++i) {
        if ((*s > 127) || (*s == 27))
            return false;
        s++;
        }
    return true;
}

inline bool strIsLowChars(const wchar_t *s, ptrdiff_t len = -1)
{   if (len < 0)
        len = (int) wcslen(s);
    for (int i = 0; i < len; ++i) {
        if (*s > 127)
            return false;
        s++;
        }
    return true;
}

inline bool strIsAsciiChars(const char *text, ptrdiff_t len)
{  const unsigned char *s = (const unsigned char *) text;
    if (len < 0)
        len = strlen((const char *)s);
    for (int i = 0; i < len; ++i) {
		unsigned char ch = *s;
        if (ch > 127)
            return false;
        if ((ch < 31) && !Ascii_IsSpace(ch))
			return false;
		s++;
        }
    return true;
}



inline bool strIsLowChars(const char *s, ptrdiff_t len = -1) {
    return strIsLowChars((const unsigned char *)s, len);
}


struct HexNum {
	char text[64];
	
	HexNum(int v) {
		I32_to_A(v, text, sizeof text, 16); }
	HexNum(unsigned int v) {
		U32_to_A(v, text, sizeof text, 16); }
	HexNum(__int64 v) {
		I64_to_A(v, text, sizeof text, 16); }
	operator const char *() { 	return text;	}
	};
	

struct PadHexNum {
	char text[64];
	PadHexNum(int v, ptrdiff_t nChars, bool bCaps = false) {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumI32(v, text, nChars, '0', 16);  
		if (bCaps)
			Ascii_strupr(text);
		}
	PadHexNum(unsigned short v, ptrdiff_t nChars, bool bCaps = false) {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumU32(v, text, nChars, '0', 16);  
		if (bCaps)
			Ascii_strupr(text);
		}
	PadHexNum(unsigned int v, ptrdiff_t nChars, bool bCaps = false) {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumU32(v, text, nChars, '0', 16);  
		if (bCaps)
			Ascii_strupr(text);
		}
	PadHexNum(unsigned long v, ptrdiff_t nChars, bool bCaps = false) {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNum64(v, text, nChars, '0', 16);  
		if (bCaps)
			Ascii_strupr(text);
		}
	PadHexNum(__int64 v, ptrdiff_t nChars, bool bCaps = false) {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNum64(v, text, nChars, '0', 16);  
		if (bCaps)
			Ascii_strupr(text);
		}
	operator const char *() {	return text; }
	};
	
struct NumImage {
	char text[64];
	NumImage(int v) {
		I32_to_A(v, text, sizeof text, 10); }
	NumImage(unsigned int v) {
		U32_to_A(v, text, sizeof text, 10); }
	NumImage(__int64 v) {
		I64_to_A(v, text, sizeof text, 10); }
	NumImage(double v, const char *fmt = 0);
	operator const char *() {	return text; }
	};

struct NumImageRadix {
	char text[64];
	NumImageRadix(int v, int radix) {
		I32_to_A(v, text, sizeof text, radix);
	}
	NumImageRadix(unsigned int v, int radix) {
		U32_to_A(v, text, sizeof text, radix);
	}
	NumImageRadix(__int64 v, int radix) {
		I64_to_A(v, text, sizeof text, radix);
	}
	operator const char *() { return text; }
};

struct CommaNum {
	char text[64];
	CommaNum(__int64 v, char comma = ',', int grouping = 3) {
		char tmp[64];
		I64_to_A(v, tmp, sizeof tmp, 10);
		formatWithComma(tmp, comma, grouping);
		}
	void formatWithComma(const char *num, char comma = ',', int grouping = 3) {
		ptrdiff_t numlen = strlen(num);
		if (numlen + numlen/grouping + 1 >= sizeof text)
			return;

		int iChar = 0;
		for (int i = 0; i < numlen; ++i) {
			text[iChar++] = num[i];
			ptrdiff_t digitsLeft = numlen - i - 1;
			if ((digitsLeft > grouping-1) && (digitsLeft % grouping == 0))
				text[iChar++] = comma;
			}
		text[iChar] = 0;
		}
	operator const char *() { return text; }
	};

struct PadCommaNum : public CommaNum {
	PadCommaNum(__int64 v, ptrdiff_t len) : CommaNum(v) {
		if (len >= _countof(text))
			len = _countof(text);
		ptrdiff_t numlen = strlen(text);
		if (numlen < len) {
			memmove(&text[len-numlen], &text[0], numlen);
			memset(text, ' ', len-numlen);
			text[len] = 0;
			}
		}
	operator const char *() { return text; }
	};
	

struct PadNum {
	char text[64];
	PadNum(int v, ptrdiff_t nChars, char padChar = '0') {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumI32(v, text, nChars, padChar, 10);  }
	PadNum(unsigned int v, ptrdiff_t nChars, char padChar = '0') {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumU32(v, text, nChars, padChar, 10);  }
#ifdef _WIN32
	PadNum(DWORD v, ptrdiff_t nChars, char padChar = '0') {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNumU32((unsigned int) v, text, nChars, padChar, 10);  }
#endif
	PadNum(__int64 v, ptrdiff_t nChars, char padChar = '0') {
		nChars = _Min(nChars, sizeof text - 1);
		strFormatNum64(v, text, nChars, padChar, 10);  }
	operator const char *() {	return text; }
	};

int ScaleValue(long num, long denom, int scale);
int ScaleValue64(__int64 num, __int64 denom, int scale);
inline int Diff64(__int64 a, __int64 b) {
	__int64 c = a - b;
	return (int) c;
	}

/* Summary
   Template class for resizable buffer of allocated items.
   Remarks
   The items must be simple types such as char or wchar_t that can be copied with
   memmove. For internal use by the other classes.                                */
template <class Type>
class DMemBuffer {
	public:
		// Allocate a buffer, specifying the number of items
		DMemBuffer(ptrdiff_t aSize = 1024) {
			init();
			if (aSize > 0)
				allocate(aSize);
			}
		// Attach an existing buffer
		DMemBuffer(void *pData, ptrdiff_t aSize) {
			init();
			attach((Type *) pData, aSize/sizeof(Type));
			}
		// Attach an existing buffer, optionally taking ownership
		DMemBuffer(Type *pData, ptrdiff_t aCount, bool shouldDelete = false) {
			init();
			attach(pData, aCount, shouldDelete);
			}
		~DMemBuffer() {
			freeBuffer();
			}
		// Allocate a specified number of the items
		bool allocate(ptrdiff_t aCount) {
			freeBuffer();
			m_buffer = new Type[aCount];
			if (m_buffer) {
				m_fOwnsBuffer = true;
				m_maxCount = aCount;
				return true;
				}
			if (aCount > 3)
				allocate(3);
			return false;
			}
		// Attach an existing buffer, optionally taking ownership
		void attach(Type *pData, ptrdiff_t aCount, bool shouldDelete = false) {
			freeBuffer();
			m_buffer = pData;
			m_maxCount = aCount;
			m_fOwnsBuffer = shouldDelete;
			}
 		// Get buffer size
		ptrdiff_t getMaxCount() {
			return m_maxCount;
			}
		// Get start of buffer
		Type *getBuffer() {
			return m_buffer;
			}
		// Store data in buffer
		bool store(const Type *pData, ptrdiff_t count) {
			if (count > m_maxCount) {
				if (!allocate(count))
					return false;
				}
			for (ptrdiff_t i = 0; i < count; ++i) {
				m_buffer[i] = pData[i];
				}
			return true;
			}
		void setAt(ptrdiff_t index, const Type& val) {
			if ((index >= 0) && (index < m_maxCount))
				m_buffer[index] = val;
			}
		// Detach the buffer, but do not delete it, and return a pointer to the start of the buffer
		Type *detachBuffer() {
			if (m_fOwnsBuffer) {
				Type *ret = m_buffer;
				init();
				return ret;
				}
			return 0;
			}
		void swap(DMemBuffer& other) {
				Swap(m_maxCount, other.m_maxCount);
				Swap(m_fOwnsBuffer, other.m_fOwnsBuffer);
				Swap(m_buffer, other.m_buffer);
			}
		bool good() const {
			return m_buffer != NULL;
		}
		void freeBuffer() {
			if (m_buffer && m_fOwnsBuffer)
				delete [] m_buffer;
			m_buffer = 0;
			m_fOwnsBuffer = 0;
			m_maxCount = 0;
			}
		bool goodOffset(ptrdiff_t pos) const {
			return (pos >= 0) && (pos < m_maxCount);
			}
		 // Try to ensure that the buffer is at least of a specified size, extending the buffer if necessary,
		 // and return false if the buffer cannot be allocated
		bool tryExtend(ptrdiff_t newSize, ptrdiff_t dataCount) {
			if (m_maxCount >= newSize)
		        return true;
			Type *newBuf = new(std::nothrow) Type[newSize];
			if (!newBuf) {
				return false;
				}
			if (m_buffer && (dataCount > 0))
				memmove(newBuf, m_buffer, sizeof(Type) * min(dataCount, newSize));
			attach(newBuf, newSize, true);
			return true;
			}
	private:
		ptrdiff_t m_maxCount;
		Type *m_buffer;
		bool m_fOwnsBuffer;

		void init() {
			m_maxCount = 0;
			m_buffer = 0;
			m_fOwnsBuffer = 0;
			}
		 // Ensure that the buffer is at least of a specified size, extending the buffer if necessary
		 void extend(ptrdiff_t newSize, ptrdiff_t dataCount) {
			 if (m_maxCount >= newSize)
		        return;
			Type *newBuf = new Type[newSize];
			if (m_buffer && (m_maxCount > 0))
				memmove(newBuf, m_buffer, sizeof(Type) * dataCount);
			attach(newBuf, newSize, true);
			}
	};

//! For converting .NET strings

#if defined(VS2005)
class CConvertedDotNetString {
	public:
		CConvertedDotNetString(System::String ^str) : contents(str? str->Length+1 : 0) {
			if (str) {
				System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str);
				const wchar_t *pChars = (const wchar_t *) p.ToPointer();
				len = str->Length;
				contents.store(pChars, len);
				contents.setAt(len, 0);
				System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
				}
			else {
				len = 0;
				}
			}
		virtual ~CConvertedDotNetString() {
			}
		operator const wchar_t *() {
			return contents.getBuffer();
			}
		ptrdiff_t getLength() {
			return len;
			}
	protected:
		DMemBuffer<wchar_t> contents;
		ptrdiff_t len;
	};

#elif defined(VS2003)
class CConvertedDotNetString {
	public:
		CConvertedDotNetString(System::String *str) : contents(str? str->Length+1 : 0) {
			if (str) {
				System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str);
				const wchar_t *pChars = (const wchar_t *) p.ToPointer();
				len = str->Length;
				contents.store(pChars, len);
				contents.setAt(len, 0);
				System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
				}
			else
				len = 0;
			}
		virtual ~CConvertedDotNetString() {
			}
		operator const wchar_t *() {
			return contents.getBuffer();
			}
		ptrdiff_t getLength() {
			return len;
			}
	protected:
		DMemBuffer<wchar_t> contents;
		ptrdiff_t len;
	};
#endif  // vs.net 2005



/* Summary
   Simple string class for use in the dtSearch <link C++ Support Classes, C++ Support classes>
   and sample code.
   Remarks
   Usually the text in a DString is stored as UTF-8, although any single-byte
   character set can be used.

   For convenience, text and numeric values can be appended to a DString using an
   iostreams-like syntax. Example:
   <code>
   DString x;
   int i = 5
   \x \<\< "This is some text and a number, which is " \<\< i;
   </code>                                                                                     */
class DString {
    public:
    	// Constructor, specifying the initial size of the string buffer
        DString(ptrdiff_t aSize = 1023);
        // Make a DString that is a copy of another string
        DString(const DString& other);
        // Make a DString that contains specified text
        DString(const char *text);
        // Make a DString that contains specified text, provided as Unicode characters
        DString(const wchar_t *text);
		// Make a DString from a buffer
		DString(char *aBuf, ptrdiff_t aBufLen) ;

		// Make a DString from a buffer keeping the text
		DString(char *aBuf, ptrdiff_t aBufLen, bool bKeeptext, bool bNoStrlen) ;
		
		// Store either ANSI or Unicode as UTF8, depending on type of TCHAR
		bool storeUiAsUtf8(const TCHAR *s, ptrdiff_t nChars = -1);
		
		// Store Ansi text, converting it to UTF-8
		bool storeAnsiAsUtf8(const char *s, ptrdiff_t nChars = -1); 

		// Store Unicode text, converting it to UTF-8
		bool storeUnicodeAsUtf8(const wchar_t *s, ptrdiff_t nChars)
		{   if (!s || !*s) {
		        clear();
		        return true;
		    }
		    if (nChars < 0)
		        nChars = swcslen(s);
		    // make space for the Utf8 encoding of the Unicode string
		    if (strIsLowChars(s, nChars)) {
		    	if (!tryExtend(nChars+1))
		    		return false;
		    	for (int i = 0; i < nChars; ++i) {
		    		text[i] = (char) s[i];
		    		}
		    	len = nChars;
		    	text[len] = 0;
		    	return true;
		    	}

		    else 
				return storeUnicodeAsUtf8_Encoding(s, nChars);
			}

		// Store Unicode text, converting it to UTF-8, appending to an existing string
		bool appendUnicodeAsUtf8(const wchar_t *s, ptrdiff_t nChars)
		{   if (!s || !*s) {
		        return true;
		    }
		    if (nChars < 0)
		        nChars = swcslen(s);
		    if (len == 0) 
				return storeUnicodeAsUtf8(s, nChars);
		    // make space for the Utf8 encoding of the Unicode string
		    if (strIsLowChars(s, nChars)) {
		    	if (!tryExtend(len + nChars+1))
		    		return false;
		    	for (int i = 0; i < nChars; ++i) {
		    		text[i+len] = (char) s[i];
		    		}
		    	len += nChars;
		    	text[len] = 0;
		    	return true;
		    	}

		    else 
				return appendUnicodeAsUtf8_Encoding(s, nChars);
			}

	private:
		bool storeUnicodeAsUtf8_Encoding(const wchar_t *s, ptrdiff_t nChars);
		bool appendUnicodeAsUtf8_Encoding(const wchar_t *s, ptrdiff_t nChars);
	public:		
		static void wcharToUtf8(char *dest, ptrdiff_t len, wchar_t ch);
		// Store Unicode text, converting it to UTF-8
		bool storeUnicodeAsUtf8(const wchar_t *ws) {
			return storeUnicodeAsUtf8(ws, swcslen(ws));
		}
		// Store Unicode text, converting it to Ansi
        void storeUnicodeAsAnsi(const wchar_t *text, ptrdiff_t len);
		// Store Unicode text, converting it to Ansi
		void storeUnicodeAsAnsi(const wchar_t *ws) {
			storeUnicodeAsAnsi(ws, swcslen(ws));
		}
        // Convert UTF-8 text to Unicode
        void decodeUtf8ToUnicode(wchar_t *dest, ptrdiff_t maxLen);

        wchar_t *decodeUtf8ToUnicode();

#if defined(BSTR_DEFINED)
		// Return the text in the DString as a COM BSTR.  The caller must free the returned value.
        BSTR allocSysString() const;
#endif
#if defined(VS2005)
		System::String ^allocNetString() const;
		static System::String ^allocNetString(const char *text);
		/* Store a .NET System::String */
		void set(System::String ^str) {
			clear();
			if (str) {
				CConvertedDotNetString cstr(str);
				storeUnicodeAsUtf8(cstr, cstr.getLength());
				}
			}
#elif defined(VS2003)
		System::String *allocNetString() const;
		static System::String *allocNetString(const char *text);
		/* Store a .NET System::String */
		void set(System::String *str) {
			clear();
			if (str) {
				CConvertedDotNetString cstr(str);
				storeUnicodeAsUtf8(cstr);
				}
			}
#endif
        virtual ~DString() {
			dealloc();
	        }
        // Clear the contents of a DString and store a single character
        DString& operator=(char c) {
			clear();
			*this << c;
			return *this;
			}
		// Copy another string
        DString& operator=(const DString& other);
		// Copy another string
        DString& operator=(const char *other);
		// Copy another string
        DString& operator=(const unsigned char *other) {
			if (other && ((const char *) other != text)) {
				clear();
				store((const char *) other, strlen((const char*) other));
				}
			return *this;
			}
		inline void copy(const DString& other) {
			setText(other, other.getLength());
			}
		// Copy another string
		inline void copy(const char *source) {
			setText(source);
			}
		// Set text contents
		inline void setText(const char *source, ptrdiff_t aLen = -1) {
			clear();
			if (aLen < 0)
				aLen = strlen(source);
			store(source, aLen);
		}
		// Remove trailing white space
		void rtwhite();
		// Remove trailing white space, checking for UTF-8 encoded hard spaces
		void rtwhiteUtf8();
		// Remove leading white space
		void rlwhite();
		// Remove leading white space, checking for UTF-8 encoded hard spaces
		void rlwhiteUtf8();
		
		// Copy a Unicode string, store it as UTF-8
        DString& operator=(const wchar_t *other);
        // copy by length, potentially including null characters
        void binaryCopy(const DString& other);
        // Append an integer to a string
        DString& operator<<(long l);
        // Append an integer to a string
        DString& operator<<(unsigned long l);
        // Append an integer to a string
        DString& operator<<(unsigned int i) {
			return *this << ((unsigned long)i);
			}
        // Append an integer to a string
        DString& operator<<(int i);
        // Append an integer to a string
		DString& operator<<(__int64 i);
        // Append an integer to a string
		DString& operator<<(unsigned __int64 s);

		DString& operator<<(NumImage& img) {
			append(img);
			return *this;
		}

		DString& operator<<(const wchar_t *ws) {
			appendUnicodeAsUtf8(ws, wcslen(ws));
			return *this;
			}

		// Append a double to a string
		DString& operator<<(double v);
        // Append a string
        DString& operator+=(const char *s) {
            append(s);
            return *this;
            }
        // Append a character
        DString& operator+=(char c) {
            *this << c;
            return *this;
            }
        // Returns true if this string equals the other string, using strcmp
        bool operator==(const DString& other) const {
                return !strcmp(text, other);
                }
        // Returns true if this string equals the other string, using strcmp
        bool operator==(const char *other) const {
				return equals(other);
                }
        bool equals(const char *other) const {
                if (!other)
                    return (*text == 0);
				return !strcmp(text, other);
				}
        // Returns false if this string equals the other string, using strcmp
        bool operator!=(const DString& other) const {
                return strcmp(text, other) != 0;
                }
        // Returns false if this string equals the other string, using strcmp
        bool operator!=(const char *other) const {
                return strcmp(text, other) != 0;
                }
        bool operator>(const char *other) const {
			return cmp(other) > 0;
			}
		bool operator<(const char *other) const {
			return cmp(other) < 0;
			}
		bool operator>=(const char *other) const {
			return cmp(other) >= 0;
			}
		bool operator<=(const char *other) const {
			return cmp(other) <= 0;
			}
        bool operator>(const DString& other) const {
			return cmp(other) > 0;
			}
		bool operator<(const DString& other) const {
			return cmp(other) < 0;
			}
		bool operator>=(const DString& other) const {
			return cmp(other) >= 0;
			}
		bool operator<=(const DString& other) const {
			return cmp(other) <= 0;
			}
        int icmp(const char *other) const {
            return _stricmp(text, other);
            }
		int cmp(const char *other) const {
			return strcmp(text, other);
			}
		// Append text to a string
        inline void append(const char *s, ptrdiff_t bytes = -1) {
                if (s) {
					if (bytes < 0)
						bytes = strlen(s);
                    store(s, bytes);
                    }
                }


		inline bool tryStore(const char *source, ptrdiff_t bytes) {
		   if (source && (bytes > 0)) {
		        if (len + bytes > maxLen) {
				    if (!tryExtend(len + bytes + maxLen))
						return false;
					}
				if (len + bytes <= maxLen) {
					memmove(text + len, source, bytes);
					len += bytes;
					text[len] = 0;
					}
				}
			return true;
			}
		// Extend will throw std::bad_alloc if memory is insufficient
		inline void store(const char *source, ptrdiff_t bytes) {
		   if (source && (bytes > 0)) {
		        if (len + bytes > maxLen) 
				    extend(len + bytes + maxLen);
				if (len + bytes <= maxLen) {
					memmove(text + len, source, bytes);
					len += bytes;
					text[len] = 0;
					}
				}
			}

		// Append a string
        inline DString& operator<<(const char *s) {
            append(s);
            return *this;
            }
		// Append a string
        inline DString& operator<<(const DString& ds)
        {   store(ds.text, (ptrdiff_t) ds.len);
            return *this;
        }
		// Append a single character
        inline void appendChar(char c)
        {   if (len < maxLen) {
                text[len++] = c;
                text[len] = '\0';
                }
            else {
                extend(maxLen*2 + 1);
                if (len < maxLen) {
					text[len++] = (char) c;
					text[len] = '\0';
					}
                }
        }
        // Append without null-termination for faster building of strings
        inline void appendCharNT(char c)
        {   if (len < maxLen) {
                text[len++] = c;
                }
            else {
                extend(maxLen*2 + 1);
                if (len < maxLen)
					text[len++] = (char) c;
                }
        }
        // Append a string without null-termination for faster building of strings
        inline void appendStrNT(const char *s) {
			while(*s) {
				appendCharNT(*s);
				s++;
				}
			}
		// Null-terminate the string
        inline void nullTerminate() {
			text[len] = 0;
			}
		// Append a single character
        inline DString& operator<<(char c)
        {	appendChar(c);
            return *this;
        }
		// Append a single unsigned character
        inline DString& operator<<(unsigned char c)
        {	appendChar((char) c);
            return *this;
        }

#ifdef _WIN64        
        inline char operator[](int i) {
            if ((i >= 0) && (i < len))
                return text[i];
            else
                return '\0';
            }
#endif        

        inline char operator[](ptrdiff_t i) {
            if ((i >= 0) && (i < len))
                return text[i];
            else
                return '\0';
            }
        // Get character at a specified position in the string
        char getChar(ptrdiff_t i) const {
            if ((i >= 0) && (i < len))
                return text[i];
            else
                return '\0';
            }
        unsigned char getUChar(ptrdiff_t i) const {
            if ((i >= 0) && (i < len))
                return (unsigned char) text[i];
            else
                return '\0';
            }
        // Get character at a specified position in the string.
        void setChar(ptrdiff_t i, char ch)  {
            if ((i >= 0) && (i < len))
                text[i] = ch;
			else if (i >= len)
				appendChar(ch);
            }
        // Returns a pointer to the internal buffer holding the string
        char *str() const {
            return text;
            }
        // Returns an unsigned character pointer to the internal buffer holding the string
        unsigned char *ustr() {
            return (unsigned char *) text;
            }
        // Returns a pointer to the internal buffer holding the string
        operator const char *() const {
            return text;
            };
       	// Returns the length of the string
        ptrdiff_t getLength() const { return len; }
        void setLength(ptrdiff_t len);
        // Get the size of the buffer used to store the string
        ptrdiff_t getMaxLength() const { return maxLen;}
        // Append text, URL-encoding it
        void appendUrlEncoded(const char *s);
        // URL-encode the contents of the string
        void urlEncode();
        // Append text, URL-encoding it but preserving filename characters
        void appendUrlEncodedFilename(const char *s);
        void urlEncodeFilename();
        // Decode URL-encoded characters
        void urlDecode();
        // Decode URL-encoded characters
        void urlDecode(const char *source, ptrdiff_t nChars = -1);
        // Decode URL-encoded characters and convert to UTF-8
        void urlDecodeU8();
		// Append text, HTML-encoding it.  If bAllowTags, then newline will be 
		// stored using a BR tag.
        void appendHtmlEncoded(const char *s, bool bAllowTags = true);
        // HTML-encode the contents of the string
        void htmlEncode();
        void clear() {
			len = 0;
			if (text)
				*text = 0;
		}
        void setToNulls(ptrdiff_t ct = -1) {
			len = 0;
			if (ct < 0)
				ct = maxLen;
			if (ct > maxLen)
				ct = maxLen+1;
			if (text)
				memset(text, 0, ct);
			}
#ifndef NO_STREAMS
        int getFrom(_std_istream& in, int bytes = -1);
        void getLine(_std_istream& in);
#endif
        // Read the contents of a file into a text buffer, automatically converting UCS-16 to UTF-8
		int getFromDecoded(const TCHAR *file, int maxToRead = -1);
        // Read the contents of a file into a text buffer, automatically converting UCS-16 to UTF-8 (UTF-8 filename)
		int getFromDecodedU8(const char *file, int maxToRead = -1);

        // Read the contents of a file into a text buffer (TCHAR filename)
        int getFrom(const TCHAR *fn, __int64 maxToRead = -1);
        // Read the contents of a file into a text buffer (UTF8 filename)
        int getFromU8(const char *utf8Name, __int64 maxToRead = -1);
        // Write contents of the string to a file, using a TCHAR filename
        int writeTo(const TCHAR *fn, int fAppend = false);
        // Write contents of the string to a file, using a UTF-8 filename
        int writeToU8(const char *utf8Name, int fAppend = false);
        // Write contents of the string to a file, using a UTF-8 filename, and throw a DFileException on error
		void writeToThrow(const char *utf8Name, int fAppend = false);

        void appendHexEncoded(const void *pData, ptrdiff_t bytes);
        void hexDecode(const char *hexData);

		// Fill string with a character
        void padTo(ptrdiff_t len, char c = ' ');
		// Fill string with a character, inserting the character in the front of the string
		void padFrontTo(int len, char c = ' ');
		// Remove a range of characters from a string
        void remove(ptrdiff_t startPos, ptrdiff_t bytes);
        // Insert a string at an offset
        void insert(const char *s, ptrdiff_t offset);
        // Insert a character at an offset
        void insert(char c, ptrdiff_t offset) {
            char s[2];
            s[0] = c;
            s[1] = '\0';
            insert(s, offset);
            }
        // Replace one string with another
        void replace(const char *from, const char *to);
        // Replace one string with another, with case-insensitive matching
        void ireplace(const char *from, const char *to);
        // Replace one character with another
        inline void replace(char from, char to) {
			char *pEnd = text+len;
			if (!from) {
				for (char *p = text; p < pEnd; ++p) {
					if (!*p)
						*p = to;
					}
				}
			else {
				for (char *p = text; p < pEnd; ++p) {
					if (*p == from)
						*p = to;
					}
				}
			}
		// Remove all instances of a character
		void removeChar(char c);
        // Truncate a string to a specified length
        void truncate(ptrdiff_t t);
        // Truncate string at offset of character (not including the character)
        void truncateAtChar(char ch);
        // Truncate string at offset of substring (not including the substring)
        void truncateBeforeString(const char *s);

		// Try to extend the string buffer to a minimum length, and return false if unsuccessful
        virtual bool tryExtend(ptrdiff_t newLen);
		// Extend the string buffer to a minimum length
        void extend(ptrdiff_t newLen);
		// Replace one set of characters with another
        void translate(const char *from, const char *to);
        // Find a substring, with an optional starting offset
        // Returns:  Starting offset of the string if found, or -1 if not found
        ptrdiff_t find(const char *s, ptrdiff_t offset = 0) const;
        // Find a character, starting at the end of the string
        // Returns: Offset of the character, or -1 if not found
        ptrdiff_t rfind(char c) const;
        // Find a character, starting at the front of the string
        // Returns: Offset of the character, or -1 if not found
        ptrdiff_t find(char c) const;
        // Returns true if char occurs in the string
        bool contains(char c) const {
			return (find(c) >= 0);
			}
        // Find a string, starting at the end of the string
        // Returns:  Offset of the string, or -1 if not found
        ptrdiff_t rfind(const char *s) const;
        // Find a string, with case-insensitive matching
        ptrdiff_t ifind(const char *s, ptrdiff_t offset = 0) const;
        // Returns true if substr occurs in this string
        bool contains(const char *substr) const {
			return find(substr) >= 0;
			}
        // Returns true if substr occurs in this string (case-insensitive)
        bool icontains(const char *substr) const {
			return ifind(substr) >= 0;
			}

        // Find first instance of any of the characters in a list
        ptrdiff_t findFirstOf(const char *charList, ptrdiff_t offset = 0) const;
        // Last character in the string, or null if the string is empty
        char last() const {
            return (len && text? text[len-1] : '\0' );
            }
        // First character in the string, or null if the string is empty
        char first() const {
            return (len && text? text[0] : '\0');
            }
        // Returns true if the string does not contain any non-whitespace characters
        bool isBlank() const {
			return strIsBlank(text);
		}
        // Remove all leading and trailing whitespace
        void trim();
        void packWhite() {
			packWhite(true);
			}
		
		void packWhite(bool bRemoveLeading);
		        
        void unQuote(char cQuote = '\"');
        void unQuote(char cQuote1, char cQuote2);
        // Convert to upper case
        void toUpper();
        // Convert to lower case
        void toLower();
        // Convert only A-Z to lower case
        inline void asciiToLower() {
			Ascii_strlwr(text);
			}
		// Convert only a-z to upper case
		inline void asciiToUpper() {
			Ascii_strupr(text);
			}

        void updateLength();
        void doubleNull() {
            if (len >= maxLen)
                extend(len+1);
            if (len < maxLen) {
				text[len] = '\0';
				text[len+1] = '\0';
				}
            }
        // Append an integer, using commas to format the number (so 12345 would be appended as 12,345)
        void commaNum(int v, const char *comma = 0);
        // Append an integer, using commas to format the number (so 12345 would be appended as 12,345)
        void commaNum(__int64 v, const char *comma = 0);
        // Append an integer, using commas to format the number (so 12345 would be appended as 12,345)
        void commaNum(unsigned int v, const char *comma = 0);
        // Append an integer, using commas to format the number (so 12345 would be appended as 12,345)
        void commaNum(unsigned long v, const char *comma = 0) {
			commaNum((__int64) v, comma);
			}
        // Append an integer, using commas to format the number (so 12345 would be appended as 12,345)
        void commaNum(long v, const char *comma = 0) {
			commaNum((__int64) v, comma);
			}
			
        long toInt();
        const char *midstr(int offset) const {
			if (offset < len)
				return text + offset;
			else
				return text;
			}
        const char *right(int aCt) const {
            if (aCt <= len)
                return text + len - aCt;
            else
                return text;
            }

        char *detachBuffer() {
            if (bOwnsBuffer) {
                bOwnsBuffer = false;
                return text;
                }
            else
                return 0;
            }
		// Append an integer formatted as a hexadecimal number without zero-padding
		void hex(int v) {
				hex((long)v);
		}
		// Append an integer formatted as a hexadecimal number without zero-padding
		void hex(unsigned int v) {
				hex((unsigned long)v);
		}
		// Append an integer formatted as a hexadecimal number without zero-padding
        void hex(long v);
		// Append an integer formatted as a hexadecimal number without zero-padding
        void hex(unsigned long v);
		// Append an integer formatted as a hexadecimal number without zero-padding
        void hex(__int64 v);
		// Append an integer formatted as a hexadecimal number without zero-padding
		void hex(__int64 v, const char *comma);
		// Append an integer formatted as a hexadecimal number without zero-padding
		void hex(unsigned __int64 v);
		// Append an integer formatted as a hexadecimal number with zero-padding
		void hexpad(int v, int nDigits) {
			PadHexNum hp(v, nDigits);
			append(hp);
		}
		void decpad(int v, int nDigits);
		void ymd(int y, int m, int d, char sep = '/');
		void hms(int h, int m, int s);
		void format(double v, const char *fmt);

        static bool isLowChars(const char *s, ptrdiff_t len = -1);

        bool istartsWith(const char *s, bool bSkipSpaces = true) const;
        bool startsWith(const char *s) const {
			return startsWith(text, s);
			}
		bool endsWith(const char *s);
		static bool endsWith(const char *s, const char *toFind);
		static bool endsWith(const wchar_t *s, const wchar_t *toFind);
        static bool istartsWith(const char *s, const char *t)  {
			return !_strnicmp(s, t, strlen(t));
			}
		static bool startsWith(const char *s, const char *t) {
			return !strncmp(s, t, strlen(t));
			}
        static bool istartsWith(const wchar_t *s, const wchar_t *t)  {
			return !_wcsnicmp(s, t, wcslen(t));
			}
		static bool startsWith(const wchar_t *s, const wchar_t *t) {
			return !wcsncmp(s, t, wcslen(t));
			}
		void makeLegalFilename();
#ifdef DTENGINE_IMPORTS
		// Get the contents of a string returned from the dtSearch Engine as a dtsStringHandle
		// Parameters:
		//    hStr:   The handle returned from the dtSearch Engine
		//    fReleaseHandle:  If true, the handle will be released after the string has been retrieved
        bool getFromStringHandle(dtsStringHandle hStr, int fReleaseHandle) {
            len = 0;
            *text = 0;
            if (!hStr)
				return false;
            ptrdiff_t stringLen = dtssStringGetLength(hStr);
            if (!tryExtend(stringLen))
				return false;
            dtssStringGetText(hStr, text, (int) maxLen+1);
            len = stringLen;
            if (fReleaseHandle)
                dtssStringDelete(hStr);
            return true;
            }
#endif
		void substr(DString& dest, ptrdiff_t offset)
		{   dest.clear();
			dest.store(text + offset, len - offset);
		}
		// Extract a substring from a string
		void substr(DString& dest, ptrdiff_t offset, ptrdiff_t substrlen)
		{   dest.clear();
			dest.store(text + offset, substrlen);
		}
		// Get an environment variable and return its value
		const char *safeGetEnv(const char *var);
		// Convert encoding from Utf8 to Ansi
		void utf8ToAnsi();
		// Convert encoding from Ansi to Utf8
		void ansiToUtf8();

		DString& operator<<(DArrayAsVector<long>& table) {
			for (int i = 0; i < table.getCount(); ++i) {
				*this << table.getValue(i) << ' ';
				}
			return *this;
			}
		void storeArray(long *v, int ct) {
			for (int i = 0; i < ct; ++i) {
				*this << v[i] << ' ';
				}
			}
			
    protected:
        ptrdiff_t len;
        ptrdiff_t maxLen;
        char *text;
        bool bOwnsBuffer;
        void allocate(ptrdiff_t aSize);
        inline void dealloc() {
			if ((text != 0) && bOwnsBuffer) {
				delete [] text;
			    bOwnsBuffer = false;
			    }
		    text = 0;
			}
    };

//! String initialized with an 80-character fixed-size buffer, to allocate short strings on the stack
class DString80 : public DString {
	public:
		DString80() :
			DString(m_fixedBuffer, 80) { };
		DString80(const char *s) :
			DString(m_fixedBuffer, 80) {
			append(s);
			}
		DString80& operator=(const char *s) {
			DString::operator=(s);
			return *this;
			};
		DString80& operator=(const DString80& other) {
			DString::binaryCopy(other);
			return *this;
			};
		DString80& operator=(const DString& other) {
			DString::binaryCopy(other);
			return *this;
			};
	protected:
		char m_fixedBuffer[80];
	};

//! String initialized with a 256-character fixed-size buffer, to allocate short strings on the stack
class DString256 : public DString {
	public:
		DString256() :
			DString(m_fixedBuffer, 256) { };
		DString256(const char *s) :
			DString(m_fixedBuffer, 256) {
			append(s);
			}
		DString256& operator=(const char *s) {
			DString::operator=(s);
			return *this;
			};
		DString256& operator=(const DString256& other) {
			DString::binaryCopy(other);
			return *this;
			};
		DString256& operator=(const DString& other) {
			DString::binaryCopy(other);
			return *this;
			};
	protected:
		char m_fixedBuffer[256];
	};

#ifndef NO_STREAMS
_std_istream& operator>>(_std_istream& in, DString& s);
_std_ostream& operator<<(_std_ostream& out, const DString& s);
#endif

/* Summary
   Simple string set class for use in the dtSearch C++ Support classes and sample
   code.
   Remarks
   Usually the text in a DStringSet is stored as UTF-8, although any single-byte
   character set can be used.

   In the dtSearch Engine's C++ API, several API elements expect a set of strings,
   such as the folders to index in dtsIndexJob or the indexes to search in
   dtsSearchJob, formatted as a null-delimited string set Each string is separated
   by a single null character, with a double-null at the end, like this:
   <code>
        "First string\\0 Second string\\0 Third string \\0\\0"
   </code>
   DStringSet provides a class to construct this type of string set. To create a
   null-delimited string set from a set of strings, declare a DStringSet, call
   append() with each string, and then call getBuffer() to get the start of the
   buffer. Example:
   <code>
        DStringSet excludeFilters;
        excludeFilters.append("*.exe");
        excludeFilters.append("*.dll");
        excludeFilters.append("*.fon");
        dtsIndexJob indexJob;
        indexJob.toAdd.excludeFilters = excludeFilters.getBuffer();
   </code>                                                                         */
class DStringSet {
    public:
    	// Constructor
        DStringSet();
        ~DStringSet();
        // Constructor specifying a starting size for the buffer
        DStringSet(long aBufSize);
        // Constructor copying data from another string set
        DStringSet(const DStringSet& other);
        // Constructor copying data from a buffer containing null-delimited string set
        DStringSet(const char *textBuf);
        // Compare two sets for equality.  Returns true if both sets have exactly the same strings, in the same order
        int operator==(const DStringSet& other) const;
        // Copy a set
        DStringSet& operator=(const DStringSet& other);
        void freeBuffer();
        // Copy a set
        void copy(const DStringSet& other) {
            *this = other;
            }
        // Copy a set from a buffer containing a null-delimited string set, terminated by a double null
        bool copyFromBuf(const char *s, int aBufLen = -1);
        // Read lines of text from a file into a string set
        int getLinesFrom(const TCHAR *file, bool bAllowEmptyLines = true);
        // Read lines of text from a file into a string set (UTF-8 filename)
        int getLinesFromU8(const char *file, bool bAllowEmptyLines = true);

        // Write lines of text to a file from a string set (UTF-8 filename)
        int writeLinesToU8(const char *file);
        
        // Parse lines of text from a text buffer into a string set
        void parseLinesFrom(DString& text, bool bAllowEmptyLines = true);
        // Append a string to the set
        bool append(const char *s, ptrdiff_t sLen = -1);
        // Append a Unicode string to the set (converting to UTF8)
        bool appendW(const wchar_t *s) {
			DString tmp;
			tmp.storeUnicodeAsUtf8(s);
			return append(tmp);
			}
        // Append a Unicode string to the set (converting to UTF8)
        bool appendW(const wchar_t *s, ptrdiff_t len) {
			DString tmp;
			tmp.storeUnicodeAsUtf8(s, len);
			return append(tmp);
			}
		// Format an integer as a string and append it to the set
        void append(long v);
        // Append all strings from a set to this set
        void append(const DStringSet& other);
        // Insert a string as the first string in this set
        void prepend(const char *s);
        // Append a string to this set only if it is not already present, using a case-insensitive search
        void iappendUnique(const char *s) {
            if (ifind(s) == FAIL)
                append(s);
            }
        // Append a string to this set only if it is not already present
        bool appendUnique(const char *s) {
            if (find(s) == FAIL) {
                append(s);
                return true;
                }
            return false;
            }
        // Append all strings in another set that are not already present, using a case-insensitive search
        void iunion(const DStringSet& other) {
            for (int i = 0; i < other.getCount(); ++i)
                iappendUnique(other.getString(i));
            }
        // Append all strings in another set that are not already present
        void unionExact(const DStringSet& other) {
            for (int i = 0; i < other.getCount(); ++i)
                appendUnique(other.getString(i));
            }
        // Create a string set from a delimited string
        void tokenize(const char *s, char sepChar, int fAllowEmptyTokens = 0);
        // Create a string set from a delimited string, allowing for multiple delimiter characters
        void tokenize(const char *s, const char *sepChars, int fAllowEmptyTokens = 0);
        // Create a string set from a delimited string, allowing for quotation
        void tokenizeq(const char *s, const char *sepChars, char quoteChar = '\"', int fAllowEmptyTokens = false);

        const char *getBuf() const;
        // Return one string from the set
        const char *getString(int i) const;
        // Return last string from the set
        const char *getLastString() const {
			if (getCount() > 0)
				return getString(getCount()-1);
			else
				return 0;
			}
        const char *getFirstString() const {
			if (getCount() > 0)
				return getString(0);
			else
				return 0;
			}

        const unsigned char *getUString(int i) const {
			return (const unsigned char *) getString(i);
			}
		void getString(int i, DString& dest) const {
			dest = getString(i);
			}
        long getInt(int i) const;
        // Return the number of strings in the set
        int getCount() const;
        // Return the size of the data in the buffer
        ptrdiff_t getLength() const;
        // Clear all strings from the set
        void clear();
        // Remove a string from the set
        void deleteString(int i);
        // Find a string in the set
        int find(const char *s) const;
        // Find a string in the set (case-insensitive)
        int ifind(const char *s) const;
        // Find a string starting with a prefix
        int findPrefix(const char *s) const;
        // Find a string starting with a prefix, searching in reverse order
        int rFindPrefix(const char *s) const;
        // Find a string starting with a prefix (case-insensitive)
        int ifindPrefix(const char *s) const;
        // Format the set as a delimited, quoted string (assumes values contain no quotes)
        void formatAsString(DString& dest, const char *delimiter = 0, char quote = '\0') const;
        // Format the set as a delimited, quoted string, minimizing the use of quotation marks
        void formatAsSimpleString(DString& dest, char delimiter = ' ', char quote = '\"') const;
		// Format the set as field: value pairs        
        void formatAsTable(DString& dest) const;

        void urlDecode(const char *s);
        void urlEncode(DString& dest);
        // Returns the start of the buffer containing the strings in this set
        char *getBuffer() const {
            return buf;
            }
        // Search for a field name.
        // Assumes the set contains a series of field names, each followed by its corresponding field value
        int findFieldName(const char *s) const;
        // Append a field-value pair (appends the field name, then appends the value)
        void appendField(const char *f, const char *v);
        // Retrieve a field value by name.
        // Returns: true if the field was found
        // Assumes the set contains a series of field names, each followed by its corresponding field value
        bool getField(const char *f, DString& v) const;
        // Sort strings in the set in alphabetical order
        void sort();
		static ptrdiff_t calcSetLength(const char *ptr);

#if defined(VS2005)
		// Append a .NET System::String
		void append(System::String ^str) {
			if (str) {
				CConvertedDotNetString cstr(str);
				appendW(cstr, cstr.getLength());
				}
			}
		// Make a string set from a .NET System::Collections::Specialized::StringCollection
		void copy(System::Collections::Specialized::StringCollection ^column) {
			clear();
			if (!column)
				return;
			for (int i = 0; i < column->Count; ++i) {
				System::String ^s = column[i];
				append(s);
				}
			}
		void tokenizeq(System::String ^s, const char *sepChars, char quoteChar = 34, int fAllowEmptyTokens = 0) {
			DString tmp;
			tmp.set(s);
			tokenizeq(tmp, sepChars, quoteChar, fAllowEmptyTokens);
			}
#elif defined(VS2003)
		// Append a .NET System::String
		void append(System::String *str) {
			if (str) {
				CConvertedDotNetString cstr(str);
				appendW(cstr);
				}
			}
		// Make a string set from a .NET System::Collections::Specialized::StringCollection
		void copy(System::Collections::Specialized::StringCollection *column) {
			clear();
			if (!column)
				return;
			for (int i = 0; i < column->Count; ++i) {
				System::String *s = column->get_Item(i);
				append(s);
				}
			}
		void tokenizeq(System::String *s, const char *sepChars, char quoteChar = 34, int fAllowEmptyTokens = 0) {
			DString tmp;
			tmp.set(s);
			tokenizeq(tmp, sepChars, quoteChar, fAllowEmptyTokens);
			}
#endif
        bool extend(ptrdiff_t newLen);
		const char **getv() {
			return index.c_vector();
			}
    protected:

        int count;
        ptrdiff_t textLen;
        ptrdiff_t maxLen;
        char *buf;
        DArrayAsVector<const char *> index;
    };


/* Summary
   Simple Unicode string class for use in the dtSearch <link C++ Support Classes, C++ Support classes>
   and sample code.                                                                                    */
class DWString {
public:
	// Construct a Unicode string from UTF-8 textt
    DWString(const char *utf8Text);
    // Construct a Unicode string from Unicode text
    DWString(const wchar_t *ws);
    // Construct a Unicode string from another Unicode string
    DWString(const DWString& other);
    // Construct an empty Unicode string
    DWString();
    // Construct an empty Unicode string, specifying the initial length of the bfufer
    DWString(long aLen);
    // Construct an empty Unicode string, using a pre-existing buffer
    DWString(wchar_t *pBuf, int aSize, bool shouldDelete) :
		m_buf(pBuf, aSize, shouldDelete) {
		updateBuffer();
		clear();
		}
    ~DWString();

	// Copy a string
    DWString& operator=(const wchar_t *other) {
		if (other != text) {
			clear();
			append(other);
			}
        return *this;
        }



	// Copy a string
    DWString& operator=(const char *other) {
		clear();
		storeUtf8AsUnicode(other);
        return *this;
        }

	// Copy a string
    DWString& operator=(const DWString& other) {
        copy(other);
        return *this;
        }

	// Copy a string
	void copy(const DWString& other) {
        if (text != other.text) {
			clear();
			store(other.text, other.len);
			}
		}


	// Returns start of the Unicode buffer
    operator const wchar_t *() const {
        return text;
        }
    // First character in the string, or null if the string is empty
    wchar_t first() const {
            return (len && text? text[0] : '\0');
            }
    // Clear all text in the string
    void clear() {
        len = 0;
        text[len] = 0;
        }
    void replace(wchar_t from, wchar_t to);
    void replace(const wchar_t *from, const wchar_t *to);
    void storeAnsiAsUnicode(const char *s);
    // Store UTF-8 text as Unicode, replacing any contents in the string
    inline void storeUtf8AsUnicode(const char *utf8String, ptrdiff_t aLen = -1)
	{   if (!utf8String) {
    	    clear();
        	return;
    	}
    	if (aLen < 0) 
        	aLen = strlen(utf8String);

    	if (!storeLowChars(utf8String, aLen)) 
    		storeUtf8AsUnicode_Decode(utf8String, aLen);
		}
private:
	void storeUtf8AsUnicode_Decode(const char *utf8String, ptrdiff_t aLen) ;
	// try to copy UTF-8 to Unicode without decoding, if the input is all
	// in the range 0..127.  If any characters are found outside the range,
	// return false so the input can be UTF-8 decoded
	inline bool storeLowChars(const char *utf8, ptrdiff_t aLen) {
		len = 0;
		extend(aLen+1);
		len = (int) aLen;
		wchar_t *pDest = text;
		while(aLen-- > 0) {
			char ch = *utf8++;
			if (ch > 0) 
				*pDest++ = ch;
			else {
				len = 0;
				text[len] = 0;
				return false;
				}
			}
		*pDest = 0;
		return true;
		}

public:
    void storeUcs16AsUnicode(const unsigned short *s, ptrdiff_t nChars);
    void invertByteOrder();
    void storeUiAsUnicode(const TCHAR *s) {
#ifdef UNICODE
        *this = s;
#else
        storeAnsiAsUnicode(s);
#endif
        }
#if defined(BSTR_DEFINED)
    BSTR allocSysString() const;
#endif
	// Convert the string to Ansi
    void getAnsi(DString& dest) const {
        dest.storeUnicodeAsAnsi(text);
        }
    // Convert the string to UTF-8
    void getUtf8(DString& dest) const {
        dest.storeUnicodeAsUtf8(text);
        }
    wchar_t *str() const {
        return text;
        }
    // Returns the number of characters in the string
    ptrdiff_t getLength() const {
        return len;
        }
    // Set the length of the string, null-terminating the contents of the buffer
    void setLength(ptrdiff_t newLen);
    // Read contents of a file as Unicode
    void getFrom(const TCHAR *fn, long maxToRead = -1);
    // Write Unicode text to a file
    int writeTo(const TCHAR *fn);
    // Transfer nChars of Unicode text from another buffer
    bool store(const wchar_t *source, ptrdiff_t nChars);

    // Extend the string buffer to a minimum length
    void extend(ptrdiff_t newLen);

    // Try to extend the string buffer to a minimum length, and return false if unsuccessful
	bool tryExtend(ptrdiff_t newLen);

	// Append Unicode text to the string
    void append(const wchar_t *s) {
		if (!s || !*s)
			return;
		store(s, swcslen(s));
		}

	// Remove a range of characters from the string
    void remove(ptrdiff_t startPos, ptrdiff_t nChars);
    // Insert characters in front of the string
    void insert(const wchar_t *s, ptrdiff_t offset);
    void translate(const wchar_t *from, const wchar_t *to);
    void translateCh(wchar_t from, wchar_t to);

	// Find a character
	ptrdiff_t find(wchar_t ch, ptrdiff_t offset = 0) const;
    
    // Find a substring (case-insensitive)
    ptrdiff_t ifind(const wchar_t *s, ptrdiff_t offset) const;
    // Find a substring
    ptrdiff_t find(const wchar_t *s, ptrdiff_t offset) const;
    // Determine whether a substring is contained in this string
    bool contains( const wchar_t * s) const {
		return find(s, 0) >= 0;
		}
	// Determine whether a characters exists in this string
	bool contains(wchar_t ch) const {
		for (int i = 0; i < len; ++i) {
			if (text[i] == ch)
				return true;
			}
		return false;
		}
    // Truncate the string
    void truncate(ptrdiff_t newLen);
    // Append a single character
    inline void appendChar(wchar_t wc) {
            if (len < maxLen) {
                text[len++] = wc;
                text[len] = '\0';
                }
            else {
                extend(maxLen*2 + 1);
                text[len++] = wc;
                text[len] = '\0';
                }
		}
    //! Append without null-termination for faster building of strings
    inline void appendCharNT(wchar_t c)
    {   if (len < maxLen) {
            text[len++] = c;
            }
        else {
            extend(maxLen*2 + 1);
            text[len++] = (char) c;
            }
    }
    //! Null-terminate the string (use after calling appendCharNT)
    inline void nullTerminate() {
		text[len] = 0;
		}
	// Append a single character
    inline DWString& operator<<(char c) {
		appendChar(c);
		return *this;
		}
	// Append a single character
    inline DWString& operator<<(unsigned char c) {
		appendChar(c);
		return *this;
		}
	// Append a single character
    inline DWString& operator<<(wchar_t wc) {
		appendChar(wc);
		return *this;
		}
#ifdef VS2005
	// Append a single character
    inline DWString& operator<<(unsigned short wc) {
		appendChar((wchar_t) wc);
		return *this;
		}
#endif
	// Append a single-byte string without conversion
	inline DWString& operator <<(const char *s)
	{   while (s && *s) {
			appendChar(*s);
	        s++;
		}
		return *this;
	}
	// Append a string
	inline DWString& operator <<(const wchar_t *ws)
	{   append(ws);
		return *this;
	}

	// Format an integer value as a string and append the string
    DWString& operator<<(long i);
	// Format an integer value as a string and append the string
    DWString& operator<<(int i);
    // Returns the last character in the string, or null if the string is empty
    wchar_t last() const {
        return (len && text? text[len-1] : '\0' );
        }
    // Returns true if the string is empty or contains only whitespace
    bool isBlank() const;
    // Returns true if the string is empty or contains only whitespace, checking for Unicode whitespace
    bool isBlankUnicode() const;
    ptrdiff_t getMaxLength() const { return maxLen;}
    wchar_t *detachBuffer() {
		wchar_t *ret = m_buf.detachBuffer();
		text = 0;
		len = 0;
		maxLen = 0;
		return ret;
		}
	inline bool equals(const DWString& other) const {
		if (other.len != len)
			return false;
		return !wcscmp(text, other.text);
		}
	inline bool equals(const wchar_t *other) const {
		return !wcscmp(text, other);
		}
	// Remove trailing whitespace
	void rtwhite();
	// Remove leading whitespace
	void rlwhite();
	// Remove leading and trailing whitespace
	void trim();
	// Remove leading whitespace and pack all other whitespace to a single space character
	void packWhite();
	// Format a date as YYYY/MM/DD
	void ymd(int y, int m, int d);
	// Format a time as HHHH/MM/SS
	void hms(int h, int m, int s);
	// Convert to lower case
	void toLower();
	// Convert to upper case
	void toUpper();
	// Get one character
	inline wchar_t getChar(int index) const {
		if ((index >= 0) && (index < len))
			return text[index];
		else
			return 0;
		}
	
	// Get one character
	inline void setChar(int index, wchar_t ch) {
		if ((index >= 0) && (index < len))
			text[index] = ch;
		}
protected:
	void updateBuffer() {
		text = m_buf.getBuffer();
		maxLen = (int) m_buf.getMaxCount()-1;
		}
    wchar_t *text;
    ptrdiff_t len;
    ptrdiff_t maxLen;

    void allocate(ptrdiff_t aLen);
    DMemBuffer<wchar_t> m_buf;
};

class DWString256 : public DWString {
	public:
		DWString256() :
			DWString(m_fixedBuffer, 256, false) { };
		DWString256(const wchar_t *s) :
			DWString(m_fixedBuffer, 256, false) {
			append(s);
			}
		DWString256& operator=(const wchar_t *s) {
			DWString::operator=(s);
			return *this;
			};
		DWString256& operator=(const DWString256& other) {
			DWString::operator=(other);
			return *this;
			};
		DWString256& operator=(const DWString& other) {
			DWString::operator=(other);
			return *this;
			};
	protected:
		wchar_t m_fixedBuffer[256];
	};

#ifndef NO_STREAMS
_std_ostream& operator<<(_std_ostream& out, const DWString& ws);
#endif

class DWStringSet {
    public:
        DWStringSet();
        ~DWStringSet();
        DWStringSet(long aBufSize);
        DWStringSet(const DWStringSet& other);
        DWStringSet(const wchar_t *textBuf);
        DWStringSet& operator=(const DWStringSet& other);
        void copy(const DWStringSet& other);
        void copyFromBuf(const wchar_t *s, ptrdiff_t aBufLen = -1);
        void append(const wchar_t *s, ptrdiff_t sLen = -1);
        void appendU8(const char *s) {
            DWString tmp;
            tmp.storeUtf8AsUnicode(s);
            append(tmp);
            }
        void append(long v);
        void append(const DWStringSet& other);
        void tokenize(const wchar_t *s, wchar_t sepChar, int fAllowEmptyTokens = 0);
        void tokenize(const wchar_t *s, const wchar_t *sepChars, int fAllowEmptyTokens = 0);
        void tokenizeq(const wchar_t *s, const wchar_t *sepChars, wchar_t quoteChar = '\"');
        const wchar_t *getBuf() const;
        const wchar_t *getString(int i) const;

        const wchar_t *getLastString() const {
			if (getCount() > 0)
				return getString(getCount()-1);
			else
				return 0;
			}
        const wchar_t *getFirstString() const {
			if (getCount() > 0)
				return getString(0);
			else
				return 0;
			}
        
        long getInt(int i) const;
        int getCount() const;
        ptrdiff_t getLength() const;
        ptrdiff_t getDataSizeInBytes() const {
			return (textLen+1) * sizeof(wchar_t);
			}
        void clear();
        int find(const wchar_t *s) const;
        int ifind(const wchar_t *s) const;
        void deleteString(int i);
        int findPrefix(const wchar_t *s) const;
        void formatAsString(DWString& dest, const wchar_t *delimiter = 0, wchar_t quote = '\0') const;
        void formatAsSimpleString(DWString& dest, wchar_t delimiter = ' ', wchar_t quote = '\"') const;
        const wchar_t *getBuffer() const {
            return buf;
            }
        void iunion(const DWStringSet& other) {
            for (int i = 0; i < other.getCount(); ++i)
                iappendUnique(other.getString(i));
            }
        void iappendUnique(const wchar_t *s) {
            if (ifind(s) == FAIL)
                append(s);
            }
		void appendField(const wchar_t *f, const wchar_t *v);
		void appendFieldU8(const char *f, const char *v);
		ptrdiff_t getMaxLen() const {
			return maxLen;
			}
    protected:
        void extend(ptrdiff_t newLen);

        int count;
        ptrdiff_t maxLen;
        ptrdiff_t textLen;
        wchar_t *buf;
        DArrayAsVector<const wchar_t *> index;
    };


#ifdef UNICODE
    typedef DWString DUiString;
#else
    typedef DString DUiString;
#endif


//!   Maintains a mapping between a set of string names and a set of
//!   string values in a StringSet.
class DStringMap {
    public:
        DStringMap();
        ~DStringMap();
        DStringMap& operator=(const DStringMap& other);
        DStringMap(const DStringMap& other);
        void clear();
        void copy(const DStringMap& other);
        void add(const char *aName, const char *aValue, int valueLen = -1, int fBuildTables = true);
        void set(const char *aName, const char *aValue, int valueLen = -1);
        const char *get(const char *aName, int which = 0);
        void getAll(const char *aName, DString& s);
        void getAll(const char *aName, DStringSet& s);
        int getIndex(const char *aName, int which = 0);
        const char *getName(int i);
        const char *getValue(int i);
        int getCount() const { return count; };
        int countValuesWithName(const char *name);
        void copyFromBuf(const char *buf, int aLen = -1);

        const char *getBuf() const;
        ptrdiff_t getLength() const;
        void remove(const char *aName);
        void buildTables();
    protected:
        DArrayAsVector<const char *> name;
        DArrayAsVector<const char *> value;
        DStringSet stringSet;
        int count;

    };

class DFieldSet : public DStringSet {
public:
    DFieldSet() {};
    int getIntField(const char *name) {
		const char *s = getField(name);
		if (strIsBlank(s))
			return 0;
		else
			return atoi(s);
    }
    const char *getField(const char *fieldName) {
        int i = findField(fieldName);
        if (i > FAIL)
            return getString(i+1);
        else
            return "";
    }
    void addField(const char *name, const char *value) {
        append(name);
        if (!*value)
            value = " ";
        append(value);
    }
    void addField(const char *name, long value) {
        append(name);
        append(value);
    }
protected:
    int findField(const char *fieldName) const {
        for (int i = 0; i < getCount(); i += 2) {
            if (!_stricmp(fieldName, getString(i)))
                return i;
        }
        return -1;
    }
};



#ifdef _IsSpace
#   undef _IsSpace
#endif


// like strncpy but guarantees null-termination and does not do extra padding
void strCopy(char *s, const char *t, ptrdiff_t len);
void strCopy(wchar_t *s, const wchar_t *t, ptrdiff_t nMaxChars);
#define strCopyBuf(s, t) strCopy(s, t, _countof(s))
bool strMatches(const char *str, const char *wildCardStr);
bool striMatches(const char *str, const char *wildCardStr);

/* Summary
   Filename class for use in the dtSearch C++ Support classes and sample code.
   Remarks
   Filenames are stored as UTF-8 in a string buffer based on DString. DFilename adds
   no data members and just provides functions to manipulate paths in a string. The
   path manipulation functions handle Unix-style and Windows-style paths.            */
class DFilename : public DString {
    public:
		// Construct a filename from a string
        DFilename(const char *s ) :
            DString(s) { };
        // Construct an empty filename
        DFilename() { };
       	// Construct a filename from another filename
        DFilename(const DFilename& other) {
            *this = other.text;
            }
		DFilename(const char *path, const char *name) :
			DString(path) {
			addTail(name);
			}
			
        // Copy a filename
        DFilename& operator=(const DFilename& other) {
			if (text != other.text) {
				clear();
				*this << other;
				}
            return *this;
            }
        // Copy a filename
        DFilename& operator=(const DString& other) {
			if (text != other.str()) {
				clear();
				*this << other;
				}
            return *this;
            }
        // Store a string as a filename
        DFilename& operator=(const char *s) {
            // check for assignment to self
            if (text == s)
                return *this;
            clear();
            *this << s;
            return *this;
            }
        // Store a Unicode string as a filename
        DFilename& operator=(const wchar_t *s) {
            clear();
            storeUnicodeAsUtf8(s);
            return *this;
            }
        // Get the folder name from a path
        void getPath(DString& dest);
		// Change the folder name
		void setPath(const char *newPath) {
			rmPath();
			DFilename t;
			t = newPath;
			t.addTail(*this);
			copy(t);
			}
        // Get the filename from the string
        // (for example, "sample.doc" if the string is "/tmp/subfolder/sample.doc")
        void getTail(DString& dest);
        // Append a filename to the path in the string, adding a slash if necessary
        void addTail(const char *s);
        // Remove the last element in the filename
        void rmTail();
        void rmQuery();
        // Remove the folder name from the path
        void rmPath();
        // Get the extension from the filename in the path
        void getExt(DString& dest) const;
        // Replace the extension in the filename
        void setExt(const char *s);
        // Add extension to the filename
        void appendExt(const char *s);
        // Remove the extension
        void rmExt() {
            setExt("");
            if (last() == '.')
                truncate(-1);
            }
		// Return true if filename has an extension
		bool hasExt() {
			DString80 ext;
			getExt(ext);
			return !ext.isBlank();
			}
        void getRoot(DString& dest);
        void getDrive(DString& dest);
        // Change the slash type in the path (for example, from backslash to slash)
        void setSlashType(char c = cSysSlash);
        // Get the slash type for a filename (backslash or slash).
        // If the name does not contain a slash, returns the default slash type for
        // the platform (cSysSlash)
        char getSlashType();
		bool hasSlash();
        // Convert the path to a relative path.
        void makeRelativeTo(const char *baseDir);

        // Convert the path to an absolute path
        void makeAbsoluteFrom(const char *baseDir);
        void makeAbsoluteFrom(const char *baseDir, DFilename& tempBuffer);
		void combinePaths(const char *baseDir, const char *relative) {
			copy(relative);
			makeAbsoluteFrom(baseDir);
			}
        // Generate a temporary filename
        void makeTempName(const char *name = 0);
        // Remove redundant path elements (example, "/tmp/subfolder/../otherfolder/sample.doc"
        // would become "/tmp/otherfolder/sample.doc"
        void simplify(bool bPreserveTrailingSlash = false);
        // Split the path into a folder name and filename
        void split(DString& path, DString& tail);
        // Extract the path portion of a URL
        void getPathFromUrl(DString& dest);
        // Get current working directory
        void getCurrentDirectory();
        
    #if defined(_WIN32) && !defined(_WIN32_WCE)
        void make8_3();
    #endif
		// Returns true if the string is a URL
        int isUrl() {
            return isUrl(text);
            }
		// Returns true if the string is a UNC path
        int isUnc() const {
            return isUnc(text);
            }
        // Returns true if the string is a mapped drive
        int isMappedDrive() {
            return isMappedDrive(text);
            }
        // Returns true if the path is relative
		int isRelative() const {
			return isRelative(text);
			}
        // Returns true if the extension matches the extension of the filename
        int isType(const char *ext) const;


		void buildVirtualPath(const char *http, const char *serverUrl, const char *virtualPath, int fPathIsUrlEncoded = false);

        static const char *findTail(const char *fn, int fZeroLenOK);
        static int isType(const char *filename, const char *ext);
        static int isUnc(const char *s);
        static int isUnc(const wchar_t *s);
        static int isUrl(const char *s);
        static int isUrl(const wchar_t *s);
        static int isHttp(const char *s);
        static int isHttp(const wchar_t *s);
        static int isMappedDrive(const char *s);
        static int isMappedDrive(const wchar_t *s);
        static int isDotDot(const char *s);
        static int isDot(const char *s);
        static int isRelative(const char *s);
        static int isRelative(const wchar_t *s);

        static inline int isSlash(char c)
			{   return(c == '\\') || (c == '/');
			}

        static inline int isSlash(wchar_t c)
			{   return(c == '\\') || (c == '/');
			}
    };

class DUrl : public DFilename {
public:
    DUrl() { };
    DUrl(const DUrl& other) {
        storeUrl(other.text);
        }
    DUrl(const char *other) {
        storeUrl(other);
        }
    DUrl& operator=(const DUrl& other) {
        storeUrl(other.text);
        return *this;
        }
    DUrl& operator=(const char *other) {
        storeUrl(other);
        return *this;
        }
    operator const char *() const {
        return text;
        }
    void storeUrl(const char *s) {
        clear();
        append(s);
        setSlashType(cUnixSlash);
        }
    void removeInternalLink() {
        ptrdiff_t i = rfind('#');
        if (i > FAIL)
            truncate(i);
        }
    int isHttp() {
        return DFilename::isHttp(text);
        }
    int isHttps() {
		return !_strnicmp(text, "https://", 8);
		}
    void cleanup() {
		// make sure we have an HTTP:// in front and fix any backslashes

		if (!DFilename::isUrl())
			insert("http://", 0);
		replace('\\', '/');

		}
    int isValid();
    void makeAbsoluteUrl(const char *aBaseUrl);
    int split(DString& service, DString& server, DString& object, DString& port) const;
    int split(DString& service, DString& server, DString& object, DString& query, DString& port) const;
    int getServer(DString& server) {
		DString service, object, port;
		return split(service, server, object, port);
		}
    
    };


class DWStringFromUiString : public DWString {
    public:
        DWStringFromUiString(const char *s) :
            DWString()
            {   storeAnsiAsUnicode(s);
            }
        DWStringFromUiString(const wchar_t *s) :
            DWString()
            {   append(s);
            }
    };

class DUtf8StringFromUiString : public DString {
    public:
        DUtf8StringFromUiString(const char *s) {
            DWStringFromUiString ws = s;
            storeUnicodeAsUtf8(ws);
            }
        DUtf8StringFromUiString(const wchar_t *ws) {
            storeUnicodeAsUtf8(ws);
            }
        operator const char *() {
            return str();
            }
    };

class DUtf8StringFromAnsiString : public DString {
public:
	DUtf8StringFromAnsiString(const char *s) {
		DWString t;
		t.storeAnsiAsUnicode(s);
		storeUnicodeAsUtf8(t);
	}
};

#ifdef UNICODE
class CUiStringFromAnsi : public DWString {
    public:
        CUiStringFromAnsi(const char *s) {
            storeAnsiAsUnicode(s);
            }
    };

class CUiStringFromUtf8 : public DWString {
    public:
        CUiStringFromUtf8(const char *s) {
            storeUtf8AsUnicode(s);
            }
        operator const TCHAR *() {
            return text;
            }
    };

class CUiStringFromUnicode : public DWString {
    public:
        CUiStringFromUnicode(const wchar_t *s) {
            append(s);
        }
    };

#else
class CUiStringFromAnsi : public DString {
    public:
        CUiStringFromAnsi(const char *s) {
            append(s);
            }
    };

class CUiStringFromUtf8 : public DString {
    public:
        CUiStringFromUtf8(const char *s) {
            DWString ws = s;
            storeUnicodeAsAnsi(ws);
            }
        operator const TCHAR *() {
            return text;
            }
    };

class CUiStringFromUnicode : public DString {
    public:
        CUiStringFromUnicode(const wchar_t *s) {
            storeUnicodeAsAnsi(s);
        }
    };
#endif

class CAnsiStringFromUnicode : public DString {
    public:
        CAnsiStringFromUnicode(const wchar_t *s) {
            storeUnicodeAsAnsi(s);
        }
    };

#define UiToUtf8  DUtf8StringFromUiString
#define Utf8ToUi  CUiStringFromUtf8
#define Utf8ToUnicode DWString
#define WToUi     CUiStringFromUnicode
#ifdef _UNICODE
	#define UiToAnsi  CAnsiStringFromUnicode
#else
	#define UiToAnsi  DString
#endif
#define AnsiToUi  CUiStringFromAnsi

/* Simple html-based file for storing and reading option settings. */
class CHtmlSettingsString : public DString {
    public:
        CHtmlSettingsString() {
			iLastSetting = 0;
			};
        ~CHtmlSettingsString() { };
        void set(const char *name, const char *value) {
            makeBeginMark(begin, name);
            makeEndMark(end, name);
            DString settingText;
            settingText << begin << "\n" << value << "\n" << end;
            ptrdiff_t iStart = find(begin);
            if (iStart < 0) {
                *this << "\n" <<  begin << "\n" << "  " << end << "\n";
                iStart = find(begin);
                }
            ptrdiff_t iEnd = find(end, iStart + begin.getLength());
            if (iEnd < 0L)
                iEnd = getLength();
            else
                iEnd += end.getLength();

            remove(iStart, iEnd - iStart);
            insert(settingText, iStart);
            }
        void set(const char *name, int val) {
            DString tmp;
            tmp << val;
            set(name, tmp);
            }
        int get(const char *name, DString& dest) {
            makeBeginMark(begin, name);
            makeEndMark(end, name);
            ptrdiff_t iStart = find(begin, iLastSetting);
            if (iStart == FAIL)
				iStart = find(begin);
            ptrdiff_t iEnd = find(end, iStart+begin.getLength());
            if ((iStart < 0) || (iEnd < 0))
                return FAIL;
            iStart += begin.getLength();
            dest.clear();
            dest.store(str() + iStart, iEnd - iStart);
            dest.trim();
            iLastSetting = iEnd + end.getLength();
            return SUCCESS;
            }
        int get(const char *name, int& val) {
            int ret = get(name, gtmp);
            if (ret != FAIL)
                val = (int) gtmp.toInt();
            return ret;
            }
        void addVariable(const char *caption, const char *name, const char *value)
        {   DString tmp;
            // Only add the caption if the item does not already exist
            if ((get(name, tmp) == FAIL) && !strIsBlank(caption))
                *this << "<BR><HR><I>" << caption << ": </I>";
            set(name, value);
        }

        void addVariable(const char *caption, const char *name, char value)
        {   char tmp[2];
            tmp[0] = value;
            tmp[1] = '\0';
            addVariable(caption, name, tmp);
        }

        void addVariable(const char *caption, const char *name, int value)
        {   DString tmp;
            tmp << value;
            addVariable(caption, name, tmp);
        }


#if defined(MfcAvailable)
        void get(const char *name, CString& csDest) {
            DString tmp;
            get(name, tmp);
            csDest = Utf8ToUi(tmp);
            }
#endif
    protected:
		DString begin;
		DString end;
		DString gtmp;
		ptrdiff_t iLastSetting;
        static void makeBeginMark(DString& s, const char *fieldName) {
            s.clear();
            s << "<!-- $Begin " << fieldName << " -->";
            }
        static void makeEndMark(DString& s, const char * /* fieldName */) {
            s.clear();
            s << "<!-- $End -->";
            }

    };

	
#define _LongMaxPath 4096	

#ifdef _WIN32

class TCFilenameBuf {
	public:
		TCFilenameBuf() {
			buf = new TCHAR[_LongMaxPath];
			memset(buf, 0, sizeof(TCHAR) * _LongMaxPath);
			}
		~TCFilenameBuf() {
			delete [] buf;
			buf = 0;
			}
		operator TCHAR *() { return buf; }
		int size() {
			return _LongMaxPath-2;
			}
	protected:
		TCHAR *buf;
	};
	
class WFilenameBuf {
	public:
		WFilenameBuf() {
			buf = new wchar_t[_LongMaxPath];
			memset(buf, 0, sizeof(wchar_t) * _LongMaxPath);
			}
		~WFilenameBuf() {
			delete [] buf;
			buf = 0;
			}
		operator wchar_t *() { return buf; }
		int size() {
			return _LongMaxPath-2;
			}
	protected:
		wchar_t *buf;
	};
	
#endif

#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif

// To conditionally define logging away
// If dlogger.h is not included before or after dfile.h, debugLog() will map to debugLogIgnore()
void debugLogIgnore(const char *);
void debugLogIgnore(const wchar_t *);
void debugLogIgnore(const char *, __int64);
void debugLogIgnore(const wchar_t *, __int64);
void debugLogIgnore(const char *, const char *);
void debugLogIgnore(const char *, const char *, const char *);
void debugLogIgnore(const char *, const wchar_t *);
void debugLogIgnore(const char *, __int64, __int64);
void debugLogIgnore(const wchar_t *, const wchar_t *,const wchar_t *);


#ifndef HaveDebugLog
#ifdef debugLog
#undef debugLog
#endif
#define debugLog debugLogIgnore
#endif

#pragma warning(pop)
#endif
