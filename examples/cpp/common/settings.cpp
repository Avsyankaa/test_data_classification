#include <stdafx.h>
#include <settings.h>
#include <dtconfig.h>
//#include <stringtools.h>

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif

using namespace std;



#define fnAfterHit          "AfterHit"
#define fnBeforeHit         "BeforeHit"
#define fnBodyStart         "BodyStart"
#define fnCrawlDepth        "CrawlDepth"
#define fnOldDocRoot        "DocRoot"
#define fnBaseFolderForRelativePaths "BaseFolderForRelativePaths"
#define fnCdDocumentsFolder  "CdDocumentsFolder"
#define fnFieldEnd          "End"
#define fnFieldStart        "Start"
#define fnFilters           "Filters"
#define fnFlags             "Flags"
#define fnHtmlHead          "HtmlHead"
#define fnIncludeFilters    "Filters"
#define fnIndexSettings     "dtSearchIndexSettings"
#define fnItem              "Item"
#define fnItemName          "Name"
#define fnItemSynonyms      "Synonyms"
#define fnLinesToScan       "LinesToScan"
#define fnMatchType         "MatchType"
#define fnName              "Name"
#define fnRecursive         "Recursive"
#define fnSplitMark         "SplitMark"
#define fnTypeId            "TypeId"
#define fnUserThesaurus     "dtSearchUserThesaurus"
#define fnViewType          "ViewType"
#define fnWhatToIndex       "WhatToIndex"
#define fnDetailedLogging   "DetailedLogging"
#define fnExtraFiles        "ExtraFiles"
#define fnBaseFolder        "BaseFolder"
#define fnFooter			"Footer"
#define fnHeader			"Header"
#define fnReportContextHeader "ReportContextHeader"
#define fnDocTypeTag        "DocTypeTag"


void CDisplayOptionsItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnBeforeHit, beforeHit);
    xmlMaker.storeField(fnAfterHit, afterHit);
    xmlMaker.storeField(fnHtmlHead, htmlHead);
    xmlMaker.storeField(fnBodyStart, bodyStart);
	xmlMaker.storeField(fnHeader, header);
	xmlMaker.storeField(fnFooter, footer);
	xmlMaker.storeField(fnReportContextHeader, reportContextHeader);
	xmlMaker.storeField(fnDocTypeTag, docTypeTag);
}

void CDisplayOptionsItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnBeforeHit, beforeHit);
    input.getFirstIn(sel, fnAfterHit, afterHit);
    input.getFirstIn(sel, fnHtmlHead, htmlHead);
    input.getFirstIn(sel, fnBodyStart, bodyStart);
	input.getFirstIn(sel, fnHeader, header);
	input.getFirstIn(sel, fnFooter, footer);
	input.getFirstIn(sel, fnReportContextHeader, reportContextHeader);
	input.getFirstIn(sel, fnDocTypeTag, docTypeTag);
}

void CFileTypeTableItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnTypeId, typeId);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnFlags, flags);
}

void CFileTypeTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnTypeId, typeId);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnFlags, flags);
}

void CSplitRulesTableItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnMatchType, matchType);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnSplitMark, splitMark);
}

void CSplitRulesTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnMatchType, matchType);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnSplitMark, splitMark);
}

int CSplitRulesTable::readV5(const char *fn)
{
   const char *CSplitRulesTableId = "dtSearch File Splitting Rules";

    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    if (lines.getCount() == 0)
		return FAIL;
		
	int iLine = 0;
    DString line;
    line = lines.getString(iLine++);
    line.trim();
    if (_stricmp(line, CSplitRulesTableId))
        return FAIL;

    while(iLine < lines.getCount()) {
        line = lines.getString(iLine++);
        if (!line.isBlank()) {
            DString filters;
            DStringSet s;
            s.tokenizeq(line, ",", '\'', true);
            if (s.getCount() >= 4) {
                CSplitRulesTableItem item;
                item.name = s.getString(0);
                item.splitMark = s.getString(1);
                filters = s.getString(2);
                item.matchType = s.getInt(3);
                item.filters.tokenizeq(filters, ",", '\"');
                append(item);
                }
            }
        }
    return SUCCESS;
}

const char *CTextFieldTableId = "dtSearch Text Field Table";

