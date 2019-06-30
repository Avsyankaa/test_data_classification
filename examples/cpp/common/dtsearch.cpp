
#include "stdafx.h"
#include <string.h>
#include <stdio.h>

#if defined(_Windows) && !defined(_INC_WINDOWS)
#   include <windows.h>
#endif
#if !defined(DTSEARCH_H)
#include <dtsearch.h>
#endif
#include <dt_util.h>
#include <dtconfig.h>

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


void dtsJobBase::reportError(int code, const char *msg, const char *arg1, const char *arg2)
{
    if (!msg || !*msg)
        msg = dtsErrorInfo::getMessageForCode(code);

    if (errorHandler)
        errorHandler->handleMessage(code, msg, arg1, arg2);
#if defined(DLOGGER_H) && defined(DSTRING_H)
    DString x;
    x << "ERROR #" << code << " " << msg;
    if (arg1 && *arg1)
        x << " " << arg1;
    if (arg2 && *arg2)
        x << " " << arg2;

    debugLog(x);
#endif
}


//
//   These constructors and copy() functions can use memset and memove
//   because the structs are guaranteed not to have virtual functions
//   and because the pointers in them are supposed to be copied
//   as pointers (i.e., not by reallocating the string).
//


dtsFileSpec::dtsFileSpec()
{
    clear();
}

void dtsFileSpec::clear()
{
    memset(this, 0, sizeof(dtsFileSpec));
}

dtsFileSpec& dtsFileSpec::copy(const dtsFileSpec& other)
{
    memmove(this, &other, sizeof(dtsFileSpec));
    return *this;
}

bool dtsFileDate::operator==(const dtsFileDate& other) const
{
    return (day == other.day) && (month == other.month) && (year == other.year) &&
           (hour == other.hour) && (minute == other.minute) && (second == other.second);
}

bool dtsFileDate::operator!=(const dtsFileDate& other) const
{
    return !(*this == other);
}

bool dtsFileDate::operator>(const dtsFileDate& other) const
{
    if (year != other.year)
        return year > other.year;
    if (month != other.month)
        return month > other.month;
    if (day != other.day)
        return day > other.day;
    if (hour != other.hour)
        return hour > other.hour;
    if (minute != other.minute)
        return minute > other.minute;
    if (second != other.second)
        return second > other.second;
    return false;
}


dtsIndexJob::dtsIndexJob()
{
    initStruct<dtsIndexJob>(*this);
}

void dtsIndexJob::clear()
{
    clearStruct<dtsIndexJob>(*this);
}

dtsIndexJob& dtsIndexJob::copy(const dtsIndexJob& other)
{
    copyStruct<dtsIndexJob>(*this, other);
    return *this;
}


dtsSearchJob::dtsSearchJob()
{
    initStruct<dtsSearchJob>(*this);
}

void dtsSearchJob::clear()
{
    clearStruct<dtsSearchJob>(*this);
}

dtsSearchJob& dtsSearchJob::copy(const dtsSearchJob& other)
{
    copyStruct<dtsSearchJob>(*this, other);
    return *this;
};

dtsInitInfo::dtsInitInfo()
{
    initStruct<dtsInitInfo>(*this);
}

dtsIndexInfo::dtsIndexInfo()
{
    initStruct<dtsIndexInfo>(*this);
}

static int _strCompare(const char *a, const char *b)
{
    if (a && b)
        return strcmp(a, b);
    if (!a && !b)
        return 0;
    if (a)
        return 1;
    else
        return -1;
}

dtsOptions::dtsOptions()
{
    initStruct<dtsOptions>(*this);

// setup default values
    indexNumbers = true;
    hyphens = dtsoHyphenAsSpace;
    binaryFiles = dtsoFilterBinaryUnicode;
    maxStoredFieldSize = 512;
    maxFieldNesting = 16;
    maxWordLength = 32;
    maxWordsToRetrieve = 8192*8;
    titleSize = 80;
    _strCopy(stemmingRulesFile, "stemming.dat", sizeof stemmingRulesFile);
    _strCopy(alphabetFile, "default.abc", sizeof alphabetFile);
    _strCopy(noiseWordFile, "noise.dat", sizeof noiseWordFile);
    _strCopy(segmentationRulesFile, "fileseg.xml", sizeof segmentationRulesFile);
    _strCopy(textFieldsFile, "fields.xml", sizeof textFieldsFile);
    _strCopy(userThesaurusFile, "thesaur.xml", sizeof userThesaurusFile);
    _strCopy(fileTypeTableFile, "filetype.xml", sizeof fileTypeTableFile);
    _strCopy(macroFile, "macros.xml", sizeof macroFile);

// Characters used for search features in search requests
    macroChar = '@';
    stemmingChar = '~';
    phonicChar = '#';
    weightChar = ':';
    synonymChar = '&';
    fuzzyChar = '%';
    matchDigitChar = '=';
    storedFieldDelimiterChar = ' ';

// Unicode filtering options
    unicodeFilterBlockSize = 256 * 1024;
    unicodeFilterRanges[0] = 1;
    unicodeFilterRanges[1] = 1;
    unicodeFilterRanges[2] = 1;
    unicodeFilterMinTextSize = 6;
    
    autoFilterSizeMB = 0;

}

static void LimitValue(long& val, long lowBound, long highBound)
{
    if (val < lowBound)
        val = lowBound;
    if (val > highBound)
        val = highBound;
}

