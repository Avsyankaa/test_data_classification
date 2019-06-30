#ifndef MFCTOOLS_H
    #define MFCTOOLS_H

#ifndef DStringProperty_H
    #include <DStringProperty.h>
#endif

#ifndef StringTools_H
#   include <StringTools.h>
#endif

#define trace_crlf          _T("\r\n")
#define XTrace(s)           TRACE(_T(s trace_crlf))
#define XTrace_u8(s, t)     TRACE(_T(s trace_crlf), CUiStringFromUtf8(t).str())
#define XTrace_a(s, t)      TRACE(_T(s trace_crlf), CUiStringFromAnsi(t).str())
#define XTrace_w(s, t)      TRACE(_T(s trace_crlf), CUiStringFromUnicode(t).str())
#define XTrace_d(s, t)      TRACE(_T(s trace_crlf), t)
#define XTrace_i(s, i)      TRACE(_T(s trace_crlf), i)

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif




struct MfcSettingItem {
    MfcSettingItem() :
        boolVal(0), intVal(0), stringVal(0), stringSetVal(0), id(0), name("")
        { };
    MfcSettingItem(const MfcSettingItem& other) {
        copy(other);
        }

    MfcSettingItem& operator=(const MfcSettingItem& other) {
        copy(other);
        return *this;
        }
    void copy(const MfcSettingItem& other) {
        boolVal = other.boolVal;
        intVal = other.intVal;
        stringVal = other.stringVal;
        stringSetVal = other.stringSetVal;
        id = other.id;
        name = other.name;
        }

    char *boolVal;
    long *intVal;
    DStringProperty *stringVal;
    DStringSetProperty *stringSetVal;
    int id;
    const char *name;
    };

// Utility class for managing MFC settings
class MfcSettingManager {
public:
    MfcSettingManager(CWnd *aDlg, const char *aProfileSection) :
      dialog(aDlg), profileSection(aProfileSection), app(AfxGetApp()) { };
    void WriteProfileInt(const char *name, long val);
    void WriteProfileFlag(const char *name, long val) {
        WriteProfileInt(name, (val? 1 : 0));
        }
    void WriteProfileDate(const char *name, const CTime& date) {
        DString tmp;
        EncodeDate(tmp, date);
        WriteProfileString(name, tmp);
        }
    void WriteProfileString(const char *name, const CString& val) {
        DStringProperty tmp;
        tmp.setUi(val);
        WriteProfileString(name, tmp);
        }
    void WriteProfileString(const char *name, const char *val);
    void WriteProfileSet(const char *name, DStringSetProperty& val);
    long GetProfileInt(const char *name, long deflt = 0);
    long GetProfileFlag(const char *name, long deflt = 0) {
        int v= GetProfileInt(name, deflt);
        if (v)
            return 1;
        else
            return 0;
        }
    void GetProfileString(const char *name, DStringProperty& val);
    void GetProfileString(const char *name, CString& val) {
        DStringProperty tmp;
        GetProfileString(name, tmp);
        tmp.getUi(val);
        }
    void GetProfileSet(const char *name, DStringSetProperty& val);
    void GetProfileDate(const char *name, CTime& date) {
        DStringProperty tmp;
        GetProfileString(name, tmp);
        date = CTime::GetCurrentTime(); // in case DecodeDate fails
        DecodeDate(tmp, date);
        }

    void SetDlgItemInt(int id, long val);
    long GetDlgItemInt(int id);
    void SetDlgItemCommaInt(int id, long val);
    void SetDlgItemText(int id, const char *text);
    void GetDlgItemText(int id, DStringProperty& text);
    int GetDlgItemCheck(int id);
    void SetDlgItemCheck(int id, int val);
    void SetDlgItemSet(int id, DStringSetProperty& Set);
    void GetDlgItemSet(int id, DStringSetProperty& Set);
    void SetDlgItemList(int id, DStringSetProperty& set);
    void GetDlgItemList(int id, DStringSetProperty& set);

