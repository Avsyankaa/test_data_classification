#include <stdafx.h>
#include <fstream>
#include <iostream>

using namespace std;


//  Demonstrates use of the dtsDataSource indexing API
//

#ifdef _WIN32
typedef wchar_t UCS16Char;
#else
typedef unsigned short UCS16Char;
#endif

const char nl = '\n';
const char quote = '\"';

#if _MSC_VER < 1400

static void strcpy_s(char *s, size_t len, const char *t)
{   if (!t || !s) {
        if (s && (len > 0))
            *s = 0;
        return;
    }
    while (*t && (len-- > 1))
        *s++ = *t++;
    *s = 0;
}

static void wcscpy_s(wchar_t *s, size_t len, const wchar_t *t)
{   if (!t || !s) {
        if (s && (len > 0))
            *s = 0;
        return;
    }
    while (*t && (len-- > 1))
        *s++ = *t++;
    *s = 0;
}

#define _countof(x)  (sizeof x/sizeof x[0])

static void strcat_s(char *dest, size_t len, char *source)
{	if (strlen(source) + strlen(dest) < len)
		strcat(dest, source);
}

#endif


//
//  A TextInputSource is a "document" to be indexed.
//  The SampleDataSource object will create a TextInputSource
//  for each document it wants to supply to the dtSearch Engine.
//  The dtSearch Engine will use the function pointers in a
//  dtsInputStream to access a TextInputSource.
//
class TextInputSource {
    public:
        TextInputSource(dtsFileInfo& info, const wchar_t *textData);
        ~TextInputSource();
        void seek(long where);
        int read(void *dest, long bytes);
        void makeInterface(dtsInputStream& dest);
        static void seekCB(void *pData, long where);
        static long readCB(void *pData, void *data, long bytes);
        static void releaseCB(void *pData);
        static TextInputSource *safeCast(void *pData);
    protected:
        long idTextInputSource;
        UCS16Char *data;
        long pos;
        long length;
        dtsFileInfo fileInfo;
    };

const long TextInputSourceID = 0x01010101;

TextInputSource::TextInputSource(dtsFileInfo& info, const wchar_t *textData) :
    data(0),
    pos(0),
    idTextInputSource(TextInputSourceID)
{   fileInfo.copy(info);
	int textLen = wcslen(textData);
    // This creates a UCS16 buffer with a 2-byte Unicode byte-order marker in front,
    // which tells dtSearch that the "file" being indexed is Unicode.
    data = new UCS16Char[textLen + 2];
    data[0] = 0xfeff;
    for (int i = 0; i < textLen; ++i) {
        data[i+1] = textData[i];
        }
    data[textLen+1] = 0;
    length = (textLen + 1) * sizeof(UCS16Char);
    fileInfo.size = length;
}

TextInputSource::~TextInputSource()
{   delete[] data;
    data = 0;
}

int TextInputSource::read(void *dest, long bytes)
{   if (bytes + pos >= length)
        bytes = length - pos;
    if (bytes > 0)
        memmove(dest, data + pos, bytes);
    pos += bytes;
    return bytes;
}

void TextInputSource::seek(long where)
{   pos = where;
}

long TextInputSource::readCB(void *pData, void *dest, long bytes)
{   TextInputSource *s = TextInputSource::safeCast(pData);
    if (s)
        return s->read(dest, bytes);
    else
        return FAIL;
}

void TextInputSource::seekCB(void *pData, long where)
{   TextInputSource *s = TextInputSource::safeCast(pData);
    if (s)
        s->seek(where);
}

void TextInputSource::releaseCB(void *pData)
{   TextInputSource *s = safeCast(pData);
    if (s)
        delete s;
}

TextInputSource *TextInputSource::safeCast(void *pData)
{   TextInputSource *s = (TextInputSource *) pData;
    if (s && (s->idTextInputSource == TextInputSourceID))
        return s;
    return NULL;
}