void CTextFieldTableItem::writeTo(CXmlMaker& xmlMaker)
{   xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnFieldStart, start);
    xmlMaker.storeField(fnFieldEnd, end);
    xmlMaker.storeField(fnIncludeFilters, includeFilters);
    xmlMaker.storeField(fnLinesToScan, linesToScan);
    xmlMaker.storeField(fnFlags, flags);
}

void CTextFieldTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnFieldStart, start);
    input.getFirstIn(sel, fnFieldEnd, end);
    input.getFirstIn(sel, fnIncludeFilters, includeFilters);
    input.getFirstIn(sel, fnLinesToScan, linesToScan);
    input.getFirstIn(sel, fnFlags, flags);
    getFlags();
}

int CTextFieldTable::readV5(const char *fn)
{
    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    if (!lines.getCount())
		return FAIL;
		
    DString line;
    int iLine = 0;
    line = lines.getString(iLine++);
    
    if (_stricmp(line, CTextFieldTableId))
        return FAIL;
    while(iLine < lines.getCount()) {
	    line = lines.getString(iLine++);

        if (!line.isBlank()) {
            DString filters;
            CTextFieldTableItem item;
            DStringSet s;
            s.tokenizeq(line, ",", '\'', true);
            if (s.getCount() >= 6) {
                DString sFilters;
                CTextFieldTableItem ftItem;
                ftItem.name = s.getString(0);
                ftItem.start = s.getString(1);
                ftItem.end = s.getString(2);
                ftItem.name.trim();
                ftItem.start.trim();
                ftItem.end.trim();
                ftItem.linesToScan = s.getInt(3);
                sFilters = s.getString(4);
                ftItem.flags = s.getInt(5);
                ftItem.includeFilters.tokenizeq(sFilters, ";, ", '\"');
                append(ftItem);
                }
            }
        }
    return SUCCESS;
}

void CUserThesaurusItem::writeTo(CXmlMaker& xmlMaker)
{   xmlMaker.storeField(fnItemName, name);
    xmlMaker.storeField(fnItemSynonyms, synonyms);
}

void CUserThesaurusItem::readFrom(CXmlReader& xmlReader, CXmlReader::Selection& sel)
{   xmlReader.getFirstIn(sel, fnItemName, name);
    xmlReader.getFirstIn(sel, fnItemSynonyms, synonyms);
}

int CUserThesaurusTable::readV5(const char *fn)
{   clear();

    const char *UserThesaurusId = "dtSearch User Thesaurus";
    DStringSet lines;
    lines.getLinesFromU8(fn);
    int ct = 0;
    DString line = lines.getString(ct++);
    line.trim();

    if (strcmp(line, UserThesaurusId))
        return FAIL;

    int i = 1;
    while(!line.isBlank() && (ct < lines.getCount())) {
        line = lines.getString(ct++);
        CUserThesaurusItem item;
        item.synonyms.tokenizeq(line, " \r\n\t", '\"');
        if (item.synonyms.getCount() > 0) {
            int fCanUseFirst = true;
            DString first = item.synonyms.getString(0);
            for (int j = 0; j < getCount(); ++j) {
                if (!_stricmp(get(j).name, first))
                    fCanUseFirst = false;
                }
            if (fCanUseFirst)
                item.name = first;
            else
                item.name << "#" << i++;
            append(item);
            }
        }
    return SUCCESS;
}



void CExtViewTableItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnViewType, viewType);
}

void CExtViewTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnViewType, viewType);
}

#define fnIndexSettings     "dtSearchIndexSettings"
#define fnStoredFields		"StoredFields"
#define fnEnumerableFields	"EnumerableFields"
#define fnWhatToIndex       "WhatToIndex"
#define fnRecursive         "Recursive"
#define fnCrawlDepth        "CrawlDepth"
#define fnUser				"User"
#define fnPassword			"Password"
#define fnExcludeFilters    "ExcludeFilters"
#define fnIndexName         "IndexName"
#define fnMaxItemsToIndex	"MaxItemsToIndex"
#define fnMaxSizeToIndex	"MaxSizeToIndex"
#define fnWaitBetweenPages	"WaitBetweenPages"
#define fnSiteTimeoutSeconds "SiteTimeoutSeconds"
#define fnServerFilters		"ServerFilters"
#define fnUserAgent			"UserAgent"
#define fnStoreId			"MapiStoreId"
#define fnFolderId			"MapiFolderId"
#define fnProfile			"MapiProfile"