//
// Enforce boundaries on option values
void dtsOptions::validate()
{
    LimitValue(maxWordsToRetrieve, 16, dtsMaxWordsToRetrieveLimit);
    LimitValue(maxStoredFieldSize, 4, dtsMaxStoredFieldSize);
    LimitValue(maxFieldNesting, 1, 32);
    LimitValue(titleSize, 4, 2048);
    LimitValue(maxWordLength, 4, 128);
    LimitValue(unicodeFilterBlockSize, 1024, 1024 * 1024 * 20);
    if (unicodeFilterMinTextSize)
        LimitValue(unicodeFilterMinTextSize, 3, 1024);
}

void dtsOptions::copy(const dtsOptions& other)
{
    copyStruct<dtsOptions>(*this, other, sizeof(dtsOptions));
}


bool dtsOptions::equals(const dtsOptions& other) const
{
    if (other.structSize != structSize)
        return false;
    if (!memcmp(this, &other, structSize))
        return true;

    if (binaryFiles != other.binaryFiles)
        return false;
    if (_strCompare(binaryFilterTextChars, other.binaryFilterTextChars))
        return false;
    if (hyphens != other.hyphens)
        return false;
    if (_strCompare(alphabetFile, other.alphabetFile))
        return false;
    if (indexNumbers != other.indexNumbers)
        return false;
    if (_strCompare(noiseWordFile, other.noiseWordFile))
        return false;
    if (_strCompare(stemmingRulesFile, other.stemmingRulesFile))
        return false;
    if (maxWordsToRetrieve != other.maxWordsToRetrieve)
        return false;
    if (maxStoredFieldSize != other.maxStoredFieldSize)
        return false;
    if (titleSize != other.titleSize)
        return false;
    if (_strCompare(xmlIgnoreTags, other.xmlIgnoreTags))
        return false;
    if (maxWordLength != other.maxWordLength)
        return false;
    if (_strCompare(segmentationRulesFile, other.segmentationRulesFile))
        return false;
    if (_strCompare(textFieldsFile, other.textFieldsFile))
        return false;
    if (_strCompare(userThesaurusFile, other.userThesaurusFile))
        return false;
    if (_strCompare(homeDir, other.homeDir))
        return false;
    if (_strCompare(privateDir, other.privateDir))
        return false;

    if (_strCompare(tempFileDir, other.tempFileDir))
        return false;
    if (_strCompare(booleanConnectors, other.booleanConnectors))
        return false;
    if (_strCompare(fileTypeTableFile, other.fileTypeTableFile))
        return false;
    if (textFlags != other.textFlags)
        return false;
    if (maxFieldNesting != other.maxFieldNesting)
        return false;
    if (_strCompare(macroFile, other.macroFile))
        return false;
    if (macroChar != other.macroChar)
        return false;
    if (fuzzyChar != other.fuzzyChar)
        return false;
    if (phonicChar != other.phonicChar)
        return false;
    if (stemmingChar != other.stemmingChar)
        return false;
    if (synonymChar != other.synonymChar)
        return false;
    if (weightChar != other.weightChar)
        return false;
    if (matchDigitChar != other.matchDigitChar)
        return false;
    if (fieldFlags != other.fieldFlags)
        return false;
    if (memcmp(unicodeFilterRanges, other.unicodeFilterRanges, sizeof other.unicodeFilterRanges))
        return false;
    if (unicodeFilterBlockSize != other.unicodeFilterBlockSize)
        return false;
    if (unicodeFilterFlags != other.unicodeFilterFlags)
        return false;
    if (unicodeFilterMinTextSize != other.unicodeFilterMinTextSize)
        return false;
    if (unicodeFilterWordOverlapAmount != other.unicodeFilterWordOverlapAmount)
        return false;
    return true;
}


dtsOptions2::dtsOptions2()
{
    initStruct<dtsOptions2>(*this);

// setup default values
    indexNumbers = true;
    hyphens = dtsoHyphenAsSpace;
    binaryFiles = dtsoFilterBinaryUnicode;
    maxStoredFieldSize = 512;
    maxFieldNesting = 16;
    maxWordLength = 32;
    maxWordsToRetrieve = 8192*8;
    titleSize = 80;
    stemmingRulesFile = "stemming.dat";
    alphabetFile = "default.abc";
    noiseWordFile = "noise.dat";
    segmentationRulesFile = "fileseg.xml";
    textFieldsFile = "fields.xml";
    userThesaurusFile = "thesaur.xml";
    fileTypeTableFile = "filetype.xml";
    macroFile = "macros.xml";

// Characters used for search features in search requests
    macroChar = '@';
    stemmingChar = '~';
    phonicChar = '#';
    weightChar = ':';
    synonymChar = '&';
    fuzzyChar = '%';
    matchDigitChar = '=';
    storedFieldDelimiterChar = ' ';

// Unicode filtering options
    unicodeFilterBlockSize = 256 * 1024;
    unicodeFilterRanges[0] = 1;
    unicodeFilterRanges[1] = 1;
    unicodeFilterRanges[2] = 1;
    unicodeFilterMinTextSize = 6;
    
    autoFilterSizeMB = 0;
}