void TextInputSource::makeInterface(dtsInputStream& dest)
{   dest.pData = this;
    dest.seek = TextInputSource::seekCB;
    dest.release = TextInputSource::releaseCB;
    dest.read = TextInputSource::readCB;
    dest.filename = fileInfo.filename;
    dest.size = length;
    dest.modified.copy(fileInfo.modified);
    dest.created.copy(fileInfo.created);
}


//
//   CDataSourceBase is an abstract base class that
//   describes the interface between the
//   data source object in the calling program and the
//   dtSearch Engine.
//

class CDataSourceBase {
    public:
        CDataSourceBase();
        void makeInterface(dtsDataSource& dest);
        virtual ~CDataSourceBase();
        virtual int getNextDoc(dtsInputStream& dest) = 0;
        virtual int rewind() = 0;
        static int getNextDocCB(void *pData, dtsInputStream& dest);
        static int rewindCB(void *pData);
        static CDataSourceBase *safeCast(void *pData);
        void makeInterface(dtsInputStream& dest);
    protected:
        long idCDataSourceBase;
    };

const long CDataSourceBaseID = 0x02020202;

CDataSourceBase::CDataSourceBase() :
    idCDataSourceBase(CDataSourceBaseID)
{
}

CDataSourceBase::~CDataSourceBase()
{   idCDataSourceBase = 0;
}

int CDataSourceBase::rewindCB(void *pData)
{   CDataSourceBase *s = CDataSourceBase::safeCast(pData);
    if (s)
        return s->rewind();
    else
        return FAIL;
}

int CDataSourceBase::getNextDocCB(void *pData, dtsInputStream& dest)
{   CDataSourceBase *s = CDataSourceBase::safeCast(pData);
    if (s)
        return s->getNextDoc(dest);
    else
        return FAIL;
}

CDataSourceBase *CDataSourceBase::safeCast(void *pData)
{   CDataSourceBase *s = (CDataSourceBase *) pData;
    if (s && (s->idCDataSourceBase == CDataSourceBaseID))
        return s;
    return NULL;
}

void CDataSourceBase::makeInterface(dtsDataSource& dest)
{   memset(&dest, 0, sizeof dest);
    dest.pData = this;
    dest.rewind = CDataSourceBase::rewindCB;
    dest.getNextDoc = CDataSourceBase::getNextDocCB;
}

class SampleDataSource : public CDataSourceBase {
    public:
        SampleDataSource(long *ids, wchar_t **data);
        virtual int getNextDoc(dtsInputStream& s);
        virtual int rewind();
        static void recordToFilename(char *dest, int destSize, long record);
        static long filenameToRecord(const char *filename);
    protected:
        long *idTable;
        wchar_t **dataTable;
        int pos;
    };

SampleDataSource::SampleDataSource(long *ids, wchar_t **data) :
    idTable(ids),
    dataTable(data),
    pos(0)
{ }

void SampleDataSource::recordToFilename(char *dest, int destSize, long record)
{
#ifdef HAVE_SAFE_SPRINTF
    sprintf_s(dest, destSize, "fil%8.8lX", record);
#else
	sprintf(dest, "fil%8.8lX", record);
#endif
}

long SampleDataSource::filenameToRecord(const char *filename)
{   return strtol(filename+3, 0, 16);
}

int SampleDataSource::getNextDoc(dtsInputStream& dest)
{   if (!idTable[pos])
        return FAIL;

    dtsFileInfo fileInfo;
    recordToFilename(fileInfo.filename, sizeof fileInfo.filename, idTable[pos]);
    fileInfo.modified.year = 1996;
    fileInfo.modified.month = 1;
    fileInfo.modified.day = 1;
    TextInputSource *s = new TextInputSource(fileInfo, dataTable[pos]);
    s->makeInterface(dest);
    pos++;
    return SUCCESS;
}

int SampleDataSource::rewind()
{   pos = 0;
    if (idTable[pos])
        return SUCCESS;
    else
        return FAIL;
}

// These will become hex filenames
long sampleIds[7] = { 0x101, 0x202, 0x303, 0x404, 0x505, 0x606, 0};

