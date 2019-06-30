#include "stdafx.h"
#include <mfctools.h>

#ifdef _UNICODE
	#define CF_TCHAR CF_UNICODETEXT
#else
	#define CF_TCHAR CF_TEXT
#endif

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
namespace dtSearch {
    #define GLOBAL dtSearch::
#else
    #define GLOBAL ::
#endif
 
CString GetClipboardText()
{	CString ret;
    if (IsClipboardFormatAvailable(CF_TCHAR)) {
		CWnd *wnd = AfxGetMainWnd();
		if (wnd && wnd->OpenClipboard()) {
			HGLOBAL hglb = GetClipboardData(CF_TCHAR);
			if (hglb) {
				const TCHAR *text = (const TCHAR *) GlobalLock(hglb);
				if (text) {
					ret = text;
					GlobalUnlock(hglb);
					}
				}
			CloseClipboard();
			}
		}
	return ret;
}

void SetClipboardText(const TCHAR *s)
{
	CWnd *wnd = AfxGetMainWnd();
	if (wnd && wnd->OpenClipboard()) {
		EmptyClipboard();
		size_t len = (_tcslen(s) + 1) * sizeof TCHAR;
		HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, len);
		if (hmem) {
			void *p = GlobalLock(hmem);
			memmove(p, s, len);
			GlobalUnlock(hmem);
			SetClipboardData(CF_TCHAR, hmem);
			}
		CloseClipboard();
		}
}

void SetWindowTextU8(CWnd *wnd, const char *text)
{	CUiStringFromUtf8 ui(text);
	wnd->SetWindowText(ui);
}

void GetWindowText(CWnd *wnd, DStringProperty& dest)
{	CString cs;
	wnd->GetWindowText(cs);
	DWStringFromUiString ws = cs;
	dest.set(ws);
}

bool ShowDlgItem(CWnd *dlg, int id, int fVisible)
{	CWnd *w = dlg->GetDlgItem(id);
	if (w) {
		if (fVisible && !w->IsWindowVisible())
			w->ShowWindow(SW_SHOW);
		else if (w->IsWindowVisible() && !fVisible)
			w->ShowWindow(SW_HIDE);
		return true;
		}
	return false;
}



void SetDlgItemTextU8(CWnd *dlg, int id, const char *text)
{	CUiStringFromUtf8 ui(text);
	dlg->SetDlgItemText(id, ui);
}

void GetDlgItemText(CWnd *dlg, int id, DStringProperty& dest)
{   CString cs;
    dlg->GetDlgItemText(id, cs);
	DWStringFromUiString s = cs;
    dest.set(s);
}

void GetDlgItemTextU8(CWnd *dlg, int id, DString& dest)
{	DStringProperty tmp;
	GetDlgItemText(dlg, id, tmp);
	tmp.getU8(dest);
}

void SetDlgItemCommaInt(CWnd *dlg, int id, __int64 val)
{	DString80 s;
	s.commaNum(val, CLocale::theLocale.sThousandSep);
	SetDlgItemTextU8(dlg, id, s);
}

void SetDlgItemSet(CWnd *dlg, int id, DStringSetProperty& set)
{	if (!dlg || !dlg->GetDlgItem(id))
		return;
    DString text;
	set.formatAsSimpleString(text);
    SetDlgItemTextU8(dlg, id, text);
}

void GetDlgItemSet(CWnd *dlg, int id, DStringSetProperty& set)
{	DStringProperty text;
    GetDlgItemText(dlg, id, text);
    set.tokenize(text);
}

void SetDlgItemSet(CWnd *dlg, int id, DStringSet& set)
{   DString text;
	set.formatAsSimpleString(text);
    SetDlgItemTextU8(dlg, id, text);
}

void GetDlgItemSet(CWnd *dlg, int id, DStringSet& set)
{	DStringProperty text;
    GetDlgItemText(dlg, id, text);
    set.tokenizeq(text, ", ", '\"');
}


void SetDlgItemList(CWnd *dlg, int id, DStringSetProperty& set)
{	CListBox *list = (CListBox *) dlg->GetDlgItem(id);
	if (!list)
		return;

	list->ResetContent();
	for (int i = 0; i < set.getCount(); ++i)  {
		DString s;
		set.getStringU8(i, s);
		CUiStringFromUtf8 ui(s);
		list->AddString(ui);
		}
}

