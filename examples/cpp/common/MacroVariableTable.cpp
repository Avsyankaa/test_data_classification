#include "stdafx.h"
#ifdef WIN32
    #include <tchar.h>
#endif
#include <MacroVariableTable.h>

static void logError(const char *s, const char *t)
{
#ifdef DLOGGER_H
	debugLog(s, t);
#endif

}

CMacroVariableTable::CMacroVariableTable()
{
}

#pragma warning(disable:4706)
void CMacroVariableTable::cleanUpMacros(DString& s)
{   // After completing variable substitution, remove any remaining %%xxxx%% sequences
    const char *p;

    ptrdiff_t offset = 0;
    while((offset < s.getLength()) && (p = strstr(s.str() + offset, "%%"))) {
        offset = p - s.str() + 1;
        const char *q = strstr(p+2, "%%");
        if (!q)
            return;
        char toReplace[256];
        ptrdiff_t len = q - p+2;
        if (len >= sizeof toReplace)
            continue;
        memmove(toReplace, p, len);
        toReplace[len] = '\0';
        int fShouldSkip = 0;
        for (int i = 0; (i < len) && !fShouldSkip; ++i) {
            unsigned char c = (unsigned char) toReplace[i];
            if (!Ascii_IsAlnum(c) && (c != '_') && (c != '%'))
                fShouldSkip = true;
            }
        if (!fShouldSkip)
            s.replace(toReplace, " ");
        }
}

void CMacroVariableTable::addHtmlVar(const char *varName, const char *value)
{   addVarName(varName);
    addValue(value);
}

void CMacroVariableTable::addTextVar(const char *varName, const char *value)
{   addVarName(varName);
	if (strIsBlank(value))
		addValue("");
	else {
	
		DString256 s = value;
	    safeHtmlEncode(s);
		addValue(s);
		}
}

void CMacroVariableTable::addTextVarT(const char *varName, const TCHAR *value)
{   addVarName(varName);
    DString256 s;
#ifdef UNICODE
    s.storeUnicodeAsUtf8(value);
#else
    DWString tmp;
    tmp.storeAnsiAsUnicode(value);
    s.storeUnicodeAsUtf8(tmp);
#endif
    safeHtmlEncode(s);
    addValue(s);
}

void CMacroVariableTable::safeHtmlEncode(DString& s)
{	// This ensures that incorrect UTF-8 used to mask angle brackets is filtered out
	wTemp.clear();
	wTemp.storeUtf8AsUnicode(s);
	s.clear();
	s.storeUnicodeAsUtf8(wTemp);
	s.htmlEncode();
}


void CMacroVariableTable::addDateVar(const char *varName, int year, int month, int day)
{
    SYSTEMTIME sd;
    memset(&sd, 0, sizeof sd);
    sd.wMonth = (WORD) month;
    sd.wYear = (WORD) year;
    sd.wDay = (WORD) day;
    TCHAR date[80];
    strClear(date);
    GetDateFormat(
        LOCALE_USER_DEFAULT,
        DATE_SHORTDATE,                     // flags
        &sd,
        NULL,                   // lpformat
        date,
        sizeof date);
    addTextVarT(varName, date);
}

void CMacroVariableTable::addIntVar(const char *varName, long v)
{	tmp.clear();
	tmp << v;
    addTextVar(varName, tmp);
}

void CMacroVariableTable::addCommaIntVar(const char *varName, __int64 v, const char *comma)
{	if (!comma)
		comma = ",";
	tmp.clear();
	tmp.commaNum(v, comma);
	addTextVar(varName, tmp);
}

void CMacroVariableTable::addVarName(const char *varName)
{   // make sure it has %%name%% format
    if (varName[0] != '%') {
		DString80 name2;
		name2 << "%%" << varName << "%%";
        nameTable.append(name2);
        }
    else
        nameTable.append(varName);
}

void CMacroVariableTable::addValue(const char *v)
{   if (strIsBlank(v))
        v = " &nbsp; ";
    valueTable.append(v);
}

void CMacroVariableTable::formatAsString(DString& dest)
{   for (int i = 0; i < nameTable.getCount(); ++i) {
        dest << nameTable.getString(i) << " = " << valueTable.getString(i) << "\n";
        }
}

void CMacroVariableTable::doReplacements(DString& s, const char *inctest)
{

    replaceVars(s);
    if (!strIsBlank(inctest))
		replaceIncludes(s, inctest);
    cleanUpMacros(s);
}


void CMacroVariableTable::replaceVars(DString& s)
{    for (int i = 0; i < nameTable.getCount(); i++ ) {
        const char *macro = nameTable.getString(i);
        const char *val = valueTable.getString(i);
        s.replace(macro, val);
        }
}

#ifdef IncludeMacro
void CMacroVariableTable::replaceIncludes(DString& s, const char *inctest)
{
	const char *BeginIncludeMark = "<!-- BeginInclude -->";
	
    // Handle file includes
    // Syntax for an include is %%include{filename}%%
    for(int x = 0; x < 10; ++x) { // allow up to 10 levels of nesting; this prevents recursive includes
        ptrdiff_t inc = s.ifind(IncludeMacro);
        if (inc == FAIL)
                return;
        DString filename = s.str() + inc + strlen(IncludeMacro);
        ptrdiff_t incEnd = filename.find("}%%");
        if (incEnd == FAIL) {
                s << "Error: replaceInclude failed because }%% missing";
                return;
                }
        filename.truncate(incEnd);
        DString contents;
        // Allows search form contents to be passed directly without reading from path
        if (!_stricmp(filename, "%%SearchForm%%") || strIsBlank(filename) || (filename.first() == '/'))
			// Just replace with search form -- ignore include directive
			contents = searchFormText;
		else
			contents.getFromU8(filename);
			
        if (contents.isBlank()) {
			DFilename fn2 = filename;
			fn2.urlDecode();
			contents.getFromU8(filename);
			if (contents.isBlank()) {
				DFilename fn3;
				fn3.urlDecodeU8();
				contents.getFromU8(fn3);
				if (contents.isBlank()) {
					s << "Error: include failed because file not found " << filename;
					logError("Error: include failed because file not found ", filename);
					return;
					}
				}
			}
		ptrdiff_t iBegin = contents.find(BeginIncludeMark);
		if (iBegin > FAIL) {
			contents.remove(0, iBegin + strlen(BeginIncludeMark));
			ptrdiff_t iEnd = contents.find("<!-- EndInclude -->");
			if (iEnd > FAIL)
				contents.truncate(iEnd);
			}
		if (!strIsBlank(inctest) && !strstr(contents, inctest)) {
			debugLog("Invalid search form -- not included");
			contents.clear();
			}
        replaceVars(contents);

        s.remove(inc+strlen(IncludeMacro), strlen(filename)+3);
        s.replace(IncludeMacro, contents);
    }
}

#else
void CMacroVariableTable::replaceIncludes(DString& , const char *)
{
}
#endif


void CMacroVariableTable::addDateVar(const char *varName, SYSTEMTIME& sd)
{   TCHAR date[160];
    TCHAR time[80];
    strClear(date);
    strClear(time);
    GetDateFormat(
        LOCALE_USER_DEFAULT,
        DATE_SHORTDATE,                     // flags
        &sd,
        NULL,                   // lpformat
        date,
        _countof(date));
    GetTimeFormat(
         LOCALE_USER_DEFAULT,
         0,
         &sd,
         0,
         time,
         _countof(time));
    _tcsncat_s(date, _T(" "), _countof(date));
    _tcsncat_s(date, time, _countof(date));
    addTextVarT(varName, date);
}