#define fnFormAuthInfo		"FormAuth"
#define fnFormAuthUrl		"URL"
#define fnFormAuthData		"Data"
#define fnFormAuthMethod	"Method"
#define fnIgnoreRobots		"IgnoreRobotsTxt"


void CHttpFormAuthInfo::getFrom(CXmlReader& reader, CXmlReader::Selection sel)
{	sel = reader.select(fnFormAuthInfo, sel);
	if (!sel) {
		clear();
		return;
		}
	reader.getFirstIn(sel, fnFormAuthUrl, authUrl);
	reader.getFirstInHidden(sel, fnFormAuthData, authData);
	DString method;
	reader.getFirstIn(sel, fnFormAuthMethod, method);
	fAuthViaPost = !_stricmp(method, "POST");
	fHaveData = true;
}

void CHttpFormAuthInfo::writeTo(CXmlMaker& writer)
{	if (!fHaveData)
		return;

	writer.beginField(fnFormAuthInfo);
	writer.storeField(fnFormAuthUrl, authUrl);
	writer.storeFieldHidden(fnFormAuthData, authData);
	writer.storeField(fnFormAuthMethod, (fAuthViaPost? "POST" : "GET"));
	writer.endField(fnFormAuthInfo);

}



void CIndexSettings::extractHeaderFields(CXmlReader& reader, CXmlReader::Selection sel)
{
    reader.getFirstIn(sel, fnIndexName, name);
    reader.getFirstIn(sel, fnBaseFolderForRelativePaths, baseFolderForRelativePaths);
    // BaseFolderForRelativePaths used to be called DocRoot
    if (baseFolderForRelativePaths.isBlank())
        reader.getFirstIn(sel, fnOldDocRoot, baseFolderForRelativePaths);
    reader.getFirstIn(sel, fnDetailedLogging, fDetailedLogging);
    reader.getFirstIn(sel, fnExtraFiles, extraFiles);
    reader.getFirstInRelative(sel, fnCdDocumentsFolder, cdDocumentsFolder);
    reader.getFirstIn(sel, fnStoredFields, storedFields);
    reader.getFirstIn(sel, fnEnumerableFields, enumerableFields);
}

int CIndexSettings::readU8(const char *fn)
{   clear();
    CXmlReader reader;
    reader.getFromU8(fn);
    CXmlReader::Selection sel = reader.select(fnIndexSettings);
    if (!sel)
        return readV5(fn);
    extractHeaderFields(reader, sel);

    reader.getFirstIn(sel, fnIncludeFilters, includeFilters);
    reader.getFirstIn(sel, fnExcludeFilters, excludeFilters);
    CXmlReader::FieldSet items = reader.find(fnItem, sel);
    for (int i = 0; i < items.getCount(); ++i) {
        ToIndexItem item;
        CXmlReader::Selection itemFields = reader.select(items, i);
        item.readFrom(reader, itemFields);
        whatToIndex[i] = item;
        }
    return SUCCESS;
}

void ToIndexItem::writeTo(CXmlMaker& xmlMaker)
{
        xmlMaker.storeRelativeField(fnItemName, name);
        xmlMaker.storeNonzero(fnCrawlDepth, crawlDepth);
        xmlMaker.storeNonzero(fnRecursive, fRecursive);
		xmlMaker.storeNonzero(fnMaxItemsToIndex, maxItemsToIndex);
		xmlMaker.storeNonzero(fnMaxSizeToIndex, maxSizeToIndex);
		xmlMaker.storeNonzero(fnWaitBetweenPages, waitBetweenPagesMillis);
		xmlMaker.storeNonzero(fnSiteTimeoutSeconds, siteTimeoutSeconds);
		xmlMaker.storeNonblank(fnUser, user);
		xmlMaker.storeNonblank(fnPassword, password);
		if (serverFilters.getCount())
			xmlMaker.storeField(fnServerFilters, serverFilters);
		xmlMaker.storeNonblank(fnUserAgent, userAgent);
		xmlMaker.storeNonzero(fnIgnoreRobots, ignoreRobotsTxt);
		formAuthInfo.writeTo(xmlMaker);
		xmlMaker.storeNonblank(fnProfile, profile);
       	xmlMaker.storeNonblank(fnStoreId, storeId);
       	xmlMaker.storeNonblank(fnFolderId, folderId);

}

