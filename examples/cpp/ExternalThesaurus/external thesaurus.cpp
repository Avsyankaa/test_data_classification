// External Thesaurus.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <string.h>
#include <dtsearch.h>
#include <stdio.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

// Minimal sample thesaurus lookup -- just returns the same
// word with 0-9 appended
// A real thesaurus would look up the word in the thesaurus
// and collect the synonyms or make an iterator to iterate
// over the synonyms, then return one synonym each time
// getNextWord() is called.

class Lookup {
	public:
		Lookup(const char *aWord) {
			strncpy(word, aWord, sizeof word);
			iSynonym = 0;
			}
		~Lookup() {
			strcpy(word, "");
			}
		long getNextWord(char *dest, long maxLen) {
			if (iSynonym > 9)
				return -1;
			char tmp[260];
			sprintf(tmp, "%s%d", word, iSynonym++);
			strncpy(dest, tmp, maxLen);
			return 0;
			}			
	protected:
		char word[256];
		int iSynonym;
	};


//
// These are the exports that the dtSearch Engine looks for to identify an 
// external thesaurus DLL.  
//
// dtsThesaurusStartLookup: Returns a handle to a "Lookup" object that will be used
//    to iterate over the synonyms.  Synonyms are returned one at a time rather than
//    all at once to provide an opportunity for the user to cancel the search.
//
// dtsThesaurusGetNextWord: Returns the next synonym found for a Lookup.  Return -1
//    if there are no more synonyms, or 0 if a synonym was returned.
//
// dtsThesaurusEndLookup: End the lookup and free any associated resources.  Normally
//    will be called after dtsThesaurusGetNextWord returns -1, but could be called
//    earlier.
//

extern "C" {
__declspec(dllexport) dtsThesaurusLookupHandle dtsThesaurusStartLookup(const char *word);
__declspec(dllexport) long dtsThesaurusGetNextWord(dtsThesaurusLookupHandle hLookup, char *dest, long maxLen);
__declspec(dllexport) void dtsThesaurusEndLookup(dtsThesaurusLookupHandle hLookup);
__declspec(dllexport) void dtsThesaurusGetName(char *dest, long maxLen);
};

__declspec(dllexport) void dtsThesaurusGetName(char *dest, long maxLen)
{	strncpy(dest, "Sample Thesaurus", maxLen);
}

__declspec(dllexport) dtsThesaurusLookupHandle dtsThesaurusStartLookup(const char *word)
{	Lookup *l = new Lookup(word);
	return (dtsThesaurusLookupHandle) l;
}

__declspec(dllexport) long dtsThesaurusGetNextWord(dtsThesaurusLookupHandle hLookup, char *dest, long maxLen)
{	Lookup *l = (Lookup *) hLookup;
	return l->getNextWord(dest, maxLen);
}

__declspec(dllexport) void dtsThesaurusEndLookup(dtsThesaurusLookupHandle hLookup)
{	Lookup *l = (Lookup *) hLookup;
	delete l;
}