// Copy values other than variable-length string values from the old dtsOptions structure
void dtsOptions2::copyFixedSizeValues(const dtsOptions& other) 
{
	binaryFiles = (int) other.binaryFiles;
	memmove(&binaryFilterTextChars, &other.binaryFilterTextChars, sizeof binaryFilterTextChars);
	hyphens = (int) other.hyphens;
	indexNumbers = (int) other.indexNumbers;
	maxWordsToRetrieve = (int) other.maxWordsToRetrieve;
	maxStoredFieldSize = (int) other.maxStoredFieldSize;
	titleSize = (int) other.titleSize;
	maxWordLength = (int) other.maxWordLength;
	updateFiles = (int) other.updateFiles;
	textFlags = (int) other.textFlags;
	maxFieldNesting = (int) other.maxFieldNesting;

	autoFilterSizeMB = (int) other.autoFilterSizeMB;
	otherFlags = (int) other.otherFlags;
	macroChar = other.macroChar;
	fuzzyChar = other.fuzzyChar;
	phonicChar = other.phonicChar;
	stemmingChar = other.stemmingChar;
	synonymChar = other.synonymChar;
	weightChar = other.weightChar;
	matchDigitChar = other.matchDigitChar;
	storedFieldDelimiterChar = other.storedFieldDelimiterChar;
	fieldFlags = (int) other.fieldFlags;
	memmove(&unicodeFilterRanges, &other.unicodeFilterRanges, sizeof unicodeFilterRanges);
	unicodeFilterBlockSize = (int) other.unicodeFilterBlockSize;
	unicodeFilterFlags = (int) other.unicodeFilterFlags;
	unicodeFilterMinTextSize = (int) other.unicodeFilterMinTextSize;
	pAnalyzer = other.pAnalyzer;
	unicodeFilterWordOverlapAmount = (int) other.unicodeFilterWordOverlapAmount;
	viewers = other.viewers;
	}


static void LimitValue(int& val, int lowBound, int highBound)
{
    if (val < lowBound)
        val = lowBound;
    if (val > highBound)
        val = highBound;
}

//
// Enforce boundaries on option values
void dtsOptions2::validate()
{
    LimitValue(maxWordsToRetrieve, 16, dtsMaxWordsToRetrieveLimit);
    LimitValue(maxStoredFieldSize, 4, dtsMaxStoredFieldSize);
    LimitValue(maxFieldNesting, 1, 32);
    LimitValue(titleSize, 4, 2048);
    LimitValue(maxWordLength, 4, 128);
    LimitValue(unicodeFilterBlockSize, 1024, 1024 * 1024 * 20);
    if (unicodeFilterMinTextSize)
        LimitValue(unicodeFilterMinTextSize, 3, 1024);
}

void dtsOptions2::copy(const dtsOptions2& other)
{
    copyStruct<dtsOptions2>(*this, other, sizeof(dtsOptions2));
}




bool dtsOptions2::equals(const dtsOptions2& other) const
{
    if (other.structSize != structSize)
        return false;
    if (!memcmp(this, &other, structSize))
        return true;

    if (binaryFiles != other.binaryFiles)
        return false;
    if (_strCompare(binaryFilterTextChars, other.binaryFilterTextChars))
        return false;
    if (hyphens != other.hyphens)
        return false;
    if (_strCompare(alphabetFile, other.alphabetFile))
        return false;
    if (indexNumbers != other.indexNumbers)
        return false;
    if (_strCompare(noiseWordFile, other.noiseWordFile))
        return false;
    if (_strCompare(stemmingRulesFile, other.stemmingRulesFile))
        return false;
    if (maxWordsToRetrieve != other.maxWordsToRetrieve)
        return false;
    if (maxStoredFieldSize != other.maxStoredFieldSize)
        return false;
    if (titleSize != other.titleSize)
        return false;
    if (_strCompare(xmlIgnoreTags, other.xmlIgnoreTags))
        return false;
    if (maxWordLength != other.maxWordLength)
        return false;
    if (_strCompare(segmentationRulesFile, other.segmentationRulesFile))
        return false;
    if (_strCompare(textFieldsFile, other.textFieldsFile))
        return false;
    if (_strCompare(userThesaurusFile, other.userThesaurusFile))
        return false;
    if (_strCompare(homeDir, other.homeDir))
        return false;
    if (_strCompare(privateDir, other.privateDir))
        return false;

    if (_strCompare(tempFileDir, other.tempFileDir))
        return false;
    if (_strCompare(booleanConnectors, other.booleanConnectors))
        return false;
    if (_strCompare(fileTypeTableFile, other.fileTypeTableFile))
        return false;
    if (textFlags != other.textFlags)
        return false;
    if (maxFieldNesting != other.maxFieldNesting)
        return false;
    if (_strCompare(macroFile, other.macroFile))
        return false;
    if (macroChar != other.macroChar)
        return false;
    if (fuzzyChar != other.fuzzyChar)
        return false;
    if (phonicChar != other.phonicChar)
        return false;
    if (stemmingChar != other.stemmingChar)
        return false;
    if (synonymChar != other.synonymChar)
        return false;
    if (weightChar != other.weightChar)
        return false;
    if (matchDigitChar != other.matchDigitChar)
        return false;
    if (fieldFlags != other.fieldFlags)
        return false;
    if (memcmp(unicodeFilterRanges, other.unicodeFilterRanges, sizeof other.unicodeFilterRanges))
        return false;
    if (unicodeFilterBlockSize != other.unicodeFilterBlockSize)
        return false;
    if (unicodeFilterFlags != other.unicodeFilterFlags)
        return false;
    if (unicodeFilterMinTextSize != other.unicodeFilterMinTextSize)
        return false;
    if (unicodeFilterWordOverlapAmount != other.unicodeFilterWordOverlapAmount)
        return false;
    return true;
}


dtsRetrievedFileTable::dtsRetrievedFileTable(long aMaxCount) :
        fileTable(0),
        sortOrder(0),
        maxCount(0),
        fileCount(0)
{
    allocTable(aMaxCount);
}