void ToIndexItem::readFrom(CXmlReader& reader, CXmlReader::Selection& sel)
{
    reader.getFirstInRelative(sel, fnItemName, name);
    reader.getFirstIn(sel, fnRecursive, fRecursive);
    reader.getFirstIn(sel, fnCrawlDepth, crawlDepth);
    reader.getFirstIn(sel, fnUser, user);
    reader.getFirstIn(sel, fnPassword, password);
    reader.getFirstIn(sel, fnMaxItemsToIndex, maxItemsToIndex);
    reader.getFirstIn(sel, fnMaxSizeToIndex, maxSizeToIndex);
    reader.getFirstIn(sel, fnWaitBetweenPages, waitBetweenPagesMillis);
    reader.getFirstIn(sel, fnSiteTimeoutSeconds, siteTimeoutSeconds);
    reader.getFirstIn(sel, fnServerFilters, serverFilters);
    reader.getFirstIn(sel, fnUserAgent, userAgent);
    reader.getFirstIn(sel, fnIgnoreRobots, ignoreRobotsTxt);
    reader.getFirstIn(sel, fnStoreId, storeId);
    reader.getFirstIn(sel, fnFolderId, folderId);
    reader.getFirstIn(sel, fnProfile, profile);
    formAuthInfo.getFrom(reader, sel);

}


// Reads up to the what-to-index list but does not read the list,
// which can be very long
void CIndexSettings::readHeader(const char *fn)
{   clear();
    DString tmp;
    tmp.getFromU8(fn, 1024L);
    ptrdiff_t iFirstItem = tmp.find("<Item>");
    if (iFirstItem > 0)
        tmp.truncate(iFirstItem);
    CXmlReader reader;
    reader.setXml(tmp);

    CXmlReader::Selection sel = reader.selectAll();
    extractHeaderFields(reader, sel);
}

int CIndexSettings::readV5(const char *inputFilename)
{   clear();
    DFilename fn = inputFilename;
    fn.rmTail();
    // The index_l file contains a list of folders to index
    fn.addTail("index_l.ix");
    DString index_l;
    index_l.getFromU8(fn);
    DStringSet toIndexSet;
    toIndexSet.tokenize(index_l, "\r\n");
    for (int i = 0; i < toIndexSet.getCount(); ++i) {
        ToIndexItem item;
        item.FromString(toIndexSet.getString(i));
        whatToIndex.append(item);
        }
    // Include and exclude filters are stored in index_s.ix, with "I" and "E" in front
    fn.rmTail();
    fn.addTail("index_s.ix");
    DString index_s;
    index_s.getFromU8(fn);
    DStringSet s;
    s.tokenize(index_s, "\r\n");
    if (s.getCount() > 1) {
        includeFilters.tokenizeq(s.getString(0) + 2, " ", '\"');
        excludeFilters.tokenizeq(s.getString(1) + 2, " ", '\"');
        }
    return whatToIndex.getCount();
}

int CIndexSettings::writeU8(const char *fn)
{   CXmlMaker xmlMaker;
	xmlMaker.setOutputU8(fn);
    xmlMaker.beginField(fnIndexSettings);
    xmlMaker.storeField(fnIndexName, name);
    xmlMaker.storeField(fnBaseFolderForRelativePaths, baseFolderForRelativePaths);
    xmlMaker.storeField(fnIncludeFilters, includeFilters);
    xmlMaker.storeField(fnExcludeFilters, excludeFilters);
    xmlMaker.storeField(fnExtraFiles, extraFiles);
    xmlMaker.storeField(fnDetailedLogging, fDetailedLogging);
    xmlMaker.storeRelativeField(fnCdDocumentsFolder, cdDocumentsFolder);
    xmlMaker.storeField(fnStoredFields, storedFields);
    xmlMaker.storeField(fnEnumerableFields, enumerableFields);
    for (int i = 0; i < whatToIndex.getCount(); ++i) {
        ToIndexItem item = whatToIndex.get(i);
        xmlMaker.beginField(fnItem);
        item.writeTo(xmlMaker);
        xmlMaker.endField(fnItem);
        }
    xmlMaker.endField(fnIndexSettings);
    return xmlMaker.writeU8(fn);
}

