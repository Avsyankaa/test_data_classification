#include "stdafx.h"

#include "SearchJob.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;
using namespace dtSearch::Engine;

// ------------------ It Just Works! (IJW) code -----------------------

class SearchJobStatus {
	protected:
		DCriticalSection critSec;
	public:
		long totalFiles;
		long totalHits;
		char foundName[256];
		long foundHits;
		SearchJobStatus():
			totalFiles(0),
			totalHits(0),
			foundHits(0)
            {   memset(foundName, 0, sizeof foundName); }
		SearchJobStatus& operator=(const SearchJobStatus& other) {
			DCriticalSection::Lock l(critSec);
            totalFiles = other.totalFiles;
            totalHits = other.totalHits;
            strcpy(foundName, other.foundName);
            foundHits = other.foundHits;
			return *this;
			}
		void clear() {
			DCriticalSection::Lock l(critSec);
			totalFiles = 0;
			totalHits = 0;
			strcpy(foundName, "");
			foundHits = 0;
			};
		void reportFound(const char *name, long hits) {
			DCriticalSection::Lock l(critSec);
			strncpy(foundName, name, sizeof foundName);
			foundHits = hits;
			totalHits += hits;
			totalFiles ++;
			}
	};


class NetSearchJob : public DSearchJob {
	public:
		NetSearchJob();
		~NetSearchJob();
		void NetSearchJob::setStatusHandler(gcroot<ISearchStatusHandler*> newHandler);
		virtual void OnFound(long totalFiles, long totalHits, const char *name, long hitsInFile, dtsSearchResultsItem& item);\
	    virtual void OnSearchingIndex(const char * indexPath );
	    virtual void OnSearchingFile(const char * file , long filesSearched , long filesToSearch  );

		virtual void OnCheckAbort();
		virtual void handleMessage(dtsMessage& msg);
		SearchJobStatus status;
	protected:
		gcroot<ISearchStatusHandler*> m_statusHandler;
		gcroot<SearchResultsItem*> m_item;
	};

NetSearchJob::NetSearchJob()
{
	m_statusHandler = NULL;
	m_item = NULL;
}

NetSearchJob::~NetSearchJob()
{
	m_statusHandler = NULL;
	m_item = NULL;
}

void NetSearchJob::handleMessage(dtsMessage& msg)
{
	DSearchJob::handleMessage(msg);
//	if (m_statusHandler && (msg.command != dtsnCheckForAbort))
//		m_statusHandler->handleMessage(msg);
}

void NetSearchJob::OnFound(long totalFiles, long totalHits, const char *fn, long hitsInFile, dtsSearchResultsItem& item)
{	LogFunction("NetSearchJob::OnFound", fn);
	status.totalFiles = totalFiles;
	status.totalHits = totalHits;
	if (m_statusHandler != NULL) {
		m_item = Converters::MakeResultsItem(&item);
		m_item->VetoThisItem = false;
		m_statusHandler->OnFound(m_item);
		if (m_item->VetoThisItem)
			VetoThisItem();
		}
}

void NetSearchJob::OnSearchingIndex(const char * indexPath )
{	if (m_statusHandler) {
		if (strIsBlank(indexPath))
			indexPath = "";
	
		m_statusHandler->OnSearchingIndex(DString::allocNetString(indexPath));
		}
}

void NetSearchJob::OnSearchingFile(const char * file , long filesSearched , long filesToSearch  )
{	if (m_statusHandler)
		m_statusHandler->OnSearchingFile(DString::allocNetString(file));
}


void NetSearchJob::OnCheckAbort()
{
    int v = dtsContinue;
    if (m_statusHandler != 0)
        v = m_statusHandler->CheckForAbort();
    if (v == dtsAbort)
        Cancel();	
}

void NetSearchJob::setStatusHandler(gcroot<ISearchStatusHandler*> newHandler)
{	
	m_statusHandler = newHandler;

}
namespace dtSearch { namespace Engine 
{
	// Constructor sets up error handling and non-zero constants
	SearchJob::SearchJob() :
		NetJobBase("SearchJob"),
		m_resultsReady(false)
	{	CountCreate(otSearchJob);
	
		m_IndexesToSearch = new StringCollection;
		m_IncludeFilters = new StringCollection;
		m_ExcludeFilters = new StringCollection;
		m_FoldersToSearch = new StringCollection;
		
		m_searchFilter = 0;
	}

