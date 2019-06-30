
#include <stdafx.h>
#ifdef _WIN32
#include <shellapi.h> 
#endif
#include <string>
#include <dtconfig.h>
#include <filesystem>
#include <locale.h>

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

const long DemoAppId = 0x00abcdef;

class DemoApp {
    public:
		DemoApp() {};
		~DemoApp() {};
		void run();
    protected:
		void parseFile(std::string input, std::string output);
		unsigned count = 0;
        };

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

void DemoApp::run() {
	unsigned int start_time = clock();
	unsigned int hour, minute, seconds;
	//parseFile("C:/Projects/media", "C:/Projects/Output/media/");
	//parseFile("C:/Projects/exe", "C:/Projects/Output/exe/");
	parseFile("C:/Projects/doc", "C:/Projects/Output/doc/");
	unsigned int end_time = clock();
	seconds = (end_time - start_time) / 1000 % 60;
	minute = (end_time - start_time) / 1000 / 60 % 60;
	hour = (end_time - start_time) / 1000 / 60 / 60 % 60;
	std::cout << "Execution time: " << hour << ":"
		<< minute << ":"
		<< seconds << std::endl;
}

void DemoApp::parseFile(std::string input, std::string output)
{
	//std::string input = "C:/Projects/media";
	//std::string output = "C:/Projects/Output/media/"; 
	for (const auto & entry : std::filesystem::recursive_directory_iterator(input)) {
		if (entry.is_directory())
			continue;
		std::string input_s = entry.path().u8string();
		std::string output_s = output + entry.path().stem().u8string() + ".txt";
		
		dtsFileConvertJob2 job;
		job.inputFile = input_s.c_str();
		
		dtsOutputInfo *info = new dtsOutputInfo();
		info->outputFile = output_s.c_str();
		
		job.output = info;
		job.flags = dtsConvertInlineContainer;
		
		dtsExtractionOptions *options = new dtsExtractionOptions();
		std::string out_images = output + entry.path().stem().u8string() + "_images";
		options->outputLocation = out_images.c_str();
		//options->outputFileListName = (out_images + "/Images.txt").c_str();
		options->unnamedAttachmentLinkText = "Image";
		options->flags = dtsExoExtractImages;
		
		job.extractionOptions = options;
		job.output->outputFormat = it_Utf8;

		short result = 0;
		dtssConvertFile2(job, result);
		if (result)
			std::cout << "dtssConvertFile returned error " << result << "\n";
		std::cout << ++count << std::endl;
	}
}

#if defined(_Windows) && !defined(__WIN32__)

#include <windows.h>

#endif

int main()
{
    DemoApp app;
    app.run();
	system("pause"); 
    return 0;
}