int CMacroTable::readV5(const char *fn)
{   DString s;
    s.getFromU8(fn);
    if (s.isBlank())
        return FAIL;
    if (_strnicmp(s, "dtSearch Macros", 15))
        return FAIL;
    for(const char *p = strstr(s, "Name = "); p; p = strstr(p + 1, "Name = ")) {
        const char *n = p + 7;
        const char *q = strstr(n, "Exp = ");
        if (q) {
            CMacroTableItem item;
            const char *e = strchr(q, '\n');
            if (!e)
                e = q + strlen(q);
            item.name.store(n, q - n);
            q += 6;
            item.expansion.store(q, e-q);
            item.name.trim();
            item.expansion.trim();
            append(item);
            }
        }
    return SUCCESS;
}

int CXmlStemmingRulesTable::readV5(const char *fn)
{	
    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    DString line;
    for (int i = 0; i < lines.getCount(); ++i) {
        CStemmingRule rule;
        line = lines.getString(i);
        if (line.first() == '*')
            continue;
        if (line.find("-----") > FAIL)
            break;

        char *s = line.str();
        char *p = strchr(s, '+');
        if (p && atoi(s)) {
            rule.minPrefix = atoi(s);
            s = p + 1;
            }
        else
            rule.minPrefix = 0;
        p = strstr(s, "->");
        if (!p)
            continue;
        *(p-1) = 0;
        rule.suffix = s;
        rule.suffix.trim();
        rule.replace = p + 2;
        rule.replace.trim();
        append(rule);
        }
    if (getCount() > 0)
        return SUCCESS;
    else
        return FAIL;
}


void CIndexLibrary::add(const CIndexLibrary& other)
{   for (int i = 0; i < other.getCount(); ++i) {
        IndexLibraryEntry e = other.get(i);
        addWithUniqueName(e);
        }
}

int CIndexLibrary::operator ==(const CIndexLibrary& other) const
{   if (getCount() != other.getCount())
        return false;

    for (int i = 0; i < other.getCount(); ++i) {
        const IndexLibraryEntry &a = m_table.get(i);
        const IndexLibraryEntry &b = other.m_table.get(i);
        if (wcscmp(a.path, b.path))
            return false;
        if (wcscmp(a.name, b.name))
            return false;
        }
    return true;
}

int CIndexLibrary::findByPathW(const wchar_t *aPath)
{   for (int i = 0; i < m_table.getCount(); ++i) {
        if (!_wcsicmp(aPath, m_table[i].path))
            return i;
        }
    return FAIL;
}

int CIndexLibrary::findByNameW(const wchar_t *aName)
{   for (int i = 0; i < m_table.getCount(); ++i) {
        if (!_wcsicmp(aName, m_table[i].name))
            return i;
        }
    return FAIL;
}

#define IndexLibraryId		"dtSearch Index Library"
#define fnIndexLibrary		"dtSearchIndexLibrary"
#define fnLibName			"Name"
#define fnLibAction			"Action"
#define	fnLibItem			"Item"
#define fnIndexPath			"IndexPath"
#define fnServerUrl			"ServerUrl"
#define fnSearchForm		"SearchForm"
#define fnServerPath		"ServerPath"

int CIndexLibrary::writeU8(const char *fn, int fMakeRelative)
{
    debugLog("CIndexLibrary::write ", fn);
    CXmlMaker xmlMaker;
    xmlMaker.setOutputU8(fn);
    xmlMaker.clearNoHeader();
    xmlMaker.beginField(fnIndexLibrary);
    xmlMaker.storeField(fnLibName, m_name);
    xmlMaker.storeField(fnLibAction, m_action);
    xmlMaker.storeRelativeField(fnBaseFolder, m_baseFolder);
    DFilename basePath;
    if (fMakeRelative && !DFilename::isRelative(fn)) {
        basePath = fn;
        basePath.rmTail();
        }
    for(int i = 0; i < getCount(); ++i) {
        IndexLibraryEntry& e = get(i);
        xmlMaker.beginField(fnLibItem);
        xmlMaker.storeField(fnIndexName, e.name);
        if (e.serverPath.isBlank()) {
            DFilename path;
            path.storeUnicodeAsUtf8(e.path);
            if (!basePath.isBlank())
                path.makeRelativeTo(basePath);
            xmlMaker.storeField(fnIndexPath, path);
            }
        if (!e.searchForm.isBlank())
            xmlMaker.storeField(fnSearchForm, e.searchForm);
        if (!e.serverUrl.isBlank())
            xmlMaker.storeField(fnServerUrl, e.serverUrl);
        if (!e.serverPath.isBlank())
            xmlMaker.storeField(fnServerPath, e.serverPath);
        xmlMaker.endField(fnLibItem);
        }
    xmlMaker.endField(fnIndexLibrary);
    xmlMaker.writeU8(fn);

    m_filename = fn;

    return SUCCESS;
}

