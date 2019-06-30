#include <stdafx.h>

// Incorrect warning about thread-safe static init in system header system_error
// when compiling with VS 2015 Update 2
#pragma warning(disable:4640)

#if !defined(_MAC) && !defined(_ANDROID)
#include <fstream>
#endif

 //
//   Utility string classes for dtSearch Engine samples
//
#ifndef NeedStringFunctions
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#endif

#include <errno.h>

#if defined(_WIN32)
#include <direct.h>
#endif

#if defined(_WIN32) && defined(_WINDOWS_) && !defined( _OLEAUTO_H_ )
    #include <oleauto.h>
#endif

#ifndef DSTRING_H
    #include <dstring.h>
#endif

#ifndef DTSEARCH_H
    #include <dtsearch.h>
#endif







#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
namespace dtSearch {
#endif


int AnsiToUnicode(wchar_t *dest, const char *aSource, ptrdiff_t buffSize)
{   int nChars = 0;
    const unsigned char *source = (const unsigned char *) aSource;
    while(*source && (buffSize > 1)) {
        *dest = *source;
        dest++;
        source++;
        buffSize--;
        nChars++;
        }
    if (buffSize > 0)
        *dest = 0;
    return nChars;
}

int UnicodeToAnsi(char *aDest, const wchar_t *source, ptrdiff_t buffSize)
{   int nChars = 0;
    unsigned char *dest = (unsigned char *) aDest;
    while(*source && (buffSize > 1)) {
        unsigned char d = (unsigned char) (*source);
        *dest = d;
        dest++;
        source++;
        buffSize--;
        nChars++;
        }
    if (buffSize > 0)
        *dest = 0;
    return nChars;
}



const char cWindowsSlash = '\\';

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";


#ifdef NeedStringFunctions

