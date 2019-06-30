#include <stdafx.h>
#ifdef _WIN32
//#include <shellapi.h> // for ShellExecute
#endif
#include <string>
#include <dtconfig.h>

using namespace std;

#ifdef _MSC_VER
#   include <direct.h>
#   define true 1
#   define false 0
#endif

#ifdef __UNIX__
#   define sSysSlash "/"
#   define cSysSlash '/'
const char *tempFolder = "/tmp";
#else
#   define sSysSlash "\\"
#   define cSysSlash '\\'
const char *tempFolder = "c:\\tmp";
#endif


#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif

int main(int argc, char **argv);

const long DemoAppId = 0x00abcdef;

class DemoApp {
public:
	DemoApp(istream& aIn, ostream & aOut, const char *home = 0, const char *work = 0);
	~DemoApp();
	void run();
	void init();
	void enableLogging();
	void runScript(const char *name);
protected:
	dtsIndexJob *m_indexJob;
	dtsSearchJob *m_searchJob;
	dtsSearchResults *m_results;

	CIndexLibrary m_indexLibrary;
	DString m_logName;
	DFilename m_errorLogFilename;
	DFilename m_miniDumpFilename;
	DFilename m_libName;
	DString m_homeDir;
	DString m_privateDir;
	dtsOptions m_options;
	long id;
	istream & in;
	ostream & out;
	int fOutputDetails;

	static DemoApp *safeCast(void *p);
	static int indexingCB(void * pData, struct dtsMessage& msg);
	static int searchingCB(void *pData, struct dtsMessage& msg);
	static int errorNotifyFn(dtsErrorInfo *info);

	int inputString(const char *prompt, char *dest, int maxLen);

	int selectIndex(DString& dest);
	void updateIndex(int fClearFirst = false);
	void indexInverted();
	void createIndex(bool caching);
	void searchReport();
	void searchIndex(int nIterations);
	void searchFiles();
	void reportErrors(dtsErrorInfo& info);
	void showItem(dtsSearchResultsItem& item);
	void listResults(int fWithDetails);
	void enableErrorLog();

	void convertFile();
	int searchingCB(struct dtsMessage& msg);
	void allocResults();
};

const char *menu =
"C=Create index, "
"CC= Create index with caching enabled, "
"I=Index files, "
"S=Search index, "
"D=List search results with details, "
"U=Unindexed search\n"
"V=Convert file using dtssConvertFile\n"
"X=Exit\n";

const char nl = '\n';

DemoApp::DemoApp(istream & aIn, ostream & aOut, const char *home, const char *work) :
	in(aIn),
	out(aOut),
	id(DemoAppId),
	m_indexJob(new dtsIndexJob),
	m_searchJob(new dtsSearchJob),
	m_results(0)
{
	char cwd[256];
	_getcwd(cwd, sizeof cwd);
	m_homeDir = home;
	if (m_homeDir.isBlank())
		m_homeDir = cwd;
	m_privateDir = work;
	if (m_privateDir.isBlank())
		m_privateDir = cwd;

}

#ifdef __UNIX__
#define Platform "Linux"
#else
#define Platform "Windows"
#endif

#ifdef _X64
#define Bits "64-bit"
#else
#define Bits "32-bit"
#endif

#if defined(_MSC_VER)
#define BuiltBy      "VC"
#elif defined(__BORLANDC__)
#define BuiltBy      "BC"
#elif defined(__GNUC__)
#define BuiltBy      "GCC " __VERSION__
#else
#define BuildBy "XC"
#endif

#define BuiltWhen    __DATE__ " " __TIME__

void DemoApp::init()
{
	out << "dtSearch Text Retrieval Engine Text-Mode Demo" << nl;
	out << "---------------------------------------------" << nl;
	out << Platform << " " << Bits << " " << BuiltBy << " " << BuiltWhen << nl;

	short result = 0;

	enableErrorLog();

	dtssGetOptions(m_options, result);
	strCopy(m_options.homeDir, m_homeDir, sizeof m_options.homeDir);
	strCopy(m_options.privateDir, m_privateDir, sizeof m_options.privateDir);
	m_options.matchDigitChar = '=';
	dtssSetOptions(m_options, result);

	dtsVersionInfo version;
	dtssVersionEx(version);
	DString ver;
	ver << version.majorVersion << "." << PadNum(version.minorVersion, 2) << " (" << version.build << ")";
	out << "dtSearch Engine " << ver << nl;

	out << "Home directory =    " << m_options.homeDir << nl;
	out << "Private directory = " << m_options.privateDir << nl;
	out << "Log file:           " << m_logName << nl;

	m_libName = m_privateDir;
	m_libName.addTail("ixlib.ilb");
	m_indexLibrary.read(Utf8ToUi(m_libName));
}

