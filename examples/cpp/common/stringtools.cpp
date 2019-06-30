#include <stdafx.h>
#include <StringTools.h>
#ifdef MfcAvailable
#ifndef __AFXINET_H_
    #include <afxinet.h>
#endif
#endif

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;

#define DUrl dtSearch::DUrl
#endif

CString Str(long n)
{   CString ret;
    ret.Format(_T("%d"), n);
    return ret;
}

CString StrHex(long n)
{   CString ret;
    ret.Format(_T("%x"), n);
    return ret;
}

CString QuoteString(const TCHAR *s)
{   CString ret;
    ret = _T("\"");
    ret += s;
    ret += _T("\"");
    return ret;
}


CFilenameFilter::CFilenameFilter()
{   fCaseInsensitive = true;
}

int CFilenameFilter::matchSet(const char *fullName, const char *tail, DStringSet& set)
{   int fMatched = 0;

    for (int i = 0; (i < set.getCount()) && !fMatched; ++i) {
        const char *filter = set.getString(i);
        if (strchr(filter, cSysSlash) || strchr(filter, cUnixSlash))
            fMatched = matchFilter(fullName, filter);
        else
            fMatched = matchFilter(tail, filter);
        }

    return fMatched;
}

void CFilenameFilter::SetInclude(const char *includes)
{   m_include.tokenizeq(preprocess(includes), "\t\r\n ,;", '\"');
}

void CFilenameFilter::SetInclude(const DStringSet& includes)
{   m_include.clear();
    for (int i = 0; i < includes.getCount(); i++) {
        AddInclude(includes.getString(i));
        }
}

void CFilenameFilter::SetExcludeExceptions(const char *includes)
{   m_excludeExceptions.tokenizeq(preprocess(includes), "\t\r\n ,;", '\"');
}

void CFilenameFilter::SetExcludeExceptions(const DStringSet& includes)
{   m_excludeExceptions.clear();
    for (int i = 0; i < includes.getCount(); i++) 
		AddExcludeException(includes.getString(i));
}

void CFilenameFilter::AddExcludeException(const char *filter)
{	m_excludeExceptions.append(preprocess(filter));
}

void CFilenameFilter::AddExclude(const char *filter)
{
    m_exclude.append(preprocess(filter));
}

void CFilenameFilter::AddInclude(const char *filter)
{   m_include.append(preprocess(filter));
}

const char *CFilenameFilter::preprocess(const char *fn)
{   tmp = fn;
    tmp.setSlashType(cSysSlash);
    if (fCaseInsensitive)
        tmp.toLower();
    return tmp;
}

void CFilenameFilter::SetExclude(const char *excludes)
{   m_exclude.tokenizeq(preprocess(excludes), "\t\r\n ,;", '\"');
}

void CFilenameFilter::SetExclude(const DStringSet& excludes)
{   m_exclude.clear();
    for (int i = 0; i < excludes.getCount(); i++) {
        AddExclude(excludes.getString(i));
        }
}

int CFilenameFilter::Matches(const char *s)
{   if ((m_include.getCount() == 0) && (m_exclude.getCount() == 0))
        return true;

    DFilename fn = preprocess(s);
    DString tail;
    fn.getTail(tail);
    int fIncluded = false;
    if (m_include.getCount() == 0)
        fIncluded = true;
    else
        fIncluded = matchSet(fn, tail, m_include);
	if (!fIncluded)
		return false;
	
	if (m_excludeExceptions.getCount() && matchSet(fn, tail, m_excludeExceptions))
		return true;
		
    if (matchSet(fn, tail, m_exclude))
        return false;

    return true;
}


const char WILDCHAR = '?';
const char WILDSTR  = '*';

// slashes always match each other
static int isSlash(char c) {
    return (c == '\\') || (c == '/');
}

int CFilenameFilter::matchFilter(const char *a, const char *b)
{	if (!a || !b)
        return (strIsBlank(a) == strIsBlank(b)) || (b && !strcmp(b, "*"));
	while (*a && *b)
        if ((*a == *b) || (*b == WILDCHAR) || (isSlash(*a) && isSlash(*b))) {
            a++;
            b++;
            }
        else if (*b == WILDSTR) {
            b++;
            if (!*b)
                return true;
            while (*a) {
                if (matchFilter(a, b))
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
    if (*b && strcmp(b, "*"))
        return false;
    return true;
}