void GetDlgItemList(CWnd *dlg, int id, DStringSetProperty& set)
{	set.clear();
	CListBox *list = (CListBox *) dlg->GetDlgItem(id);
	if (!list)
		return;

	for (int i = 0; i < list->GetCount(); ++i) {
		CString cs;
		list->GetText(i, cs);
		set.appendUi(cs);
		}
}


void MfcSettingManager::SetDialogControlValues()
{	for (int i = 0; i < itemTable.getCount(); ++i) {
		MfcSettingItem& item = itemTable[i];
		if (item.stringVal) {
			DString ansiString;
			item.stringVal->getA(ansiString);
			SetDlgItemText(item.id, ansiString);
			}
		else if (item.stringSetVal)
			SetDlgItemSet(item.id, *item.stringSetVal);
		}
}

void MfcSettingManager::GetDialogControlValues()
{	for (int i = 0; i < itemTable.getCount(); ++i) {
		MfcSettingItem& item = itemTable[i];
		if (item.stringVal)
			GetDlgItemText(item.id, *item.stringVal);
		else if (item.stringSetVal)
			GetDlgItemSet(item.id, *item.stringSetVal);
		}
}

void MfcSettingManager::WriteProfileSettings()
{	for (int i = 0; i < itemTable.getCount(); ++i) {
		MfcSettingItem& item = itemTable[i];
		if (item.stringVal)
			WriteProfileString(item.name, *item.stringVal);
		else if (item.stringSetVal)
			WriteProfileSet(item.name, *item.stringSetVal);
		}
}

void MfcSettingManager::GetProfileSettings()
{	for (int i = 0; i < itemTable.getCount(); ++i) {
		MfcSettingItem& item = itemTable[i];
		if (item.stringVal)
			GetProfileString(item.name, *item.stringVal);
		else if (item.stringSetVal)
			GetProfileSet(item.name, *item.stringSetVal);
		}
}

void MfcSettingManager::WriteProfileInt(const char *name, long val)
{   CString csName = Utf8ToUi(name);
    app->WriteProfileInt(profileSection, csName, val);
}

void MfcSettingManager::WriteProfileString(const char *name, const char *val)
{	// Store data in the registry as either Ansi or Unicode
	CUiStringFromUtf8 v = val;
	CUiStringFromUtf8 n = name;

    app->WriteProfileString(profileSection, n, v);
}

long MfcSettingManager::GetProfileInt(const char *name, long deflt)
{   CString csName = Utf8ToUi(name);
    return app->GetProfileInt(profileSection, csName, deflt);
}

void MfcSettingManager::GetProfileString(const char *name, DStringProperty& val)
{	CUiStringFromUtf8 n = name;
    CString csVal = app->GetProfileString(profileSection, n, TEXT(""));
	DWStringFromUiString wVal = csVal;
    val.set(wVal);
}

void MfcSettingManager::WriteProfileSet(const char *name, DStringSetProperty& val)
{   DString formatted;
    val.formatAsString(formatted);
    WriteProfileString(name, formatted);
}

void MfcSettingManager::GetProfileSet(const char *name, DStringSetProperty& val)
{   DStringProperty formatted;
    GetProfileString(name, formatted);
    val.tokenize(formatted);
}

void MfcSettingManager::SetDlgItemInt(int id, long val)
{	
	if (!dialog) return;
    dialog->SetDlgItemInt(id, val);
}

void MfcSettingManager::SetDlgItemCommaInt(int id, long v)
{	if (!dialog) return;

	::SetDlgItemCommaInt(dialog, id, v);
}

long MfcSettingManager::GetDlgItemInt(int id)
{	if (!dialog) return 0;
   return dialog->GetDlgItemInt(id);
}

void MfcSettingManager::SetDlgItemText(int id, const char *text)
{	if (!dialog) return;
	SetDlgItemTextU8(dialog, id, text);
}


void MfcSettingManager::GetDlgItemText(int id, DStringProperty& text)
{	if (!dialog) return;
	GLOBAL GetDlgItemText(dialog, id, text);
}

int MfcSettingManager::GetDlgItemCheck(int id)
{	if (!dialog) return 0;
    CButton *b = (CButton *) dialog->GetDlgItem(id);
    if (b)
        return b->GetCheck();
    else
        return 0;
}

void MfcSettingManager::SetDlgItemCheck(int id, int val)
{	if (!dialog) return;
   CButton *b = (CButton *) dialog->GetDlgItem(id);
    if (b)
        b->SetCheck(val);
}

void MfcSettingManager::SetDlgItemSet(int id, DStringSetProperty& set)
{	if (!dialog) return;
	::SetDlgItemSet(dialog, id, set);
}

