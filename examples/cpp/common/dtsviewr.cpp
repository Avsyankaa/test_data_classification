#include <stdafx.h>
#include <stdlib.h>
#include <string.h>
#if defined(_Windows) && !defined(_INC_WINDOWS)
#   include <windows.h>
#endif
#include <dtconfig.h>
#if !defined(DTSVIEWR_H)
#include <dtsviewr.h>
#endif
#include <dt_util.h>

#ifdef _MSC_VER
#   pragma warning(disable : 4018)  // signed/unsigned comparison
#endif

static void _strCopy(char *s, const char *t, int len)
{
    if (!t || !s) {
        if (s && (len > 0))
            *s = 0;
        return;
    }
    while (*t && (len-- > 1))
        *s++ = *t++;
    *s = 0;
}

///////////////////////////////////////////////////////////////////////
//
//   Constructors and copy functions for the structures used in
//   the external viewer API
//


dtsBookMark::dtsBookMark()
{
    clear();
}

dtsBookMark& dtsBookMark::copy(const dtsBookMark& other)
{
    memmove(this, &other, sizeof(dtsBookMark));
    return *this;
}

void dtsBookMark::clear()
{
    memset(this, 0, sizeof(dtsBookMark));
}

dtsTextBlock::dtsTextBlock()
{
    textLen = 0;
    bufferSize = 0;
    buf = 0;
    fractionRead = 0;
}

ptrdiff_t dtsTextBlock::append(const char *s, ptrdiff_t bytes) {
	if (bytes < 0)
		bytes = strlen(s);
	if (bytes + textLen + 1 > bufferSize)
		bytes = bufferSize - textLen - 1;
	if (bytes > 0) {
		textLen += (int) bytes;
		memmove(buf + textLen, s, bytes);
		buf[textLen] = 0;
		}
	return bytes;
	}


void dtsTextBlock::copy(const dtsTextBlock& other)
{
    int toCopy = (int) _Min(other.textLen, bufferSize);
   
    if (toCopy > 0)
        memmove(buf, other.buf, toCopy);
    textLen = toCopy;
    fractionRead = other.fractionRead;
    bookMark.copy(other.bookMark);
};

dtsInputStream::dtsInputStream()
{
    initStruct<dtsInputStream>(*this);
}

// sizeof dtsInputStream before structSize was added
const int oldInputStreamSize = 74;

dtsInputStream& dtsInputStream::copy(const dtsInputStream& other)
{
    copyStruct<dtsInputStream>(*this, other, oldInputStreamSize);
    return *this;
}

void dtsInputStream::setSize(__int64 s) {
    size64 = s;
    if (s > 0x7fffffff)
        size = 0;
    else
        size = (long) s;
}

int dtsInputStream::isValid()
{
#ifdef HAVE_WINAPI
    return !(IsBadCodePtr((FARPROC) seek) || IsBadCodePtr((FARPROC) read));
#else
    return 1;
#endif
}

void dtsInputStream::clear()
{
    clearStruct<dtsInputStream>(*this, oldInputStreamSize);
}

// Size of dtsFileInfo before structSize was added
static const int oldFileInfoSize = 1622;

dtsFileInfo::dtsFileInfo()
{
    initStruct<dtsFileInfo>(*this);
}

dtsFileInfo& dtsFileInfo::copy(const dtsFileInfo& other)
{
    copyStruct<dtsFileInfo>(*this, other, oldFileInfoSize);
    return *this;
}

void dtsFileInfo::clear()
{
    clearStruct<dtsFileInfo>(*this, oldFileInfoSize);
}

void dtsFileInfo::setTypeId(int aTypeId, const char *aTypeName)
{	if (typeId != aTypeId) {
		typeId = (unsigned short) aTypeId;
		if (aTypeName && *aTypeName)	
			_strCopy(typeName, aTypeName, sizeof typeName);
		else
			_strCopy(typeName, dtsSearchResultsItem::getTypeName(aTypeId), sizeof typeName);
		}
}

dtsRecognitionSignature::dtsRecognitionSignature()
{
    clear();
}

dtsRecognitionSignature& dtsRecognitionSignature::copy(const dtsRecognitionSignature& other)
{
    memmove(this, &other, sizeof(dtsRecognitionSignature));
    return *this;
}

void dtsRecognitionSignature::clear()
{
    memset(this, 0, sizeof(dtsRecognitionSignature));
}

dtsViewerInfo::dtsViewerInfo()
{
    clear();
}

dtsViewerInfo& dtsViewerInfo::copy(const dtsViewerInfo& other)
{
    memmove(this, &other, sizeof(dtsViewerInfo));
    return *this;
}

void dtsViewerInfo::clear()
{
    memset(this, 0, sizeof(dtsViewerInfo));
}