int CIndexLibrary::readU8(const char *fn)
{   debugLog("CIndexLibrary::read ", fn);
    clear();
    DString xml;
    xml.getFromU8(fn);
    if (xml.isBlank())
        return FAIL;
    if (readFromXml(xml, fn) == FAIL)
        return readOld(fn);
    else
        return SUCCESS;
}

int CIndexLibrary::readFromXml(const char *xml, const char *fn)
{
    CXmlReader xmlReader;
    xmlReader.setXml(xml);
    CXmlReader::Selection sel = xmlReader.select(fnIndexLibrary);

    if (!sel)
        return FAIL;

    if (!fn)
        fn = "";
    DFilename basePath = fn;
    basePath.rmTail();

    xmlReader.setFilenameU8(fn);
    xmlReader.getFirstIn(sel, fnLibName, m_name);
    xmlReader.getFirstIn(sel, fnLibAction, m_action);
    xmlReader.getFirstInRelative(sel, fnBaseFolder, m_baseFolder);

    CXmlReader::FieldSet items = xmlReader.find(fnLibItem, sel);
    for (int i = 0; i < items.getCount(); ++i) {
        CXmlReader::Selection itemFields = xmlReader.select(items, i);
        IndexLibraryEntry e;
        DFilename path;
        xmlReader.getFirstIn(itemFields, fnIndexName, e.name);
        xmlReader.getFirstIn(itemFields, fnSearchForm, e.searchForm);
        xmlReader.getFirstIn(itemFields, fnIndexPath, path);
        xmlReader.getFirstIn(itemFields, fnServerPath, e.serverPath);
        xmlReader.getFirstIn(itemFields, fnServerUrl, e.serverUrl);
        if (e.serverPath.isBlank()) {
            path.makeAbsoluteFrom(basePath);
            e.path.storeUtf8AsUnicode(path);
            }
        else
            e.generateServerPath();
        addWithUniqueName(e);
        }
    m_filename = fn;
    return SUCCESS;
}

int CIndexLibrary::readOld(const char *fn)
{

    clear();

	DStringSet lines;
	lines.getLinesFromU8(fn);
	if (lines.getCount() == 0) {
        return FAIL;
        }
    DString line;
    int iLine = 0;
    line = lines.getString(iLine++);
    line.trim();
    if (_stricmp(line, IndexLibraryId)) {
        debugLog("  Read failed, header was not a library id, = ", line);
        return FAIL;
        }
    m_name = lines.getString(iLine++);

    DFilename basePath;
    basePath = fn;
    basePath.rmTail();

    while(iLine < lines.getCount()) {
        line = lines.getString(iLine++);

        if (!line.isBlank()) {
            DStringSet s;
            s.tokenizeq(line, ", ");
            IndexLibraryEntry e;
            e.name.storeUtf8AsUnicode(s.getString(0));
            DFilename path = s.getString(1);
            path.makeAbsoluteFrom(basePath);

            e.path.storeUtf8AsUnicode(path);
            addWithUniqueName(e);
            }
        }
    m_filename = fn;
    return SUCCESS;

}

int CIndexLibrary::addWithUniqueName(IndexLibraryEntry& e)
{   if (add(e, true) == SUCCESS)
        return SUCCESS;
    // do not add the same index twice
    if (findByPathW(e.path) == SUCCESS)
        return FAIL;
    DString origName;
    e.name.getUtf8(origName);
    for (int i = 2; i < 999; ++i) {
        DString newName;
        newName << origName << " #" << i;
        e.name.storeUtf8AsUnicode(newName);
        if (add(e, true) == SUCCESS)
            return SUCCESS;
        }
    return FAIL;
}