void DemoApp::enableLogging()
{
	m_logName.clear();
	m_logName << m_privateDir << sSysSlash << "dtsearch.log";
	dtssDebugLogEx(m_logName, dtsLogCommit);
}

// Logging of unhandled exceptions
void DemoApp::enableErrorLog()
{
	m_errorLogFilename = m_privateDir;
	m_errorLogFilename.addTail("dts_err.txt");
	m_miniDumpFilename = m_errorLogFilename;
	m_miniDumpFilename.setExt(".dmp");
	dtsCrashHandlerInfo crashHandlerInfo;
	crashHandlerInfo.logName2 = m_errorLogFilename;
	crashHandlerInfo.miniDumpName = m_miniDumpFilename;
	dtssInstallCrashDiagnostics(crashHandlerInfo);

}

DemoApp::~DemoApp()
{
	delete m_indexJob;
	m_indexJob = 0;

	delete m_searchJob;
	m_searchJob = 0;

	delete m_results;
	m_results = 0;
}

void DemoApp::allocResults()
{
	if (m_results)
		delete m_results;
	m_results = new dtsSearchResults(256);
	m_searchJob->resultsHandle = m_results->getHandle();
	m_searchJob->output.listingFlags |= dtsListNoDisk;
	m_searchJob->maxFilesToRetrieve = 256;
}

DemoApp *DemoApp::safeCast(void *p)
{
	DemoApp *pDemoApp = (DemoApp *)p;
	if (pDemoApp && pDemoApp->id == DemoAppId)
		return pDemoApp;
	else
		return NULL;
}


void DemoApp::runScript(const char *name)
{
	short result;
	char tmp[256];
	strCopy(tmp, name, sizeof tmp);
	dtssRunScript(tmp, result);
}

void DemoApp::run()
{
	while (1) {
		char cmd[1024];
		strClear(cmd);
		out << menu;
		inputString("Enter command", cmd, sizeof cmd);
		char c = (char)toupper(cmd[0]);
		char c2 = (char)toupper(cmd[1]);
		int intArg = 0;
		if (isdigit(cmd[1]))
			intArg = atoi(cmd + 1);
		switch (c) {
		case 'C':
			createIndex(c2 == 'C');
			break;
		case 'I':
			updateIndex(c2 == 'C');
			break;
		case 'R':
			indexInverted();
			break;
		case 'S':
			searchIndex(intArg);
			break;
		case 'U':
			searchFiles();
			break;
		case 'D':
			listResults(true);
			break;
		case 'X':
			return;
		case 'L':
			dtssDebugLog("");
			break;
		case 'V':
			convertFile();
			break;
		case 'P':
			searchReport();
			break;
		case 'W':
			fOutputDetails = true;
			break;
		default:
			return;
		}
	}
}

int DemoApp::inputString(const char *prompt, char *dest, int maxLen)
{
	memset(dest, 0, maxLen);

	out << prompt << std::endl;
#ifdef __UNIX__
	std::string text;
	getline(in, text);
	strncpy(dest, text.c_str(), maxLen);
	//    in >>dest;
#else
	in.getline(dest, maxLen, '\n');
#endif

	// remove trailing whitespace
	ptrdiff_t l = strlen(dest) - 1;
	while ((l >= 0) && isspace(dest[l])) {
		l--;
		dest[l] = '\0';
	}

	if (strlen(dest) > 0)
		return SUCCESS;
	else
		return FAIL;
}

int DemoApp::selectIndex(DString& dest)
{
	out << "--------------------Select Index---------------------------" << nl;
	for (int i = 0; i < m_indexLibrary.getCount(); ++i) {
		IndexLibraryEntry& e = m_indexLibrary.get(i);
		DString name, path;
		name.storeUnicodeAsAnsi(e.name);
		path.storeUnicodeAsAnsi(e.path);
		out << i << ". " << name << " in " << path << nl;
	}
	out << "-----------------------------------------------------------" << nl;
	char sel[40];
	inputString("Select an index: ", sel, sizeof sel);

	int k = FAIL;
	if (isdigit(sel[0])) {
		k = atoi(sel);
		if ((k >= 0) && (k < m_indexLibrary.getCount()))
			dest.storeUnicodeAsAnsi(m_indexLibrary.get(k).path);
		else
			k = FAIL;
	}
	if (k == FAIL)
		return FAIL;
	else
		return SUCCESS;
}