    void SetDlg(CWnd *aDlg) {
        dialog = aDlg;
        }

    void AddStringItem(int id, const char *name, DStringProperty& val) {
        MfcSettingItem item;
        item.id = id;
        item.name = name;
        item.stringVal = &val;
        itemTable.append(item);
        }
    void AddStringSetItem(int id, const char *name, DStringSetProperty& val) {
        MfcSettingItem item;
        item.id = id;
        item.name = name;
        item.stringSetVal = &val;
        itemTable.append(item);
        }
    virtual void GetDialogControlValues();
    virtual void SetDialogControlValues();
    virtual void WriteProfileSettings();
    virtual void GetProfileSettings();
protected:
    CWnd *dialog;
    CString profileSection;
    CWinApp *app;
    DArrayAsVector<MfcSettingItem> itemTable;

    static void EncodeDate(DString& dest, const CTime& d);
    static void DecodeDate(const char *s, CTime& d);
};

int GetTextWidth(CWnd *wnd, const TCHAR *s);
int GetTextWidth(CWnd *wnd, int nChars);
int GetFontHeight(CWnd *wnd, CFont& font);

#if _MFC_VER >= 0x0700

class CMultiSelectFileDialog: public CFileDialog {
public:
    CMultiSelectFileDialog(CWnd *parent) :
        CFileDialog(true,
				    NULL,
				    NULL,
				    OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_EXPLORER,
					_T("(*.*)|*.*||"),
					parent,
					0)
        {	const int bufSize = 65536;
            buf = new TCHAR[bufSize];
            memset(buf, 0, bufSize * sizeof(TCHAR));
            m_ofn.lpstrFile = buf;
            m_ofn.nMaxFile = bufSize;
            count = -1;
            index = 0;
        }
    ~CMultiSelectFileDialog() {
            delete[] index;
            index = 0;
            delete[] buf;
            buf = 0;
            count = -1;
            }
    int GetCount() {
        if (count >= 0)
            return count;
        const TCHAR *p;
        count = 0;
        for (p = buf; *p; p = p + (_tcslen(p)+1))
            count++;
        index = new const TCHAR *[count];
        int i = 0;
        for (p = buf; *p; p = p + (_tcslen(p)+1))
            index[i++] = p;
        // If more than one file is returned, the first
        // item is the path and the rest are filenames
        if (count > 1)
            count--;
        return count;
        }
    void GetNthFile(int i, CString& dest) {
        if ((count == 1) && (i == 0)) {
            dest = buf;
            return;
            }
        if (i >= count) {
            dest = "";
            return;
            }

        dest = index[0];
        dest += _T("\\");
        dest += index[i+1];
        }
protected:
    TCHAR *buf;
    const TCHAR **index;
    int count;
};

#endif

class CDirectoryBrowser {
    public:
        CDirectoryBrowser(HWND hwParent, const TCHAR *aCaption = 0, const TCHAR *aStartingDir = 0);
		CDirectoryBrowser(HWND hwParent, int idsCaption, int idEdit);
        int DoModal();
        CString result;
        void SetBaseFolder(const TCHAR *s) {
            m_baseFolder = s;
            m_startingDir = s;
            }
        void SetStartingDir(const TCHAR *s) {
            m_startingDir = s;
            }
		void SetCaption(int id) {
			m_caption.LoadString(id);
		}
		void SetEditControl(int id);
		CString GetStartingDir() {
			return m_startingDir;
		}
    protected:
        static int CALLBACK BrowseDirCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData);
        static void _SHFree(void *p);
        CString m_baseFolder;
        CString m_startingDir;
        CString m_caption;
        HWND hwParent;
		int m_editControl;
    };

class CSaveCurrentDir {
	public:
		CSaveCurrentDir();
		~CSaveCurrentDir() {
			restore();
			}
		void restore();
	protected:
		CString savedDir;
		bool bRestored;
	};