void dtsRetrievedFileTable::copy(const dtsRetrievedFileTable& other)
{
    fileCount = other.fileCount;
    allocTable(other.maxCount);
    memmove(fileTable, other.fileTable, sizeof(dtsRetrievedFile) * other.maxCount);
    memmove(sortOrder, other.sortOrder, (sizeof sortOrder[0]) * other.maxCount);
}

void dtsRetrievedFileTable::allocTable(long aMaxCount)
{
    if (fileTable) {
        delete [] fileTable;
        fileTable = 0;
    }
    if (sortOrder) {
        delete [] sortOrder;
        sortOrder = 0;
    }
    if (aMaxCount) {
        fileTable = new dtsRetrievedFile[aMaxCount];
        sortOrder = new long[aMaxCount];
    }
    maxCount = aMaxCount;
}

dtsRetrievedFileTable::~dtsRetrievedFileTable()
{
    allocTable(0);
}

void dtsRetrievedFileTable::clear()
{
    fileCount = 0;
}

dtsRetrievedFile *dtsRetrievedFileTable::getInOrder(int i)
{
    if ((i >= 0) && (i < fileCount)) {
        long j = sortOrder[i];
        return &fileTable[j];
    }
    return 0;
}

dtsFileConvertJob::dtsFileConvertJob()
{
    initStruct<dtsFileConvertJob>(*this);
}

dtsFileConvertJob2::dtsFileConvertJob2()
{
    initStruct<dtsFileConvertJob2>(*this);
}

dtsIndexMergeJob::dtsIndexMergeJob()
{
    initStruct<dtsIndexMergeJob>(*this);
}

dtsMergeStatusInfo::dtsMergeStatusInfo()
{
    initStruct<dtsMergeStatusInfo>(*this);
}

dtsIndexVerifyJob::dtsIndexVerifyJob()
{
    initStruct<dtsIndexVerifyJob>(*this);
}

dtsVerifyStatusInfo::dtsVerifyStatusInfo()
{
    initStruct<dtsVerifyStatusInfo>(*this);
}

dtsIndexProgressInfo::dtsIndexProgressInfo()
{
    initStruct<dtsIndexProgressInfo>(*this);
}

// Copy only the integer values
// char * values are not safe to copy this way, so clear them after copying
// the struct
void dtsIndexProgressInfo::copyNumbers(const dtsIndexProgressInfo& other)
{ 	copyStruct<dtsIndexProgressInfo>(*this, other, sizeof(dtsIndexProgressInfo));
	indexName = 0;
    indexPath = 0;
    driveName = 0;
	userFields = 0;
	file.name = 0;
	file.location = 0;
	file.type = 0;
	file.openFailMessage = 0;
}


dtsMapHitsJob::dtsMapHitsJob(dtsSearchResultsItem& aItem)
{
    initStruct<dtsMapHitsJob>(*this);

    filename = aItem.filename;

    table = new dtsHitLocation[aItem.hitCount];
    nTableEntries = aItem.hitCount;

    for (long i = 0; i < aItem.hitCount; ++i)
        table[i].wordOffset = aItem.hits[i];
}

dtsMapHitsJob::dtsMapHitsJob(long tableSize)
{
    initStruct<dtsMapHitsJob>(*this);

    table = new dtsHitLocation[tableSize];
    nTableEntries = tableSize;
    filename = 0;
}

dtsMapHitsJob::~dtsMapHitsJob()
{
    delete[] table;
    table = 0;
    nTableEntries = 0;
}

dtsErrorInfo::dtsErrorInfo(int aMaxMessages, int aBufSize) :
        structSize(sizeof (dtsErrorInfo) ),
        pUserData(0),
        messageCode(new long[aMaxMessages]),
        buf(new char[aBufSize]),
        messageText(new const char *[aMaxMessages]),
        messageArg1(new const char *[aMaxMessages]),
        messageArg2(new const char *[aMaxMessages]),
        bufSize(aBufSize),
        maxMessages(aMaxMessages),
        pNotifyFn(0),
        messageCount(0),
        textSize(0),
        fOverflowed(0)
{
    memset(&reserved, 0, sizeof reserved);
}

void dtsErrorInfo::clear()
{
    messageCount = 0;
    textSize = 0;
    fOverflowed = 0;
}

dtsErrorInfo::~dtsErrorInfo()
{
    delete[] messageCode;
    delete[] buf;
    delete [] messageText;
    delete [] messageArg1;
    delete [] messageArg2;
}

int dtsErrorInfo::overflowed() const
{
    return (fOverflowed ? 1 : 0);
}

void dtsErrorInfo::setNotify(int (*aNotifyFn)(dtsErrorInfo *pHandler))
{
    pNotifyFn = aNotifyFn;
}

long dtsErrorInfo::getCount() const
{
    return messageCount;
}

int dtsErrorInfo::handleMessage(long code, const char *msg, const char *arg1,
                                const char *arg2)
{
    if (messageCount >= maxMessages) {
        fOverflowed = true;
        if (pNotifyFn)
            // Since there is a notify function, it is useful to send all of the
            // messages.  Otherwise, just stop storing messages when the buffer fills
            clear();
        else
            return -1;
    }
// if msg is not formated in correct message format, add the necessary
// codes.  Messages are supposed to start with NNNN $X where
// X = message type (Q = question, I = info, E = error, D = Diagnostic)
// NNNN = message code
    if (!msg)
        msg = "";

    if (msg[0] != '$') {
        int msgsize = (int) strlen(msg) + 64;
        char *tmp = new char[msgsize+1];
#ifdef HAVE_SAFE_SPRINTF
        _snprintf_s(tmp, msgsize, _TRUNCATE, "$E %4.4ld %s", code, msg);
#else
        _snprintf(tmp, msgsize, "$E %4.4ld %s", code, msg);
#endif
        messageText[messageCount] = storeString(tmp);
        delete [] tmp;
    }
    else
        messageText[messageCount] = storeString(msg);

    messageArg1[messageCount] = storeString(arg1);
    messageArg2[messageCount] = storeString(arg2);
    messageCode[messageCount] = code;
    messageCount++;
    if (pNotifyFn)
        return pNotifyFn(this);
    else
        return -1;
}