int DemoApp::indexingCB(void * pData, struct dtsMessage& msg)
{
	DemoApp *pDemoApp = DemoApp::safeCast(pData);

	dtsIndexProgressInfo *prog = (dtsIndexProgressInfo *)msg.ptrParam;
	switch (msg.command) {
	case dtsnCheckForAbort:
		msg.result = dtsContinue;
		return dtsContinue;
	case dtsnIndexStartingFile:
		pDemoApp->out << "Indexing " << prog->file.name << "...\n";
		pDemoApp->out.flush();
		break;
	case dtsnIndexFileDone: {
		DString report;
		report << "Indexed:          " << prog->file.name << "\n";
		report << "   Type:          " << dtsSearchResultsItem::getTypeName(prog->file.typeId) << "\n";
		report << "   Words:         " << prog->file.wordCount << "\n";
		if (!strIsBlank(prog->userFields)) {
			DStringSet userFields;
			userFields.copyFromBuf(prog->userFields);
			DString s;
			userFields.formatAsSimpleString(s);
			report << "   Stored fields: " << s << "\n";
		}
		pDemoApp->out << report.str();
		break;
	}
	default:
		break;
	}
	return dtsContinue;
}

void DemoApp::createIndex(bool caching)
{
	m_indexJob->clear();

	if (inputString("Directory to create index in: ",
		m_indexJob->indexPath, sizeof m_indexJob->indexPath)
		== SUCCESS) {
		if (inputString("Name of index to create: ",
			m_indexJob->indexName, sizeof m_indexJob->indexName)
			== SUCCESS) {
			m_indexJob->action.create = true;
			m_indexJob->createFlags.relativePaths = true;
			m_indexJob->createFlags.addToLibrary = true;
			if (caching)
				m_indexJob->indexingFlags = (dtsIndexCacheText | dtsIndexCacheOriginalFile);
			short result;
			dtssDoIndexJob(*m_indexJob, result);
			if (result == FAIL)
				out << "FAILED -- Unable to create index in " << m_indexJob->indexPath << nl;
			else {
				IndexLibraryEntry libEntry;
				libEntry.name.storeAnsiAsUnicode(m_indexJob->indexName);
				libEntry.path.storeAnsiAsUnicode(m_indexJob->indexPath);
				m_indexLibrary.add(libEntry, true);
				m_indexLibrary.write(Utf8ToUi(m_libName), true);
				out << "OK" << nl;
			}
		}
	}
}

void DemoApp::updateIndex(int fClearFirst)
{
	char includeFilters[80];
	char excludeFilters[80];
	short result = 0;

	m_indexJob->clear();

	dtsErrorInfo errorInfo;
	errorInfo.pUserData = this;
	errorInfo.setNotify(errorNotifyFn);
	m_indexJob->errorHandler = &errorInfo;

	DString path(m_indexJob->indexPath, sizeof m_indexJob->indexPath);
	if (selectIndex(path) == FAIL)
		return;

	char dir[256];
	inputString("Directory to index: ", dir, sizeof dir);
	if (strIsBlank(dir))
		return;
	if (dir[0] == '@')  // list of filenames
		strCopy(m_indexJob->toAdd.fileListName, &dir[1], sizeof m_indexJob->toAdd.fileListName);
	else {
		strCopy(m_indexJob->toAdd.dir, dir, sizeof m_indexJob->toAdd.dir);
		inputString("Filename filter (i.e., *.DOC): ", includeFilters,
			sizeof includeFilters);
		inputString("Exclude filter: ", excludeFilters,
			sizeof excludeFilters);
		m_indexJob->toAdd.includeFilters = includeFilters;
		m_indexJob->toAdd.excludeFilters = excludeFilters;
	}
	char storedFields[260];
	inputString("Stored fields: ", storedFields, sizeof storedFields);
	m_indexJob->storedFields = storedFields;
	m_indexJob->action.add = true;
	m_indexJob->action.create = (short)fClearFirst;
	m_indexJob->pReportCallBack = indexingCB;
	m_indexJob->pReportData = this;
	dtssDoIndexJob(*m_indexJob, result);
	if (result == FAIL)
		out << "FAILED -- Unable to update index in " << m_indexJob->indexPath << nl;
	else
		out << "OK" << nl;
}