class CFileBrowser {
    public:
        CFileBrowser(CWnd *aParent, int aId = -1, int aIsOpen = TRUE, const TCHAR *aExt = 0);
        void SetExt(const TCHAR *aExt) {
            m_ext = aExt;
            }
		// "Alphabet files (*.abc)|*.abc|...||
        void SetFilter(const TCHAR *aFilter) {
            m_filter = aFilter;
            }
        void SetFilter(int rcId) {
            m_filter.LoadString(rcId);
            }
        void SetDefaultName(const TCHAR *name) {
            m_defaultName = name;
            }
        void SetCaptionByLabel(int aId) {
			if (m_parent)
				m_parent->GetDlgItemText(aId, m_caption);
			}
		void SetCaptionByStringId(int id) {
			m_caption.LoadString(id);
			}
		void SetCaption(const TCHAR *s) {
			m_caption = s;
		}
		void SetTypeSave() {
			m_fIsOpen = false;
			}
		void SetOfnFlags(int f) {
			m_ofnFlags = f;
			}
		int GetOfnFlags() {
			return m_ofnFlags;
			}
        int DoModal();

        CString m_filename;
        DWORD m_errorCode;
    protected:
        CWnd *m_parent;
        CString m_caption;
        int m_fIsOpen;
        int m_id;
		int m_ofnFlags;
        const TCHAR *m_ext;
        CString m_filter;
        CString m_defaultName;
        
        void fixMissingExtension(CFileDialog& dlg);
    };



int FindAppFile(DString& dest, const char *file, const char *folder, const char *subFolders);

BOOL FileExists(const TCHAR *fn);
__int64 GetFileSize(const TCHAR *fn);
BOOL FileExistsA(const char *ansiFile);
int CanWriteFile(const TCHAR *fn);
int IsDirectory(const TCHAR *fn);

CString GetSelString(CListBox *lb);

void GetSelString(CListBox *lb, DString& dest);

void GetSelString(CComboBox *cb, DString& dest);
CString GetSelString(CComboBox *cb);

void FormatDate(CString& dest, const CTime& t);
void FormatDate(CString& dest, const SYSTEMTIME& st);

void FormatTime(CString& dest, const CTime& t);
void FormatTime(CString& dest, const SYSTEMTIME& st);

void FormatDateTime(CString& dest, const CTime& t);
void FormatDateTime(CString& dest, const SYSTEMTIME& st);

void FormatDiskSize(DString& dest, __int64 size, __int64 units = 1);

CString FormatDateTime(CTime& t);

bool ShowDlgItem(CWnd *dlg, int id, int fVisible);
void SetDlgItemCommaInt(CWnd *dlg, int id, __int64 val);
void SetDlgItemSet(CWnd *dlg, int id, DStringSetProperty& Set);
void GetDlgItemSet(CWnd *dlg, int id, DStringSetProperty& Set);
void SetDlgItemSet(CWnd *dlg, int id, DStringSet& Set);
void GetDlgItemSet(CWnd *dlg, int id, DStringSet& Set);
void SetDlgItemList(CWnd *dlg, int id, DStringSetProperty& set);
void GetDlgItemList(CWnd *dlg, int id, DStringSetProperty& set);
void SetDlgItemTextU8(CWnd *dlg, int id, const char *text);
void GetDlgItemText(CWnd *dlg, int id, DStringProperty& dest);
void GetDlgItemTextU8(CWnd *dlg, int id, DString& dest);
void EnableDlgItem(CWnd *dlg, int id, int val);
void EnableDlgItems(CWnd *dlg, int *ids, int val);
void SetWindowTextU8(CWnd *wnd, const char *text);
void GetWindowText(CWnd *wnd, DStringProperty& dest);
CString GetClipboardText();
void SetClipboardText(const TCHAR *s);