const char *dtsErrorInfo::storeString(const char *s)
{
    if (!s)
        s = "";
    ptrdiff_t l = strlen(s) + 1;
    const char *ret = "";
    if (textSize + l < bufSize) {
        _strCopy(buf + textSize, s, (int) (bufSize - textSize));
        ret = buf + textSize;
        textSize += (long) l;
    }
    else
        fOverflowed = true;
    return ret;
}

const char *dtsErrorInfo::getMessage(long iMessage) const
{   // Default is to get the last message
    if (iMessage == -1)
        iMessage = messageCount-1;

    if ((iMessage < messageCount) && (iMessage >= 0))
        return messageText[iMessage];
    else
        return "";
}

long dtsErrorInfo::getMessageCode(long iMessage) const
{   // Default is to get the last message
    if (iMessage == -1)
        iMessage = messageCount-1;
    if ((iMessage < messageCount) && (iMessage >= 0))
        return messageCode[iMessage];
    else
        return 0;
}

bool dtsErrorInfo::haveMessage(long code) const
{
    for (int i = 0; i < messageCount; ++i) {
        if (messageCode[i] == code)
            return true;
    }
    return false;
}

const char *dtsErrorInfo::getMessageArg1(long iMessage) const
{   // Default is to get the last message
    if (iMessage == -1)
        iMessage = messageCount-1;
    if ((iMessage < messageCount) && (iMessage >= 0))
        return messageArg1[iMessage];
    else
        return "";
}

const char *dtsErrorInfo::getMessageArg2(long iMessage) const
{   // Default is to get the last message
    if (iMessage == -1)
        iMessage = messageCount-1;
    if ((iMessage < messageCount) && (iMessage >= 0))
        return messageArg2[iMessage];
    else
        return "";
}

void dtsErrorInfo::forwardTo(dtsErrorInfo& other) const
{
    for (int i = 0; i < getCount(); ++i)
        other.handleMessage(getMessageCode(i), getMessage(i), getMessageArg1(i), getMessageArg2(i));
}

void dtsErrorInfo::copy(const dtsErrorInfo& other)
{
    clear();
    if (other.textSize >= bufSize) {
        delete [] buf;
        bufSize = other.textSize + 1;
        buf = new char[bufSize];
    }
    if (other.messageCount > maxMessages) {
        delete [] messageCode;
        delete [] messageText;
        delete [] messageArg1;
        delete [] messageArg2;
        maxMessages = other.messageCount;
        messageCode = new long[maxMessages];
        messageText = new const char *[maxMessages];
        messageArg1 = new const char *[maxMessages];
        messageArg2 = new const char *[maxMessages];
    }
    pNotifyFn = 0;
    for (int i = 0; i < other.messageCount; ++i) {
        handleMessage(other.getMessageCode(i),
                      other.getMessage(i),
                      other.getMessageArg1(i),
                      other.getMessageArg2(i));
    }
    pNotifyFn = other.pNotifyFn;
    structSize = sizeof(dtsErrorInfo);
    fOverflowed = other.fOverflowed;
}

// dtsSearchResultsItem is used to obtain information about search results from the
// dtSearch Engine.  If the dtSearch Engine DLL is a different version, structures
// returned from the engine may be larger or smaller than the calling program
// expects.  Therefore (1) check the structSize on copy() and clear() operations, and
// (2) preserve the structSize in the structure.
dtsSearchResultsItem::dtsSearchResultsItem()
{
    initStruct<dtsSearchResultsItem>(*this);
}

void dtsSearchResultsItem::copy(const dtsSearchResultsItem& other)
{
    copyStruct<dtsSearchResultsItem>(*this, other, sizeof(dtsSearchResultsItem));
}


void dtsSearchResultsItem::clear()
{
    clearStruct<dtsSearchResultsItem>(*this);
}

//  A file map is a table mapping word counts to binary offsets and page/paragraph
//  offsets in a document.  This makes it possible to find hits in long documents
//  more quickly.
void dtsSearchResultsItem::getFileMapEntryForWord(long w, dtsFileMapEntry& dest)
{
    dest.clear();

    if (!fileMapEntryCount || !fileMapTable)
        return;

// If the first entry in the file map table is past the word,
// return the blank dtsFileMapEntry, which indicates the start of the file.
    if (fileMapTable[0].wordCount > w)
        return;

    for (int p = 0; p < fileMapEntryCount; ++p) {
        dtsFileMapEntry& fme = fileMapTable[p];
        if (fme.wordCount <= w) {
            if ((p == fileMapEntryCount-1) ||
                    (w <= fileMapTable[p+1].wordCount)) {

                dest = fme;
                return;
            }
        }
    }
}

static int isBlank(const char *p)
{
    if (!p || !*p)
        return true;
    if ((*p > 32) || (*p < 0))
        return false;

    while (*p) {
		if (*p < 0)
			return false;
        if (!isspace(*p))
            return false;
        p++;
    }
    return true;
}

static const char *findTail(const char *fn)
{
    if (!fn || !*fn)
        return 0;
    ptrdiff_t last = strlen(fn)-1;
    if (last == 0)
        return fn;

    for (ptrdiff_t i =  last; i >= 0; --i) {
        if  ((fn[i] == '\\') || (fn[i] == '/'))
            return fn + i + 1;
    }
    return fn;
}