void MfcSettingManager::GetDlgItemSet(int id, DStringSetProperty& set)
{	if (!dialog) return;
   ::GetDlgItemSet(dialog, id, set);
}

void MfcSettingManager::SetDlgItemList(int id, DStringSetProperty& set)
{	if (!dialog) return;
	::SetDlgItemList(dialog, id, set);
}

void MfcSettingManager::GetDlgItemList(int id, DStringSetProperty& set)
{	::GetDlgItemList(dialog, id, set);
}

void MfcSettingManager::EncodeDate(DString& dest, const CTime& d)
{	dest.clear();
	dest << d.GetYear() << "/" << d.GetMonth() << "/" << d.GetDay();
}

void MfcSettingManager::DecodeDate(const char *s, CTime& d)
{	DStringSet tmp;
	tmp.tokenize(s, "/");
	if (tmp.getCount() == 3) {
		int year = atoi(tmp.getString(0));
		int month = atoi(tmp.getString(1));
		int day = atoi(tmp.getString(2));
		CTime d2(year, month, day, 0, 0, 0, 0);
		d = d2;
		}
	else
		d = CTime::GetCurrentTime();
}

int GetTextWidth(CWnd *wnd, const TCHAR *s)
{   CFont *font = wnd->GetFont();
	CWindowDC dc(wnd);
    CFont *oldFont = dc.SelectObject(font);
    size_t len = _tcslen(s);
    CSize sz = dc.GetTextExtent(s, (int) len);
	dc.SelectObject(oldFont);
    return sz.cx;
}

int GetTextWidth(CWnd *wnd, int nChars)
{	TCHAR *temp = new TCHAR[nChars+1];
	for (int i = 0; i < nChars; ++i)
		temp[i] = 'a';
	temp[nChars] = '\0';
	int ret = GetTextWidth(wnd, temp);
	delete temp;
	return ret;
}

int GetFontHeight(CWnd *wnd, CFont& font)
{	CWindowDC dc(wnd);
	CFont *oldFont = dc.SelectObject(&font);
	TEXTMETRIC metrics;
	dc.GetTextMetrics(&metrics);
	int fontHeight = metrics.tmHeight + metrics.tmExternalLeading;
	dc.SelectObject(oldFont);
	return fontHeight;
}

CFileBrowser::CFileBrowser(CWnd *aParent, int aId, int aIsOpen, const TCHAR *aExt) :
			m_parent(aParent), m_id(aId), m_fIsOpen(aIsOpen), m_ext(aExt), m_errorCode(0)
{	if (!m_parent)
		m_parent = AfxGetMainWnd();

	if (m_parent)
		m_parent->GetWindowText(m_caption);
	else
		m_caption = (aIsOpen? _T("OpenFile") : _T("SaveFile"));

	if (m_fIsOpen)
		m_ofnFlags = OFN_HIDEREADONLY ;
	else
		m_ofnFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	m_ofnFlags |= (OFN_ENABLESIZING | OFN_EXPLORER);
}

void CFileBrowser::fixMissingExtension(CFileDialog& dlg)
{
	// Check extension
	DFilename fn = UiToUtf8(m_filename);
	DString ext;
	fn.getExt(ext);
	if (!strIsBlank(ext)) 
		return;
	
	OPENFILENAME& ofn = dlg.GetOFN();
	int iFilter = 2*(ofn.nFilterIndex-1)+1;
	
	DStringSet filterSet;
	filterSet.tokenize(UiToUtf8(m_filter), '|', true);
	if (iFilter > filterSet.getCount())
		return;
	const char *filter = filterSet.getString(iFilter);
	TCHAR newExt[4];
	memset(newExt, 0, sizeof newExt);
	if ((filter[0] != '*') || (filter[1] != '.'))
		return;
	if (filter[2] == '*')
		return;
	for (size_t i= 2; (i < strlen(filter)) && (filter[i] != ';') && (i-2 < 3); ++i) 
		newExt[i-2] = filter[i];
	m_filename += _T(".");
	m_filename += newExt;
}