int DemoApp::searchingCB(void * pData, dtsMessage& msg)
{
	DemoApp *pDemoApp = (DemoApp *)pData;

	return pDemoApp->searchingCB(msg);
}

void DemoApp::showItem(dtsSearchResultsItem& item)
{
	out << "    Retrieved file information:\n";
	out << "        Filename:       " << item.filename << nl;
	out << "        DisplayName:    " << item.displayName << nl;
	out << "        File size:      " << item.size << nl;
	out << "        Word count:     " << item.wordCount << nl;

	DString d;
	d.ymd(item.modified.year, item.modified.month, item.modified.day);
	d << " ";
	d.hms(item.modified.hour, item.modified.minute, item.modified.second);

	out << "        Modified:       " << d << nl;


	//
	// List any user-defined fields for this document.  User-defined
	// fields are created using the fieldsToStore parameter in
	// dtsIndexJob based on text or database field definitions,
	// and are returned as a null-delimited series of fieldname, value
	// pairs.
	//
	if (item.userFields && *item.userFields) {
		const char *p = item.userFields;

		while (*p) {
			out << "        " << p << ":    ";
			p += (strlen(p) + 1);
			out << p << nl;
			p += (strlen(p) + 1);
		}
	}
	if (item.hitsByWord && *item.hitsByWord) {
		const char *p = item.hitsByWord;
		out << "        Hits by word:   ";
		while (*p) {
			out << p;
			p += (strlen(p) + 1);
			if (*p)
				out << "; ";
		}
		out << nl;
	}

	if (item.hitsByWord2 && *item.hitsByWord2) {
		const char *p = item.hitsByWord2;
		out << "        Hits by word2:  ";
		while (*p) {
			out << p;
			p += (strlen(p) + 1);
			if (*p)
				out << "; ";
		}
		out << nl;
	}

	out << "        Hit locations:  ";
	for (int h = 0; h < item.hitCount; ++h)
		out << item.hits[h] << " ";
	out << nl;


	//
	//  List the contents of the file map for this document,
	//  which provides file offset-to-word count data
	//
	//  For plain text files, the filePtr is a simple file offset,
	//  but for other formats it generally is not a simple offset.
	//
	out << "        file map:       " << ios::hex;
	int ct = 0;
	for (int m = 0; m < item.fileMapEntryCount; ++m) {
		dtsFileMapEntry& entry = item.fileMapTable[m];
		out << entry.filePtr << "->" << entry.wordCount << ' ';
		if (ct++ > 5) {
			out << "\n                        ";
			ct = 0;
		}
	}

	out << nl << ios::dec;
}


int DemoApp::searchingCB(dtsMessage& msg)
{
	switch (msg.command) {
	case dtsnCheckForAbort:
		msg.result = dtsContinue;
		return dtsContinue;
	case dtsnSearchFound: {
		out << msg.paramA << " hits in " << msg.strParam << nl;
		if (fOutputDetails) {
			dtsSearchResultsItem *pItem = (dtsSearchResultsItem *)msg.paramB;
			showItem(*pItem);
		}
		break;
	}
	default:
		break;
	}
	out.flush();
	return dtsContinue;
}

//  Output errors collected in a dtsErrorInfo
void DemoApp::reportErrors(dtsErrorInfo& info)
{
	if (info.getCount())
		out << "Errors reported during the operation and logged in dtsErrorInfo: " << info.getCount() << nl;
	for (int i = 0; i < info.getCount(); ++i) {
		out << '(' << info.getMessageCode(i) << ')' << info.getMessage(i) << '\n';
		if (strlen(info.getMessageArg1(i)))
			out << "    [" << info.getMessageArg1(i) << ']' << '\n';
		if (strlen(info.getMessageArg2(i)))
			out << "    [" << info.getMessageArg2(i) << ']' << '\n';
	}
}

//  Output error reported through a dtsErrorInfo's pNotifyFn
int DemoApp::errorNotifyFn(dtsErrorInfo *info)
{
	DemoApp *d = (DemoApp *)info->pUserData;
	d->out << "Error reported through dtsErrorInfo: " << info->getMessage() << "\n";
	return 0;
}