void dtsViewerInfo::setVersion(const char *s)
{
    _strCopy(version, s, sizeof version);
}

void dtsViewerInfo::setName(const char *s)
{
    _strCopy(name, s, sizeof name);
}



dtsViewerSetupInfo::dtsViewerSetupInfo()
{
    memset(this, 0, sizeof(dtsViewerSetupInfo));
}

dtsContainerItemInfo::dtsContainerItemInfo()
{
    initStruct<dtsContainerItemInfo>(*this);
}

void dtsContainerItemInfo::clear()
{

	clearStruct<dtsContainerItemInfo>(*this);
}

dtsContainerItemInfo& dtsContainerItemInfo::copy(const dtsContainerItemInfo& other)
{	copyStruct<dtsContainerItemInfo>(*this, other);
	setIndexInContainer(other.getIndexInContainer());
    return *this;
}

dtsMakeViewerParams::dtsMakeViewerParams()
{
    initStruct<dtsMakeViewerParams>(*this);
}

//////////////////////////////////////////////////////////////////////
//
//   dtsInputStreamReader provides a wrapper around dtsInputStream for
//   convenient access.
//

dtsInputStreamReader::dtsInputStreamReader()
{
    source.clear();
    pos = 0;
}

dtsInputStreamReader::dtsInputStreamReader(dtsInputStream& aSource)
{
    source.clear();
    pos = 0;
    attach(aSource);
}

dtsInputStreamReader::~dtsInputStreamReader()
{
}

void dtsInputStreamReader::attach(dtsInputStream& aSource)
{
    if (aSource.isValid())
        source.copy(aSource);
    else
        source.clear();
    pos = 0;
}

dtsInputStream& dtsInputStreamReader::getSource()
{
    return source;
}

void dtsInputStreamReader::seek(__int64 aPos)
{	if (aPos >= 0) {
		pos = aPos;
		if (source.seek64)
			source.seek64(source.pData, pos);
		else
			source.seek(source.pData, (int) pos);
		}
}

long dtsInputStreamReader::read(void *dest, long bytes)
{	if (bytes <= 0)
		return 0;
		
    if (bytes > source.getSize() - pos) {
        if (pos >= source.getSize())
            return 0;

        __int64 b = source.getSize();
        b -= pos;
        bytes = (long) b;
    }
    unsigned long bytesRead = 0;

    if (bytes > 0)
        bytesRead = source.read(source.pData, dest, bytes);
    return bytesRead;
}


__int64 dtsInputStreamReader::getSize() const
{
    return source.getSize();
}

int dtsInputStreamReader::getSize32() const
{
    return (int) getSize();
}


const char *dtsInputStreamReader::getName() const
{
    return source.filename;
}

dtsFileDate& dtsInputStreamReader::getModified()
{
    return source.modified;
}

dtsFileDate& dtsInputStreamReader::getCreated()
{
    return source.created;
}

__int64 dtsInputStreamReader::tell() const
{
    return pos;
}

int dtsInputStreamReader::getFractionRead(int scale)
{
    if (source.getFractionRead)
        return source.getFractionRead(source.pData, scale);
    else {
        __int64 v = tell() * 1000;
        v /= getSize();
        return (int) v;
    }
}


void dtsInputStreamReader::getFileInfo(dtsFileInfo& fi)
{
    _strCopy(fi.filename, source.filename, sizeof fi.filename);
    if (source.displayName)
        _strCopy(fi.displayName, source.displayName, sizeof fi.displayName);
    else
        _strCopy(fi.filename, source.filename, sizeof fi.filename);

    fi.modified.copy(source.modified);
    fi.created.copy(source.created);
    fi.setSize(source.getSize());
    fi.title[0] = '\0';
};

dtsDataSource::dtsDataSource()
{
    initStruct<dtsDataSource>(*this);
}

void dtsDataSource::copy(const dtsDataSource& other)
{
    copyStruct<dtsDataSource>(*this, other);
}

dtsDataSourceDocExtractor::dtsDataSourceDocExtractor()
{
    initStruct<dtsDataSourceDocExtractor>(*this);
}

dtsEngineFunctions::dtsEngineFunctions()
{
    initStruct<dtsEngineFunctions>(*this);
}


#ifndef __UNIX__

#ifdef UNICODE
static FARPROC __GetProcAddress(HINSTANCE hi, const char *fn)
{
    return ::GetProcAddress(hi, fn);
}
#else
#define __GetProcAddress        GetProcAddress
#endif

#ifndef HAVE_SAFE_STRUPR
void _strupr_s(char *s, size_t /* size */)
{
    _strupr(s);
}
#endif