int CFileBrowser::DoModal() {
	CWinApp *app = AfxGetApp();
	if (m_parent && (m_id > 0))
		m_parent->GetDlgItemText(m_id, m_defaultName);
	if (m_defaultName.IsEmpty())
		m_defaultName = app->GetProfileString( _T("FileBrowser"), m_caption, m_defaultName);

	m_errorCode = 0;
	TCHAR name[MAX_PATH];
	memset(name, 0, sizeof name);
	if (!m_defaultName.IsEmpty())
		strCopy(name, m_defaultName, MAX_PATH);
	else if (m_id > 0)
		m_parent->GetDlgItemText(m_id, name, MAX_PATH);

	// Don't let the file dialog box change the current directory
	CSaveCurrentDir saveCurrentDir;

	CFileDialog dlg(m_fIsOpen, m_ext, name, m_ofnFlags, m_filter, m_parent, 0);
	dlg.GetOFN().nFilterIndex = 1;
	dlg.m_ofn.lpstrFile = name;

	if (!m_caption.IsEmpty())
		dlg.m_ofn.lpstrTitle = m_caption;

	if (dlg.DoModal() == IDOK) { 
		m_filename = dlg.GetPathName();
		if (!m_fIsOpen)
			fixMissingExtension(dlg);		

		app->WriteProfileString( _T("FileBrowser"), m_caption, m_filename);
		if (m_id > 0)
			m_parent->SetDlgItemText(m_id, m_filename);
		return IDOK;
		}
	else {
		m_errorCode = CommDlgExtendedError();
		return IDCANCEL;
		}
}



CDirectoryBrowser::CDirectoryBrowser(HWND aParent, int idsCaption, int idEdit) :
	m_startingDir(_T("")),
	m_caption(_T("")),
	hwParent(aParent),
	m_editControl(idEdit)
{	SetCaption(idsCaption);
	CWinApp *app = AfxGetApp();
	m_startingDir = app->GetProfileString( _T("DirectoryBrowser"), m_caption);
	SetEditControl(idEdit);
}

CDirectoryBrowser::CDirectoryBrowser(HWND aParent, const TCHAR *aCaption, const TCHAR *aStartingDir) :
	m_startingDir(aStartingDir? aStartingDir : _T("")),
	m_caption(aCaption? aCaption : _T("Select Folder")),
	hwParent(aParent),
	m_editControl(0)
{
	if (aParent && strIsBlank(aCaption)) {
		TCHAR parentCaption[MAX_PATH];
		memset(parentCaption, 0, sizeof parentCaption);
		::GetWindowText(aParent, parentCaption, MAX_PATH);
		if (!strIsBlank(parentCaption))
			m_caption = parentCaption;
		}

	if (!m_startingDir.GetLength()) {
		CWinApp *app = AfxGetApp();
		m_startingDir = app->GetProfileString( _T("DirectoryBrowser"), m_caption);
		}
}

void CDirectoryBrowser::SetEditControl(int id)
{	m_editControl = id;

	TCHAR editPath[MAX_PATH];
	memset(&editPath, 0, sizeof editPath);
	::GetDlgItemText(hwParent, m_editControl, editPath, _countof(editPath));
	if (!strIsBlank(editPath))
		m_startingDir = editPath;
}

void CDirectoryBrowser::_SHFree(void *p)
{
    IMalloc *pm;
    SHGetMalloc(&pm);
    if (pm)
    {
        pm->Free(p);
        pm->Release();
    }
}

int CALLBACK CDirectoryBrowser::BrowseDirCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{	CDirectoryBrowser *db = (CDirectoryBrowser *) pData;
	if (!db)
		return 0;
	// make this static so the dialog box will remember the directory
    switch(uMsg) {
        case BFFM_INITIALIZED:
            if (db->m_startingDir.GetLength())
                SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)((const TCHAR *) db->m_startingDir));
		    break;
        case BFFM_SELCHANGED: {
            // Set the status window to the currently selected path.
            TCHAR szTemp[MAX_PATH];
            if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szTemp)) {
				if (!db->m_baseFolder.IsEmpty()) {
					// Verify that we are under the base folder
					if (_tcsnicmp(db->m_baseFolder, szTemp, db->m_baseFolder.GetLength()-1)) {
						CString cs = _T("Please select a folder under ");
						cs += db->m_baseFolder;
						strCopy(szTemp, cs, MAX_PATH);
						SendMessage(hwnd, BFFM_ENABLEOK, 0, 0);
						}
					else
						SendMessage(hwnd, BFFM_ENABLEOK, 0, 1);
					}
                SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szTemp);

                }
            break;
            }
        default:
            break;
         }
         return 0;
}


