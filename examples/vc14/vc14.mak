#
#  This makefile requires an environment variable PFX86
#  that points to the c:\Program Files (x86) folder on X64 systems
#  and to the c:\Program Files folder on X86 systems.
# 
#  It also assumes that Visual Studio was installed to the default location.
#


VSROOT14 = 		$(PFX86)\Microsoft Visual Studio 14.0
MSDEV14 =        "$(VSROOT14)\Common7\IDE\devenv.com"

VC14_EXAMPLES = .
LOGS = $(VC14_EXAMPLES)

VC14_DTSFCLIB = 				"$(VC14_EXAMPLES)\dtsfclib\dtsfclib.sln"

VC14_TEXTDEMO = 				"$(VC14_EXAMPLES)\textdemo\textdemo.sln"
VC14_DSOURCE = 				"$(VC14_EXAMPLES)\dsource\dsource.sln"

VC14_MFCDEMO = 				"$(VC14_EXAMPLES)\mfcdemo\mfcdemo.sln"
VC14_EXTERNALTHESAURUS = 	"$(VC14_EXAMPLES)\externalthesaurus\external thesaurus.sln"
VC14_INDEXCACHEDEMO = 		"$(VC14_EXAMPLES)\indexcachedemo\indexcachedemo.sln"
VC14_LANGUAGEANALYZER = 		"$(VC14_EXAMPLES)\languageanalyzer\languageanalyzer.sln"
VC14_EXTERNALFILEPARSER = 	"$(VC14_EXAMPLES)\externalfileparser\externalfileparser.sln"

all : VC14 VC14d

VC14 :  VC14_dtsfclib  VC14_textdemo  VC14_mfcdemo  VC14_ExternalThesaurus  VC14_IndexCacheDemo VC14_LanguageAnalyzer VC14_dsource VC14_ExternalFileParser

VC14d : VC14d_dtsfclib VC14d_textdemo VC14d_mfcdemo VC14d_ExternalThesaurus VC14d_IndexCacheDemo VC14d_LanguageAnalyzer VC14d_dsource


clean : 
	del $(VC14_EXAMPLES)\*.obj /s
	del $(VC14_EXAMPLES)\*.pch /s
	del $(VC14_EXAMPLES)\*.ncb /s

VC14_dtsfclib :
    @echo VC14 - dtsfclib - Release Static
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Release Static|Win32" /out $(LOGS)/$@.log /nologo > $(LOGS)/$@.txt
    @echo VC14 - dtsfclib - Release 
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Release|Win32" /out $(LOGS)/$@.log /nologo > $(LOGS)/$@.txt
    @echo VC14 - dtsfclib - Release Static - x64
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Release Static|x64" /out $(LOGS)/$@.log /nologo > $(LOGS)/$@.txt
    @echo VC14 - dtsfclib - Release - x64
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Release|x64" /out $(LOGS)/$@.log /nologo > $(LOGS)/$@.txt

VC14d_dtsfclib :
    @echo VC14 - dtsfclib - Debug
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo > $(LOGS)/$@.txt
    @echo VC14 - dtsfclib - Debug - x64
    @$(MSDEV14) $(VC14_DTSFCLIB) /build "Debug|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    
VC14_textdemo :
    @echo VC14 - textdemo - Release 
    @$(MSDEV14) $(VC14_TEXTDEMO) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - textdemo - Release - x64
    @$(MSDEV14) $(VC14_TEXTDEMO) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_textdemo :
    @echo VC14 - textdemo - Debug 
    @$(MSDEV14) $(VC14_TEXTDEMO) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - textdemo - Debug - x64
    @$(MSDEV14) $(VC14_TEXTDEMO) /build "Debug|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14_mfcdemo :
    @echo VC14 - mfcdemo - Release 
    @$(MSDEV14) $(VC14_MFCDEMO) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - mfcdemo - Release - x64
    @$(MSDEV14) $(VC14_MFCDEMO) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_mfcdemo :
    @echo VC14 - mfcdemo - Debug 
    @$(MSDEV14) $(VC14_MFCDEMO) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - mfcdemo - Debug - x64
    @$(MSDEV14) $(VC14_MFCDEMO) /build "Debug|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14_IndexCacheDemo :
    @echo VC14 - IndexCacheDemo - Release
    @$(MSDEV14) $(VC14_INDEXCACHEDEMO) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - IndexCacheDemo - Release - x64
    @$(MSDEV14) $(VC14_INDEXCACHEDEMO) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_IndexCacheDemo :
    @echo VC14 - IndexCacheDemo - Debug
    @$(MSDEV14) $(VC14_INDEXCACHEDEMO) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14_LanguageAnalyzer :
    @echo VC14 - LanguageAnalyzer - Release
    @$(MSDEV14) $(VC14_LANGUAGEANALYZER) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - LanguageAnalyzer - Release - x64
    @$(MSDEV14) $(VC14_LANGUAGEANALYZER) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    
VC14_ExternalFileParser :
	@echo VC14 - ExternalFileParser - Release   
    @$(MSDEV14) $(VC14_EXTERNALFILEPARSER) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
	@echo VC14 - ExternalFileParser - Release - x64
    @$(MSDEV14) $(VC14_EXTERNALFILEPARSER) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    
VC14_ExternalThesaurus :
    @echo VC14 - ExternalThesaurus - Release
    @$(MSDEV14) $(VC14_EXTERNALTHESAURUS) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - ExternalThesaurus - Release - x64
    @$(MSDEV14) $(VC14_EXTERNALTHESAURUS) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_ExternalThesaurus :
    @echo VC14 - ExternalThesaurus - Debug
    @$(MSDEV14) $(VC14_EXTERNALTHESAURUS) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_LanguageAnalyzer :
    @echo VC14 - LanguageAnalyzer - Debug
    @$(MSDEV14) $(VC14_LANGUAGEANALYZER) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14_dsource : 
    @echo VC14 - dsource - Release 
    @$(MSDEV14) $(VC14_DSOURCE) /build "Release|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt 
    @echo VC14 - dsource - Release - x64
    @$(MSDEV14) $(VC14_DSOURCE) /build "Release|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt

VC14d_dsource :
    @echo VC14 - dsource - Debug 
    @$(MSDEV14) $(VC14_DSOURCE) /build "Debug|Win32" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
    @echo VC14 - dsource - Debug - x64
    @$(MSDEV14) $(VC14_DSOURCE) /build "Debug|x64" /out $(LOGS)/$@.log /nologo >> $(LOGS)/$@.txt
