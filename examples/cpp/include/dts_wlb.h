#ifndef DTS_WLB_H
    #define DTS_WLB_H

#ifndef DTSEARCH_H
    #include <dtsearch.h>
#endif
#ifndef DSTRING_H
    #include <dstring.h>
#endif

/*
	Summary:
	Handle to a word list builder

	Remarks:
	Use the dtsWordListBuilder class to access search results through a dtsWordListBuilder
*/
typedef void *WordListBuilderHandle;

//
// dtSearch Engine functions implementing WordListBuilder
//
// begin-internal
extern "C" {
WordListBuilderHandle DLLFUNC dtsWLB_Alloc();
void DLLFUNC dtsWLB_Free(WordListBuilderHandle hBuilder);
long DLLFUNC dtsWLB_OpenIndex(WordListBuilderHandle hBuilder, const char *indexPath);
void DLLFUNC dtsWLB_CloseIndex(WordListBuilderHandle hBuilder);
void DLLFUNC dtsWLB_ListWords(WordListBuilderHandle hBuilder,
    const char *cursorWord, long aRange);
void DLLFUNC dtsWLB_ListMatchingWords(WordListBuilderHandle hBuilder,
    const char *toMatch, long maxCount, long searchFlags, long fuzziness);
void DLLFUNC dtsWLB_ListFieldValues(WordListBuilderHandle hBuilder, 
	const char *fieldName, const char *word, long maxCount);
void DLLFUNC dtsWLB_SetSearchFilter(WordListBuilderHandle hBuilder, dtsSearchFilterHandle hFilter);    
void DLLFUNC dtsWLB_GetWord(WordListBuilderHandle hBuilder,
    long iWord, char *buf, long bufSize, long *pCount);
void DLLFUNC dtsWLB_GetWord2(WordListBuilderHandle hBuilder,
    long iWord, char *buf, long bufSize, long *pCount, long *pDocCount);
long DLLFUNC dtsWLB_GetCount(WordListBuilderHandle hBuilder);
long DLLFUNC dtsWLB_GetLastError(WordListBuilderHandle hBuilder);
long DLLFUNC dtsWLB_GetFlags(WordListBuilderHandle hBuilder);
void DLLFUNC dtsWLB_SetFlags(WordListBuilderHandle hBuilder, long wordListBuilderFlags);
bool DLLFUNC dtsWLB_Sort(WordListBuilderHandle hBuilder, long flags);
void DLLFUNC dtsWLB_GetField(WordListBuilderHandle hBuilder,
	long iWord, char *buf, long bufSize);
void DLLFUNC dtsWLB_ListFields(WordListBuilderHandle hBuilder,
	long maxCount);
	
void DLLFUNC dtsWLB_GetFieldUnicode(WordListBuilderHandle hBuilder,
	long iWord, wchar_t *buf, long bufSize);
void DLLFUNC dtsWLB_GetWordUnicode(WordListBuilderHandle hBuilder,
		long iWord, wchar_t* buf, long bufSize, long *pCount, long *pDocCount);
	
}
// end-internal


/* Summary
   dtsWordListBuilder provides a way to list words, field names, or field values in
   an index.
   Remarks
   dtsWordListBuilder is intended for quick enumeration of words, field names, or
   field values in an index.
   
   Two ways of listing words are provided, one for listing the words before and
   after a word in an index, and one for listing words that match a search term in
   an index.
   
   The scrolling list of indexed words that updates as a user enters a search
   request in dtSearch Desktop is implemented using WordListBuilder's ListWords
   method. The "Browse Words" dialog box in dtSearch Desktop that lists words
   matching an expression is implemented using the ListMatchingWords method.
   
   Listing of field values only lists values of fields that were designated as
   EnumerableFields when the documents were indexed.
   
   For speed, WordListBuilder does not actually enumerate the references for each
   word and instead relies on counts incrementally stored in the index. Therefore,
   the reported counts may include artifacts of the indexing process such as
   reindexed or removed documents, so the counts may be higher than the actual count
   \of references in the index. Compressing an index will remove these extra
   references.
                                                                                     */
class dtsWordListBuilder {
    public:
    	// Create a dtsWordListBuilder
        dtsWordListBuilder() {
                hBuilder = dtsWLB_Alloc();
                bGood = false;
                };
        ~dtsWordListBuilder() {
                dtsWLB_Free(hBuilder);
                };

        /* Open an index
           Parameters:
           	indexPath:   The index to open
           Returns: 0 on success, or non-zero if the index could not be opened
        */

        long openIndex(const char *indexPath) {;
               long v = dtsWLB_OpenIndex(hBuilder, indexPath);
               bGood = (v == 0);
               return v;
               }
        /* Close the currently-open index */
        void closeIndex() {
               dtsWLB_CloseIndex(hBuilder);
               bGood = false;
               }
        bool good() {
            return bGood;
            }
        /* List aRange words around cursorWord in the index.
		   Parameters:
		 	 cursorWord:   The word the user is currently typing
			 aRange:       Number of words to list before and after cursorWord in the index
		*/
        void listWords(const char *cursorWord, long aRange) {
                dtsWLB_ListWords(hBuilder, cursorWord, aRange);
                }
        /* Summary:
           List all fields in the index
           Parameters:
           	  maxCount:  Maximum number of fields to list
         */
		void listFields(long maxCount = -1) {
				dtsWLB_ListFields(hBuilder, maxCount);
				}
		/* Summary:
		   List all words matching an expression
		   Parameters:
		   		toMatch:      The expression to match, which may include wildcards
		   		maxWords:     Maximum number of words to include in the list
		   		searchFlags:  SearchFlags values indicating how to match the words
		   		fuzziness:    If searchFlags includes dtsSearchFuzzy, the level of fuzziness
		   	Remarks:
		   		toMatch can include a field name to limit the search to words in a particular field.
		   		Example:   name::abc*  would match any word beginning with abc in the name field.
		*/
		void listMatchingWords(const char *toMatch, long maxWords, long searchFlags, long fuzziness) {
				dtsWLB_ListMatchingWords(hBuilder, toMatch, maxWords, searchFlags, fuzziness);
				}