// This is a static rather than a member function so that dtsviewr.h does not
// need to know what an HINSTANCE is (bringing in windows.h)
static void getDtsProc(HINSTANCE dllHandle, const char *funcName, uintptr_t * ptr)
{
    FARPROC n;
    char fname2[80];
    n = __GetProcAddress(dllHandle, funcName);
    if (!n) {
        _strCopy(fname2, "_", sizeof fname2);
        _strCopy(fname2+1, funcName, (sizeof fname2)-1);
        n = __GetProcAddress(dllHandle, fname2);
    }
    char fname[80];
    if (!n) {
        _strCopy(fname, funcName, sizeof fname);
        _strupr_s(fname, sizeof fname);
        n = __GetProcAddress(dllHandle, fname);
    }
    if (!n) {
        _strupr_s(fname2, sizeof fname2);
        n = __GetProcAddress(dllHandle, fname2);
    }

    *ptr = (uintptr_t) n;
}


#define GetProc(n) getDtsProc(dllHandle, #n, (uintptr_t *) &n)

void dtsEngineFunctions::makeFromHandle(void *aHandle)
{
    HINSTANCE dllHandle = (HINSTANCE) aHandle;
    memset(this, 0, sizeof (dtsEngineFunctions));
    structSize = sizeof(dtsEngineFunctions);
	GetProc(dtssDoIndexJob);
    GetProc(dtssMergeIndexes);
    GetProc(dtssDoSearchJob);
    GetProc(dtssGetIndexInfo);
    GetProc(dtssVersion);
    GetProc(dtssVersionEx);
    GetProc(dtssDoInit);
    GetProc(dtssDoShutDown);
    GetProc(dtssDoGetFileText);
    GetProc(dtssDebugLog);
    GetProc(dtssDebugLogEx);
    GetProc(dtssAddToLog);
    GetProc(dtssSetOptions);
    GetProc(dtssGetOptions);
    GetProc(dtssDeleteIndex);
    GetProc(dtssRunScript);
    GetProc(dtssMapHitsInFile);
    GetProc(dtssConvertFile);
    GetProc(dtssConvertFile2);
    GetProc(dtssGetIndexList);
    GetProc(dtsSR_Alloc);
    GetProc(dtsSR_Free);
    GetProc(dtsSR_Copy);
    GetProc(dtsSR_GetDocInfo);
    GetProc(dtsSR_SetDocInfo);
    GetProc(dtsSR_SetSortKey);
    GetProc(dtsSR_GetDocInfoStrings);
    GetProc(dtsSR_GetCounts);
    GetProc(dtsSR_GetSearchRequest);
    GetProc(dtsSR_Sort);
    GetProc(dtsSR_MakeReport);
    GetProc(dtsSR_MakePdfHighlightFile);
    GetProc(dtsSR_MakePdfWebHighlightFile);
    GetProc(dtsSR_ReadDocInfo);
    GetProc(dtsSR_Serialize);
    GetProc(dtsSR_SerializeAsXml);
    GetProc(dtsSR_SerializeFromXml);
    GetProc(dtsSR_LogDocInfo);
    GetProc(dtsSR_UnionWith);
    GetProc(dtsSR_IntersectionOf);
    GetProc(dtsSR_UrlDecodeItem);
    GetProc(dtsSR_UrlEncodeItem);
    GetProc(dtsSR_UrlEncodeItem2);
    GetProc(dtsSR_GetSearchDate);
    GetProc(dtsSR_AddItem);
    GetProc(dtsSR_AddItems);
    GetProc(dtsSR_GetSearchFlags);
    GetProc(dtsSR_SetSelections);
    GetProc(dtsSR_GetSelections);
    GetProc(dtssGetOptions2);
    GetProc(dtssSetOptions2);
    GetProc(dtssVerifyIndex);
	GetProc(dtsSF_AddIndex);
	GetProc(dtsSF_Alloc);
	GetProc(dtsSF_Free);
#ifdef _WIN32	
    GetProc(dtssConvertPath);
    GetProc(dtssConvertPath2);
#endif

}

#endif


void dtsEngineFunctions::copy(const dtsEngineFunctions& other)
{
    memmove(this, &other, sizeof(dtsEngineFunctions));
}

dtsHighlightDataRequest::dtsHighlightDataRequest()
{
    initStruct<dtsHighlightDataRequest>(*this);
}

dtsDataSourceFileInfo::dtsDataSourceFileInfo()
{
    initStruct<dtsDataSourceFileInfo>(*this);
}


dtsDataSourceFileInfo::dtsDataSourceFileInfo(const dtsDataSourceFileInfo& other)
{
    initStruct<dtsDataSourceFileInfo>(*this);
    copy(other);
}

void dtsDataSourceFileInfo::copy(const dtsDataSourceFileInfo& other)
{
    copyStruct<dtsDataSourceFileInfo>(*this, other);
}

void dtsDataSourceFileInfo::clear()
{
    clearStruct<dtsDataSourceFileInfo>(*this);
}