void DemoApp::searchIndex(int nIterations)
{
	if (nIterations < 1)
		nIterations = 1;
	// Set up search job
	m_searchJob->clear();
	dtsErrorInfo errorInfo;
	errorInfo.pUserData = this;
	errorInfo.pNotifyFn = errorNotifyFn;
	m_searchJob->errorHandler = &errorInfo;
	m_searchJob->action.searchIndexes = true;

	// Get index to search and search request
	DString indexToSearch;
	if (selectIndex(indexToSearch) == FAIL)
		return;
	indexToSearch.doubleNull();
	m_searchJob->indexesToSearch = indexToSearch;

	char request[512];
	if (inputString("Request: ", request, sizeof request) == FAIL)
		return;
	m_searchJob->request2 = request;

	for (int i = 0; i < nIterations; ++i) {
		out << "Searching" << nl;
		// Set up output search results object
		allocResults();

		short fError = 0;
		dtssDoSearchJob(*m_searchJob, fError);
	}

	out << "Total files: " << m_searchJob->totalFiles << nl;
	out << "Total hits:  " << m_searchJob->totalHits << nl;
	listResults(false);
	reportErrors(errorInfo);
}

void DemoApp::listResults(int fWithDetails)
{
	if (!m_results) {
		out << "No search results to list" << nl;
		return;
	}
	m_results->sortByName();
	for (int i = 0; i < m_results->getCount(); ++i) {
		dtsSearchResultsItem item;
		m_results->getDocInfo(i, item);
		m_results->logDocInfo(i, &item);
		if (fWithDetails)
			showItem(item);
		else
			out << item.filename << " " << item.hitCount << nl;
	}
}

void DemoApp::searchReport()
{   // now make a search report form the search results
	DFilename fn = tempFolder;
	fn.addTail("temp.html");

	dtsSearchReportJob rjob;
	dtsErrorInfo errorInfo;

	rjob.errorHandler = &errorInfo;
	rjob.pReportCallBack = searchingCB;
	rjob.pReportData = this;
	dtsOutputInfo output;
	output.outputFormat = it_HTML;
	output.outputFile = fn;
	output.beforeHit = "<B>";
	output.afterHit = "</B>";
	rjob.output = &output;

	rjob.flags = dtsReportByWord | dtsReportIncludeAll;
	rjob.amountOfContext = 10; // two paragraphs around each hit
	rjob.fileHeader =
		"<P><H4>%%Hits%% %%Filename%% in %%Location%%, modified %%Date%% size %%Size%%</H4><HR>";
	rjob.contextHeader = "<P><B>[Page %%Page%% Paragraph %%Paragraph%%]</B><BR>";
	m_results->makeReport(rjob);
#ifdef _WIN32
	// open the output file in a web browser
   // ShellExecute(0, _T("open"), Utf8ToUi(output.outputFile), 0, 0, SW_SHOW);
#endif
	reportErrors(errorInfo);

}

void DemoApp::searchFiles(void)
{
	short result = 0;
	m_searchJob->clear();
	dtsErrorInfo errorInfo;
	errorInfo.pUserData = this;
	errorInfo.setNotify(errorNotifyFn);
	m_searchJob->errorHandler = &errorInfo;
	m_searchJob->pReportCallBack = searchingCB;
	m_searchJob->pReportData = this;
	m_searchJob->action.searchFiles = true;

	char request[512];
	if (inputString("Request: ", request, sizeof request) == FAIL)
		return;

	m_searchJob->request2 = request;

	char dirToSearch[256];
	char filter[256];
	memset(dirToSearch, 0, sizeof dirToSearch);
	memset(filter, 0, sizeof filter);
	inputString("Directory to search: ", dirToSearch, sizeof dirToSearch);
	inputString("Filter: ", filter, sizeof filter);
	m_searchJob->filesToSearch.dirList = dirToSearch;
	m_searchJob->filesToSearch.includeFilters = filter;

	allocResults();

	/* Write output to a listing file and a dtSearch search results file */
	m_searchJob->sortType = dtsSortByHits;

	if (fOutputDetails)
		m_searchJob->searchFlags |= (dtsSearchWantHitsByWord | dtsSearchWantHitsByWord2);

	dtssDoSearchJob(*m_searchJob, result);
	listResults(false);
	reportErrors(errorInfo);
}


