#include <stdafx.h>
#ifndef DInputStream_H
#include <dinputstream.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif

const int InputSourceBaseID = 0x77776666L;

DInputStreamBase::DInputStreamBase(dtsFileInfo& info) :
    id(InputSourceBaseID),
    m_errorCode(0),
    m_typeId(0),
    m_bSuppressDeleteOnReleaseCB(false)
{   setFileInfo(info);

}

DInputStreamBase::DInputStreamBase() :
    id(InputSourceBaseID),
    m_errorCode(0),
    m_typeId(0),
    m_bSuppressDeleteOnReleaseCB(false)
{
}

DInputStreamBase::~DInputStreamBase()
{   id = 0;
    fileInfo.clear();
} 

long DInputStreamBase::readCB(void *pData, void *dest, long bytes)
{
    DInputStreamBase *s = DInputStreamBase::safeCast(pData);
    long v = 0;
    if (s)
        v = s->read(dest, bytes);

    return v;
}

int DInputStreamBase::getFractionReadCB(void *pData, int scale)
{   DInputStreamBase *s = DInputStreamBase::safeCast(pData);
    int v = 0;
    if (s)
        v = s->getFractionRead(scale);

    return v;
}


void DInputStreamBase::seekCB(void *pData, long where)
{	seekCB64(pData, where);
}

void DInputStreamBase::seekCB64(void *pData, DFilePosition where)
{
    DInputStreamBase *s = DInputStreamBase::safeCast(pData);
    if (s)
		s->seek( where);
}


void DInputStreamBase::releaseCB(void *pData)
{   DInputStreamBase *s = safeCast(pData);
    if (s && !s->m_bSuppressDeleteOnReleaseCB)
        delete s;

}

DInputStreamBase *DInputStreamBase::safeCast(void *pData)
{   DInputStreamBase *s = (DInputStreamBase *) pData;
    if (s && (s->id == InputSourceBaseID))
        return s;
    return NULL;
}

void DInputStreamBase::setOpenFailInfo(int code, const char *msg)
{	m_errorCode = code;
	m_errorText = msg;
}

void DInputStreamBase::makeInterface(dtsInputStream& dest)
{   dest.pData = this;
    dest.seek = DInputStreamBase::seekCB;
    dest.seek64 = DInputStreamBase::seekCB64;
    dest.release = DInputStreamBase::releaseCB;
    dest.getFractionRead = DInputStreamBase::getFractionReadCB;
    dest.read = DInputStreamBase::readCB;
    dest.filename = fileInfo.filename;
    dest.displayName = fileInfo.displayName;
    if (!tempFile.isBlank())
        dest.tempname = tempFile;
	else
		dest.tempname = 0;

    dest.setSize(fileInfo.getSize());
    dest.modified.copy(fileInfo.modified);
    dest.created.copy(fileInfo.created);
    dest.fields = Fields;
    dest.openError = m_errorCode;
    dest.openErrorMessage = m_errorText;
    // m_typeId is used to indicate when we know the input
    // is UTF-8 text (when it comes from DMemoryInputStream::setTextData())
    dest.typeId = m_typeId;
}

dtsInputStream *DInputStreamBase::getInterface()
{   makeInterface(inputSourceInterface);
    return &inputSourceInterface;
}

DMemoryInputStream::DMemoryInputStream(dtsFileInfo& info, const char *textData) :
    DInputStreamBase(info),
    m_length(0),
    m_pos(0)
{   setTextData(textData);
}

DMemoryInputStream::DMemoryInputStream() :
    m_length(0), m_pos(0), m_data(0)
{
}

DMemoryInputStream::~DMemoryInputStream()
{	m_length = 0;
	m_pos = 0;
}

void DMemoryInputStream::setTextData(const char *textData)
{   if (textData)
        setBinaryData(textData, (long) strlen(textData));
    else
        setBinaryData("", 0);
    m_typeId = it_Utf8;
}

void DMemoryInputStream::setBinaryData(const char *p, ptrdiff_t bytes)
{   if (!m_data.allocate(bytes))
		throw std::bad_alloc();
    memmove(m_data.getBuffer(), p, bytes);
    setDataSize(bytes);
    m_errorCode = 0;
}