    #ifdef isspace
        #undef isspace
    #endif
    #ifdef tolower
        #undef tolower
    #endif
    #ifdef toupper
        #undef toupper
    #endif



static int tolower(char c) {
    if ((c >= 'A') && (c <= 'Z'))
        return c + ('a' - 'A');
    else
        return c;
}

static int toupper(char c) {
    if ((c >= 'a') && (c <= 'z'))
        return c - ('a' - 'A');
    else
        return c;
}

char * strlwr(char *s)
{   while (s && *s) {
        *s = tolower(*s);
        s++;
    }
    return s;
}

char *strupr(char *s)
{   while (s && *s) {
        *s = toupper(*s);
        s++;
    }
    return s;
}




char *_i64toa(__int64 val, char *buf, int base){
    if (base == 10)
        sprintf(buf, "%lld", val);
    else if (base == 16)
        sprintf(buf, "%llx", val);
	return buf;
}

char *itoa(int i, char *dest, int radix)
{   return ltoa(i, dest, radix);
}

char *ltoa(long i, char *dest, int radix)
{   if (i < 0) {
        i = -i;
        *dest++ = '-';
    }

    char s[20];
    char *p = s + 19;
    *p-- = '\0';
    do {
        *p = digits[i % radix];
        p--;
        i /= radix;
    }
    while (i != 0);
    p++;
    strcpy(dest, p);
    return dest;
}



int stricmp(const char *a, const char *b)
{
    while (a && b && *a && *b) {
        int v = toupper(*a) - toupper(*b);
        if (v != 0)
            return v;
        a++;
        b++;
    }
    if (a && *a)
        return 1;
    else if (b && *b)
        return -1;
    else
        return 0;
}

int strnicmp(const char *a, const char *b, ptrdiff_t len)
{   if (!a || !b) {
        if (a)
            return 1;
        else if (b)
            return -1;
        else
            return 0;
        }
    while ((len > 0) && *a && *b) {
        int v = toupper(*a) - toupper(*b);
        if (v != 0)
            return v;
        a++;
        b++;
        len--;
    }
    if (len <= 0)
        return 0;
    else if (*a)
        return 1;
    else if (*b)
        return -1;
    else
        return 0;
}

#ifdef _ANDROID
#define	toupper(x)  AsciiToUpper(x)
#endif

int wcsicmp(const wchar_t *a, const wchar_t *b)
{   while (a && b && *a && *b) {
        int v = toupper(*a) - toupper(*b);
        if (v != 0)
            return v;
        a++;
        b++;
    }
    if (a && *a)
        return 1;
    else if (b && *b)
        return -1;
    else
        return 0;
}

int wcsnicmp(const wchar_t *a, const wchar_t *b, ptrdiff_t len)
{   if (!a || !b) {
        if (a)
            return 1;
        else if (b)
            return -1;
        else
            return 0;
        }
    while ((len > 0) && *a && *b) {
        int v = toupper(*a) - toupper(*b);
        if (v != 0)
            return v;
        a++;
        b++;
        len--;
    }
    if (len <= 0)
        return 0;
    else if (*a)
        return 1;
    else if (*b)
        return -1;
    else
        return 0;
}


#endif

char *uitoa(unsigned int i, char *dest, ptrdiff_t maxLen, unsigned int radix)
{	const int bufSize = 32;
    char s[bufSize];
    char *p = s + bufSize - 1;
    *p-- = '\0';
    do {
		unsigned int d = i % radix;
        *p = digits[d];
        p--;
        i /= radix;
    }
    while (i != 0);
		p++;
    strCopy(dest, p, maxLen);
    return dest;
}


#ifdef _IsSpace
    #undef _IsSpace
#endif


bool I32_to_A(int v, char *buf, int bufSize, int radix)
{	*buf = 0;
#ifdef HAVE_SAFE_ITOA
	return (_itoa_s(v, buf, bufSize, radix) == 0);
#else
	char t[80];
	ltoa(v, t, radix);
	strCopy(buf, t, bufSize);
	return true;
#endif
}

bool U32_to_A(unsigned int v, char *buf, int bufSize, int radix)
{	*buf = 0;
#ifdef HAVE_SAFE_ITOA
	return (_ultoa_s(v, buf, bufSize, radix) == 0);
#else
	char t[80];
	uitoa(v, t, sizeof t, radix);
	strCopy(buf, t, bufSize);
	return true;
#endif
}


bool U64_to_A(unsigned __int64 v, char *buf, int bufSize, int radix)
{	*buf = 0;
#ifdef HAVE_SAFE_I64TOA
	return (_ui64toa_s(v, buf, bufSize, radix) == 0);
#else
	char t[80];
#ifdef _WIN32
	_ui64toa(v, t, radix);
#else
	_i64toa(v, t, radix);
#endif
	strCopy(buf, t, bufSize);
	return true;
#endif
}

bool I64_to_A(__int64 v, char *buf, int bufSize, int radix)
{	*buf = 0;
#ifdef HAVE_SAFE_I64TOA
	return (_i64toa_s(v, buf, bufSize, radix) == 0);
#else
	char t[80];
	_i64toa(v, t, radix);
	strCopy(buf, t, bufSize);
	return true;
#endif
}

#if defined(_WIN32) && (_MSC_VER < 1400)

ptrdiff_t strnlen (const char *s, ptrdiff_t maxlen)
{
	const char *end = (const char *) memchr (s, '\0', maxlen);

	if (end)
		return (ptrdiff_t) (end - s);
	else
		return maxlen;
}
#endif

bool strFormatNumU32(unsigned int v, char *dest, ptrdiff_t maxDigits, char padChar, int radix)
{	char temp[64];
	if (maxDigits > 64)
		maxDigits = 64;
	U32_to_A(v, temp, sizeof temp, radix);
	ptrdiff_t len = strlen(temp);
	if (padChar && (len < maxDigits)) {
		memset(dest, padChar, maxDigits - len);
		memmove(dest + maxDigits-len, temp, len);
		dest[maxDigits] = '\0';
	}
	else
		strCopy(dest, temp, maxDigits+1);
	return len <= maxDigits;
}


bool strFormatNumI32(int v, char *dest, ptrdiff_t maxDigits, char padChar, int radix)
{	char temp[64];
	if (maxDigits > 64)
		maxDigits = 64;
	U32_to_A(v, temp, sizeof temp, radix);
	ptrdiff_t len = strlen(temp);
	if (padChar && (len < maxDigits)) {
		memset(dest, padChar, maxDigits - len);
		memmove(dest + maxDigits-len, temp, len);
		dest[maxDigits] = '\0';
	}
	else
		strCopy(dest, temp, maxDigits+1);
	return len <= maxDigits;
}

bool strFormatNum64(__int64 v, char *dest, ptrdiff_t maxDigits, char padChar, int radix)
{	char temp[64];
	if (maxDigits > 64)
		maxDigits = 64;
	I64_to_A(v, temp, sizeof temp, radix);
	ptrdiff_t len = strlen(temp);
	if (padChar && (len < maxDigits)) {
		memset(dest, padChar, maxDigits - len);
		memmove(dest + maxDigits-len, temp, len);
		dest[maxDigits] = '\0';
	}
	else
		strCopy(dest, temp, maxDigits+1);
	return len <= maxDigits;
}




char *stristr(const char *s, const char *sub)
{   if (!sub || !*sub || !s || !*s)
        return NULL;
    ptrdiff_t l = strlen(sub);
    while (*s) {
        if (!_strnicmp(s, sub, l))
            return(char *) s;
        s++;
    }
    return NULL;
}

const char *strrstr(const char *text, const char *sub)
{   if (!sub || !*sub || !text || !*text)
        return NULL;

    ptrdiff_t len = strlen(sub);
    for (const char *p = text + strlen(text) - len; p >= text; --p) {
        if (!strncmp(p, sub, len))
            return p;
    }
    return 0;
}

int wcsIsBlank(const wchar_t *p)
{   if (!p)
        return true;
    while (*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
    }
    return true;
}


void wcsmove(wchar_t *dest, const wchar_t *source, ptrdiff_t cch)
{   memmove(dest, source, cch * sizeof(wchar_t));
}

#ifndef NO_STREAMS
std::istream& operator>>(std::istream& in, DString& dest)
{   dest.clear();
    dest.getLine(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DString& s)
{   out << s.str();
    return out;
}

std::ostream& operator<<(std::ostream& out, const DWString& ws)
{   out.write((char *) ws.str(), ws.getLength() * (sizeof(wchar_t)));
    return out;
}

#endif

// If either of these is defined, use the UTF-8 conversion functions in
// ConvertUtf.c (from www.unicode.org) instead of the ones in the dtSearch Engine.
// This eliminates the only dependency in the dstring.cpp on the dtSearch Engine DLL.
#if defined(DSTRING_UTF8) || defined(NoUtf8Support) || defined(NO_DTSEARCH_LIB)

// If compiling with /clr, ConvertUtf.c must be compiled as C++
#ifdef __cplusplus_cli
#include <ConvertUTF.h>
#else
extern "C" {
#include <ConvertUTF.h> // from www.unicode.org
}
#endif


long dtssUtf8Encode(char *target, long targetLen, const UTF16 *source, long sourceLen, long flags)
{   UTF8 *targetStart = (UTF8 *) target;
    if (sourceLen < 0) {
		sourceLen = 0;
		while(source[sourceLen])
			sourceLen++;
        }
    if (sizeof(wchar_t) == 2) {
        ConversionResult ret = ConvertUTF16toUTF8(
            &source, source + sourceLen,
            &targetStart, targetStart + targetLen, lenientConversion);
        }
    else {
			const UTF32 *source32 = (const UTF32 *) source32;
            ConversionResult ret = ConvertUTF32toUTF8(
            &source32, source32+sourceLen,
            &targetStart, targetStart + targetLen, lenientConversion);
        }
    ptrdiff_t len = targetStart - (UTF8 *) target;
    if (len < targetLen)
        target[len] = '\0';
    return (long) len;
}

long dtssUtf8Decode(UTF16 *target, long targetLen, const char *source, long sourceLen, long flags)
{
    ptrdiff_t len = 0;
    if (sourceLen < 0)
        sourceLen = (long) strlen(source);
    const UTF8 *sourceUtf8 = (const UTF8 *) source;
    if (sizeof(wchar_t) == 2) {
        UTF16 *targetStart = target;
        ConversionResult ret = ConvertUTF8toUTF16(
            &sourceUtf8, sourceUtf8 + sourceLen,
            &targetStart, targetStart + targetLen, lenientConversion);
        len = targetStart - target;
        }
    else {
        UTF32 *targetStart = (UTF32 *) target;
        ConversionResult ret = ConvertUTF8toUTF32(
            &sourceUtf8, sourceUtf8+sourceLen,
            &targetStart, targetStart + targetLen, lenientConversion);
        len = targetStart - (UTF32 *) target;
        }
    if (len < targetLen)
        target[len] = '\0';
    return (long) len;
}

// In older versions of Visual C++, wchar_t is not a built-in type
#if defined(WCHAR_IS_16_BITS)  && (_MSC_VER >= 1300) && defined(_NATIVE_WCHAR_T_DEFINED) // dtconfig.h
long dtssUtf8Encode(char *target, long targetLen, const wchar_t *source, long sourceLen, long flags)
{	return dtssUtf8Encode(target, targetLen, (const UTF16 *) source, sourceLen, flags);
}

long dtssUtf8Decode(wchar_t *target, long targetLen, const char *source, long sourceLen, long flags)
{   return dtssUtf8Decode((UTF16 *)target, targetLen, source, sourceLen, flags);
}

#endif

#endif

//////////////////////////////////////////////////////////////////////
//
//   DString is an unlimited-length null-terminated string
//

DString::DString(ptrdiff_t aSize) :
text(0),
len(0),
maxLen(0),
bOwnsBuffer(false)
{   allocate(aSize);
}



DString::DString(const DString& other) :
text(0),
maxLen(0),
len(0),
bOwnsBuffer(false)
{   allocate(other.len);
    if (text) {
        len = maxLen;
        memmove(text, other.text, len);
        text[len] = '\0';
    }
}

void DString::allocate(ptrdiff_t aSize)
{	dealloc();

    maxLen = 0;
    text = new char[aSize+1];
    if (text) {
        bOwnsBuffer = true;
        *text = '\0';
        maxLen = aSize;
    }
}

DString::DString(const char *s) :
text(0),
len(0),
maxLen(0),
bOwnsBuffer(false)
{   if (!s)
        s = "";
	ptrdiff_t l = strlen(s);
    allocate(l);
	if (text)
		store(s, l);
}

// estimate maximum length of utf8 equivalent of a string
static ptrdiff_t Utf8Length(ptrdiff_t wchars)
{   return 5 * wchars + 12;
}

DString::DString(const wchar_t *s) :
text(0),
len(0),
maxLen(0),
bOwnsBuffer(false)
{   if (!s)
        s = L"";
    ptrdiff_t l = wcslen(s);
    allocate(Utf8Length(l));
    if (text) {
        memset(text, 0, maxLen+1);
        storeUnicodeAsUtf8(s, l);
    }
}


bool DString::isLowChars(const char *s, ptrdiff_t len)
{	if (len < 0)
		len = strlen(s);
	const unsigned char *us = (const unsigned char *) s;
	for (int i = 0; i < len; ++i) {
		unsigned char ch = *us++;
		if (ch > 127)
			return false;
		if (ch < 9 )
			return false;
		}
	return true;
}

bool DString::istartsWith(const char *s, bool bSkipSpaces) const
{	const char *p = text;
	if (bSkipSpaces) {
		while(*p && _IsSpace(*p))
			p++;
		}
	ptrdiff_t slen = strlen(s);
	return !_strnicmp(p, s, slen);
}

bool DString::endsWith(const char *s) {
	ptrdiff_t l = strlen(s);
	if (l > len)
		return false;
	return !strcmp(text + len - l, s);
}

// Static version
bool DString::endsWith(const char *s, const char *toFind)
{
	ptrdiff_t lenToFind = strlen(toFind);
	ptrdiff_t len = strlen(s);
	if (lenToFind > len)
		return false;
	return !strcmp(s + len - lenToFind, toFind);
}

bool DString::endsWith(const wchar_t *s, const wchar_t *toFind)
{
	ptrdiff_t lenToFind = wcslen(toFind);
	ptrdiff_t len = wcslen(s);
	if (lenToFind > len)
		return false;
	return !wcscmp(s + len - lenToFind, toFind);
}



// Called by storeUnicodeAsUtf8() when UTF-8 encoding is needed (as opposed to just
// simple copying of chars below 128)
bool DString::storeUnicodeAsUtf8_Encoding(const wchar_t *s, ptrdiff_t nChars)
{
    if (!tryExtend(5*nChars+12)) 
    	return false;
	dtssUtf8Encode(text, (long) maxLen+1, s, (long) nChars, 0);
	len = strlen(text);
	return true;
}

void DString::wcharToUtf8(char *dest, ptrdiff_t len, wchar_t ch)
{	dtssUtf8Encode(dest, (long) len, &ch, 1, 0);
}


// Called by appendUnicodeAsUtf8() when UTF-8 encoding is needed (as opposed to just
// simple copying of chars below 128)
bool DString::appendUnicodeAsUtf8_Encoding(const wchar_t *s, ptrdiff_t nChars)
{
    if (!tryExtend(len + 5*nChars+12)) 
    	return false;
	ptrdiff_t added = dtssUtf8Encode(text + len, (long) (maxLen+1-len), s, (long) nChars, 0);
	len = len + added;
	return true;
}


    		
// Called by storeUtf8AsUnicode() when UTF-8 decoding is needed (as opposed to just
// simple copying of chars below 128)
void DWString::storeUtf8AsUnicode_Decode(const char *utf8String, ptrdiff_t aLen) 
{	extend(aLen+1);
	// buffer size is maxLen + 1 for trailing null
	len = dtssUtf8Decode(text, (long) (maxLen+1), utf8String, (long) aLen, 0);
	text[len] = 0;
}



#ifdef _WIN32
void DString::storeUnicodeAsAnsi(const wchar_t *ws, ptrdiff_t chars)
{   if (!ws || !*ws) {
        clear();
        return;
    }

    if (chars < 0)
        chars = swcslen(ws);

    BOOL usedDefaultChar = 0;
    long ansiLen = 0;
    if (bOwnsBuffer) {
        ansiLen = WideCharToMultiByte(
                                     CP_ACP,        // code page
                                     0,             // performance and mapping flags
                                     ws,            // address of wide-character string
                                     (int) chars,         // number of characters in string
                                     NULL,          // address of buffer for new string
                                     0,             // size of buffer
                                     "$",           // address of default for unmappable characters
                                     &usedDefaultChar);
        if (ansiLen+4 > maxLen)
            extend(ansiLen+4);
    }
    ansiLen = WideCharToMultiByte(
                                 CP_ACP,        // code page
                                 0,             // performance and mapping flags
                                 ws,            // address of wide-character string
                                 (int) chars,         // number of characters in string
                                 text,          // address of buffer for new string
                                 (int) maxLen,             // size of buffer
                                 "$",           // address of default for unmappable characters
                                 &usedDefaultChar);
    text[ansiLen] = '\0';
    len = ansiLen;
}
#elif defined(__UNIX__)
void DString::storeUnicodeAsAnsi(const wchar_t *ws, ptrdiff_t chars)
{   if (!ws || !*ws) {
        clear();
        return;
    }

    if (chars < 0)
        chars = wcslen(ws);
    long ansiLen = 0;
    if (bOwnsBuffer)
        extend(chars+4);

    ansiLen = UnicodeToAnsi(text, ws, maxLen);
    if (ansiLen > maxLen)
        ansiLen = maxLen;
    text[ansiLen] = '\0';
    len = ansiLen;
}

#else

#error No method storeUnicodeAsAnsi

#endif

void DString::decodeUtf8ToUnicode(wchar_t *dest, ptrdiff_t destMaxLen)
{   dtssUtf8Decode(dest,(long) destMaxLen, text, (long) (len+1), 0);
}

wchar_t *DString::decodeUtf8ToUnicode()
{   wchar_t *buf = new wchar_t[len+1];
    decodeUtf8ToUnicode(buf, len+1);
    return buf;
}

// Store either ANSI or Unicode as UTF8, depending on type of TCHAR
bool DString::storeUiAsUtf8(const TCHAR *s, ptrdiff_t nChars)
{	clear();
	if (strIsBlank(s))
		return true;
#ifdef _UNICODE
	if (nChars < 0)
		nChars = _tcslen(s);
	return storeUnicodeAsUtf8(s, nChars);
#else
	if (nChars < 0)
		nChars = strlen(s);
	return storeAnsiAsUtf8(s, nChars);
#endif
}

bool DString::storeAnsiAsUtf8(const char *s, ptrdiff_t nChars) {
	clear();
	if (strIsBlank(s))
		return true;
	if (nChars < 0)
		nChars = strlen(s);
	DWString wtemp;
	wtemp.storeAnsiAsUnicode(s);

	return storeUnicodeAsUtf8(wtemp, wtemp.getLength());
}



DString::DString(char *aBuf, ptrdiff_t aBufLen, bool bKeeptext, bool bNoStrlen) :
text(aBuf),
maxLen(aBufLen-1),
len(0),
bOwnsBuffer(false)
{	// If len is -1, use strlen to compute size of text
	if ((aBufLen < 0) && text && bKeeptext) {
		len = strlen(text);
		maxLen = len;
		}
	// Attach to existing contents and keep contents
  	else if (bKeeptext && text) {
  		if (bNoStrlen) {
  			// Not necessarily null-terminated
  			len = aBufLen;
  			maxLen = aBufLen;
  			}
  		else
			len = strnlen(text, aBufLen);
		}
	// attach to existing contents and clear buffer
	else if (text && (maxLen > 0))
		text[0] = '\0';
	// Either null buffer or bad maxLen passed in, so make valid
	else {
		if (aBufLen < 512)
			aBufLen = 512;
		allocate(aBufLen);
		}
}

DString::DString(char *aBuf, ptrdiff_t aBufLen) :
text(aBuf),
maxLen(aBufLen-1),
len(0),
bOwnsBuffer(false)
{
	// attach to existing contents and clear buffer
	if (text && (maxLen > 0))
		text[0] = '\0';
	// Either null buffer or bad maxLen passed in, so make valid
	else {
		if (aBufLen < 512)
			aBufLen = 512;
		allocate(aBufLen);
		}
}


#ifndef NO_STREAMS

void DString::getLine(std::istream& in)
{   memset(text, 0, maxLen);
    in.getline(text, maxLen);
    len = strlen(text);
    while ((len > 0) && _IsSpace(text[len-1]))
        len--;
    text[len] = '\0';
}

#endif

DString& DString::operator=(const DString& other)
{	if (text != other.text)
	    binaryCopy(other);
    return *this;
}

void DString::binaryCopy(const DString& other)
{   if (this == &other)
        return;

    if (text == other.text)
        return;

    clear();
	if (!other.text || (other.getLength() <= 0))
		return;

    ptrdiff_t newLen = other.getLength();
    extend(newLen + 1);

	if (text && (maxLen >= newLen)) {
		memmove(text, other.text, newLen);
		text[newLen] = '\0';
		len = newLen;
		}
} 

DString& DString::operator=(const char *other)
{   // check for assignment to self
    if (other != text) {
	    clear();
		*this << other;
		}
    return *this;
}

DString& DString::operator=(const wchar_t *other)
{   clear();
    storeUnicodeAsUtf8(other);
    return *this;
}


void DString::toUpper()
{
#ifdef HAVE_SAFE_STRUPR
	_strupr_s(text, maxLen+1);
#else
	strupr(text);
#endif
}

void DString::toLower()
{
#ifdef HAVE_SAFE_STRLWR
	if (len > 0)
		_strlwr_s(text, maxLen+1);
#else
    strlwr(text);
#endif
}



ptrdiff_t DString::findFirstOf(const char *charList, ptrdiff_t offset) const
{	for (ptrdiff_t i = offset; i < len; ++i) {
		if (strchr(charList, text[i]))
			return i;
		}
	return -1;
}


ptrdiff_t DString::ifind(const char *s, ptrdiff_t offset) const
{   if (!s)
        return -1;
    ptrdiff_t l = strlen(s);
    if (!l)
        return -1;
    for (ptrdiff_t i = offset; i <= len - l; ++i) {
        if (!_strnicmp(s, text + i, l))
            return i;
    }
    return -1;
}

ptrdiff_t DString::find(char c) const
{   if (!c)
        return FAIL;
    const char *s = strchr(text, c);
    if (s)
        return s - text;
    else
        return FAIL;
}

ptrdiff_t DString::find(const char *s, ptrdiff_t offset) const
{   if (!s)
        return -1;
    ptrdiff_t l = strlen(s);
    if (!l)
        return -1;
    for (ptrdiff_t i = offset; i <= len - l; ++i) {
        if (!strncmp(s, text + i, l))
            return i;
    }
    return -1;
}

ptrdiff_t DString::rfind(char c) const
{   if (!c)
        return FAIL;
    for (ptrdiff_t i = len-1; i >= 0; --i) {
        if (text[i] == c)
            return i;
    }
    return -1;
}

ptrdiff_t DString::rfind(const char *s) const
{   const char *p = strrstr(text, s);
    if (p)
        return p - text;
    return -1;
}

void DString::padTo(ptrdiff_t l, char c)
{	extend(l+1);
    if (l >= maxLen)
        l = maxLen - 1;
    if (l <= len)
        return;

    memset(text + len, c, l - len);
    len = l;
    text[len] = 0;
}

void DString::padFrontTo(int l, char c)
{	extend(l+1);
	ptrdiff_t toAdd = l - len;
	if (toAdd < 0)
		return;
	memmove(text + toAdd, text, len);
	memset(text, c, toAdd);
	len += toAdd;
	text[len] = 0;

}


void DString::setLength(ptrdiff_t newLen)
{   if (newLen < 0)
        newLen = 0;

    if (newLen < maxLen)
        len = newLen;
    else
        len = maxLen;
    text[len] = '\0';
}

void DString::truncate(ptrdiff_t aLen)
{   if (aLen < 0) {
        if (aLen + len < 0)
            len = 0;
        else
			len += aLen;
        text[len] = 0;
    }

    else if ((aLen >= 0) && (aLen < len)) {
        len = aLen;
        text[len] = 0;
    }
}

void DString::truncateBeforeString(const char *s)
{	const char *pos = strstr(text, s);
	if (pos)
		truncate(pos - text);
}


void DString::truncateAtChar(char ch)
{   const char *pos = strchr(text, ch);
    if (pos)
        truncate(pos - text);
}


void DString::appendUrlEncoded(const char *p)
{   while (*p) {
        unsigned char c = * ((unsigned char *)p++);
        if ((c < 128) && isalnum(c))
            *this << c;
        else {
			*this << '%';
			*this << PadHexNum((int) c, 2);
			}
        }
    
}

void DString::urlEncode()
{   DString tmp = text;
    clear();
    appendUrlEncoded(tmp);
}

void DString::urlDecode(const char *source, ptrdiff_t nChars)
{	clear();
    if (nChars < 0)
		nChars = strlen(source);
	for (int i = 0; i < nChars; ++i) {
		char c = source[i];
		switch(c) {
			case '+':
				appendChar(' ');
				break;
			case '%': {
                if (nChars > i+2) {
	                char hex[3];
					hex[0] = source[i+1];
					hex[1] = source[i+2];
					hex[2] = '\0';
					if (Ascii_IsXDigit(hex[0]) && Ascii_IsXDigit(hex[1])) {
						int v = (int) strtol(hex, 0, 16);
						appendChar((char) v);
						i += 2;
						}
					else
						appendChar(c);
		            }
				else
					appendChar(c);
                break;
                }
            default:
                appendChar(c);
                break;
			}
		}
}

void DString::urlDecode()
{	if (!contains("%"))
		return;
	DString t;
	t.urlDecode(text, getLength());
	binaryCopy(t);
}


void DString::urlDecodeU8()
{
	bool bWasLowChars = strIsLowChars(text);
	urlDecode();
	if (!strIsLowChars(text)) {
		if (bWasLowChars) {
			DWString wTemp;
			wTemp.storeAnsiAsUnicode(*this);
			clear();
			storeUnicodeAsUtf8(wTemp);
			}
		}
}

void DString::urlEncodeFilename()
{   DString tmp = text;
    clear();
    appendUrlEncodedFilename(tmp);
}

static int isFilenameChar(char c)
{   return strchr("/\\:.-_?&=", c) != NULL;
}

void DString::appendUrlEncodedFilename(const char *p)
{   while (*p) {
        unsigned char c = * ((unsigned char *)p++);
        if ((c < 128) && (Ascii_IsAlnum(c) || isFilenameChar(c)))
            *this << c;
        else if (c >= 128)
			*this << c;
		else {
            char s[64];
            s[0] = '%';
            I32_to_A((int) c, s+1, (sizeof s)-1, 16);
            append(s);
        }
    }
}


void DString::htmlEncode()
{   DString tmp = *this;
    clear();
    appendHtmlEncoded(tmp);
}

void DString::appendHtmlEncoded(const char *s, bool bAllowTags)
{   while (*s) {
        unsigned char c = *((unsigned char *)s++);
        switch (c) {
            case '\r':
                break;
            case '\n':
				if (bAllowTags)
					*this << "<BR>";
				else
					*this << c;
                break;
            case '<':
                *this << "&lt;";
                break;
            case '>':
                *this << "&gt;";
                break;
            case '&':
                *this << "&amp;";
                break;
            case '\"':
                *this << "&quot;";
                break;
            case '\'':
				*this << "&#39;";
				break;
            default:
                if ((c >= 32))
                    *this << c;
                else {
                    *this << '&' << '#';
                    int code = c;
                    *this << code << ";";
                }
                break;
        }
    }
}

void DString::appendHexEncoded(const void *pData, ptrdiff_t bytes)
{   const unsigned char *pChar = (const unsigned char *) pData;
    int width = 0;
    while (bytes-- > 0) {
        unsigned char c = *pChar++;
        char hex[64];
		I32_to_A(c, hex, sizeof hex, 16);
        if (c < 16)
            append("0");
        append(hex);
        width++;
        if (width == 32) {
            append("\r\n");
            width = 0;
        }
    }
}

// Decode hex data into a string buffer
// Note that the buffer can contain NULL characters after this
void DString::hexDecode(const char *hexData)
{   clear();
    extend(strlen(hexData)/2);
    while (*hexData) {
        if (_IsSpace(*hexData))
            hexData++;
        else {
            char hex[3];
            hex[0] = *hexData++;
            hex[1] = *hexData++;
            hex[2] = '\0';
            int v = (int) strtol(hex, 0, 16);
            text[len++] = (char) v;
        }
    }
}

bool DString::tryExtend(ptrdiff_t newLen)
{   if (maxLen >= newLen)
        return true;

    char *newstr = new(std::nothrow) char[newLen+1];
    if (!newstr)
        return false;
    if (text)
        memmove(newstr, text, len+1);
    else
        *newstr = 0;

    dealloc();

    bOwnsBuffer = true;
    text = newstr;

    maxLen = newLen;
	return true;
}

void DString::extend(ptrdiff_t newLen)
{	if (!tryExtend(newLen)) 
        throw std::bad_alloc();
}


void DString::insert(const char *s, ptrdiff_t offset)
{   if (!s)
        return;
    ptrdiff_t l = strlen(s);
    if (!l)
        return;

    if (offset >= len) {
        *this << s;
        return;
    }
    extend(l + len + 1);
    if (l + len > maxLen)
        l = maxLen - len;
    memmove(text + offset + l, text + offset, len - offset);
    memmove(text + offset, s, l);
    len += l;
    text[len] = '\0';
}

void DString::remove(ptrdiff_t pos, ptrdiff_t count)
{   if ((pos < 0) || (pos >= len) || (count < 1))
        return;
    if (count + pos >= len) {
        text[pos] = '\0';
        len = pos;
    }
    else {
        memmove(text + pos, text + pos + count, len - (pos + count));
        len -= count;
        text[len] = '\0';
    }
}

void DString::rtwhite()
{	len = strlen(text);
    while ((len > 0) && _IsSpace(text[len-1]))
        len--;
    text[len] = '\0';
}

static bool isUtf8HardSpace(const char *s) {
	return ((unsigned char) s[0] == 0xc2)  && ((unsigned char) s[1] == 0xa0);
	}

void DString::rtwhiteUtf8()
{	len = strlen(text);
    while (len > 0) {
		if (_IsSpace(text[len-1]))
			len--;
		else if ((len > 1) && isUtf8HardSpace(text + len -2))
			len -= 2;
		else
			break;
		}
			
    text[len] = '\0';
}

void DString::rlwhite()
{   int frontSpaces = 0;
    const char *p = text;
    while (_IsSpace(*p++))
        frontSpaces++;

    if (frontSpaces > 0)
        remove(0, frontSpaces);
}

void DString::rlwhiteUtf8()
{   int frontSpaces = 0;
    const char *p = text;
    while(*p) {
		if (_IsSpace(*p)) {
			frontSpaces++;
			p++;
			}
		else if (isUtf8HardSpace(p)) {
			frontSpaces += 2;
			p += 2;
			}
		else
			break;
		}

    if (frontSpaces > 0)
        remove(0, frontSpaces);
}

void DString::trim()
{	rtwhite();
	rlwhite();
}

void DString::unQuote(char qchar)
{   if (text[0] == qchar)
        remove(0, 1);
    if ((len > 0) && (text[len-1] == qchar)) {
        len--;
        text[len] = '\0';
    }
}

void DString::unQuote(char cQuote1, char cQuote2)
{   if (text[0] == cQuote1)
        remove(0, 1);
    if ((len > 0) && (text[len-1] == cQuote2)) {
        len--;
        text[len] = '\0';
    }
}


void DString::updateLength()
{   len = strnlen(text, maxLen);
}

#pragma warning(disable: 4706) // assignment within conditional expression

void DString::packWhite(bool bRemoveLeading)
{   char c;
    bool prevSp = bRemoveLeading;
    char *source = text;
    char *dest = text;
    while ((c = *source++)) {
        if (!_IsSpace(c)) {
            *dest++ = c;
            prevSp = false;
        }
        else if (!prevSp) {
            prevSp = true;
            *dest++ = ' ';
        }
    }
    *dest = '\0';
    len = strlen(text);
}

void DWString::packWhite()
{   wchar_t c;
    bool prevSp = true;
    wchar_t *source = text;
    wchar_t *dest = text;
    while ((c = *source++)) {
        if (!_IsSpace(c)) {
            *dest++ = c;
            prevSp = false;
        }
        else if (!prevSp) {
            prevSp = true;
            *dest++ = ' ';
        }
    }
    *dest = '\0';
    len = (dest - text);
}

void DString::removeChar(char ch)
{	char *p = text;
	char *dest = text;
	ptrdiff_t newLen = len;
	for (int i = 0; i < len; ++i) {
		if (*p != ch) {
			*dest = *p;
			dest++;
			p++;
			}
		else {
			p++;
			newLen--;
			}
		}
	len = newLen;
	text[len] = 0;
}
			

// Replace one string with another (all occurances)
void DString::replace(const char *from, const char *to)
{	if (!to)
		to = "";
	if (!from)
		return;
   // Check for loops
    if (strstr(to, from) != NULL)
        return;

    char *p = text;
    ptrdiff_t l = strlen(from);
    int fSameLen = (strlen(from) == strlen(to));
    while ((p = strstr(p, from))) {
		if (fSameLen)
			memmove(p, to, l);
		else {
			ptrdiff_t i = p - text;
			remove(i, l);
			insert(to, i);
			p = text + i + 1;
			}
    }
}


void DString::ireplace(const char *from, const char *to)
{   // Check for loops
    if (stristr(to, from) != NULL)
        return;

    char *p;
    ptrdiff_t l = strlen(from);
    while ((p = stristr(text, from))) {
        ptrdiff_t i = p - text;
        remove(i, l);
        insert(to, i);
    }
}


DString& DString::operator<<(double v)
{	format(v, "%g");
	return *this;
}

DString& DString::operator<<(long l)
{   char s[64];
    I32_to_A((int) l, s, sizeof s, 10);
    append(s);
    return *this;
}

DString& DString::operator<<(__int64 i)
{
	char s[64];
	I64_to_A(i, s, sizeof s, 10);
	append(s);
	return *this;
}

DString& DString::operator<<(unsigned long l)
{   char s[64];
	U32_to_A((unsigned int)l, s, sizeof s, 10);
    append(s);
    return *this;
}

DString& DString::operator<<(int i)
{   *this << (long) i;
    return *this;
}

DString& DString::operator<<(unsigned __int64 v)
{	char s[64];

	U64_to_A(v, s, sizeof s, 10);
	append(s);
	return *this;
}

#ifndef NO_STREAMS
int DString::getFrom(std::istream& in, int bytes)
{
   if (bytes > 0) {
        if (!tryExtend(bytes+1))
			return FAIL;
		}
    char c = '\0';
    len = 0;
    in.get(c);
    while (in.good() && !in.eof()) {
        text[len++] = c;
        if (len >= maxLen)
            extend(maxLen * 2);
        if ((bytes > 0) && (len >= bytes))
			break;
        in.get(c);
	    }

    text[len] = '\0';
    // remove trailing nulls
    while ((len > 0) && (text[len-1] == '\0'))
        len--;
	return SUCCESS;
}
#endif

#if defined(DFILE_H)
void DString::writeToThrow(const char *utf8Fn, int fAppend)
{
	DFile out;
	int flags = (fAppend? F_ANY : F_ANY | F_TRUNC);

	if (out.openU8(utf8Fn, flags) == SUCCESS) {
		if (fAppend)
			out.seek(out.getLength());
		if (out.write(text, len) == SUCCESS)
			return;
	}
	out.throwOnError("writing text file");
}
#endif

int DString::writeToU8(const char *utf8Fn, int fAppend)
{
    int ret = FAIL;

#if defined(DFILE_H)
	DFile out;
	int flags = (fAppend? F_ANY : F_ANY | F_TRUNC);

	if (out.openU8(utf8Fn, flags) == SUCCESS) {
		if (fAppend)
			out.seek(out.getLength());
		out.write(text, len);
		ret = SUCCESS;
		}
	ret = out.getLastError();

#elif defined(_WIN32)
    HANDLE hFile;
    int flags = (fAppend? OPEN_ALWAYS : CREATE_ALWAYS);

    hFile = CreateFile(Utf8ToUi(utf8Fn), GENERIC_WRITE | GENERIC_READ, 0 /*no share */, 0, flags, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        unsigned long nWritten = 0;
        if (fAppend)
			SetFilePointer(hFile, 0, 0, SEEK_END);
        if (WriteFile(hFile, text, (DWORD) len, &nWritten, 0))
            ret = SUCCESS;
        CloseHandle(hFile);
    }
#else
    std::ofstream out(Utf8ToUi(utf8Fn), (fAppend? std::ios::out | std::ios::binary | std::ios::app : std::ios::out | std::ios::binary));
    if (out.good()) {
        out.write(text, len);
        ret = SUCCESS;
    }
#endif
    return ret;
}


int DString::getFromU8(const char *fn, __int64 maxToRead)
{
   clear();
#if defined(DFILE_H)
	DFile in;
	if (in.openU8(fn, F_READ | F_CONCURRENT) == SUCCESS) {
		DFilePosition bytes = in.getLength();
        if (maxToRead < 0)
            maxToRead = bytes;
        if (bytes > maxToRead)
            bytes = maxToRead;
		if (bytes > 1024*1024*1024)
			return FAIL;
		if (!tryExtend(ToPtrDiffT(bytes)))
			return FAIL;
		if (maxLen >= bytes) {
			in.read(text, bytes);
			text[bytes] = '\0';
            len = ToPtrDiffT(bytes);
			}
		return SUCCESS;
		}
		
#elif defined(_WIN32)
    HANDLE hFile;
    hFile = CreateFile(Utf8ToUi(fn), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytes = 0;
        DWORD dummy = 0, bytesRead = 0;
        bytes = ::GetFileSize(hFile, &dummy);
        if (maxToRead < 0)
            maxToRead = bytes;
        if (bytes > maxToRead)
            bytes = (DWORD) maxToRead;
        if (!tryExtend((ptrdiff_t) bytes+5))
			return FAIL;
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        ReadFile(hFile, text, bytes, &bytesRead, 0);
        // make sure buffer is null-terminated, even if the data is Unicode
        text[bytesRead] = '\0';
        text[bytesRead+1] = '\0';
        text[bytesRead+2] = '\0';
        text[bytesRead+3] = '\0';
        len = bytesRead;
        CloseHandle(hFile);
        return SUCCESS;
		}

#else
    std::ifstream in(Utf8ToUi(fn), std::ios_in);
    if (in.good()) {
        return getFrom(in, maxToRead);
        }
#endif

#ifdef DLOGGER_H
	debugLogLastError("Unable to read file", fn, "");
#endif
	return FAIL;
}

int DString::getFrom(const TCHAR *fn, __int64 maxToRead) {
	return getFromU8(UiToUtf8(fn), maxToRead);
	}
	
int DString::writeTo(const TCHAR *fn, int fAppend)
    {
	return writeToU8(UiToUtf8(fn), fAppend);
	}


//
//  Translate maps characters in from to the characters in to.
//  If to is blank, characters in from are stripped from the string.
//
void DString::translate(const char *from, const char *to)
{   if (*from && (!to || !*to)) {
        char *p = text;
        char *q = text;
        while (*p) {
            if (!strchr(from, *p))
                *q++ = *p;
            p++;
        }
        *q = 0;
        len = strlen(text);
        return;
    }
    while (*from && *to) {
        char *p = text;
        while (*p) {
            if (*p == *from)
                *p = *to;
            p++;
        }
        from++;
        to++;
    }
}

long DString::toInt()
{   return atol(text);
}


#if defined(_WIN32) || defined(_WINDOWS_)

#ifdef BSTR_DEFINED
BSTR DString::allocSysString() const
{
    DWString ws = text;
    BSTR bstr = ::SysAllocStringLen(ws, ToInt(ws.getLength()));
    return bstr;
}
#endif

#endif

#if defined(VS2005)
System::String ^DString::allocNetString() const {
	ptrdiff_t l = len;
	if (text[0] && (l == 0))
		l = strlen(text);
	DWString wtemp;
	if (!wtemp.tryExtend(l))
		return nullptr;
	wtemp.storeUtf8AsUnicode(text, l);
	return System::Runtime::InteropServices::Marshal::PtrToStringUni(
		safe_cast<System::IntPtr>(wtemp.str()), (int) wtemp.getLength());
	}

System::String ^DString::allocNetString(const char *text) {
	DWString wtemp;
	ptrdiff_t len = strlen(text);
	if (!wtemp.tryExtend(len))
		return nullptr;
	wtemp.storeUtf8AsUnicode(text);
	return System::Runtime::InteropServices::Marshal::PtrToStringUni(
		safe_cast<System::IntPtr>(wtemp.str()), (int) wtemp.getLength());
	}
#elif defined(VS2003)
System::String *DString::allocNetString() const {
	int l = len;
	if (text[0] && (l == 0))
		l = strlen(text);
	DWString wtemp;
	wtemp.storeUtf8AsUnicode(text, l);
	return System::Runtime::InteropServices::Marshal::PtrToStringUni(
		wtemp.str(), wtemp.getLength());
	}

System::String *DString::allocNetString(const char *text) {
	DWString wtemp;
	wtemp.storeUtf8AsUnicode(text);
	return System::Runtime::InteropServices::Marshal::PtrToStringUni(
		wtemp.str(), wtemp.getLength());
	}
#endif


void DString::hex(long v)
{   char buf[80];
    I32_to_A((int)v, buf, sizeof buf, 16);
    append(buf);
}

void DString::hex(__int64 v)
{   char buf[80];
    I64_to_A(v, buf, sizeof buf, 16);
    append(buf);
}

void DString::hex(unsigned __int64 v)
{   char buf[80];
    U64_to_A(v, buf, sizeof buf, 16);
    append(buf);
}

void DString::hex(unsigned long v)
{	char buf[80];
	U32_to_A((unsigned int)v, buf, sizeof buf, 16);
	append(buf);
}

// Format a date as YYYY/MM/DD
void DString::ymd(int y, int m, int d, char sep)
{	decpad(y, 4);
	appendChar(sep);
	decpad(m, 2);
	appendChar(sep);
	decpad(d, 2);
}

// Format a time as HHHH/MM/SS
void DString::hms(int h, int m, int s)
{	decpad(h, 2);
	appendChar(':');
	decpad(m, 2);
	appendChar(':');
	decpad(s, 2);
}

void DString::decpad(int v, int nDigits)
{	if (nDigits < 64) {
		char s[64];
		strFormatNumI32(v, s, nDigits, '0', 10);
		append(s);
	}
}

NumImage::NumImage(double v, const char *fmt)
{
	if (!fmt)
		fmt = "%g";
	#ifdef HAVE_SAFE_SPRINTF
	_snprintf_s(text, sizeof text-1, _TRUNCATE, fmt, v);
	#else
	_snprintf(text, sizeof text-1, fmt, v);
	text[sizeof text - 1] = 0;
#endif
}


// Append a floating-point value to a string
void DString::format(double v, const char *fmt)
{	NumImage n(v, fmt);
	append(n);
}

static const char *DeviceNames = 
	"CON|PRN|AUX|NUL|COM1|COM2|COM3|COM4|COM5|COM6|COM7|COM8|COM9|LPT1|LPT2|LPT3|LPT4|LPT5|LPT6|LPT7|LPT8|LPT9|"	;

static bool isDeviceName(const char *n)
{	char toFind[7];
	memset(toFind, 0, sizeof toFind);
	strCopy(toFind, n, sizeof toFind);
	if (toFind[3] == '.')
		toFind[3] = 0;
	if (toFind[4] == '.')
		toFind[4] = 0;
	ptrdiff_t len = strlen(toFind);
	if (len < 3)
		return false;
	if (len > 4)
		return false;
	Ascii_strupr(toFind);
	toFind[len] = '|';
	toFind[len+1] = 0;
	if (strstr(DeviceNames, toFind))
		return true;
	return false;
}	

void DString::makeLegalFilename()
{	// Do not turn a blank or empty filename into a nonblank filename
	if (strIsBlank(text))
		return;
		
	replace('{', '[');
	replace('}', ']');
	replace('<', '[');
	replace('>', ']');
	replace('\"', '\'');
	replace('^', '-');
	translate(IllegalFilenameChars, "_________________");
	for (int i = 0; i < len; ++i) {
	char ch = text[i];
	if ((ch > 0) && (ch < 32))
		text[i] = '_';
	}
	trim();
	packWhite();
	// Prevent name ending in .
	while((getLength() > 0) && (last() == '.')) {
		truncate(-1);
		trim();
		}
	if (isDeviceName(text) || strIsBlank(text))
		insert("_", 0);	
	
}




static void appendWithCommas(DString& str, const char *tmp, int grouping, const char *comma)
{	if (!comma)
		comma = ",";
	ptrdiff_t numlen = strlen(tmp);
	for (int i = 0; i < numlen; ++i) {
		str.appendCharNT(tmp[i]);
		ptrdiff_t digitsLeft = numlen - i - 1;
		if ((digitsLeft > (grouping-1)) && (digitsLeft % grouping == 0))
			str.appendStrNT(comma);
		}
	str.nullTerminate();
}

void DString::commaNum(__int64 val, const char *comma)
{
	if (val < 0) {
        appendChar('-');
        val = -val;
	    }

 	char tmp[80];
	I64_to_A(val, tmp, sizeof tmp, 10);

	appendWithCommas(*this, tmp, 3, comma);
}

void DString::commaNum(int val, const char *comma)
{
    if (val < 0) {
        appendCharNT('-');
        val = -val;
	    }

 	char tmp[80];
	I32_to_A(val, tmp, sizeof tmp, 10);

	appendWithCommas(*this, tmp, 3, comma);
}

void DString::commaNum(unsigned int val, const char *comma)
{

 	char tmp[80];
	U32_to_A(val, tmp, sizeof tmp, 10);

	appendWithCommas(*this, tmp, 3, comma);
}

void DString::hex(__int64 v, const char *comma)
{
	if (v < 0) {
        appendChar('-');
        v = -v;
	    }

 	char tmp[80];
	I64_to_A(v, tmp, sizeof tmp, 16);
	appendWithCommas(*this, tmp, 4, comma);
}


// Convert encoding from Utf8 to Ansi
void DString::utf8ToAnsi()
{
	DWString w;
	w.storeUtf8AsUnicode(text, len);
	clear();
	storeUnicodeAsAnsi(w, w.getLength());
}

// Convert encoding from Ansi to Utf8
void DString::ansiToUtf8()
{	DWString w;
	w.storeAnsiAsUnicode(text);
	clear();
	storeUnicodeAsUtf8(w, w.getLength());
}

DWString::DWString() :
text(0), len(0), maxLen(0)
{   allocate(1024);
}

DWString::DWString(long aLen):
text(0), len(0), maxLen(0) 
{   if (aLen < 2)
        aLen = 2;
    allocate(aLen);
}

DWString::DWString(const char *utf8Text) :
text(0), len(0), maxLen(0) 
{   if (!utf8Text)
        utf8Text = "";
    allocate(strlen(utf8Text) + 1);
    dtssUtf8Decode(text, (long) (maxLen+1), utf8Text, -1, 0);
    len = wcslen(text);
}

DWString::DWString(const wchar_t *ws) :
text(0), len(0), maxLen(0)
{   if (ws)
        append(ws);
    if (!text)
        allocate(1024);
}

DWString::DWString(const DWString& other) :
text(0), len(0), maxLen(0)
{   append(other.text);
    if (!text)
        allocate(1024);
}


DWString::~DWString()
{
}

#if defined(BSTR_DEFINED)

BSTR DWString::allocSysString() const
{
    BSTR bstr = ::SysAllocStringLen(text, (int) len);
    return bstr;
}

#endif

void DWString::setLength(ptrdiff_t newLen)
{   if (newLen < 0)
        newLen = 0;

    if (newLen < maxLen)
        len = newLen;
    else
        len = maxLen;
    text[len] = '\0';
}

void DWString::allocate(ptrdiff_t aLen)
{	m_buf.allocate(aLen+1);
	updateBuffer();
	clear();
};

ptrdiff_t DWString::find(wchar_t ch, ptrdiff_t offset) const
{   for (ptrdiff_t i = offset; i < len; ++i) {
        if (text[i] == ch)
            return i;
    }
    return -1;
}



ptrdiff_t DWString::ifind(const wchar_t *s, ptrdiff_t offset) const
{   if (!s)
        return -1;
    ptrdiff_t l = wcslen(s);
    if (!l)
        return -1;
    for (ptrdiff_t i = offset; i <= len - l; ++i) {
        if (!_wcsnicmp(s, text + i, l))
            return i;
    }
    return -1;
}

ptrdiff_t DWString::find(const wchar_t *s, ptrdiff_t offset) const
{   if (!s)
        return -1;
    ptrdiff_t l = wcslen(s);
    if (!l)
        return -1;
    for (ptrdiff_t i = offset; i <= len - l; ++i) {
        if (!wcsncmp(s, text + i, l))
            return i;
    }
    return -1;
}


#ifdef _WIN32
void DWString::storeAnsiAsUnicode(const char *ansiText)
{    clear();

    if (!ansiText || !*ansiText) {
        return;
    }
    ptrdiff_t ansiLen = strlen(ansiText);
    long l = MultiByteToWideChar(
                                CP_ACP,     // code page
                                0,          // character type options
                                ansiText,   // string to map
                                ToInt(ansiLen),    // number of bytes in string
                                NULL,       // address of wide-character buffer
                                0);         // size of buffer
    extend(l + len);
    MultiByteToWideChar(
                       CP_ACP,     // code page
                       0,          // character type options
                       ansiText,   // string to map
                       ToInt(ansiLen),    // number of bytes in string
                       text + len,         // address of wide-character buffer
                       ToInt(maxLen - len));  // size of buffer
    len += l;
    text[len] = '\0';
}

#else
void DWString::storeAnsiAsUnicode(const char *ansiText)
{   clear();

    if (!ansiText || !*ansiText) {
        return;
    }
    long ansiLen = strlen(ansiText);
    extend(len + ansiLen + 1);
    long l = AnsiToUnicode(text+len, ansiText, maxLen);
    len += l;
    text[len] = '\0';
}
#endif


bool DWString::store(const wchar_t *source, ptrdiff_t cch)
{   if (!source)
        return false;
	if (cch < 0)
		cch = swcslen(source);

    ptrdiff_t newLen = len + cch;
    if (newLen > maxLen) {
        if (!tryExtend(len + cch + maxLen))
			return false;
		}
    if (newLen <= maxLen) {
		wchar_t *dest = text + len;
		wcsmove(dest, source, cch);
		len += cch;
	    text[len] = '\0';
	    }
	return true;
}

void DWString::extend(ptrdiff_t newLen)
{
   if (maxLen > newLen)
        return;

	if (m_buf.tryExtend(newLen+1, len))
		updateBuffer();
	else
		throw std::bad_alloc();
}

bool DWString::tryExtend(ptrdiff_t newLen)
{   if (maxLen >= newLen)
        return true;

    if (!m_buf.tryExtend(newLen+1, len))
    	return false;
    updateBuffer();
	return true;
}

void DWString::toLower()
{
#if defined(HAVE_SAFE_STRLWR)
	_wcslwr_s(text, len+1);
#elif defined(_WIN32)
	_wcslwr(text+1);
#else
	for (int i = 0; i < len; ++i) {
		if (iswalpha(text[i]))
			text[i] = towlower(text[i]);
	}

#endif	
}

void DWString::toUpper()
{
#if defined(HAVE_SAFE_STRUPR)
	_wcsupr_s(text, len);
#elif defined(_WIN32)
	_wcsupr(text);
#else
	for (int i = 0; i < len; ++i) {
		if (iswalpha(text[i]))
			text[i] = towupper(text[i]);
	}
#endif	
}


void DWString::replace(wchar_t from, wchar_t to)
{   for (int i = 0; i < len; ++i) {
        if (text[i] == from)
            text[i] = to;
    }
}

// Replace one string with another (all occurances)
void DWString::replace(const wchar_t *from, const wchar_t *to)
{   // Check for loops
    if (wcsstr(to, from) != NULL)
        return;

    wchar_t *p;
    ptrdiff_t l = wcslen(from);
    while ((p = wcsstr(text, from))) {
        ptrdiff_t i = p - text;
        remove(i, l);
        insert(to, i);
    }
}


void DWString::insert(const wchar_t *s, ptrdiff_t offset)
{   if (!s)
        return;
    ptrdiff_t l = wcslen(s);
    if (!l)
        return;

    if (offset >= len) {
        *this << s;
        return;
    }
    extend(l + len + 1);
    if (l + len > maxLen)
        l = maxLen - len;
    wcsmove(text + offset + l, text + offset, len - offset);
    wcsmove(text + offset, s, l);
    len += l;
    text[len] = '\0';
}



void DWString::remove(ptrdiff_t pos, ptrdiff_t count)
{   if ((pos < 0) || (pos >= len) || (count < 1))
        return;
    if (count + pos >= len) {
        text[pos] = '\0';
        len = pos;
    }
    else {
        wcsmove(text + pos, text + pos + count, len - (pos + count));
        len -= count;
        text[len] = '\0';
    }
}

void DWString::rtwhite()
{	len = wcslen(text);
    while ((len > 0) && _IsSpace(text[len-1]))
        len--;
    text[len] = '\0';
}

void DWString::rlwhite()
{	len = wcslen(text);
    int frontSpaces = 0;
    const wchar_t *p = text;
    while (_IsSpace(*p++))
        frontSpaces++;

    if (frontSpaces > 0)
        remove(0, frontSpaces);
}

void DWString::trim()
{   rlwhite();
	rtwhite();
}




void DWString::truncate(ptrdiff_t aLen)
{   if (aLen < 0) {
        if (aLen + len < 0)
            aLen = len;
        len += aLen;
        text[len] = 0;
    }

    else if ((aLen >= 0) && (aLen < len)) {
        len = aLen;
        text[len] = 0;
    }
}

//
//  Translate maps characters in from to the characters in to.
//  If to is blank, characters in from are stripped from the string.
//
void DWString::translate(const wchar_t *from, const wchar_t *to)
{   if (*from && (!to || !*to)) {
        wchar_t *p = text;
        wchar_t *q = text;
        while (*p) {
            if (!wcschr(from, *p))
                *q++ = *p;
            p++;
        }
        *q = 0;
        len = wcslen(text);
        return;
    }
    while (*from && *to) {
        wchar_t *p = text;
        while (*p) {
            if (*p == *from)
                *p = *to;
            p++;
        }
        from++;
        to++;
    }
}

void DWString::translateCh(wchar_t from, wchar_t to)
{   if (from && !to) {
        wchar_t *p = text;
        wchar_t *q = text;
        while (*p) {
            if (*p != from)
                *q++ = *p;
            p++;
        }
        *q = 0;
        len = wcslen(text);
        return;
    }
    else {
        wchar_t *p = text;
        while (*p) {
            if (*p == from)
                *p = to;
            p++;
        }
    }
}


void DWString::storeUcs16AsUnicode(const unsigned short *ucs16, ptrdiff_t nChars)
{   clear();
    while(nChars-- > 0) {
        *this << (wchar_t) *ucs16++;
        }
}

void DWString::invertByteOrder() {
	for (int i = 0; i < len; ++i) {
		wchar_t c = text[i];
		int lo = LOBYTE(c);
		int hi = HIBYTE(c);
		c = MAKEWORD(hi, lo);
		text[i] = c;
		}
}




DWString& DWString::operator<<(long i)
{   char s[64];
    I32_to_A((int)i, s, sizeof s, 10);
    wchar_t ws[20];
    for (int c = 0; (c == 0) || s[c-1]; ++c)
        ws[c] = s[c];
    append(ws);
    return *this;
}

DWString& DWString::operator<<(int i)
{   *this << (long) i;
    return *this;
}

bool DWString::isBlank() const
{   if (!this)
        return true;
    if (!text)
        return true;

    const wchar_t *p = text;
    while (*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
    }
    return true;
}


bool DWString::isBlankUnicode() const
{   if (!this)
        return true;
    if (!text)
        return true;

    const wchar_t *p = text;
    while (*p) {
		wchar_t ch = *p;
        if (!_IsSpace(ch) && !IsUnicodeSpace(ch))
            return false;
        p++;
    }
    return true;
}




#ifdef _WIN32
static const char *UnicodeByteOrderHeader = "\xff\xfe";
// Assumes that wchar_t is 16-bits, which is true under Windows but not GCC
int DWString::writeTo(const TCHAR *fn)
{
    int ret = FAIL;

    HANDLE hFile;
    hFile = CreateFile(fn, GENERIC_WRITE | GENERIC_READ, 0 /*no share */, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        unsigned long nWritten = 0;
        WriteFile(hFile, UnicodeByteOrderHeader, 2, &nWritten, 0);
        nWritten = 0;
        if (WriteFile(hFile, text, ToInt(len * 2), &nWritten, 0))
            ret = SUCCESS;
        CloseHandle(hFile);
    }
    return ret;
}
#endif



void DWString::getFrom(const TCHAR *fn, long maxToRead)
{   clear();
#ifdef _WIN32
    HANDLE hFile;
    hFile = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        long bytes = 0;
        DWORD dummy = 0, bytesRead = 0;
        bytes = GetFileSize(hFile, &dummy);
        bytes -= 2;  // skip Unicode byte order header
        int chars = bytes/2;
        if (maxToRead < 0)
            maxToRead = bytes;
        if (bytes > maxToRead)
            bytes = maxToRead;
        extend(chars + 1);
        wchar_t header = 0;

        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        ReadFile(hFile, &header, sizeof(wchar_t), &bytesRead, 0);
        if ((header == 0xfffe) || (header == 0xfeff)) {
			SetFilePointer(hFile, 2, 0, FILE_BEGIN);
			ReadFile(hFile, text, bytes, &bytesRead, 0);
			int charsRead = bytesRead/2;
			text[charsRead] = '\0';
			len = charsRead;
			}
        CloseHandle(hFile);
    }
#else
	// Read UCS-16
	clear();
	DString tmp;
	tmp.getFrom(fn);
	const unsigned short *p = (const unsigned short *) tmp.str();
	if (*p == 0xfeff)
		storeUcs16AsUnicode(p+1, tmp.getLength()/2 - 1);

#endif
}

void DFilename::rmQuery()
{	ptrdiff_t iQuery = find('?');
	if (iQuery > FAIL)
		truncate(iQuery);
}

void DFilename::rmTail()
{   if (!len)
        return;
    for (ptrdiff_t i = len - 1; i >= 0; --i) {
        if (isSlash(text[i])) {
            if ((i == 2) && (text[1] == ':')) {
				text[i+1] = 0;
				len = i+1;
				}
			else {
				text[i] = 0;
				len = i;
				}
            return;
        }
    }
}

void DFilename::rmPath()
{   if (!len)
        return;
    for (ptrdiff_t i = len - 1; i >= 0; --i) {
        if ((text[i] == cSysSlash) || (text[i] == cUnixSlash)) {
            len = strlen(text + i + 1);
            memmove(text, text + i + 1, len);
            text[len] = 0;
            return;
        }
    }
}

void DFilename::getTail(DString& dest)
{   if (!len) {
        dest.clear();
        return;
    }

    for (ptrdiff_t i = len - 1; i >= 0; --i) {
        if ((text[i] == cSysSlash) || (text[i] == cUnixSlash)) {
            dest.clear();
            dest << (text + i + 1);
            return;
        }
    }
    // If there is no backslash in the name, copy the whole thing
    dest = *this;
}

const char *DFilename::findTail(const char *fn, int fZeroLenOK)
{   if (!fn || !*fn)
        return 0;
    ptrdiff_t last = strlen(fn)-1;
    if (!fZeroLenOK)
        last--;
    if (last < 0)
        return 0;

    for (ptrdiff_t i = last; i >= 0; --i) {
        if ((fn[i] == cSysSlash) || (fn[i] == cUnixSlash))
            return fn + i + 1;
    }
    return fn;
}

void DFilename::addTail(const char *s)
{   char slash = cSysSlash;
	if (isMappedDrive() || isUnc())
		slash = cWindowsSlash;
    else if (isUrl())
        slash = '/';
    if (!isSlash(*s) &&  !isSlash(last()) && (len > 0))
        *this << slash;
    *this << s;
}


int DFilename::isType(const char *filename, const char *ext)
{   if (!ext)
        ext = "";
    if (*ext == '.')
        ext++;

    ptrdiff_t extLen = strlen(ext);
    ptrdiff_t filenamelen = strlen(filename);
    if (filenamelen < extLen+1)
        return false;
    filename += (filenamelen - extLen-1);
    if (*filename != '.')
        return false;
    filename++;
    if (Ascii_stricmp(filename, ext))
        return false;
    return true;
}


int DFilename::isType(const char *ext) const
{
    if (!ext)
        ext = "";
    DString tmp;
    getExt(tmp);
    if (*ext == '.')
        ext++;
    return !Ascii_stricmp(ext, tmp);
}

// In theory an extension can be any length on some file systems,
// but a too-long extension or an "extension" that contains spaces or 
// sentence punctuation is likely to be a misinterpretation of a long filename
// that contains a period
const int maxExtToTruncate = 12;
#define ExtensionDisallowedChars " \\/?\"\'<>,:;"

void DFilename::appendExt(const char *ext)
{	
	if (*ext != '.')	
		*this << '.';
	*this << ext;
}

void DFilename::getExt(DString& dest) const
{   dest.clear();
    for (ptrdiff_t i = len - 1; (i >= len-maxExtToTruncate) && (i >= 0); --i) {
        if (text[i] == '.') {
            dest << (text + i + 1);
            return;
        }
        else if (strchr(ExtensionDisallowedChars, text[i]))
            return;
    }
}

void DFilename::setExt(const char *ext)
{   if (!len)
        return;
    if (*ext == '.')
        ext++;
    for (ptrdiff_t i = len - 1; (i >= len - maxExtToTruncate) && (i >= 0) && !strchr(ExtensionDisallowedChars, text[i]); --i) {
        if (text[i] == '.') {
            truncate(i+1);
            *this << ext;
            return;
        }
    }
    if (*ext)
		*this << '.' << ext;
}


void DFilename::getPath(DString& dest)
{   for (ptrdiff_t i = len - 1; i >= 0; --i) {
        if ((text[i] == cSysSlash) || (text[i] == cUnixSlash)) {
            dest.clear();
            // Location of /xx is /, not blank string
            if (i == 0)
                i++;
            // Location of http:// is http://, not http:/
            else if ((i > 1) && !strncmp(text+i-2, "://", 3))
				i++;
            dest.store(*this, i);
            return;
        }
    }
    dest.clear();
}

void DFilename::split(DString& dir, DString& tail)
{   getTail(tail);
	getPath(dir);
}

void DFilename::getCurrentDirectory()
{	
	clear();
#ifdef _WIN32	
	TCFilenameBuf path;
	GetCurrentDirectory(path.size(), path);
	append(UiToUtf8(path));
#else
	::getcwd(text, maxLen);
#endif
}

void DFilename::makeTempName(const char *name)
{   clear();

	DString tempFolder;
#ifdef HaveGetOptionTempDir
    GetOptionTempDirU8(tempFolder);
#endif     

#ifdef _WIN32
	if (tempFolder.isBlank()) {
		TCFilenameBuf path;
		GetTempPath(path.size(), path);
		tempFolder = UiToUtf8(path);
		}
	if (tempFolder.isBlank())
		tempFolder = "c:\\";
#endif
	
#ifdef __UNIX__
	if (tempFolder.isBlank()) {
		tempFolder = getenv("TEMP");
		if (tempFolder.isBlank())
			tempFolder = "/tmp";
		}
#endif
    *this = tempFolder;
    if (!strIsBlank(name))
        addTail(name);
}

// UNC filename = \\drive\sharename
int DFilename::isUnc(const char *fn)
{   if (!fn || !*fn || (strlen(fn) < 4))
        return false;

    if ((fn[0] != cSysSlash) || (fn[1] != cSysSlash))
        return false;
    if (fn[2] == cSysSlash)
        return false;
    if (!strchr(fn+2, cSysSlash))
        return false;

    return true;
}

int DFilename::isUnc(const wchar_t *fn)
{   if (!fn || !*fn || (wcslen(fn) < 4))
        return false;

    if ((fn[0] != cSysSlash) || (fn[1] != cSysSlash))
        return false;
    if (fn[2] == cSysSlash)
        return false;
    if (!wcschr(fn+2, cSysSlash))
        return false;

    return true;
}

void DFilename::setSlashType(char c)
{   for (int i = 0; i < len; ++i) {
        if ((text[i] == '\\') || (text[i] == '/'))
            text[i] = c;
    }
}

char DFilename::getSlashType()
{
	for (int i = 0; i < len; ++i) {
        if ((text[i] == '\\') || (text[i] == '/'))
        	return text[i];
		}
	return cSysSlash;
}

bool DFilename::hasSlash()
{
	if (contains('\\') || contains('/'))
		return true;
	return false;
}


int DFilename::isUrl(const char *s)
{	if (!s || !*s)
		return false;
	while (*s) {
		unsigned char ch = (unsigned char) *s++;
		if (!isalpha(ch)) {
			if ((ch == ':') && !strncmp(s, "//", 2))
				return true;
			return false;
			}
		}
	return false;
}

int DFilename::isUrl(const wchar_t *s)
{	if (!s || !*s)
		return false;
	while (*s) {
		wchar_t wch = (unsigned char) *s++;
		if (wch > 127)
			return false;
		char ch = (char) wch;
		if (!isalpha(ch)) {
			if ((ch == ':') && !wcsncmp(s, L"//", 2))
				return true;
			return false;
			}
		}
	return false;
}

int DFilename::isHttp(const char *s)
{   return s && (strlen(s) > 7) && (!_strnicmp(s, "http://", 7) || !_strnicmp(s, "https://", 8));
}

#ifdef WIN32
int DFilename::isHttp(const wchar_t *s)
{   return s && (wcslen(s) > 7) && (!_wcsnicmp(s, L"http://", 7) || !_wcsnicmp(s, L"https://", 8));
}
#endif


void DFilename::getPathFromUrl(DString& dest)
{   dest.clear();
    const char *p = strstr(text, "://");
    if (!p)
        return;
    p = strchr(p+3, '/');
    dest = p;
}

// Mapped filename = X:/
int DFilename::isMappedDrive(const char *s)
{   if (!s || !*s || (strlen(s) < 3))
        return false;
    if (!Ascii_IsAlpha(*s) || (s[1] != ':') || !isSlash(s[2]))
        return false;
    return true;
}

int DFilename::isRelative(const char *s)
{
    if (isSlash(*s))
        return false;
    return(!isMappedDrive(s) && !isUnc(s) && !isUrl(s));
}

// Mapped filename = X:/
int DFilename::isMappedDrive(const wchar_t *s)
{   if (!s || !*s || (wcslen(s) < 3))
        return false;
    if (!Ascii_IsAlpha(*s) || (s[1] != ':') || !isSlash(s[2]))
        return false;
    return true;
}

int DFilename::isRelative(const wchar_t *s)
{
    if (isSlash(*s))
        return false;
    return(!isMappedDrive(s) && !isUnc(s) && !isUrl(s));
}

int DFilename::isDotDot(const char *s)
{   return(s && (strlen(s) == 2) && (s[0] == '.') && (s[1] == '.'));
}

int DFilename::isDot(const char *s)
{   return(s && (strlen(s) == 1) && (*s == '.'));
}

void DFilename::getRoot(DString& dest)
{   dest.clear();
    if (isMappedDrive(text))
        dest.store(text, 3);
    else if (isUnc(text)) {
        const char *p = strchr(text+2, cSysSlash);
        p++;
        while (*p && (*p != cSysSlash))
            p++;
        dest.store(text, p - text);
    }
    else if ((text[0] == cSysSlash) || (text[0] == cUnixSlash)) {
        dest.store(text, 1);
    }
    else if (isalpha(text[0]) && (text[1] == ':') && (text[2] == '\0'))
        dest << text << cSysSlash;
}

void DFilename::getDrive(DString& dest)
{	getRoot(dest);
	if ((dest.getLength() > 1) && isSlash(dest.last()))
		dest.truncate(-1);
}

// For parsing relative/absolute paths
class TokenizedFilename {
public:
    TokenizedFilename(const char *s);
    int getCount() {
        return tokens.getCount();
    }