	// Destructor aborts thread if running
	SearchJob::~SearchJob ()
	{
		debugLog("SearchJob::~SearchJob...");
		m_IndexesToSearch = 0;
		m_IncludeFilters = 0;
		m_ExcludeFilters = 0;
		m_FoldersToSearch = 0;
		Dispose(false);
		CountDelete(otSearchJob);
		debugLog("SearchJob::~SearchJob...Done");
	}
	
	void SearchJob::DisposeUnmanagedItems()
	{	debugLog("SearchJob::DisposeUnmanagedItems...");
		freeFilter();
		NetJobBase::DisposeUnmanagedItems();
		debugLog("SearchJob::DisposeUnmanagedItems...Done");
	}
	
	void SearchJob::DisposeManagedItems()
	{	// This would not be safe because caller may still have references to 
		// m_Results
		// if (m_Results) {
		//	m_Results->Dispose();
		//	m_Results = 0;
		//	}
	}
	
	bool SearchJob::IsThreadDone ()
	{	return NetJobBase::IsThreadDoneImp() && m_resultsReady;
	}
	

	void SearchJob::SetFilter (SearchFilter* newFilter)
	{	freeFilter();
		if (newFilter) 
			m_searchFilter = new dtsSearchFilter(*newFilter->getImplementation ());
	}

	bool SearchJob::Execute ()
	{	debugLog("<.NET SearchJob::Execute>");
		m_resultsReady = false;
		
		NetSearchJob *sj = new NetSearchJob;
		makeJob (*sj);
		
		bool ret = ExecuteJob(sj);
		
		debugLog("ExecuteJob done... 1");

		// Get results
		DSearchResults *res = sj->DetachResults();

		// Update counts from results
		if (res)
		{
			m_FileCount = res->getTotalFiles();
			m_HitCount = res->getTotalHits();
		}
		else {
			m_FileCount = 0;
			m_HitCount = 0;
			}

		debugLog("ExecuteJob done... 2");

		// Save the results into accessible object
		m_Results = new SearchResults(res, true);


		debugLog("ExecuteJob done... 3");
		
		// Save the results as Filter
		if (m_WantResultsAsFilter)
		{	dtsSearchFilter *filter = sj->DetachOutputSearchFilter();
			m_ResultsAsFilter = new SearchFilter (filter);
			delete filter;
		}

		debugLog("</.NET SearchJob::Execute>");
		m_resultsReady = true;	
		return ret;
	}

	void SearchJob::ExecuteInThread ()
	{	LogFunction("SearchJob::ExecuteInThread", "");
		NetJobBase::ExecuteInThreadImp();
	}

	bool SearchJob::IsThreadDoneWait (long millisecondsToWait)
	{	return NetJobBase::IsThreadDoneWaitImp(millisecondsToWait) && m_resultsReady;
	}

	void SearchJob::AbortThread ()
	{	NetJobBase::AbortThread(AbortValue::Cancel);
	}


	// Sets up an unmanaged search job from managed properties
	void SearchJob::makeJob(NetSearchJob& job)
	{	LogFunction("SearchJob::makeJob", "");
	
		job.ClearSearchFlags();
		job.SetSearchFlag(m_SearchFlags, true);
		job.SetFuzzySearching(m_Fuzziness);
		job.SetMaxFiles(m_MaxFilesToRetrieve);
		job.SetTimeout(m_TimeoutSeconds);
		job.SetStopLimit(m_AutoStopLimit);
		
		job.ExcludeFilters.copy(m_ExcludeFilters);
		job.IncludeFilters.copy(m_IncludeFilters);
		
		job.IndexesToSearch.copy(m_IndexesToSearch);
		job.FoldersToSearch.copy(m_FoldersToSearch);
		job.SetWantResultsAsFilter( m_WantResultsAsFilter );

		job.Request.set(m_Request);
		job.FileConditions.set(m_FileConditions);
		job.BooleanConditions.set(m_BooleanConditions);
		job.FieldWeights.set(m_FieldWeights);
		
		if (m_searchFilter != NULL)
			job.SetSearchFilter (m_searchFilter->getHandle());
			
		if (m_StatusHandler != NULL)
			job.setStatusHandler(m_StatusHandler);
		debugLog("makeJob done");
	}
	

}}