int CDirectoryBrowser::DoModal()
{
	// Don't let it change the current directory
	CSaveCurrentDir saveCurrentDir;

	if (!m_baseFolder.IsEmpty())
		m_startingDir = m_baseFolder;

    BROWSEINFO bi;
    TCHAR buf[MAX_PATH];

    memset(&bi, 0, sizeof bi);
	memset(buf, 0, sizeof buf);
	bi.lParam = (LPARAM) this;
    bi.hwndOwner = hwParent;
    bi.pszDisplayName = buf;

    bi.lpszTitle = m_caption;
    bi.ulFlags = (BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS |  BIF_STATUSTEXT | BIF_EDITBOX);  // new style does not display text | BIF_NEWDIALOGSTYLE);

    bi.lpfn = BrowseDirCallbackProc;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl) {
        int ok = SHGetPathFromIDList(pidl, buf);
		if (ok) {
			result = buf;
			if (result.GetLength() > 1) {
				if (result.GetAt(result.GetLength()-1) != '\\')
					result += _T("\\");
				}
			CWinApp *app = AfxGetApp();
			app->WriteProfileString( _T("DirectoryBrowser"), m_caption, buf);
			}
        _SHFree(pidl);
		if (ok) {
			if (m_editControl)
				SetDlgItemText(hwParent, m_editControl, result);
			return IDOK;
		}
        }
	return IDCANCEL;
}

// Find a file in a folder relative to the specified folder.
// If no folder is specified, use the application folder.
// For example, FindAppFile(dest, "myname.hlp", "", ".;..\help;help") would look
// for a help file in the current folder, a parallel help folder, or a help subfolder.
int FindAppFile(DString& dest, const char *file, const char *folder, const char *subFolders)
{	DFilename baseFolder = folder;
	if (baseFolder.isBlank()) {
		CWinApp *app = AfxGetApp();
		if (app) {
			TCHAR moduleName[MAX_PATH];
			memset(moduleName, 0, sizeof moduleName);
			GetModuleFileName(app->m_hInstance, moduleName, sizeof moduleName);
			baseFolder = moduleName;
			baseFolder.rmTail();
			}
		}

	DStringSet subs;
	subs.tokenizeq(subFolders, ",; ");
	for (int i = 0; i < subs.getCount(); ++i) {
		DFilename fn = baseFolder;
		fn.addTail(subs.getString(i));
		fn.simplify();
		fn.addTail(file);
		if (FileExists(CUiStringFromUtf8(fn))) {
			dest = fn;
			return 0;
			}
		}
	return -1;
}

int CanWriteFile(const TCHAR *fn)
{   int ret = false;
    int fShouldDelete = !FileExists(fn);
    HANDLE hFile = CreateFile(fn, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        ret = true;
        CloseHandle(hFile);

        if (fShouldDelete) {
            __int64 size = GetFileSize(fn);
            if (size == 0)
                DeleteFile(fn);
            }
        }
    else {
		int err = GetLastError();
		}

    return ret;
}

CString GetFullPath(const TCHAR *fn)
{	TCHAR fullPath[_MAX_PATH];
	GetFullPathName(fn, _MAX_PATH, fullPath, 0);
	CString ret = fullPath;
	return ret;
}

