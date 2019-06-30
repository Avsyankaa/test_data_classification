#ifndef ApiConvertJob_H
	#define ApiConvertJob_H

class CInternalApiConvertJob : public DFileConvertJob {
    public:
        DStringSetProperty InputFields;
        DStringProperty InputText;
        CInternalApiConvertJob() {
            debugLog("CInternalApiConvertJob::CInternalApiConvertJob()");
            }
        ~CInternalApiConvertJob() {
            debugLog("CInternalApiConvertJob::~CInternalApiConvertJob()");
            }
        void BuildInputStream(const char *filename, const void *pData, int len);
        void BuildInputStream(const char *filename);
        void Execute();
        void SetTabDelimitedFields(const char *s) {
			DStringSet t;
			t.tokenize(s, '\t', true);
			InputFields.copy(t);
			}
		wchar_t *makeOutputStringW(ptrdiff_t& len) {
			m_outputStringW.clear();
			m_outputStringW.storeUtf8AsUnicode(GetOutputString(), 
				GetOutputStringLen());
			len = m_outputStringW.getLength();
			return m_outputStringW.str();
			}
    protected:
        DHeapPtr<DMemoryInputStream> m_pMemoryStream;
        DHeapPtr<DFileInputStream> m_pFileStream;
        DString m_errorText;
		DWString m_outputStringW;
    };


#endif