// These will become "documents" associated with the filenames
wchar_t *sampleStrings[] = {
    L"This is sample document 101 ",
    L"This is sample document 202 ",
    L"This is sample document 303 ",
    L"This is sample document 404 ",
    L"This is sample document 505 ",
    L"This is sample document 606 \x4e35 ",
    0 };

SampleDataSource sampleData(sampleIds, sampleStrings);

///////////////////////////////////////////////////////////////////////////
//
//  Initalize the dtSearch Engine, build an index of the sample data,
//  and search the sample data.
//

// Used to hold search results passed back from the Engine
struct SearchResults {
    long retrieved[10];
    int count;
    SearchResults();
    void clear();
    void append(long recordNum);
    int wasRetrieved(long recordNum);
    };

SearchResults::SearchResults() :
    count(0)
{ }

void SearchResults::clear()
{   count = 0;
}

void SearchResults::append(long recordNum)
{   if (count < 10)
        retrieved[count++] = recordNum;
}

int SearchResults::wasRetrieved(long recordNum)
{   for (int i = 0; i < count; ++i)
        if (retrieved[i] == recordNum)
            return true;
    return false;
}


class DataSourceDemo {
    public:
        DataSourceDemo(ostream& aOut, const char *aExeName);
        int Run();
    protected:
        int Init();
        void BuildIndex(int create);
        void SearchIndex(const char *request);
        void Shutdown();
        void Remove(long recordNum);
        static int IndexingCallback(void * pData, dtsMessage& msg);
        static int SearchingCallback(void * pData, struct dtsMessage& msg);
        ostream& out;
        DString exeName;
	DString indexPath;
        SearchResults results;
    };

DataSourceDemo::DataSourceDemo(ostream& aOut, const char *aExeName) :
    out(aOut)
{   exeName = aExeName;
}




int DataSourceDemo::IndexingCallback(void * pData, dtsMessage& msg)
{   DataSourceDemo *demo = (DataSourceDemo *) pData;

    dtsIndexProgressInfo *prog;
    prog = (dtsIndexProgressInfo *)msg.ptrParam;
    switch(msg.command) {
        case dtsnCheckForAbort:
            msg.result = dtsContinue;
            return dtsContinue;
        case dtsnIndexStartingFile:
            demo->out << "Indexing " << prog->file.name << nl;
            break;
        case dtsnIndexMergeProgress: // ignore
            break;
        default:
            break;
        }
    return dtsContinue;
}

// Path of index to create.  Terminate with double-null so it
// can also be used for "indexesToSearch" in a dtsSearchJob.

void DataSourceDemo::BuildIndex(int create)
{   if (create)
        out << "Building Index...\n";
    else
        out << "Updating Index...\n";

    dtsIndexJob *indexJob = new dtsIndexJob;
    indexJob->action.create = create;
    indexJob->action.add = true;
    indexJob->pReportData = this;
    indexJob->pReportCallBack = IndexingCallback;
    indexJob->indexingFlags = dtsIndexCacheOriginalFile | dtsIndexCacheText;
#ifdef _DEBUG
// dtsIndexTestDataSource causes the data source to be tested before being indexed.
// To test the data source, the dtSearch Engine will call rewind() and getNextDoc() 
// and the seek() and read() methods of all returned dtsInputStreams to fully read
// the data in the data source
//	indexJob->indexingFlags |= dtsIndexTestDataSource;
#endif    
    
    DFilename fn;
    fn.makeTempName("sample.ix");
    indexPath = fn;
    strcpy_s(indexJob->indexPath, _countof(indexJob->indexPath), indexPath);

    dtsDataSource dataSource;
    sampleData.makeInterface(dataSource);
    indexJob->dataSourceToIndex = &dataSource;

    short result = 0;
    dtssDoIndexJob(*indexJob, result);
    delete indexJob;
}