void DMemoryInputStream::setDataSize(ptrdiff_t bytes)
{   m_length = bytes;
	fileInfo.setSize(bytes);
    makeInterface(inputSourceInterface);
}

void DMemoryInputStream::deleteData(ptrdiff_t offset, ptrdiff_t bytes)
{	if (offset >= m_length)
		return;

	if (offset + bytes > m_length) {
		m_length = offset;
		return;
		}
	ptrdiff_t toMove = m_length - offset - bytes;
	char *dest = m_data.getBuffer() + offset;
	char *source = dest + bytes;
	memmove(dest, source, toMove);
	m_length -= bytes;
}

void DMemoryInputStream::setBinaryDataAsBuffer(void *pData, ptrdiff_t bytes)
{	m_data.attach((char *)pData, bytes, false);
	setDataSize(bytes);
}

DMemoryInputStream *DMemoryInputStream::clone()
{	DMemoryInputStream *ret = new DMemoryInputStream;
	ret->setFileInfo(fileInfo);
	ret->m_errorCode = m_errorCode;
	ret->m_errorText = m_errorText;
	ret->tempFile = tempFile;
	ret->allocate(getDataSize());
	ret->setBinaryData(getBuffer(), getDataSize());
	return ret;
}

long DMemoryInputStream::read(void *dest, long bytes)
{   if (bytes + m_pos >= m_length)
        bytes = (int) (m_length - m_pos);
    if (bytes > 0)
        memmove(dest, m_data.getBuffer() + m_pos, bytes);
    m_pos += bytes;
    return bytes;
}

void DMemoryInputStream::seek(DFilePosition where)
{   m_pos = (int) where;
}

#ifdef MfcAvailable
static void copyDate(CTime& ctime, dtsFileDate& date)
{   date.year = (unsigned short) ctime.GetYear();
    date.month = (char) ctime.GetMonth();
    date.day = (char) ctime.GetDay();
    date.hour = (char) ctime.GetHour();
    date.minute = (char) ctime.GetMinute();
    date.second = (char) ctime.GetSecond();
}
#endif

DFileInputStream::DFileInputStream() :
    pos(0),
    length(0),
    fShouldDeleteFile(false)
{
}

int DFileInputStream::open(const char *fn)
{   m_errorCode = 0;

    if (file.open(fn, F_READ) == FAIL)
        return FAIL;

    length = file.getLength();
    file.getCreatedDate(fileInfo.created);
    file.getModifiedDate(fileInfo.modified);
    fileInfo.setSize(length);
    strCopy(fileInfo.filename, fn, sizeof fileInfo.filename);

    tempFile = fileInfo.filename;
    pos = 0;
    return SUCCESS;
}

DFileInputStream::~DFileInputStream()
{   close();
}

void DFileInputStream::close()
{   file.close();
}

long DFileInputStream::read(void *dest, long bytes)
{   if (pos >= length)
        return 0;

    long ret = 0;
    file.seek(pos);
    long bytesRead = file.read((char *)dest, bytes);
    pos += bytesRead;
    ret = bytesRead;
    return ret;
}

void DFileInputStream::seek(DFilePosition where)
{   pos = where;
}

// Use this to construct a dtsInputStream for a collection of document
// properties
DInputStreamBase *DInputStreamBase::makeInputSource(dtsFileInfo& fileInfo,
    const char *docText, const DStringSet& docFields, int fDocIsFile)
{   if (!docText)
        docText = "";
    DInputStreamBase *ret = 0;
    if (fDocIsFile) {
        DHeapPtr<DFileInputStream> fs(new DFileInputStream);
        fs->open(fileInfo.filename);
        if (!fs->good()) {
			// Make an error input stream so the file access failure will be reported
	        DHeapPtr<DMemoryInputStream> ms(new DMemoryInputStream(fileInfo, docText));
		    ms->Fields.appendU8(docFields);
		    ms->setOpenFailInfo(fs->getLastError(), "Unable to open file");
			ret = ms.detach();
            }
        else {
            fs->Fields.appendU8(docFields);
            if (!strIsBlank(docText)) {
                fs->Fields.appendU8(" ");
                fs->Fields.appendU8(docText);
                }
            // override the dtsFileInfo properties of the disk file
            // with what the data source provided
            dtsFileInfo tmp;
            fs->getFileInfo(tmp);
            strCopy(tmp.filename, fileInfo.filename, sizeof tmp.filename);
            strCopy(tmp.displayName, fileInfo.displayName, sizeof tmp.filename);
            tmp.modified.copy(fileInfo.modified);
            tmp.created.copy(fileInfo.created);
            fs->setFileInfo(tmp);
            ret = fs.detach();
            }
        }
    if (!ret) {
        DHeapPtr<DMemoryInputStream> ms(new DMemoryInputStream(fileInfo, docText));
        ms->Fields.appendU8(docFields);
        ret = ms.detach();
        }
    return ret;
}