// Radio button mapping
class CButtonMap {
    public:
        CButtonMap(long *mapping) :
            m_mapping(mapping) { }
        long GetFlags(CWnd *wnd) {
            long ret = 0;
            for (int i = 0; m_mapping[i]; i += 2) {
                if (wnd->IsDlgButtonChecked(m_mapping[i]))
                     ret |= m_mapping[i+1];
                }
            return ret;
            }

        void CheckButtons(CWnd *wnd, long val) {
            for (int i = 0; m_mapping[i]; i += 2) {
                long id = m_mapping[i];
                long flag = m_mapping[i+1];
                wnd->CheckDlgButton(id, val & flag);
                }
            }
        void CheckButton(CWnd *wnd, long val) {
            for (int i = 0; m_mapping[i]; i += 2) {
                long id = m_mapping[i];
                long flag = m_mapping[i+1];
                wnd->CheckDlgButton(id, val == flag);
                }
            }
        void EnableControls(CWnd *wnd, int fOn) {
            for (int i = 0; m_mapping[i]; i += 2) {
                CWnd *button = wnd->GetDlgItem(m_mapping[i]);
                if (button)
                    button->EnableWindow(fOn);
                }
            }
    protected:
        long *m_mapping;
    };

class CLocale {
    public:
        CLocale();
        TCHAR tcThousandSep[20];
        TCHAR tcTimeSep[20];
        TCHAR tcDecimal[20];
        DString sThousandSep;
        DString sTimeSep;
        DString sDecimal;

        static void commaNum(TCHAR *dest, int destLen, long v);
#ifdef _UNICODE
        static void commaNum(char *dest, int destlen, long v);
#endif
        static CLocale theLocale;
    };

void Notepad(const TCHAR *file);

void ListFiles(CUiStringSet& dest, const TCHAR *folder, const TCHAR *filter);
void ListDirectories(CUiStringSet& dest, const TCHAR *folder, const TCHAR *filter = 0);

int ShellLaunch(const TCHAR *s);
int ShellLaunchW(const wchar_t *s);

__int64 GetDiskAvail64(const char *path);

class CErrorMessage : public TCString {
    public:
        CErrorMessage();
        CErrorMessage(const TCHAR *s);
        CErrorMessage(const CErrorMessage& other) {
            *this = other;
            }
        CErrorMessage(long rcId, const char *arg1 = 0, const char *arg2 = 0);
        CErrorMessage& operator=(const CErrorMessage& other) {
            clear();
            append(other.text);
            m_args = other.m_args;
            m_originalText = other.m_originalText;
            m_messageId = other.m_messageId;
            return *this;
            }
        operator const TCHAR *() {
            return text;
            }

        void SetText(const TCHAR *s);

        void SetText(long rcId);

        // Replace %n in the string with the argument
        void SetArg(int iArg, const char *s) ;

        void SetArgUi(int iArg, const TCHAR *s);

        void SetArg(int iArg, long v);
        
        void SetArg64(int iArg, __int64 v);
        
        void SetSizeArg(int iArg, __int64 arg, __int64 units = 1);

        void SetArgW(int iArg, const wchar_t *ws) ;

        int SendAsError(CWnd *parent = 0);

        void SendAsInfo(CWnd *parent = 0);

        int AskQuestion(CWnd *parent = 0);

        void SetDlgItemText(CDialog *parent, int id);

        CString GetCString();

        void FormatSystemError(long v = -1);
        
        void AppendSystemError(long v = -1);
        
        void LogAsInfo();
        void LogAsError();
        CString GetArgUi(int i) {
            if (i > 0)
                i--;
            CString ret;
            if (i < m_args.getCount())
                ret = m_args.getString(i);
            return ret;
            }
    protected:
        CUiStringSet m_args;
        TCString m_originalText;
        int m_messageId;
        void LogEvent(int type);
    };


#ifdef USE_DTSEARCH_NAMESPACE

    } // namespace dtSearch

#endif

#endif
