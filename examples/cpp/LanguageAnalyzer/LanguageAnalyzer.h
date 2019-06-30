// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LANGUAGEANALYZER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LANGUAGEANALYZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LANGUAGEANALYZER_EXPORTS
#define LANGUAGEANALYZER_API __declspec(dllexport)
#else
#define LANGUAGEANALYZER_API __declspec(dllimport)
#endif

// This class is exported from the LanguageAnalyzer.dll
class LANGUAGEANALYZER_API CLanguageAnalyzer {
public:
	CLanguageAnalyzer(void);
	// TODO: add your methods here.
};

extern LANGUAGEANALYZER_API int nLanguageAnalyzer;

LANGUAGEANALYZER_API int fnLanguageAnalyzer(void);