const long DDataSourceBaseID = 0x02020202;

DDataSourceBase::DDataSourceBase() :
    idDDataSourceBase(DDataSourceBaseID)
{
}

dtsDataSource *DDataSourceBase::getInterface()
{   makeInterface(dataSourceInterface);
    return &dataSourceInterface;
}

DDataSourceBase::~DDataSourceBase()
{   idDDataSourceBase = 0;
}

int DDataSourceBase::rewindCB(void *pData)
{   DDataSourceBase *s = DDataSourceBase::safeCast(pData);
    if (s)
        return s->rewind();
    else
        return FAIL;
}

int DDataSourceBase::getNextDocCB(void *pData, dtsInputStream& dest)
{   DDataSourceBase *s = DDataSourceBase::safeCast(pData);
    if (s)
        return s->getNextDoc(dest);
    else
        return FAIL;
}

int DDataSourceBase2::getNextDocInfoCB(void *pData, dtsDataSourceFileInfo& dest)
{   DDataSourceBase2 *s = DDataSourceBase2::safeCast(pData);
    if (s)
        return s->getNextDocInfo(dest);
    else
        return FAIL;
}

int DDataSourceBase2::getCurrentDocCB(void *pData, dtsInputStream& dest)
{   DDataSourceBase2 *s = DDataSourceBase2::safeCast(pData);
    if (s)
        return s->getCurrentDoc(dest);
    else
        return FAIL;
}

int DDataSourceBase2::getDocInfoByNameCB(void *pData, const char *docName, const char *userFields, dtsDataSourceFileInfo& fi)
{   DDataSourceBase2 *s = DDataSourceBase2::safeCast(pData);
    if (s)
        return s->getDocInfoByName(docName, userFields, fi);
    else
        return FAIL;
}



DDataSourceBase *DDataSourceBase::safeCast(void *pData)
{   DDataSourceBase *s = (DDataSourceBase *) pData;
    if (s && (s->idDDataSourceBase == DDataSourceBaseID))
        return s;
    return NULL;
}

void DDataSourceBase::makeInterface(dtsDataSource& dest)
{   memset(&dest, 0, sizeof dest);
    dest.pData = this;
    dest.rewind = DDataSourceBase::rewindCB;
    dest.getNextDoc = DDataSourceBase::getNextDocCB;
    dest.pErrorHandler = &errorHandler;
}
const long DDataSourceBase2ID = 0x03030303;

DDataSourceBase2::DDataSourceBase2()
{	idDDataSourceBase2 = DDataSourceBase2ID;
}

DDataSourceBase2::~DDataSourceBase2()
{	idDDataSourceBase2 = 0;
}



void DDataSourceBase2::makeInterface(dtsDataSource& dest)
{	DDataSourceBase::makeInterface(dest);
	dest.getNextDocInfo = getNextDocInfoCB;
	dest.getCurrentDoc = getCurrentDocCB;
	dest.getDocInfoByName = getDocInfoByNameCB;
}

DDataSourceBase2 *DDataSourceBase2::safeCast(void *pData)
{   DDataSourceBase2 *s = (DDataSourceBase2 *) pData;
    if (s && (s->idDDataSourceBase == DDataSourceBaseID) && (s->idDDataSourceBase2 == DDataSourceBase2ID))
        return s;
    return NULL;
}

void DDataSourceFileInfo::copy(const dtsDataSourceFileInfo& other)
{	dtsDataSourceFileInfo::copy(other);
	Filename = other.filename;
	ErrorMessage = other.errorMessage;
}

void DDataSourceFileInfo::getFrom(const dtsInputStream& inputStream)
{	Filename = inputStream.filename;
	modified.copy(inputStream.modified);
	created.copy(inputStream.created);
	size = inputStream.getSize();
}