// Don't use CFile::GetStatus because it crashes on CDs in MFC 7.1
// due to ATL CTime constructor throwing an exception on null time value
__int64 GetFileSize(const TCHAR *fn)
{	if (!fn)
		return -1;

	CString fullPath = GetFullPath(fn);
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(fullPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return -1;
	FindClose(hFind);
    __int64 length = ((__int64)(ffd.nFileSizeHigh)) * (0x100000000i64) +
                       (__int64)(ffd.nFileSizeLow);
	return length;
}

// Don't use CFile::GetStatus because it crashes on CDs in MFC 7.1
// due to ATL CTime constructor throwing an exception on null time value
BOOL FileExists(const TCHAR *fn)
{	CString csFullPath = GetFullPath(fn);
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(csFullPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	FindClose(hFind);
	return true;
}



BOOL FileExistsA(const char *ansiFile)
{
#ifdef UNICODE
	DWString ws;
	ws.storeAnsiAsUnicode(ansiFile);
	return FileExists(ws);
#else
	return FileExists(ansiFile);
#endif

}

int IsDirectory(const TCHAR *fn)
{	// Root folder
	if (!_tcscmp(fn+1, _T(":\\")))
		return true;
	CFileFind finder;
	if (!finder.FindFile(fn))
		return false;
	finder.FindNextFile();
	if (!finder.IsDirectory())
		return false;
	return true;
}

CString GetSelString(CListBox *lb)
{   CString csReturn;
    if (lb) {
	    int i = lb->GetCurSel();
	    if ((i > -1) && (i < lb->GetCount()) )
    		lb->GetText(i, csReturn);
        }
	return csReturn;
}

void GetSelString(CListBox *lb, DString& dest)
{	CString csSel = GetSelString(lb);
	DWStringFromUiString wsSel = csSel;
	dest.storeUnicodeAsUtf8(wsSel);
}

CString GetSelString(CComboBox *cb)
{   CString ret;
    if (cb) {
        int iSel = cb->GetCurSel();
	    if (iSel > -1)
    		cb->GetLBText(iSel, ret);
        }
	return ret;
}


void GetSelString(CComboBox *cb, DString& dest)
{	dest.clear();
    if (cb) {
	    int iSel = cb->GetCurSel();
	    if (iSel > -1) {
		    CString csSel;
		    cb->GetLBText(iSel, csSel);
		    DWStringFromUiString wsSel = csSel;
		    dest.storeUnicodeAsUtf8(wsSel);
		    }
        }
}

void FormatDate(CString& dest, const CTime& t)
{
	SYSTEMTIME st;
	memset(&st, 0, sizeof st);
	t.GetAsSystemTime(st);
	FormatDate(dest, st);
}

void FormatDate(CString& dest, const SYSTEMTIME& st)
{	TCHAR buf[80];
	memset(buf, 0, sizeof buf);
    GetDateFormat(
        LOCALE_USER_DEFAULT,
        DATE_SHORTDATE,
        &st,
        NULL,
        buf,
        sizeof buf/sizeof buf[0]);
    dest = buf;
}

void FormatTime(CString& dest, const CTime& t)
{	SYSTEMTIME st;
	memset(&st, 0, sizeof st);
	t.GetAsSystemTime(st);
	FormatTime(dest, st);
}

void FormatTime(CString& dest, const SYSTEMTIME& st)
{	TCHAR buf[80];
	memset(buf, 0, sizeof buf);
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, 0, buf, sizeof buf/sizeof buf[0]);
    dest = buf;
}

CString FormatDateTime(CTime& t)
{	CString ret;
	FormatDateTime(ret, t);
	return ret;
}

void FormatDateTime(CString& dest, const SYSTEMTIME& st)
{	CString d, t;
	FormatDate(d, st);
	FormatTime(t, st);
	if (!d.IsEmpty()) {
		int extraSpaces = 11 - t.GetLength();
		switch(extraSpaces) {
			case 2:
				dest = d + _T("   ") + t;
				break;
			case 1:
				dest = d + _T("  ") + t;
				break;
			default:
				dest = d + _T(" ") + t;
				break;
			}
		}
}

void FormatDateTime(CString& dest, const CTime& t)
{	SYSTEMTIME st;
	memset(&st, 0, sizeof st);
	t.GetAsSystemTime(st);
	FormatDateTime(dest, st);
}

void FormatDiskSize(DString& dest, __int64 size, __int64 units)
{
	size *= units;
	const char *x = "";
	const __int64 bignum = 10000;
	if (size > bignum) {
		size /= 1024;
		x = "k";
		}
	if (size > bignum) {
		size /= 1024;
		x = "MB";
		}
	if (size > bignum) {
		size /= 1024;
		x = "GB";
		}
	
	dest.commaNum(size, CLocale::theLocale.sThousandSep);
	dest << ' ';
	dest << x;
}


void EnableDlgItem(CWnd *dlg, int id, int val)
{	if (dlg) {
		CWnd *ctrl = dlg->GetDlgItem(id);
		if (ctrl)
			ctrl->EnableWindow(val);
		}
}

void EnableDlgItems(CWnd *dlg, int *ids, int val)
{	while(ids && *ids)
		EnableDlgItem(dlg, *ids++, val);
}

CLocale::CLocale()
{	strCopy(tcThousandSep, _T(","), _countof(tcThousandSep));
	strCopy(tcTimeSep, _T(":"), _countof(tcTimeSep));
	strCopy(tcDecimal, _T("."), _countof(tcDecimal));
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, tcThousandSep, sizeof tcThousandSep);
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, tcTimeSep, sizeof tcTimeSep);
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, tcDecimal, sizeof tcDecimal);
	sThousandSep << tcThousandSep;
	sTimeSep << tcTimeSep;
	sDecimal << tcDecimal;
}

