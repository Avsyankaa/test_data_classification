#include "stdafx.h"
#include <stdio.h>


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported function.
EXTERNALFILEPARSER_API int fnExternalFileParser(void)
{
	return 42;
}




//
//  Demonstrate external viewer API with viewer for ROT13 files
//
//  A Rot13 file is a file format in which the letters a-m are
//  replaced with n-z and vice-versa.  A rot13 file is recognized
//  by the Rot13 version of "Rot13" in the first line.
//
//  This file demonstrates the use of the dtSearch external file
//  parser API to add support for this file format to the dtSearch Engine
//  or to the dtSearch end-user product.
//

const int TypeId = it_LastInternal + 101;
const char *TypeName = "Rot13";

class Rot13Viewer {
    protected:
        dtsInputSourceReader reader;
		dtsFileInfo fileInfo;

    public:
        Rot13Viewer(dtsInputSource& source);
        ~Rot13Viewer();
        void readTextBlock(dtsTextBlock& block);
        void gotoBookMark(dtsBookMark& bookMark);
        void getFileInfo(dtsFileInfo& fi);


        // Static callback functions
        static dtsViewerHandle makeViewerCB(dtsInputSource& source);
        static void destroyViewerCB(dtsViewerHandle handle);
        static void readTextBlockCB(dtsViewerHandle handle, dtsTextBlock& block);
        static void gotoBookMarkCB(dtsViewerHandle handle, dtsBookMark& bookMark);
        static void getFileInfoCB(dtsViewerHandle handle, dtsFileInfo& fi);
    };

Rot13Viewer::Rot13Viewer(dtsInputSource& source)
{   reader.attach(source);
    reader.getFileInfo(fileInfo);
    fileInfo.typeId = TypeId;
    strncpy(fileInfo.typeName, TypeName, sizeof fileInfo.typeName);
};

Rot13Viewer::~Rot13Viewer()
{
}

//  Perform rot13 encoding/decoding
void rot13(char *s, int len)
{   while(len--) {
        char c = *s, d;
        if (isalpha(c)) {
            if (isupper(c)) {
                d = c - 'A';
                d = (d + 13) % 26;
                c = d + 'A';
                }
            else {
                d = c - 'a';
                d = (d + 13) % 26;
                c = d + 'a';
                }
            *s = c;
            }
        s++;
        }
}

void Rot13Viewer::readTextBlock(dtsTextBlock& block)
{   block.bookMark.ptr = reader.tell();
	block.textLen = reader.read(block.buf, block.bufferSize);
    reader.seek(reader.tell() + block.bufferSize);
    rot13(block.buf, block.textLen);
}

void Rot13Viewer::gotoBookMark(dtsBookMark& bookMark)
{   reader.seek(bookMark.ptr);
}

void Rot13Viewer::getFileInfo(dtsFileInfo& fi)
{   fi.copy(fileInfo);
}

void Rot13Viewer::destroyViewerCB(dtsViewerHandle handle)
{   Rot13Viewer *v = (Rot13Viewer *)handle;
    delete v;
}

dtsViewerHandle Rot13Viewer::makeViewerCB(dtsInputSource& source)
{   return (dtsViewerHandle) (new Rot13Viewer(source));
}

void Rot13Viewer::readTextBlockCB(dtsViewerHandle handle, dtsTextBlock& block)
{   Rot13Viewer *v = (Rot13Viewer *)handle;
    v->readTextBlock(block);
}

void Rot13Viewer::gotoBookMarkCB(dtsViewerHandle handle, dtsBookMark& bookMark)
{   Rot13Viewer *v = (Rot13Viewer *)handle;
    v->gotoBookMark(bookMark);
}

void Rot13Viewer::getFileInfoCB(dtsViewerHandle handle, dtsFileInfo& fi)
{   Rot13Viewer *v = (Rot13Viewer *)handle;
    v->getFileInfo(fi);
}

// Demonstrates use of external highlight data function 
static void MakeHighlightData(dtsHighlightDataRequest& req)
{	char temp[1000];
	sprintf(temp, "<SampleHighlightData>%s</SampleHighlightData>", req.resultsItem->filename);
	strncpy(req.dest, temp, req.maxLen);
	req.dataSize = strlen(temp);
}

static void MakeRot13ViewerInfo(dtsViewerInfo& viewerInfo)
{   viewerInfo.clear();

    // Build a ViewerInfo to pass to RegisterViewer
    strcpy(viewerInfo.name, "rot13");
	viewerInfo.typeId = it_LastInternal + 101;
    viewerInfo.signature.offset = 0;
    viewerInfo.signature.length = 5;
    viewerInfo.signature.text = "ebg13"; // rot13 version of "rot13"
    viewerInfo.signature.confidence = dtsMaxConfidence;
    viewerInfo.recognize = 0;

    viewerInfo.blockSize = 256;

    // Callbacks
    viewerInfo.makeViewer = Rot13Viewer::makeViewerCB;
    viewerInfo.destroyViewer = Rot13Viewer::destroyViewerCB;
    viewerInfo.gotoBookMark = Rot13Viewer::gotoBookMarkCB;
    viewerInfo.readTextBlock = Rot13Viewer::readTextBlockCB;
    viewerInfo.getFileInfo = Rot13Viewer::getFileInfoCB;
	viewerInfo.makeHighlightData = MakeHighlightData;
}

//
//  If this program is compiled to a DLL, and the DLL is
//  placed in a subdirectory named "viewers" of the directory
//  where either dtSearch or the dtSearch Engine is installed
//  (32-bit versions only), dtSearch will look for an exported
//  function named dtsMakeViewerInfo and will call it to
//  register the file format.
//
//
extern "C" {
	__declspec(dllexport) int dtsMakeViewerInfo(int i, dtsViewerInfo& info);
	};

__declspec(dllexport) int dtsMakeViewerInfo(int i, dtsViewerInfo& info)
{	switch(i) {
		case 0:
			MakeRot13ViewerInfo(info);
			return 0;
		default:
			return -1;  // no more viewers in this DLL
		}
}

