#include "stdafx.h"

#include "SearchJob.h"
#include "IndexCache.h"

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
            strCopy(foundName, other.foundName, sizeof foundName);
            foundHits = other.foundHits;
			return *this;
			}
		void clear() {
			DCriticalSection::Lock l(critSec);
			totalFiles = 0;
			totalHits = 0;
			strCopy(foundName, "", sizeof foundName);
			foundHits = 0;
			};
		void reportFound(const char *name, long hits) {
			DCriticalSection::Lock l(critSec);
			strCopy(foundName, name, sizeof foundName);
			foundHits = hits;
			totalHits += hits;
			totalFiles ++;
			}
	};


class NetSearchJob : public DSearchJob {
	public:
		NetSearchJob();
		~NetSearchJob();
		void setStatusHandler(gcroot<ISearchStatusHandler ^> newHandler);
		void setStatusHandler2(gcroot<ISearchStatusHandler2 ^> newHandler2);
		virtual void OnFound(long totalFiles, long totalHits, const char *name, long hitsInFile, dtsSearchResultsItem& item);
		virtual void OnCheckAbort();
		virtual void OnSearchingIndex(const char *indexPath);
		virtual void OnSearchingFile(const char *file, long toSearch, long searched);
		virtual void OnProgressUpdate(dtsSearchProgressInfo& info);
		virtual void handleMessage(dtsMessage& msg);
		SearchJobStatus status;
	protected:
		gcroot<ISearchStatusHandler^> m_statusHandler;
		gcroot<SearchResultsItem^> m_item;
		gcroot<ISearchStatusHandler2^> m_statusHandler2;
		bool bHaveStatusHandler2;
	};

NetSearchJob::NetSearchJob()
{	bHaveStatusHandler2 = false;
	m_statusHandler = nullptr;
	m_statusHandler2 = nullptr;
	m_item = nullptr;
}

NetSearchJob::~NetSearchJob()
{
	m_statusHandler = nullptr;
	m_statusHandler2 = nullptr;
	m_item = nullptr;
}

void NetSearchJob::handleMessage(dtsMessage& msg)
{
	DSearchJob::handleMessage(msg);
}

void NetSearchJob::OnFound(long totalFiles, long totalHits, const char *fn, long hitsInFile, dtsSearchResultsItem& item)
{	LogFunction("NetSearchJob::OnFound", fn);
	status.totalFiles = totalFiles;
	status.totalHits = totalHits;
	if (m_statusHandler) {
		m_item = Converters::MakeResultsItem(&item);
		m_item->VetoThisItem = false;
		m_statusHandler->OnFound(m_item);
		if (m_item->VetoThisItem)
			VetoThisItem();
		}
}


void NetSearchJob::OnSearchingFile(const char *file, long toSearch, long searched)
{
	if (m_statusHandler)
		m_statusHandler->OnSearchingFile(DString::allocNetString(file));
}

void NetSearchJob::OnSearchingIndex(const char *indexPath)
{
	if (m_statusHandler)
		m_statusHandler->OnSearchingIndex(DString::allocNetString(indexPath));
}	


void NetSearchJob::OnCheckAbort()
{
    int v = dtsContinue;
    if (m_statusHandler)
        v = (int) m_statusHandler->CheckForAbort();
    if (v == dtsAbort)
        Cancel();	
}

void NetSearchJob::OnProgressUpdate(dtsSearchProgressInfo& info)
{	if (m_statusHandler2) {
		SearchProgressInfo ^ret = gcnew SearchProgressInfo;
		ret->FilesSearched = info.filesSearched;
		ret->FilesToSearch = info.filesToSearch;
		ret->TotalFileCount = info.totalFileCount;
		ret->TotalHitCount = info.totalHitCount;
		ret->UpdateType = (dtSearch::Engine::MessageCode) info.updateType;
		if (info.currentFile && !strIsBlank(info.currentFile->name)) {
			ret->File->Name = Converters::MakeNetString(info.currentFile->name);
			if (!strIsBlank(info.currentFile->type))
				ret->File->Type = Converters::MakeNetString(info.currentFile->type);
			ret->File->TypeId = (dtSearch::Engine::TypeId) info.currentFile->typeId;
			ret->File->WordCount = info.currentFile->wordCount;
			ret->File->Size = info.currentFile->size;
			ret->File->Flags = (dtSearch::Engine::FileInfoFlags) info.currentFile->fileInfoFlags;
			}
		m_statusHandler2->OnProgressUpdate(ret);
		}
}

void NetSearchJob::setStatusHandler(gcroot<ISearchStatusHandler ^> newHandler)
{
	m_statusHandler = newHandler;
	bHaveStatusHandler2 = false;
}

void NetSearchJob::setStatusHandler2(gcroot<ISearchStatusHandler2 ^> newHandler2)
{
	m_statusHandler = newHandler2;
	m_statusHandler2 = newHandler2;
	bHaveStatusHandler2 = false;

}