void CLocale::commaNum(TCHAR *dest, int maxLen, long v)
{	DUiString tmp;
	tmp << v;
	GetNumberFormat(LOCALE_USER_DEFAULT, 0, tmp, 0, dest, maxLen);
	TCHAR *pDecimal = _tcsstr(dest, theLocale.tcDecimal);
	if (pDecimal)
		*pDecimal = 0;
}

#ifdef _UNICODE
void CLocale::commaNum(char *dest, int maxLen, long v)
{	
	TCHAR tmp[80];
	commaNum(tmp, sizeof tmp, v);
	DString t(dest, maxLen);
	t << tmp;
}
#endif

CLocale CLocale::theLocale;

void Notepad(const TCHAR *s)
{
	CString csCmdLine;
	csCmdLine += _T("notepad.exe ");
	csCmdLine += _T("\"");
	csCmdLine += s;
	csCmdLine += _T("\"");

	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof startupInfo);
	startupInfo.cb = sizeof startupInfo;

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof processInfo);

	TCHAR tmp[512];
	strCopy(tmp, csCmdLine, _countof(tmp));
	CreateProcess(
		0,              // name of executable module
		tmp,			// command line string
		0, 0, 0, 0, 0, 0, &startupInfo, &processInfo);
}

void ListDirectories(CUiStringSet& dest, const TCHAR *folder, const TCHAR *filter)
{	CSFilename toFind = folder;
	if (strIsBlank(filter))
		filter = _T("*");
	toFind.addTail(filter);
	CFileFind finder;
	BOOL bWorking = finder.FindFile(toFind);
	while (bWorking) {
      	bWorking = finder.FindNextFile();
      	CString name = finder.GetFilePath();
		if (finder.IsDirectory() && !finder.IsDots())
			dest.append(name);
		}
}

void ListFiles(CUiStringSet& dest, const TCHAR *folder, const TCHAR *filter)
{	CSFilename toFind = folder;
	toFind.addTail(filter);
	CFileFind finder;
	BOOL bWorking = finder.FindFile(toFind);
	while (bWorking) {
      	bWorking = finder.FindNextFile();
      	CString name = finder.GetFilePath();
		dest.append(name);
		}
}

#ifndef SEE_MASK_NOASYNC
	#define SEE_MASK_NOASYNC 0x00000100
#endif	

int ShellLaunch(const TCHAR *f)
{
	debugLog("ShellLaunch: ", f);

	SHELLEXECUTEINFO shex;
    memset(&shex, 0, sizeof shex);
    shex.cbSize = sizeof(shex);
    shex.fMask = SEE_MASK_NOASYNC;
    shex.lpVerb = NULL; //  _T("open"); NULL = use default verb, in case there is no "open"
    shex.lpFile = f;
    shex.nShow = SW_SHOWNORMAL;
    if (ShellExecuteEx(&shex)) {
		debugLog("ShellExecuteEx succeeded");
        return SUCCESS;
		}

	uintptr_t err = (uintptr_t) shex.hInstApp;
	debugLog("ShellExecuteEx failed, returned error ", err);
    long v = GetLastError();
	CErrorMessage msg;
	msg.FormatSystemError(v);
	debugLog("Last error: ", v);
	debugLog(msg);
	
    return FAIL;
}

int ShellLaunchW(const wchar_t *s)
{
#ifdef _UNICODE
    return ShellLaunch(s);
#else
    DString ansiName;
    DWString ws = s;
    ws.getAnsi(ansiName);
    return ShellLaunch(ansiName);
#endif
}

//
// GetFreeDiskSpace: Function to Measure Available Disk Space (from MSDN sample amcap.cpp)
//
__int64 GetDiskAvail64(const char *path)
{
    DFilename dsPath = UiToUtf8(path);
    DString drive;
    dsPath.getRoot(drive);
    if (drive.last() != '\\')
        drive << '\\';
    CSFilename csRoot = Utf8ToUi(drive);
    ULARGE_INTEGER freeToCaller, total, totalFree;
    int ret = GetDiskFreeSpaceEx(csRoot, &freeToCaller, &total, &totalFree);
    if (!ret)
        return -1;

    return freeToCaller.QuadPart;
}

CErrorMessage::CErrorMessage()
{   m_messageId = 0;
 }


CErrorMessage::CErrorMessage(const TCHAR *s)
{   SetText(s);
    m_messageId = 0;
}

CErrorMessage::CErrorMessage(long rcId, const char *arg1, const char *arg2)
{   SetText(rcId);
    if (arg1)
        SetArg(1, arg1);
    if (arg2)
        SetArg(2, arg2);
}

        // Automatically replace %e with operating system error message