const char *dtsSearchResultsItem::getDisplayName(int fPdfUseTitleAsName, int fHtmlUseTitleAsName) const
{
    const char *ret = findTail(filename);
    if (!ret)
        return filename;
// check for container filename
    if (strchr(ret, '>') && strchr(ret, '|')) {
        ret = strrchr(ret, '|');
        ret++;
    }
    bool bHaveDisplayName = (!isBlank(displayName) && _stricmp(displayName, filename));
    if (typeId == it_PDF) {
        if (fPdfUseTitleAsName && bHaveDisplayName)
            ret = displayName;
    }
    else if (typeId == it_HTML) {
        if (fHtmlUseTitleAsName && bHaveDisplayName)
            ret = displayName;
    }
    else if (bHaveDisplayName)
        ret = displayName;
    return ret;
}

const char *dtsSearchResultsItem::getTitle(int fPdfUseTitleAsName, int fHtmlUseTitleAsName) const
{
    const char *ret = title;
    if ((typeId == it_PDF) && (fPdfUseTitleAsName == 0) && !isBlank(displayName))
        ret = displayName;
    if ((typeId == it_HTML) && (fHtmlUseTitleAsName == 0) && !isBlank(displayName))
        ret = displayName;
    return ret;
}

// Get a string that describes the type name
// Note: The same type name may have more than one parser (for example,
// Microsoft Word), so multiple typeId values may map to a single string.
const char *dtsSearchResultsItem::getTypeName()
{
    int id = (int) typeId;
    if ((id == it_RTF) && strchr(filename, '|'))
        id = it_DatabaseRecord;
    return getTypeName(id);
}