//
//  Remove a record from the index.
//
void DataSourceDemo::Remove(long recordNum)
{
    char fn[80];
    SampleDataSource::recordToFilename(fn, _countof(fn), recordNum);
    DFilename toRemoveFile;
    toRemoveFile.makeTempName("toremove.tmp");

    ofstream out(toRemoveFile);
    out << fn << nl;
    out.close();

    dtsIndexJob *indexJob = new dtsIndexJob;
    indexJob->action.remove = true;
    strcpy_s(indexJob->toRemoveList, _countof(indexJob->toRemoveList), toRemoveFile);
    indexJob->pReportData = this;
    indexJob->pReportCallBack = IndexingCallback;
    strcpy_s(indexJob->indexPath, _countof(indexJob->indexPath), indexPath);

    short result = 0;
    dtssDoIndexJob(*indexJob, result);
    delete indexJob;
}

//
//   Called from the Engine to get reports of retrieved documents
//   and status updates.
//
//   Converts filenames passed in to record numbers and appends them
//   to the search results.
//
int DataSourceDemo::SearchingCallback(void * pData, struct dtsMessage& msg)
{   DataSourceDemo *demo = (DataSourceDemo *) pData;
    switch(msg.command) {
        case dtsnCheckForAbort:
            msg.result = dtsContinue;
            return dtsContinue;
        case dtsnSearchFound: {
            demo->out << msg.paramA << " hits in " <<
                msg.strParam << nl;
            long record = SampleDataSource::filenameToRecord(msg.strParam);
            demo->results.append(record);
            break;
            }
        default:
            break;
        }
    return dtsContinue;
}

//
//  Search the index.  After the search, results will contain
//  an array of matching records.
//
void DataSourceDemo::SearchIndex(const char *request)
{
    dtsSearchJob *searchJob = new dtsSearchJob;

    results.clear();
    searchJob->pReportCallBack = SearchingCallback;
    searchJob->pReportData = this;
    searchJob->indexesToSearch = indexPath;
    searchJob->action.searchIndexes = true;
    strcpy_s(searchJob->request, _countof(searchJob->request), request);
    searchJob->sortType = dtsSortNone;

    short result = 0;
    out << "Searching for: " << quote << searchJob->request << quote << nl;
    dtssDoSearchJob(*searchJob, result);
    delete searchJob;
}

//
//  Test the dtsDataSource API
//
int DataSourceDemo::Run()
{
//#if DebugLogging
    DFilename logName;
    logName.makeTempName("dsource.log");
    out << "Debug log: " << logName << "\n";
    dtssDebugLogEx(logName, dtsLogCommit);
//#endif
    int ret = SUCCESS;

    // Build index and test with a search
    BuildIndex(true);
    SearchIndex("sample document w/5 404");
    if ((results.count == 1) && (results.wasRetrieved(0x404)))
        out << "Search for record 404 PASSED\n";
    else {
        out << "Search for record 404 FAILED\n";
        ret = FAIL;
        }

    // Remove a record and test that it is NOT retrieved
    Remove(0x404);
    SearchIndex("sample document w/5 404");
    if (results.wasRetrieved(0x404) == 0)
        out << "Search for removed record 404 PASSED\n";
    else {
        out << "Search for removed record 404 FAILED\n";
        ret = FAIL;
        }

    // Update the index and test that the removed record is back
    BuildIndex(false);
    SearchIndex("sample document w/5 404");
    if ((results.count == 1) && (results.wasRetrieved(0x404)))
        out << "Search for updated record 404 PASSED\n";
    else {
        out << "Search for updated record 404 FAILED\n";
        ret = FAIL;
        }
	
	// Search for Unicode text
	char request[64];
	wchar_t *wRequest = L"\x4e35";
	dtssUtf8Encode(request, _countof(request), wRequest, 1, 0);
	SearchIndex(request);
	if ((results.count == 1) && (results.wasRetrieved(0x606)))
		out << "Search for Unicode in record 606 PASSED\n";
	else
		out << "Search for Unicode in record 606 FAILED\n";
    return ret;
}


int main(int, char **argv)
{
    DataSourceDemo demo(cout, argv[0]);
    return demo.Run();
}