void CErrorMessage::SetText(const TCHAR *s)
{   clear();
    append(s);
    if (_tcsstr(text, _T("%e"))) {
        long v = GetLastError();
        CErrorMessage sysErr;
        if (v)
            sysErr.FormatSystemError();
        else
            v = errno;
        sysErr << '[' << v << ']';
        replace(_T("%e"), sysErr);
        }
    m_originalText = s;
}

void CErrorMessage::SetText(long rcId)
{   CString cs;
    cs.LoadString(rcId);
    SetText(cs);
    m_messageId = rcId;
}

// Replace %n in the string with the argument
void CErrorMessage::SetArgUi(int iArg, const TCHAR *s) {
    if ((iArg > 0) && (iArg <= 9)) {
        TCHAR rep[3];
        rep[0] = '%';
        rep[1] = (TCHAR) (iArg + '0');
        rep[2] = '\0';
        replace(rep, s);

        if (iArg-1 == m_args.getCount())
            m_args.append(s);
        }
}


void CErrorMessage::SetSizeArg(int iArg, __int64 arg, __int64 units)
{	DString80 num;
	FormatDiskSize(num, arg, units);
	SetArg(iArg, num);
}

void CErrorMessage::SetArg(int iArg, long v) {
    DString80 n;
    n.commaNum(v, CLocale::theLocale.sThousandSep);
    SetArg(iArg, n);
}

void CErrorMessage::SetArg64(int iArg, __int64 v) {
    DString80 n;
    n.commaNum(v, CLocale::theLocale.sThousandSep);
    SetArg(iArg, n);
}


void CErrorMessage::SetArg(int iArg, const char *s) {
    SetArgUi(iArg, Utf8ToUi(s));
}

void CErrorMessage::SetArgW(int iArg, const wchar_t *ws) {
    SetArgUi(iArg, CUiStringFromUnicode(ws));
}

int CErrorMessage::SendAsError(CWnd *parent) {
    CString cs = GetCString();
	if (parent)
		::MessageBox(*parent, cs, _T("dtSearch"), MB_ICONSTOP);
	else
		MessageBox(0, cs, _T("dtSearch"), MB_ICONSTOP);
    return -1;
}

void CErrorMessage::SendAsInfo(CWnd *parent) {
    CString cs = GetCString();
	if (parent)
		::MessageBox(*parent, cs, _T("dtSearch"), MB_ICONINFORMATION);
	else
		MessageBox(0, cs, _T("dtSearch"), MB_ICONINFORMATION);
}

int CErrorMessage::AskQuestion(CWnd *parent) {
    CString cs = GetCString();
    return ::MessageBox(*parent, cs, _T("dtSearch"), MB_ICONQUESTION | MB_YESNOCANCEL);
}

void CErrorMessage::SetDlgItemText(CDialog *parent, int id) {
    CString cs = GetCString();
    parent->SetDlgItemText(id, cs);
}

CString CErrorMessage::GetCString() {
    CString s = text;
    return s;
}

void CErrorMessage::FormatSystemError(long v) {
    if (v == -1)
        v = GetLastError();
    
    const TCHAR *lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        v,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
        );
    SetText(lpMsgBuf);
    LocalFree((void *) lpMsgBuf);
}

void CErrorMessage::AppendSystemError(long v) {
   if (v == -1)
        v = GetLastError();
    const TCHAR *lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        v,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
        );

    if (!isBlank())
		*this << _T("\r\n");
	*this << lpMsgBuf;
    LocalFree((void *) lpMsgBuf);
}

void CErrorMessage::LogEvent(int /* type */)
{   // Not implemented yet
}

void CErrorMessage::LogAsError()
{   LogEvent(EVENTLOG_ERROR_TYPE);
}

void CErrorMessage::LogAsInfo()
{

}

CSaveCurrentDir::CSaveCurrentDir()
{
	TCHAR dir[MAX_PATH];
	memset(&dir, 0, sizeof(dir));
	GetCurrentDirectory(_countof(dir), dir);
	savedDir = dir;
	bRestored = false;
}

void CSaveCurrentDir::restore()
{
	if (!bRestored) {
		TCHAR dir[MAX_PATH];
		memset(&dir, 0, sizeof(dir));
		GetCurrentDirectory(_countof(dir), dir);
		if (_tcscmp(dir, savedDir))
			SetCurrentDirectory(savedDir);
		bRestored = true;
		}

}


#ifdef USE_DTSEARCH_NAMESPACE

    };

#endif