namespace dtSearch { namespace Engine 
{
	// Constructor sets up error handling and non-zero constants
	SearchJob::SearchJob() :
		NetJobBase("SearchJob"),
		m_resultsReady(false)

	{	CountCreate(otSearchJob);
	
		IndexesToSearch = gcnew StringCollection;
		IncludeFilters = gcnew StringCollection;
		ExcludeFilters = gcnew StringCollection;
		FoldersToSearch = gcnew StringCollection;
		
		m_searchFilter = nullptr;
		m_pIndexCache = nullptr;
		m_pObjIndexCache = nullptr;
	}

	// Destructor aborts thread if running
	SearchJob::~SearchJob ()
	{
		IndexesToSearch = nullptr;
		IncludeFilters = nullptr;
		ExcludeFilters = nullptr;
		FoldersToSearch = nullptr;
		CountDelete(otSearchJob);
		m_pIndexCache = nullptr;
		m_pObjIndexCache = nullptr;
	}
	
	void SearchJob::DisposeUnmanagedItems()
	{	debugLog("SearchJob::DisposeUnmanagedItems");
		freeFilter();
		NetJobBase::DisposeUnmanagedItems();
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
	{		return NetJobBase::IsThreadDoneImp() && m_resultsReady;
	}
	
	void SearchJob::SetIndexCache(IndexCache ^aCache)
	{	// Keep a reference to the object so it does not get garbage collected
		m_pObjIndexCache = aCache;
		if (aCache != nullptr)
			m_pIndexCache = aCache->getImplementation();
		else
			m_pIndexCache = nullptr;
	}
	
	void SearchJob::SetFilter (SearchFilter^ newFilter)
	{	freeFilter();
		if (newFilter == nullptr) 
			reportError("SetFilter called with null filter");
		else {
			dtsSearchFilter *imp = newFilter->getImplementation();
			if (!imp) 
				reportError("SetFilter called with disposed filter");
			else if (!imp->good())
				reportError("SetFilter called with null handle");
			else if (!imp->checkHandle()) 
				reportError("SetFilter called with invalid handle");
			else {
				m_searchFilter = new dtsSearchFilter(*newFilter->getImplementation ());
				if (!m_searchFilter->good()) {
					reportError("Unable to copy SearchFilter -- SetFilter failed");
					freeFilter();
				}
				else if (!m_searchFilter->checkHandle()) {
					reportError("Unable to copy SearchFilter -- SetFilter failed");
					freeFilter();
				}
			}

		}
	}

	bool SearchJob::Execute ()
	{	LogFunction(".NET SearchJob::Execute", "");
		m_resultsReady = false;

		NetSearchJob *sj = new NetSearchJob;
		makeJob (*sj);
		
		bool ret = ExecuteJob(sj);

		// Get results
		DSearchResults *res = sj->DetachResults();

		// Update counts from results
		if (res)
		{
			FileCount = res->getTotalFiles();
			HitCount = res->getTotalHits();
		}
		else {
			FileCount = 0;
			HitCount = 0;
			}

		// Save the results into accessible object
		Results = gcnew SearchResults(res, true);

		// Save the results as Filter
		if (WantResultsAsFilter)
		{	dtsSearchFilter *filter = sj->DetachOutputSearchFilter();
			ResultsAsFilter = gcnew SearchFilter (filter, true);
			delete filter;
		}

		debugLog("SearchJob done", "");
		
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
		job.SetSearchFlag((int) SearchFlags, true);
		job.SetUnindexedSearchFlags((short) UnindexedSearchFlags);
		job.SetFuzzySearching(Fuzziness);
		job.SetMaxFiles(MaxFilesToRetrieve);
		job.SetTimeout(TimeoutSeconds);
		job.SetStopLimit(AutoStopLimit);
		
		job.ExcludeFilters.copy(ExcludeFilters);
		job.IncludeFilters.copy(IncludeFilters);
		
		job.IndexesToSearch.copy(IndexesToSearch);
		job.FoldersToSearch.copy(FoldersToSearch);
		job.SetWantResultsAsFilter( WantResultsAsFilter );

		job.Request.set(Request);
		job.FileConditions.set(FileConditions);
		job.BooleanConditions.set(BooleanConditions);
		job.FieldWeights.set(FieldWeights);
		
		if (m_searchFilter != nullptr)
			job.SetSearchFilter (m_searchFilter->getHandle());
			
		if (StatusHandler != nullptr) {
			ISearchStatusHandler2 ^h2 = dynamic_cast<ISearchStatusHandler2 ^>(StatusHandler);
			if (h2 != nullptr)
				job.setStatusHandler2(h2);
			else
				job.setStatusHandler(StatusHandler);
			}
			
		if (m_pIndexCache != nullptr)
			job.SetIndexCache(*m_pIndexCache);
		debugLog("makeJob done");
	}
	

}}