const char *dtsSearchResultsItem::getTypeName(int typeId) {
    switch (typeId) {
    case it_Ami              :
        return "Ami Pro";
    case it_Ansi             :
        return "Ansi";
    case it_Ascii            :
        return "Ascii";
    case it_Binary           :
        return "Binary";
    case it_CompoundDoc      :
        return "Office";
    case it_XBaseAsDocument:
    case it_XBase: 
    case it_DBF              :
        return "XBase";
    case it_FilteredBinary   :
        return "Binary";
    case it_HyperText        :
        return "Hypertext";
    case it_MS_Word          :
        return "Microsoft Word";
    case it_MS_Works         :
        return "Microsoft Works";
    case it_Multimate        :
        return "Multimate";
    case it_RTF              :
        return "RTF";
    case it_WS_2000          :
        return "WordStar 2000";
    case it_WS_5             :
        return "WordStar";
    case it_WordForDos:
        return "Word for DOS";
    case it_WordForWindows2: 
		return "Microsoft Word 2.0";
    case it_WordForWindows1: 
		return "Microsoft Word 1.0";
    case it_WordForWin       :
        return "Microsoft Word";
    case it_WordForWin6      :
        return "Microsoft Word 6.0";
    case it_WordForWin97	 :
        return "Microsoft Word";
    case it_MicrosoftWord	 :
        return "Microsoft Word";
    case it_DocFile          :
        return "Microsoft DocFile";
    case it_WordPerfect42    :
        return "WordPerfect 4.2";
    case it_WordPerfect5     :
        return "WordPerfect 5";
    case it_WordPerfect6     :
        return "WordPerfect";
    case it_WordStar         :
        return "WordStar";
    case it_XyWrite          :
        return "XyWrite";
    case it_ZIP              :
        return "ZIP";
    case it_Properties       :
        return "Document Summary";
    case it_Excel5           :
        return "Microsoft Excel 5";
    case it_HTML             :
        return "HTML";
    case it_PDF              :
        return "PDF";
    case it_PdfWithAttachments:
		return "PDF with attachments";
    case it_Excel97          :
        return "Microsoft Excel 97-2003";
    case it_PowerPoint       :
        return "Microsoft PowerPoint";
    case it_PowerPoint3      :
    	return "Microsoft PowerPoint 3";
    case it_PowerPoint4      :
    	return "Microsoft PowerPoint 4";
    case it_PowerPoint95      :
    	return "Microsoft PowerPoint 95";
    case it_EncryptedHtml    :
        return "HTML*";
    case it_DatabaseRecord   :
        return "Database Record";
    case it_SegmentedText    :
        return "Segmented Text";
    case it_XML              :
        return "XML";
    case it_WordPerfectEmbedded :
        return "WordPerfect";
    case it_Unicode         :
        return "Unicode";
    case it_EudoraMessage    :
        return "Eudora Message";
    case it_OutlookExpressMessage :
        return "Outlook Express Message";
    case it_Utf8                :
        return "UTF-8";
    case it_DjVu             :
        return "DjVu";
    case it_SingleByteText   :
        return "Text";
    case it_FilteredBinaryUnicode:
		return "Filtered Binary";
    case it_ZIP_zlib	   :
        return "ZIP";
    case it_OutlookMsgFile :
        return "Outlook MSG";
    case it_TreepadHjtFile :
        return "TreePad HJT";
    case it_PfsProfessionalWrite :
        return "PFS Write";
    case it_FilteredBinaryUnicodeStream :
        return "Filtered Binary";
    case it_Media :
        return "Music/Video";
    case it_NonTextData :
        return "Data";
    case it_DatabaseRecord2:
        return "Database Record";
    case it_Excel2003Xml:
        return "Microsoft Excel 2003 XML";
    case it_Word2003Xml:
        return "Microsoft Word 2003 XML";
    case it_OpenOfficeDocument:
        return "OpenOffice Document";
    case it_IFilter:
        return "IFilter";
    case it_Word12:
        return "Word 2007";
    case it_Excel12:
        return "Excel 2007";
    case it_Excel4:
        return "Excel 4";
    case it_Excel3:
        return "Excel 3";
    case it_Excel2:
        return "Excel 2";
    case it_PowerPoint12:
        return "PowerPoint 2007";
    case it_TNEF:
        return "TNEF message";
    case it_XPS:
        return "Microsoft XPS";
    case it_WMF:
        return "Windows Metafile Format";
    case it_EMF:
        return "Enhanced Metafile Format";
    case it_TAR:
        return "TAR Archive";
    case it_Gzip:
        return "GZIP Archive";
    case it_CSV:
    case it_CsvAsDocument:
        return "Comma-Separated Values (CSV)";
    case it_Lotus123:
    	return "Lotus 123";
    case it_QuattroPro:
    	return "Quattro Pro";  // *.qpbw, versions after 9
    case it_QuattroPro8:
    	return "Quattro Pro (1-8)";  // *.wb1, *.wb2, *.wb3 
	case it_MsWorksWps4:
		return "Microsoft Works WPS 4/5";
	case it_MsWorksWps6:
		return "Microsoft Works WPS 6/7/8/9";
	case it_MicrosoftAccess2:
	case it_MicrosoftAccessAsDocument:
		return "Microsoft Access";
	case it_FlashSWF:
		return "Flash SWF";
	case it_RAR:
		return "RAR Archive";
	case it_NoContent:
		return "No Content";
	case it_Excel12xlsb: 
		return "Excel 2007 XLSB";
	case it_ASF:
		return "Windows Media File";
	case it_MP3:
		return "MP3 File";
	case it_QuickTime:
		return "QuickTime File";
	case it_MP4:
		return "MP4 File";
	case it_M4A:
		return "M4A File";
	case it_MPG:
		return "MPG File";
	case it_JPEG:
		return "JPEG Image";
	case it_TIFF:
		return "TIFF Image";
	case it_MDI:
		return "Microsoft Document Imaging";
	case it_GIF:
		return "GIF Image";
	case it_PNG:
		return "PNG Image";
	case it_CAB:
		return "CAB Archive";
	case it_dtSearchIndex:
		return "dtSearch Index File";
	case it_HtmlHelp:
		return "HTML Help File";
	case it_TrueTypeFont:
		return "True Type Font File";
	case it_WAV:
		return "WAV Sound File";
	case it_BMP:
		return "BMP Image";
	case it_AVI:
		return "Animation File";	
	case it_VisioXml:
		return "Visio 2003 XML";
	case it_Visio:
		return "Visio";
	case it_Ichitaro:
		return "Ichitaro";
	case it_Ichitaro5:
		return "Ichitaro (5-7)";
	case it_SolidWorks:
		return "SolidWorks CAD file";	
	case it_Unigraphics:
		return "Unigraphics CAD file";
	case it_Unigraphics2:
		return "Unigraphics CAD file";
	case it_DWG:
		return "AutoCAD DWG file";
	case it_DXF:
		return "AutoCAD DXF file";
	case it_DWF:
		return "AutoCAD DWF file";
	case it_OutlookPst: 
		return "Outlook PST";
	case it_MifFile:
		return "Framemaker MIF";
    case it_MBoxArchive    :
        return "MBOX Archive (dtSearch 7.50 and earlier)";
	case it_MBoxArchive2:
		return "MBOX Archive";
	case it_MIDI: 
		return "MIDI File";
    case it_MimeMessage      :
        return "MIME file (dtSearch 6.4 and earlier)";
    case it_EML 			:
        return "MIME file (email or MHT)";
	case it_MimeContainer:
		return "MIME Container";
	case it_OutlookMsgAsContainer:
		return "Outlook MSG Container";
	case it_MicrosoftOfficeThemeData:
		return "Microsoft Office Theme Data";
	case it_OneNote2003:
		return "Microsoft OneNote 2003";
	case it_OneNote2007:
		return "Microsoft OneNote 2007";
	case it_OneNote2010:
		return "Microsoft OneNote 2010";
	case it_OneNoteOnline:
		return "Microsoft OneNote Online";
	case it_JpegXR:
		return "JPEG XR";
	case it_WindowsExecutable:
		return "Windows Executable";
	case it_ElfExecutable:
		return "ELF Executable";
	case it_ICalendar:
		return "iCalendar";
	case it_MicrosoftPublisher:
		return "Microsoft Publisher";
	case it_MicrosoftPhotoEditor:
		return "Microsoft Photo Editor";
	case it_OutlookExpressMessageStore2:
		return "Outlook Express";
	case it_PhotoshopImage:
		return "Photoshop Image";
	case it_Cals:
		return "CALS metadata (MIL-STD-1840C)";
	case it_iWork2009:
		return "Apple iWork 2009";
	case it_iWork2009Pages:
		return "Apple Pages 2009";
	case it_iWork2009Numbers:
		return "Apple Numbers 2009";
	case it_iWork2009Keynote:
		return "Apple Keynote 2009";
	case it_Visio2013:
		return "Microsoft Visio 2013";
	case it_SASF:
		return "SASF file";
	case it_OpenOfficeTextDocument:
		return "OpenOffice Text Document";
	case it_OpenOfficeSpreadsheetDocument:
		return "OpenOffice Spreadsheet Document";
	case it_OpenOfficePresentationDocument:
		return "OpenOffice Presentation Document";
	case it_OleDataMso:
		return "OLE Data MSO object";
	default:
        return " ";
    }
}