void DemoApp::convertFile()
{
	char input[80], output[80], fmt[80];
	if (inputString("Input file: ", input, sizeof input) == FAIL)
		return;
	if (inputString("Output file: ", output, sizeof output) == FAIL)
		return;
	if (inputString("Output format: (h=HTML, a=Ansi, r=RTF, u=Utf8)", fmt, sizeof fmt) == FAIL)
		return;

	dtsFileConvertJob job;
	job.inputFile = input;
	job.outputFile = output;
	job.beforeHit = "<<<";
	job.afterHit = ">>>";
	long sampleHits[3] = { 3, 5, 7 };
	job.hits = sampleHits;
	job.hitCount = 3;
	switch (toupper(fmt[0])) {
	case 'H':
		job.outputFormat = it_HTML;
		break;
	case 'R':
		job.outputFormat = it_RTF;
		break;
	case 'A':
		job.outputFormat = it_Ansi;
		break;
	case 'U':
		job.outputFormat = it_Utf8;
		break;
	default:
		job.outputFormat = it_Ansi;
		break;
	}

	short result;
	dtssConvertFile(job, result);
	if (result)
		out << "dtssConvertFile returned error " << result << "\n";
	ifstream in(output);
	DString text;
	text.getFrom(in);
	out << text << nl;
}

// Demonstrates how the OnIndexWord callback function can be used to 
// modify text being indexed.  In this case, each word is indexed along with an
// inverted version of the word.
void OnIndexWord_Inverter(void * /* pData */, dtsOnIndexWordInfo* pInfo)
{   // First time we are called for a word, just request another callback (nAdditionalWords = 1)
	if (pInfo->iWhichCallback == 0) {
		pInfo->nAdditionalWords = 1;
		return;
	}
	// Second time we are called for a word, return the inverted version
	if (pInfo->iWhichCallback == 1) {
		size_t l = strlen(pInfo->originalWord);
		if (l >= sizeof pInfo->wordToIndex)
			l = sizeof pInfo->wordToIndex - 1;
		for (size_t i = 0; i < l; ++i)
			pInfo->wordToIndex[i] = pInfo->originalWord[l - i - 1];
		pInfo->wordToIndex[l] = '\0';
		return;
	}
}

void DemoApp::indexInverted()
{
	char includeFilters[80];
	char excludeFilters[80];
	short result = 0;

	m_indexJob->clear();
	m_indexJob->pOnIndexWordFn = OnIndexWord_Inverter;
	m_indexJob->pOnIndexWordData = this;

	dtsErrorInfo errorInfo;
	errorInfo.pUserData = this;
	errorInfo.setNotify(errorNotifyFn);
	m_indexJob->errorHandler = &errorInfo;

	DString path(m_indexJob->indexPath, sizeof m_indexJob->indexPath);
	if (selectIndex(path) == FAIL)
		return;

	inputString("Directory to index: ", m_indexJob->toAdd.dir,
		sizeof m_indexJob->toAdd.dir);
	inputString("Filename filter (i.e., *.DOC): ", includeFilters,
		sizeof includeFilters);
	inputString("Exclude filter: ", excludeFilters,
		sizeof excludeFilters);

	m_indexJob->toAdd.includeFilters = includeFilters;
	m_indexJob->toAdd.excludeFilters = excludeFilters;
	m_indexJob->action.add = true;
	m_indexJob->pReportCallBack = indexingCB;
	m_indexJob->pReportData = this;
	dtssDoIndexJob(*m_indexJob, result);
	if (result == FAIL)
		out << "FAILED -- Unable to update index in " << m_indexJob->indexPath << nl;
	else
		out << "OK" << nl;
}


#if defined(_Windows) && !defined(__WIN32__)

#include <windows.h>

#endif

const char *help =
"-s scriptName    Execute batch script named scriptName\n"
"-h homeDir       Set home dir\n"
"-p privateDir    Set private dir\n"
"-l               Enable debug logging\n";

int main(int argc, char **argv)
{
	DString homeDir, privateDir, script;
	int fEnableLogging = false;

	for (int i = 1; i < argc; ++i) {
		const char *arg = argv[i];
		const char *next = (i + 1 < argc ? argv[i + 1] : "");
		if ((*arg == '/') || (*arg == '-')) {
			char c = (char)toupper(arg[1]);
			switch (c) {
			case 'S':
				script = next;
				i++;
				break;
			case 'H':
				homeDir = next;
				i++;
				break;
			case 'P':
				privateDir = next;
				i++;
				break;
			case 'L':
				fEnableLogging = true;
				break;
			default:
				cout << help;
				return 0;
				break;
			}
		}
	}

	DemoApp app(cin, cout, homeDir, privateDir);
	if (fEnableLogging)
		app.enableLogging();
	app.init();
	if (!script.isBlank())
		app.runScript(script);
	else
		app.run();

	return 0;
}

