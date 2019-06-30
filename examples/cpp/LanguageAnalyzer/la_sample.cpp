#include <stdafx.h>
#include <dts_la.h>

// Sample data that is inititialized once per process.
// In an real analyzer, this would usually be large dictionary files
// that are time-consumining to load.
struct SamplePerProcessData {
	wchar_t charToDelete;
	wchar_t charToAppend;
	wchar_t charToMakeWordBreak;
	SamplePerProcessData() {
		charToDelete = 0;
		charToAppend = 0;
		charToMakeWordBreak = 0;
		}
	void initialize() {
		charToDelete = 'e';
		charToMakeWordBreak = 'q';
		charToAppend = 0x3c0;
		}
	void shutdown() {
		charToDelete = 0;
		charToAppend = 0;
		charToMakeWordBreak = 0;
		}
	};

class CSampleAnalyzerJob : public CLanguageAnalyzerJobBase {
	public:
		CSampleAnalyzerJob(SamplePerProcessData& whatToDo) {
			m_pWhatToDo = &whatToDo;
			}
		virtual bool analyze(dtsLaJob& job);
	protected:
		SamplePerProcessData *m_pWhatToDo;
	};

class CSampleLanguageAnalyzer : public CLanguageAnalyzerBase {
	public:
		CSampleLanguageAnalyzer() {
			m_data.charToDelete = 'e';
			m_data.charToMakeWordBreak = 'q';
			m_data.charToAppend = 0x3c0;  // Greek letter pi
			}
		virtual CLanguageAnalyzerJobBase *makeAnalyzerInstance() {
			return new CSampleAnalyzerJob(m_data);
			}
		static void makeInterface(dtsLanguageAnalyzerInterface& la) {
			CLanguageAnalyzerBase::makeInterface(la);
			la.pCreateAnalyzer = createCB;
			la.name = "Sample Analyzer";
			}
		static dtsAnalyzerHandle createCB() {
			CSampleLanguageAnalyzer *ret = new CSampleLanguageAnalyzer();
			return (dtsAnalyzerHandle) ret;
			}
	protected:
		SamplePerProcessData m_data;
	};


// Simple language analyzer demo
// Only a-z are considered letters.  Anything else causes a word break.
// To demonstrate morphology, it removes the letter charToDelete from every word, and appends
// the letter charToAppend
//
bool CSampleAnalyzerJob::analyze(dtsLaJob& job)
{

	m_outputBuf.clear();
	m_wordTable.clear();
	wchar_t toDelete = m_pWhatToDo->charToDelete;
	wchar_t toAppend = m_pWhatToDo->charToAppend;
	wchar_t charToMakeWordBreak = m_pWhatToDo->charToMakeWordBreak;
	int fWasLetter = false;
	int wordStart = 0;
	DWString wordBuf;
	for (int i = 0; i < job.inputTextLength + 1; ++i) {
		wchar_t ch = ' ';
		if (i < job.inputTextLength)
			ch = job.inputBuffer[i];

		if (ch == toDelete)
			continue;

		if (ch == charToMakeWordBreak)
			ch = ' ';

		// If the line contains any instances of the ! character, the sample analyzer will decline to
		// handle word-breaking and return false.  When a language analyzer returns false,
		// dtSearch applies its default word breaking.
		if (ch == '!')
			return false;

		if (ch > 127)
			continue;
		if (!isalpha(ch) && fWasLetter) {
			// End of a word, so store it in the table
			wordBuf << toAppend;
			dtsLaWordInfo wordInfo;
			wordInfo.offsetInInputBuffer = wordStart;
			wordInfo.lengthInInputBuffer = i - wordStart;
			wordInfo.offsetInOutputBuffer = m_outputBuf.getLength();
			wordInfo.lengthInOutputBuffer = wordBuf.getLength();
			m_wordTable.append(wordInfo);
			m_outputBuf << wordBuf << ' ';

			wordBuf.clear();
			}
		if (isalpha(ch)) {
			if (!fWasLetter) {
				wordStart = i;
				fWasLetter = true;
				}
			wordBuf << ch;
			}
		else
			fWasLetter = false;

		}

	// NOTE:  linkBuffers() must ALWAYS be called after analyze() to provide updated buffer pointers
	// to the dtsLaJob.  Otherwise dtsLaJob will either have null or potentially dangling pointers.
	linkBuffers(job);
	return true;
}

 __declspec(dllexport) BOOL GetLanguageAnalyzer(dtsLanguageAnalyzerInterface& la)
{	CSampleLanguageAnalyzer::makeInterface(la);
	return true;
}