dtsFileMapEntry::dtsFileMapEntry()
{
    memset(this, 0, sizeof(dtsFileMapEntry));
}

void dtsFileMapEntry::clear()
{
    memset(this, 0, sizeof(dtsFileMapEntry));
}

int dtsFileMapEntry::operator==(const dtsFileMapEntry& other) const
{
    return (wordCount == other.wordCount);
}

dtsFileMapEntry::dtsFileMapEntry(const dtsFileMapEntry& other)
{
    *this = other;
}

dtsFileMapEntry& dtsFileMapEntry::operator=(const dtsFileMapEntry& other)
{
    memmove(this, &other, sizeof(dtsFileMapEntry));
    return *this;
}

dtsExtractionOptions::dtsExtractionOptions()
{	initStruct<dtsExtractionOptions>(*this);
}

void dtsExtractionOptions::clear()
{
    clearStruct<dtsExtractionOptions>(*this);
}

void dtsExtractionOptions::copy(const dtsExtractionOptions& other)
{
    copyStruct<dtsExtractionOptions>(*this, other);
}


dtsOutputInfo::dtsOutputInfo()
{
    initStruct<dtsOutputInfo>(*this);
}

void dtsOutputInfo::clear()
{
    clearStruct<dtsOutputInfo>(*this);
}

void dtsOutputInfo::copy(const dtsOutputInfo& other)
{
    copyStruct<dtsOutputInfo>(*this, other);
}

dtsSearchReportJob::dtsSearchReportJob()
{
    initStruct<dtsSearchReportJob>(*this);
}

void dtsSearchReportJob::copy(const dtsSearchReportJob& other)
{
    copyStruct<dtsSearchReportJob>(*this, other);
}

dtsOnIndexWordInfo::dtsOnIndexWordInfo()
{
    initStruct<dtsOnIndexWordInfo>(*this);
}

dtsCrashHandlerInfo::dtsCrashHandlerInfo()
{
    initStruct<dtsCrashHandlerInfo>(*this);
}

void dtsCrashHandlerInfo::copy(const dtsCrashHandlerInfo& other)
{
    copyStruct<dtsCrashHandlerInfo>(*this, other);
}

dtsListIndexJob::dtsListIndexJob()
{
    initStruct<dtsListIndexJob>(*this);
}

void dtsIndexInfo::clear()
{
    initStruct<dtsIndexInfo>(*this);
}

void dtsIndexInfo::copy(const dtsIndexInfo& other)
{	copyStruct<dtsIndexInfo>(*this, other);
}

dtsSearchProgressInfo::dtsSearchProgressInfo() {
    initStruct<dtsSearchProgressInfo>(*this);
}

void dtsSearchProgressInfo::copy(const dtsSearchProgressInfo& other) 
{	copyStruct<dtsSearchProgressInfo>(*this, other);
}

dtsSearchFileInfo::dtsSearchFileInfo() {
    initStruct<dtsSearchFileInfo>(*this);
}


const char *dtsErrorInfo::getMessageForCode(int	code) {
    switch (code) {
	case dtsErWrFile:
		return "Unable to write output file";
	case dtsErAccIndex:
        return "Unable to access index ";
    case dtsAskDiskFullOverride:
        return "There may not be enough space to update index.  Continue anyway?";
    case dtsErOpenTemp:
        return "Unable to open temporary file.  ";
    case dtsErIxWrongVersion:
        return "Index built by incompatible version of dtSearch";
    case dtsErIxInterrupted:
        return "Index corrupt because indexing was interrupted.";
    case dtsErAccDirectory:
        return "Unable to create or access directory";
    case dtsErAccFile:
        return "Unable to access input file";
    case dtsErIndexFull:
        return "Index is full";
    case dtsErPdfLzwNotLicensed:
        return "Attempt to index a PDF file with LZW and LZW decoding is not licensed";
    case dtsErTimeout:
        return "Operation timed out";
    case dtsErCreateFailed:
        return "Unable to create index";
    case dtsErCommitFailed:
        return "Unable to commit changes to index";
    case dtsErLowMemory:
        return "Search halted due to low memory available on system";
    case dtsErBadRequest:
        return "Syntax error in search request";
    case dtsErSearchRequestEmpty:
    	return "The search request contained no text";
    case dtsErSearchLimitReached :
        return "Search found more files than the limit for the search job";
    case dtsErNoFilesRetrieved:
        return "No files retrieved in search";
    case dtsErRequestTooLong:
        return "Search request was too long";
    case dtsErMaxWords:
        return "Too many words retrieved in index";
    case dtsErDiskFull:
        return "Disk full -- indexing halted";
    case dtsErIxCorrupt:
        return "Index is corrupt";
    case dtsErAccessDenied:
        return "Access to the index was denied";
    case dtsErFileNotFound:
        return "The file was not found in the index";
    case dtsErFileEncrypted:
        return "The file is encrypted";
    case dtsErFileCorrupt:
        return "The file is corrupt";
    case dtsErAccCachedDoc:
        return "Unable to to access cached document in index";
    case dtsErIndexingError:
        return "Indexing operation could not complete due to an error accessing the index";
    case dtsErOutOfMemory:
        return "Out of memory";
    case dtsErUnknownException:
        return "Unknown Exception";
    case dtsErDataSourceException:
    	return "The data source threw an exception";
	case dtsErConflictingDocIdRanges:
		return "Index merge halted due to a conflict between ranges of document ids among the indexes being merged" ;   	
    default:
        return "Unknown	error code";
    }
}