		/* Summary
		   List all values of an enumerable field.
		   Parameters
		   fieldName :  The name of the field to enumerate
		   toMatch :    The expression to match, which may include wildcards
		   maxWords :   Maximum number of words to include in the list
		   Remarks
		   The field must have been designated as an enumerable field (in
		   dtsIndexJob.enumerableFields) when the index was created. If fieldName is not an
		   enumerable field, no values will be returned.
		   
		   To limit the values returned to documents that are part of search results, use
		   setSearchFilter.
		   
		   To improve performance in cases where the same field values have to be enumerated
		   repeatedly with different SearchFilters, you can set the flag dtsWordListEnableFieldValuesCache
		   using setFlags().  This will make listFieldValues calls from the same WordListBuilder
		   faster at the cost of substantial memory use, because the field values and occurrences
		   in the index will be stored in memory until the WordListBuilder is deleted.
		   The amount of memory required is proportional to the number of documents in the index
		   times the number of values in the enumerable field that is cached.
		   
		   See Also
		   <link dtsIndexJob::enumerableFields, dtsIndexJob::enumerableFields Data Member>
		   
		   <link Limiting searches with SearchFilters>
		   
		   <link dtsSearchJob::outputSearchFilterHandle, dtsSearchJob::outputSearchFilterHandle Data Member>
		                                                                                                     */
		void listFieldValues(const char *fieldName, const char *toMatch, long maxWords) {
				dtsWLB_ListFieldValues(hBuilder, fieldName, toMatch, maxWords);
				}
				
		/* Summary
		   Use a search filter (dtsSearchFilter) to limit the items that can be returned
		   from listMatchingWords or listFieldValues.
		   Parameters
		   hFilter :  Search filter to use to limit the results of listFieldValues or
		              listMatchingWords                                                  */
		void setSearchFilter(dtsSearchFilterHandle hFilter) {
				dtsWLB_SetSearchFilter(hBuilder, hFilter);
			}
									
		/* Summary
		   Get the text of one word in the list
		   Parameters
		   iWord :    The word to get
		   buf :      Buffer to receive the text
		   bufSize :  Size of the buffer pointed to by buf
		   count :    [Output] Number of times the word occurs in the index 
		   
		   \Note: To make WordListBuilder fast, it does not actually enumerate all of the references
		   for each word, so in some cases the counts can include artifacts of the indexing
		   process such as removed or reindexed documents, which can make the reported
		   counts higher than the actual counts in the index. Compressing an index will
		   eliminate these extra references.                                                         */
        void getWord(long iWord, char *buf, long bufSize, long& count) {
                dtsWLB_GetWord(hBuilder, iWord, buf, bufSize, &count);
                }
		/* Summary
		   Get the text of one word in the list
		   Parameters
		   iWord :     The word to get
		   buf :       Buffer to receive the text
		   bufSize :   Size of the buffer pointed to by buf
		   count :     [Output] Number of times the word occurs in the index
		   docCount :  [Output] Number of documents in which the word occurs
		   \Note: To make WordListBuilder fast, it does not actually enumerate all of the references
		   for each word, so in some cases the counts can include artifacts of the indexing
		   process such as removed or reindexed documents, which can make the reported
		   counts higher than the actual counts in the index. Compressing an index will
		   eliminate these extra references.                                                         */
        void getWord(long iWord, char *buf, long bufSize, long& count, long& docCount) {
                dtsWLB_GetWord2(hBuilder, iWord, buf, bufSize, &count, &docCount);
                }
		/* Returns the number of items in the list */
        long getCount() {
               return dtsWLB_GetCount(hBuilder);
               }
		/* Summary:
		  Get the name of the field a word occurs in.
		   Parameters:
		   		iWord:        The item in the list
		   		buf: 		  Buffer to receive the field
		   		bufSize:      Size of the buffer pointed to by buf
		   Remarks:
		   getField() only works when listMatchingWords was called with the dtsWlbListFields
		   flag set in searchFlags
		*/
        void getField(long iWord, char *buf, long bufSize) {
        		dtsWLB_GetField(hBuilder, iWord, buf, bufSize);
        		}
        /* Summary
           Sort words in the WordListBuilder
           Parameters
           sortFlags :  SortFlags values specifying the type of sort to perform
           Remarks
           The following sortFlags are supported: dtsSortByWord, and dtsWortByDocCount. Use
           dtsSortAscending to specify ascending order. Otherwise the sort will be in
           descending order.
           
           Sort() only works with ListFieldValues and ListMatchingWords.                    */	
        bool sort(long sortFlags) {
			return dtsWLB_Sort(hBuilder, sortFlags);
			}
		/*
			Returns zero if the last operation succeeded, or an ErrorCodes value if
			the last operation failed */
		long getLastError() {
			return dtsWLB_GetLastError(hBuilder);
			}
		/* Set option settings for the WordListBuilder using WordListBuilderFlags */
		void setFlags(long wordListBuilderFlags) {
			dtsWLB_SetFlags(hBuilder, wordListBuilderFlags);
			}
		/* Returns WordListBuilderFlags with the options currently in effect. */
		long getFlags() {
			return dtsWLB_GetFlags(hBuilder);
			}			
    protected:
        WordListBuilderHandle hBuilder;
        bool bGood;
    };


#endif