    void insertToken(int iPos);
    void removeToken(int iPos);
    void makeName(DString& dest);
    const char* getToken(int iPos) {
        if ((iPos >= 0) && (iPos < getCount()))
            return tokens.getString(iPos);
        else
            return NULL;
    }
    // A "Unix" path is one with no drive letter.
    // cSysSlash may still be backslash (as in Windows CE)
    // or forward slash
    int isUnixPath() {
        return fIsUnixPath;
    }
protected:
    DStringSet tokens;
    int fIsUnixPath;
};

TokenizedFilename::TokenizedFilename(const char *s) :
fIsUnixPath(0)
{   if (!s)
        s = "";
    DFilename f(s);
    f.setSlashType(cSysSlash);
    DString r;
    f.getRoot(r);
    if (r.getLength() > 0) {
        f.remove(0, r.getLength());
        if (r.last() == cSysSlash)
            r.truncate(-1);
        if (r.isBlank())
            fIsUnixPath = true;
        else
            tokens.append(r);
    }
    tokens.tokenize(f, cSysSlash);
}

void DFilename::makeRelativeTo(const char *baseDir)
{   if (isUrl(text))
        return;

    // Make relative to root /
    if (isSlash(*baseDir) && isSlash(*text) && (strlen(baseDir) == 1)) {
        remove(0, 1);
        return;
    }

    TokenizedFilename b(baseDir);
    TokenizedFilename a(text);

    // Count the number of common filename components
    int commonCt = 0;
    int i;
    for (i = 0; i < a.getCount(); ++i) {
        if (b.getToken(i) && !_stricmp(a.getToken(i), b.getToken(i)))
            commonCt++;
        else
            break;
    }

    if (commonCt == 0)
        return;

    // Make dot-dots for each component of the base dir that is
    // not common
    int ddCt = b.getCount() - commonCt;
    DFilename x;
    for (i = 0; i < ddCt; ++i)
        x << ".." << cSysSlash;

    // Add the non-common components of this path
    for (i = commonCt; i < a.getCount(); ++i)
        x.addTail(a.getToken(i));

    *this = x;
    if (isBlank()) {
        strCopy(text, ".", maxLen);
        len = 1;
    }
}

void DFilename::makeAbsoluteFrom(const char *baseDir)
{   if (!isRelative(text))
        return;
	DFilename temp;
	makeAbsoluteFrom(baseDir, temp);
}

static inline int isDotDotSlash(const char *p) {
	return (p[0] == '.') && (p[1] == '.') && DFilename::isSlash(p[2]);
}

void DFilename::makeAbsoluteFrom(const char *basePath, DFilename& temp)
{   if (!isRelative(text))
        return;

    // Combine ..\..\docs with c:\base\something to make c:\docs
    const char *p = text;
    if (isDot(p))
		p++;
    int levelCount = 0;
    while(isDotDotSlash(p)) {
    	p += 3;
    	levelCount++;
    	}
    temp = basePath;
    if (isSlash(temp.last()) && (temp.getLength() > 3))
		temp.truncate(-1);
    while (levelCount > 0) {
		if (temp.hasSlash())
    		temp.rmTail();
		else
			temp.clear();
    	levelCount--;
    	}
    if (*p)
		temp.addTail(p);

    *this = temp;
}


// get rid of .. and . terms in a name
void DFilename::simplify(bool bPreserveTrailingSlash)
{   TokenizedFilename tf(text);
    DFilename x;
    bool bTrailingSlash = (bPreserveTrailingSlash && isSlash(last()));
    char cLastSlash = last();
    if (tf.isUnixPath())
        x << cSysSlash;
    for (int i = 0; i < tf.getCount(); ++i) {
        const char *s = tf.getToken(i);
        if (isDot(s)) {
        }
        else if (isDotDot(s))
            x.rmTail();
        else
            x.addTail(s);
    }
    if (bTrailingSlash && !isSlash(x.last()))
		x << cLastSlash;

    *this = x;

}

#if defined(HAVE_WINAPI) 
void DFilename::make8_3()
{
    char tmp[256];
    tmp[0] = 0;
    if (GetShortPathNameA(text, tmp, sizeof tmp) > 0)
        *this = tmp;
}
#endif


void DFilename::buildVirtualPath(const char *http, const char *serverUrl, const char *virtualPath, int fPathIsUrlEncoded)
{	if (strIsBlank(http))
		http = "http://";
	DFilename tmp;
	tmp.append(http);
	tmp.append(serverUrl);
	if (fPathIsUrlEncoded)
		tmp.addTail(virtualPath);
	else {
		DFilename fn;
		fn.appendUrlEncodedFilename(virtualPath);
		tmp.addTail(fn);
		}
	*this = tmp;
}

//////////////////////////////////////////////////////////////////////
//
//   DStringSet is an unlimited-length set of null-terminated strings
//

const int defaultLen = 1024;

DStringSet::DStringSet() :
buf(new char[defaultLen]),
textLen(0),
maxLen(defaultLen),
count(0)
{   clear();
}

DStringSet::DStringSet(long aBufSize) :
buf(new char[aBufSize]),
maxLen(aBufSize),
textLen(0),
count(0)
{   clear();
}

DStringSet::DStringSet(const DStringSet& other) :
buf(new char[other.textLen+2]),
maxLen(other.textLen+2),
count(0)
{   clear();
    copyFromBuf(other.buf);
}

DStringSet& DStringSet::operator=(const DStringSet& other)
{   if (this != &other) {
        clear();
        copyFromBuf(other.buf);
    }
    return *this;
}

int DStringSet::operator==(const DStringSet& other) const
{   if (getCount() != other.getCount())
        return false;
    for (int i = 0; i < count; ++i) {
        if (strcmp(getString(i), other.getString(i)))
            return false;
    }
    return true;
}


DStringSet::DStringSet(const char *aBuf) :
buf(new char[defaultLen]),
maxLen(defaultLen),
textLen(0),
count(0)
{   clear();
    copyFromBuf(aBuf);
}

bool DStringSet::extend(ptrdiff_t newLen)
{   if (newLen < maxLen)
        return true;
    if (newLen < 3)
        newLen = 3;

    char *oldBuf = buf;
    buf = new char[newLen];
    if (!buf) {
		buf = oldBuf;
		return false;
		}

    maxLen = newLen;

    // Copy data from old buffer
    memmove(buf, oldBuf, textLen);
    buf[textLen] = '\0';
    delete [] oldBuf;

    // Rebuild table of pointers
    count = 0;
    textLen = 1;
    const char *p = buf;
    while (*p) {
        index[count++] = p;
        ptrdiff_t l = (strlen(p) + 1);
        p += l;
        textLen += l;
    }
    return true;
}

// Tokenize string based on single separator character, no quoting
void DStringSet::tokenize(const char *s, char sepChar, int fAllowEmptyTokens)
{   DString tmp;
    while (s && *s) {
        if (*s == sepChar) {
            if (fAllowEmptyTokens || !tmp.isBlank()) {
                append(tmp);
                tmp.clear();
            }
        }
        else
            tmp << *s;
        s++;
    }
    if (!tmp.isBlank())
        append(tmp);
}

// Tokenize string based on multiple separator characters, no quoting
void DStringSet::tokenize(const char *s, const char *sepChars, int fAllowEmptyTokens)
{   DString tmp;
    while (s && *s) {
        if (strchr(sepChars, *s)) {
            if (fAllowEmptyTokens || !tmp.isBlank()) {
                append(tmp);
                tmp.clear();
            }
        }
        else
            tmp << *s;
        s++;
    }
    if (!tmp.isBlank())
        append(tmp);
}

// Tokenize string based on multiple separator characters, with quoting
// If quote char is doubled, treat it as an escaped quote
void DStringSet::tokenizeq(const char *s, const char *sepChars, char quoteChar, int fAllowEmptyTokens)
{   DString tmp;
    int fQuoted = 0;
    int fHaveToken = 0;
    char c;
    while (s && (c = *s)) {
        if (c == quoteChar) {
			if (s[1] == quoteChar) {
				tmp << c;
				s++;
				}
			else
            	fQuoted = !fQuoted;
			}
        else if ((!fQuoted) && strchr(sepChars, c)) {
            if (tmp.getLength())
                fHaveToken = true;
            // repeat of non-white space token separator can indicate a blank token
            if (fAllowEmptyTokens && (c != ' '))
                fHaveToken = true;
        }
        else
            tmp << c;
        if (fHaveToken) {
			// Handle this:  abc,"",def
			// Double of quote char is normally escaped, but here the
			// context clearly indicates an empty value
			if ((tmp.getLength() == 1) && (tmp.first() == quoteChar) && fAllowEmptyTokens)
				tmp.clear();
							
            append(tmp);
            tmp.clear();
            fHaveToken = false;
        }
        s++;
    }
	if (!tmp.isBlank()) {
		if ((tmp.getLength() == 1) && (tmp.first() == quoteChar) && fAllowEmptyTokens)
			tmp.clear();
        append(tmp);
        }
}

int DString::getFromDecoded(const TCHAR *file, int maxToRead)
{	return getFromDecodedU8(UiToUtf8(file), maxToRead);
}

int DString::getFromDecodedU8(const char *file, int maxToRead)
{ 	if (getFromU8(file, maxToRead) == FAIL)
		return FAIL;
	if (getLength() == 0)
		return SUCCESS;

	unsigned char utf8Header[3] = {
		0xEF, 0xBB, 0xBF
		};
	unsigned char ucs16Header[3] = {
		0xFF, 0xFE
		};
	if (!memcmp(utf8Header, text, 3))
		remove(0, 3);
	else if (!memcmp(ucs16Header, text, 2)) {
		DWString tmp;
		ptrdiff_t wlen = (getLength() - 2)/2;
		const unsigned short *data = (const unsigned short *) text;
		data++; // skip header
		tmp.storeUcs16AsUnicode(data, wlen);
		clear();
		storeUnicodeAsUtf8(tmp, tmp.getLength());
		}
	return SUCCESS;
}

// Get an environment variable and return its value
const char *DString::safeGetEnv(const char *var)
{	clear();
	if (strIsBlank(var))
		return 0;
	extend(256);
#ifdef HAVE_SAFE_GETENV
	size_t size = 0;
	int ret = getenv_s(&size, text, maxLen, var);
	if (ret == EINVAL) {
		if ((ptrdiff_t) size > maxLen) {
			extend(size+1);
			ret =getenv_s(&size, text, maxLen, var);
		}
	}
	if (ret != EINVAL)
		len = sstrlen(text);

#else
	const char *v = getenv(var);
	if (v)
		*this << v;
#endif
	if (strIsBlank(text))
		return 0;
	return text;

}


#define isLineBreak(ch)  ((ch == '\n') || (ch == '\r') || (ch == '\0'))

int DStringSet::getLinesFrom(const TCHAR *file, bool bAllowEmpty)
{	return getLinesFromU8(UiToUtf8(file), bAllowEmpty);
}

int DStringSet::getLinesFromU8(const char *file, bool bAllowEmpty)
{	DString text;
    if (text.getFromDecodedU8(file) == FAIL)
		return FAIL;

	parseLinesFrom(text, bAllowEmpty);
	return SUCCESS;
}

void DStringSet::parseLinesFrom(DString& text, bool bAllowEmptyLines)
{
	clear();
	DString line;
	for (int i = 0; i <= text.getLength(); ++i) {
		char ch = text.getChar(i);
		if (isLineBreak(ch)) {
			if (bAllowEmptyLines || !line.isBlank()) {
				append(line);
				line.clear();
				}
			if (ch) {
				// Skip over second half of \r\n or \n\r
				char next = text.getChar(i+1);
				if (isLineBreak(next) && (next != ch))
					i++;
				}
			}
		else
			line << ch;
		}
}

int DStringSet::writeLinesToU8(const char *fn)
{	DString x;
	formatAsString(x, "\r\n");
	return x.writeToU8(fn);
}


void DStringSet::formatAsString(DString& dest, const char *delimiter, char quote) const
{   if (!delimiter)
        delimiter = ", ";
    for (int i = 0; i < getCount(); ++i) {
        if (i > 0)
            dest << delimiter;
        if (quote)
            dest << quote;
        dest << getString(i);
        if (quote)
            dest << quote;
    }
}

// same as formatAsString but try to avoid using quotes if possible
void DStringSet::formatAsSimpleString(DString& dest, char delimiter, char quote) const
{   if (!delimiter)
        delimiter = ' ';
    for (int i = 0; i < getCount(); ++i) {
        if (i > 0)
            dest << delimiter;
        int fNeedQuote = (strchr(getString(i), delimiter) != NULL);
        if (fNeedQuote && quote)
            dest << quote;
        dest << getString(i);
        if (fNeedQuote && quote)
            dest << quote;
    }
}

void DStringSet::formatAsTable(DString& dest) const
{   for (int i = 0; i < getCount(); i += 2) {
		dest << "    " << getString(i) << ": ";
		if (i+1 < getCount()) 
			dest << getString(i+1);
		else 
			dest << "<null>";
		dest << "\n";
		}
    }



bool DStringSet::copyFromBuf(const char *setBuf, int aLen)
{   if (setBuf == buf)
        return true;

    clear();
    if (!setBuf)
        return true;

    if (aLen > 0) {
        if (!extend(aLen))
			return false;
		}
    const char *p = setBuf;
    while (*p) {
        if (!append(p))
			return false;
        p += (strlen(p) + 1);
    }
	return true;
}

ptrdiff_t DStringSet::calcSetLength(const char *ptr)
{	const char *pEnd = ptr;
    while (*pEnd) {
        pEnd += (strlen(pEnd) + 1);
		}
	return pEnd - ptr + 1;
}

void DStringSet::prepend(const char *s)
{   DStringSet tmp = *this;
    clear();
    append(s);
    append(tmp);
}


void DStringSet::append(long v)
{   char s[64];
    I32_to_A((int) v, s, sizeof s, 10);
    append(s);
}

void DStringSet::appendField(const char *f, const char *v)
{	if (!strIsBlank(f) && !strIsBlank(v)) {
		append(f);
		append(v);
		}
}

void DWStringSet::appendFieldU8(const char *f, const char *v)
{	if (!strIsBlank(f) && !strIsBlank(v)) {
		appendU8(f);
		appendU8(v);
		}
}

bool DStringSet::getField(const char *f, DString& val) const
{	int iItem = findFieldName(f);
	if (iItem < 0)
		return false;
	if (iItem+1 >= getCount())
		return false;
	val = getString(iItem+1);
	return true;
}

#ifdef getInt
    #undef getInt
#endif

long DStringSet::getInt(int i) const
{   const char *s = getString(i);
    if (strIsBlank(s))
        return -1;
    else
        return atol(s);
}

DStringSet::~DStringSet()
{   freeBuffer();
}

void DStringSet::freeBuffer()
{   if (buf) {
        delete [] buf;
        buf = 0;
        }
}

void DStringSet::clear()
{   count = 0;
    textLen = 1;
    buf[0] = '\0';
    buf[1] = '\0';
}

int DStringSet::getCount() const
{   return count;
}

int DStringSet::find(const char *s) const
{   for (int i = 0; i < getCount(); ++i) {
		const char *x = getString(i);
        if (!strcmp(s, x))

            return i;
    }
    return -1;
}

int DStringSet::ifind(const char *s) const
{   for (int i = 0; i < getCount(); ++i) {
		const char *x = getString(i);
        if (!Ascii_stricmp(s, x))
            return i;
    }
    return -1;
}

int DStringSet::findFieldName(const char *s) const
{	// For comparison, skip over hidden-stored field marker (**) if present in field name
	if (!strncmp(s, "**", 2))
		s += 2;
    for (int i = 0; i < getCount(); i += 2) {
		const char *t = getString(i);
		if (!strncmp(t, "**", 2))
			t += 2;
        if (!Ascii_stricmp(s, t))
            return i;
    }
    return -1;
}

int DStringSet::findPrefix(const char *s) const
{   ptrdiff_t l = strlen(s);
    for (int i = 0; i < getCount(); ++i) {
        if (!strncmp(s, getString(i), l))
            return i;
    }
    return -1;
}

int DStringSet::rFindPrefix(const char *s) const
{   ptrdiff_t l = strlen(s);
    for (int i = getCount()-1; i >= 0; --i) {
        if (!strncmp(s, getString(i), l))
            return i;
    }
    return -1;
}

int DStringSet::ifindPrefix(const char *s) const
{   ptrdiff_t l = strlen(s);
    for (int i = 0; i < getCount(); ++i) {
        if (!_strnicmp(s, getString(i), l))
            return i;
    }
    return -1;
}

void DStringSet::deleteString(int toDel)
{	if (toDel == count-1) {
		if (count == 1) {
			clear();
			return;
			}
		count--;
		ptrdiff_t l = strlen(getString(toDel));
		textLen -= (l+1);
		buf[textLen-1] = 0;
		return;
		}
    DStringSet temp;
    for (int i = 0; i < getCount(); ++i)  {
        if (i != toDel)
            temp.append(getString(i));
    }
    *this = temp;
}

static int compareWords(const void *a, const void *b)
{   return strcmp(*(char **)a, *(char **)b);
}

void DStringSet::sort()
{	qsort(index.c_vector(), index.getCount(), sizeof(char *), compareWords);
}


bool DStringSet::append(const char *s, ptrdiff_t sLen)
{   // Don't allow zero-length strings to be appended
    if (!s || !*s)
        s = " ";

    if (sLen == -1)
        sLen = strlen(s);
    if (sLen + 1 + textLen >= maxLen) {
        if (!extend(maxLen * 2 + sLen + 1))
			return false;
		}
    // Store new string at textLen - 1 (before the
    // second null)
    index[count] = buf + textLen - 1;
    memmove(buf + textLen - 1, s, sLen);

    // terminate with double-null
    buf[textLen - 1 + sLen] = '\0';
    buf[textLen + sLen] = '\0';
    textLen += (sLen + 1);
    count++;
    return true;
}

const char *DStringSet::getString(int i) const
{   if ((i >= 0) && (i < index.getCount()))
        return index.get(i);
    else
        return "";
}

const char *DStringSet::getBuf() const
{   return buf;
}

ptrdiff_t DStringSet::getLength() const
{   return textLen;
}

void DStringSet::append(const DStringSet& other)
{   for (int i = 0; i < other.getCount(); ++i)
        append(other.getString(i));
}

#ifndef _WIN32_WCE

void DStringSet::urlEncode(DString& dest)
{   for (int i = 0; i < getCount(); i += 2) {
        const char *name = getString(i);
        const char *value = getString(i+1);
        dest.appendUrlEncoded(name);
        dest << '=';
        dest.appendUrlEncoded(value);
        if (i+2 < getCount())
            dest << "&";
    }
}


void DStringSet::urlDecode(const char *s)
{	if (strIsBlank(s))
		return;

    ptrdiff_t l = strlen(s)-1;
    while ((l > 0) && ((s[l] == '\xff') || _IsSpace(s[l])))
        l--;

    clear();
    DString name, value;
    DString *pDest = &name;
    for (int i = 0; i <= l; ++i) {
        char c = s[i];
        switch (c) {
            case '&':
                if (name.getLength()) {
                    if (!value.getLength())
                        value = " ";
                    append(name);
                    append(value);
                    name.clear();
                    value.clear();
                }
                pDest = &name;
                break;
            case '=':
                if (!name.getLength())
                    name = " ";
                pDest = &value;
                break;
            case '+':
                *pDest << ' ';
                break;
            case '%': {
                    char hex[3];
                    hex[0] = s[i+1];
                    hex[1] = s[i+2];
                    hex[2] = '\0';
                    int v = (int) strtol(hex, 0, 16);
                    *pDest << (char) v;
                    i += 2;
                    break;
                }
            default:
                *pDest << c;
                break;
        }
    }
    if (name.getLength()) {
        if (!value.getLength())
            value = " ";
        append(name);
        append(value);
    }
}

#endif

//////////////////////////////////////////////////////////////////////
//
//   DStringMap is a mapping of string names and string values
//

DStringMap::DStringMap() :
count(0)
{
}

DStringMap::~DStringMap()
{   count = 0;
}

DStringMap::DStringMap(const DStringMap& other)
{   copy(other);
}

DStringMap& DStringMap::operator=(const DStringMap& other)
{   copy(other);
    return *this;
}

void DStringMap::clear()
{   count = 0;
    stringSet.clear();
}

void DStringMap::copy(const DStringMap& other)
{   copyFromBuf(other.stringSet.getBuf());
}

//  Set creates a (name value) pair and replaces any existing
//  pair with the same name.  add() just appends, which makes it
//  possible to have multiple pairs with the same name.
void DStringMap::set(const char *aName, const char *aValue, int valueLen)
{   remove(aName);

    add(aName, aValue, valueLen);
}

void DStringMap::remove(const char *aName)
{
    int index = getIndex(aName);
    if (index < 0)
        return;

    stringSet.deleteString(2*index);
    stringSet.deleteString(2*index);
    buildTables();
}


void DStringMap::add(const char *aName, const char *aValue, int valueLen, int fBuildTables)
{   stringSet.append(aName);
    stringSet.append(aValue, valueLen);
    if (fBuildTables)
		buildTables();
}

void DStringMap::copyFromBuf(const char *buf, int aBufLen)
{   stringSet.copyFromBuf(buf, aBufLen);
    buildTables();
}

void DStringMap::buildTables()
{   count = stringSet.getCount()/2;
    for (int i = 0; i < count; ++i) {
        name[i] = stringSet.getString(i*2);
        value[i] = stringSet.getString(i*2 + 1);
    }
}

const char *DStringMap::getBuf() const
{   return stringSet.getBuf();
}

ptrdiff_t DStringMap::getLength() const
{   return stringSet.getLength();
}

int DStringMap::getIndex(const char *aName, int which)
{   for (int i = 0; i < count; ++i) {
        if (!Ascii_stricmp(name[i], aName)) {
            if (which == 0)
                return i;
            which--;
        }
    }
    return -1;
}

const char *DStringMap::get(const char *aName, int which)
{   for (int i = 0; i < count; ++i) {
        if (!Ascii_stricmp(name[i], aName)) {
            if (which == 0)
                return value[i];
            which--;
        }
    }
    return NULL;
}

void DStringMap::getAll(const char *aName, DString& s)
{   for (int i = 0; i < count; ++i) {
        if (!Ascii_stricmp(name[i], aName)) {
            s << value[i] << ' ';
        }
    }
}

void DStringMap::getAll(const char *aName, DStringSet& s)
{   for (int i = 0; i < count; ++i) {
        if (!Ascii_stricmp(name[i], aName)) {
            s.append(value[i]);
        }
    }
}

const char *DStringMap::getName(int i)
{   return name[i];
}

const char *DStringMap::getValue(int i)
{   return value[i];
}

int DStringMap::countValuesWithName(const char *aName)
{   int ct = 0;
    for (int i = 0; i < count; ++i) {
        if (!Ascii_stricmp(aName, name[i]))
            ct++;
    }
    return ct;
}




//////////////////////////////////////////////////////////////////////
//
//   DWStringSet is an unlimited-length set of null-terminated strings
//

DWStringSet::DWStringSet() :
buf(new wchar_t[defaultLen]),
maxLen(defaultLen),
textLen(0),
count(0)
{   clear();
}

DWStringSet::DWStringSet(long aBufSize) :
buf(new wchar_t[aBufSize]),
maxLen(defaultLen),
textLen(0),
count(0)
{   clear();
}

DWStringSet::DWStringSet(const DWStringSet& other) :
buf(new wchar_t[other.textLen+2]),
maxLen(other.textLen+2),
count(0)
{   clear();
    copyFromBuf(other.buf);
}

DWStringSet& DWStringSet::operator=(const DWStringSet& other)
{	DWStringSet::copy(other);
    return *this;
}

void DWStringSet::copy(const DWStringSet& other)
{	if (buf != other.buf) {
		clear();
		copyFromBuf(other.buf);
		}
}


DWStringSet::DWStringSet(const wchar_t *aBuf) :
buf(new wchar_t[defaultLen]),
maxLen(defaultLen),
textLen(0),
count(0)
{   clear();
    copyFromBuf(aBuf);
}

void DWStringSet::extend(ptrdiff_t newLen)
{   if (newLen < maxLen)
        return;
    if (newLen < 3)
        newLen = 3;

    wchar_t *oldBuf = buf;
    buf = new wchar_t[newLen];

    maxLen = newLen;

    // Copy data from old buffer
    wcsmove(buf, oldBuf, textLen);
    buf[textLen] = '\0';
    delete [] oldBuf;

    // Rebuild table of pointers
    count = 0;
    textLen = 1;
    const wchar_t *p = buf;
    while (*p) {
        index[count++] = p;
        ptrdiff_t l = (wcslen(p) + 1);
        p += l;
        textLen += l;
    }
}

void DWStringSet::appendField(const wchar_t *f, const wchar_t *v) {
		if (!strIsBlank(f) && !strIsBlank(v)) {
			append(f);
			append(v);
			}
	}

// Tokenize string based on single separator wchar_tacter, no quoting
void DWStringSet::tokenize(const wchar_t *s, wchar_t sepchar, int fAllowEmptyTokens)
{   DWString tmp;
    while (s && *s) {
        if (*s == sepchar) {
            if (fAllowEmptyTokens || !tmp.isBlank()) {
                append(tmp);
                tmp.clear();
            }
        }
        else
            tmp << *s;
        s++;
    }
    if (!tmp.isBlank())
        append(tmp);
}

// Tokenize string based on multiple separator wchar_tacters, no quoting
void DWStringSet::tokenize(const wchar_t *s, const wchar_t *sepchars, int fAllowEmptyTokens)
{   DWString tmp;
    while (s && *s) {
        if (wcschr(sepchars, *s)) {
            if (fAllowEmptyTokens || !tmp.isBlank()) {
                append(tmp);
                tmp.clear();
            }
        }
        else
            tmp << *s;
        s++;
    }
    if (!tmp.isBlank())
        append(tmp);
}

// Tokenize string based on multiple separator wchar_tacters, with quoting
void DWStringSet::tokenizeq(const wchar_t *s, const wchar_t *sepchars, wchar_t quoteChar)
{   DWString tmp;
    int fQuoted = 0;
    int fHaveToken = 0;
    wchar_t c;
    while (s && (c = *s)) {
        if (c == quoteChar) {
            fQuoted = !fQuoted;
            if (!fQuoted)
                fHaveToken = true;
        }

        else if ((!fQuoted) && wcschr(sepchars, c)) {
            if (!tmp.isBlank())
                fHaveToken = true;
        }
        else
            tmp << c;
        if (fHaveToken) {
            append(tmp);
            tmp.clear();
            fHaveToken = false;
        }
        s++;
    }
    if (!tmp.isBlank())
        append(tmp);
}

void DWStringSet::formatAsString(DWString& dest, const wchar_t *delimiter, wchar_t quote) const
{   if (!delimiter)
        delimiter = L", ";
    for (int i = 0; i < getCount(); ++i) {
        if (i > 0)
            dest << delimiter;
        if (quote)
            dest << quote;
        dest << getString(i);
        if (quote)
            dest << quote;
    }
}

// same as formatAsString but try to avoid using quotes if possible
void DWStringSet::formatAsSimpleString(DWString& dest, wchar_t delimiter, wchar_t quote) const
{   if (!delimiter)
        delimiter = ' ';
    for (int i = 0; i < getCount(); ++i) {
        if (i > 0)
            dest << delimiter;
        int fNeedQuote = (wcschr(getString(i), delimiter) != NULL);
        if (fNeedQuote && quote)
            dest << quote;
        dest << getString(i);
        if (fNeedQuote && quote)
            dest << quote;
    }
}

void DWStringSet::deleteString(int toDel)
{   DWStringSet temp;
    for (int i = 0; i < getCount(); ++i)  {
        if (i != toDel)
            temp.append(getString(i));
    }
    *this = temp;
}

void DWStringSet::copyFromBuf(const wchar_t *setBuf, ptrdiff_t aLen)
{   clear();
    if (!setBuf)
        return;

    if (aLen > 0)
        extend(aLen);
    const wchar_t *p = setBuf;
    while (*p) {
        append(p);
        p += (wcslen(p) + 1);
    }
}

void DWStringSet::append(long v)
{   char s[64];
    I32_to_A((int) v, s, sizeof s, 10);
    wchar_t ws[20];
	ptrdiff_t l = strlen(s);
    AnsiToUnicode(ws, s, l);
    append(ws);
}

long DWStringSet::getInt(int i) const
{   const wchar_t *ws = getString(i);
    if (wcsIsBlank(ws))
        return -1;
    else {
        char s[20];
        UnicodeToAnsi(s, ws, sizeof s);
        return atol(s);
    }
}

DWStringSet::~DWStringSet()
{   delete [] buf;
}

void DWStringSet::clear()
{   count = 0;
    textLen = 1;
    buf[0] = '\0';
    buf[1] = '\0';
}

int DWStringSet::getCount() const
{   return count;
}

int DWStringSet::find(const wchar_t *s) const
{   for (int i = 0; i < getCount(); ++i) {
        if (!wcscmp(s, getString(i)))
            return i;
    }
    return -1;
}

int DWStringSet::ifind(const wchar_t *s) const
{   for (int i = 0; i < getCount(); ++i) {
        if (!_wcsicmp(s, getString(i)))
            return i;
    }
    return -1;
}

int DWStringSet::findPrefix(const wchar_t *s) const
{   ptrdiff_t l = wcslen(s);
    for (int i = 0; i < getCount(); ++i) {
        if (!wcsncmp(s, getString(i), l))
            return i;
    }
    return -1;
}

void DWStringSet::append(const wchar_t *s, ptrdiff_t sLen)
{   // Don't allow zero-length strings to be appended
    if (!s || !*s)
        s = L" ";

    if (sLen == -1)
        sLen = wcslen(s);
    if (sLen + 1 + textLen >= maxLen)
        extend(maxLen * 2 + sLen + 1);

    // Store new string at textLen - 1 (before the
    // second null)
    index[count] = buf + textLen - 1;
    wcsmove(buf + textLen - 1, s, sLen);

    // terminate with double-null
    buf[textLen - 1 + sLen] = '\0';
    buf[textLen + sLen] = '\0';
    textLen += (sLen + 1);
    count++;
}

const wchar_t *DWStringSet::getString(int i) const
{   if ((i >= 0) && (i < index.getCount()))
        return index.get(i);
    else
        return L"";
}

const wchar_t *DWStringSet::getBuf() const
{   return buf;
}

ptrdiff_t DWStringSet::getLength() const
{   return textLen;
}

void DWStringSet::append(const DWStringSet& other)
{   for (int i = 0; i < other.getCount(); ++i)
        append(other.getString(i));
}

int DUrl::isValid() {
#ifdef __AFXINET_H_
        try {
            DWORD serviceType;
            CString server, object;
            unsigned short port;
            return (AfxParseURL(CUiStringFromUtf8(text), serviceType, server, object, port));
            }
        catch (...) {
            return false;
            }
#else
	if (!DFilename::isUrl(text))
		return false;
	return true;
#endif
        }

void DUrl::makeAbsoluteUrl(const char *aBaseUrl) {
        // If it already has :// in it, it must be absolute
        if (DFilename::isUrl(text) > 0)
            return;
        DUrl baseUrl(aBaseUrl);
        if (!baseUrl.isValid())
            return;

        // remove the http:// from the baseUrl and make it start with /,
        // then use relative filename logic
        DString service, port;
        DFilename object, server;
        DUrl result;
        baseUrl.split(service, server, object, port);

        if (*text == '/')
            result << service << server  << port << text;
        else {
            // remove the last component of the object and replace it
            // with this text, and then combine with the other components
            // of the base
            object.rmQuery();
            object.rmTail();
            object.addTail(text);
            object.simplify(true);
            result << service << server << port;
            result.addTail(object); // guarantees single slash separation
            }
        storeUrl(result);
        }

static const char *strchrOrEnd(const char *str, char ch)
{	if (!str)
		return 0;
	const char *ret = strchr(str, ch);
	if (!ret)
		ret = str + strlen(str);
	return ret;
}

int DUrl::split(DString& service, DString& server, DString& object, DString& port) const {
        service.clear();
        server.clear();
        object.clear();
        port.clear();
		if (!DFilename::isUrl(text))
			return FAIL;
        const char *pServer = strstr(text, "://");
        if (!pServer)
            return FAIL;
        pServer += 3;
        const char *pEnd = text + len;
        const char *pObject = strchrOrEnd(pServer, '/');
        const char *pEndObject = pEnd;
        const char *pEndServer = pObject;
        const char *pPort = strchrOrEnd(pServer, ':');
        const char *pEndPort = (pPort > pObject? pPort : pObject);
        if (!pPort || (pPort > pObject) || !Ascii_IsDigit(pPort[1]))
			pPort = pEnd;
		else
			pEndServer = pPort;

        if (pServer > text)
			service.store(text, pServer-text);
		if (pEndServer > pServer)
			server.store(pServer, pEndServer - pServer);
		if (pEndObject > pObject)
			object.store(pObject, pEndObject - pObject);
		if (pEndPort > pPort)
			port.store(pPort, pEndPort - pPort);
        return SUCCESS;
        }

int DUrl::split(DString& service, DString& server, DString& object, DString& query, DString& port) const
{	query.clear();
	int ret = split(service, server, object, port);
	ptrdiff_t iQuery = object.find('?');
	if (iQuery > FAIL) {
		query = object.str() + iQuery;
		object.truncate(iQuery);
		}
	return ret;
}


#ifdef NoInlineStringFuncs

int _IsSpace(char c) {
    if (c > 32)
        return false;
    return(c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t');
}

int _IsSpace(unsigned char c) {
    if (c > 32)
        return false;
    return(c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t');
}

int _IsSpace(wchar_t c) {
    if (c > 32)
        return false;
    return(c == ' ') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\t');
}

int strIsBlank(const char *p)
{   if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while (*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
    }
    return true;
}

int strIsBlank(const wchar_t *p)
{   if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while (*p) {
        if (!_IsSpace(*p))
            return false;
        p++;
    }
    return true;
}

void wcsmove(wchar_t *dest, const wchar_t *source, long cch)
{   memmove(dest, source, cch * sizeof(wchar_t));
}


#endif

void strCopy(char *s, const char *t, ptrdiff_t len)
{   if (!t || !s) {
        if (s && (len > 0))
            *s = 0;
        return;
    }
    while (*t && (len-- > 1))
        *s++ = *t++;
    *s = 0;
}

void strCopy(wchar_t *s, const wchar_t *t, ptrdiff_t len)
{   if (!t || !s) {
        if (s && (len > 0))
            *s = 0;
        return;
    }
    while (*t && (len-- > 1))
        *s++ = *t++;
    *s = 0;
}

static const char MatchOneChar = '?';
static const char MatchAnyChar  = '*';
static const char MatchAnyStr[2] = { MatchAnyChar, 0 };

bool strMatches(const char *aa, const char *bb)
{   if (!aa || !bb)
        return (strIsBlank(aa) == strIsBlank(bb)) || (bb && !strcmp(bb, MatchAnyStr));
    const unsigned char *a = (const unsigned char *) aa;
    const unsigned char *b = (const unsigned char *) bb;
    while (*a && *b)
        if ((*a == *b) || (*b == MatchOneChar)) {
            a++;
            b++;
            }
        else if (*b == MatchAnyChar) {
			while(*b == MatchAnyChar) 
				b++;
            if (!*b)
                return true;
            while (*a) {
                if (strMatches((const char *)a, (const char *)b))
                    return true;
                else
                    a++;
                }
            return false;
            }
        else
            return false;
    if (*a)
        return false;
    if (*b && strcmp((const char *) b, MatchAnyStr))
        return false;
    return true;
}


bool striMatches(const char *aa, const char *bb)
{   if (!aa || !bb)
        return (strIsBlank(aa) == strIsBlank(bb)) || (bb && !strcmp(bb, MatchAnyStr));

    const unsigned char *a = (const unsigned char *) aa;
    const unsigned char *b = (const unsigned char *) bb;
    while (*a && *b)
        if ((AsciiToUpper((char) *a) == AsciiToUpper((char) *b)) || (*b == MatchOneChar)) {
            a++;
            b++;
            }
        else if (*b == MatchAnyChar) {
            b++;
            if (!*b)
                return true;
            while (*a) {
                if (striMatches((const char *)a, (const char *)b))
                    return true;
                else
                    a++;
                }
            return false;
            }
        else
            return false;
    if (*a)
        return false;
    if (*b && strcmp((const char *) b, MatchAnyStr))
        return false;
    return true;
}



int ScaleValue(long num, long denom, int scale)
{	if ((denom == 0) || (scale == 0) || (num == 0))
		return 0;

	__int64 ret = num;
	__int64 scale64 = scale;
	__int64 denom64 = denom;
	ret *= scale64;
	ret /= denom64;

    return (int) ret;
}

int ScaleValue64(__int64 num, __int64 denom, int scale)
{	if ((denom == 0) || (scale == 0) || (num == 0))
		return 0;

	__int64 ret = num;
	__int64 scale64 = scale;
	__int64 denom64 = denom;
	ret *= scale64;
	ret /= denom64;

    return (int) ret;
}




#ifdef USE_DTSEARCH_NAMESPACE
    } // namespace dtSearch
#endif

void debugLogIgnore(const char *)  { }
void debugLogIgnore(const wchar_t *)  { }
void debugLogIgnore(const char *, __int64) { }
void debugLogIgnore(const wchar_t *, __int64) { }
void debugLogIgnore(const char *, const char *) { }
void debugLogIgnore(const char *, const char *, const char *) { }
void debugLogIgnore(const char *, const wchar_t *) { }
void debugLogIgnore(const char *, __int64, __int64) { }
void debugLogIgnore(const wchar_t *, const wchar_t *, const wchar_t *) { }
