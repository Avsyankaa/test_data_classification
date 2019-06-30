#include "stdafx.h"

#include "SearchReportJob.h"
#include "SearchResults.h"


using namespace System;

namespace dtSearch { namespace Engine 
{
	SearchReportJob::SearchReportJob ()
	{	debugLog("dtSearch.Engine.SearchReportJob::SearchReportJob");
		m_OutputFormat = itHTML;
		m_OutputToString = 0;
		m_OutputStringMaxSize = 0;
		m_TimeoutSeconds = 0;
		m_WordsOfContext = 0;
		m_WordsOfContextExact = 0;
		m_ParagraphsOfContext = 0;
		m_MaxWordsToRead = 0;
		m_MaxContextBlocks = 0;
		m_Flags = (dtSearch::Engine::ReportFlags) 0;
		m_pObjParentResults = NULL;
		m_pResults = NULL;

		m_Errors = new JobErrorInfo;
		
		CountCreate(otSearchReportJob);
	}

	SearchReportJob::~SearchReportJob ()
	{	debugLog("dtSearch.Engine.SearchReportJob::~SearchReportJob");
		Dispose(false);
		CountDelete(otSearchReportJob);
	}

	void SearchReportJob::SetResults(SearchResults* parent)
	{
		m_pObjParentResults = parent;
		m_pResults = parent->getImplementation();
		m_job = new DSearchReportJob ();
		m_job->AttachResults(m_pResults, false);
	}

	void SearchReportJob::ClearSelections ()
	{
		if (m_job != NULL)
			m_job->ClearSelections ();
	}

	void SearchReportJob::SelectAll ()
	{
		if (m_job != NULL)
			m_job->SelectAll ();
	}

	void SearchReportJob::SelectItems (long iFirstItem, long iLastItem)
	{
		if (m_job != NULL)
			m_job->SelectRange (iFirstItem, iLastItem);
	}

	void SearchReportJob::Execute ()
	{
		// Set output destination
		if (m_OutputToString)
			m_job->SetOutputToString(m_OutputStringMaxSize);
		else
			m_job->OutputFile.set(m_OutputFile);

		// Set properties
		m_job->SetOutputFormat(m_OutputFormat);
		m_job->BeforeHit.set((m_BeforeHit));
		m_job->AfterHit.set((m_AfterHit));
		m_job->Footer.set((m_Footer));
		m_job->Header.set((m_Header));
		m_job->ContextHeader.set((m_ContextHeader));
		m_job->ContextFooter.set((m_ContextFooter));
		m_job->ContextSeparator.set((m_ContextSeparator));
		m_job->FileHeader.set((m_FileHeader));
		m_job->FileFooter.set((m_FileFooter));
		m_job->SetMaxContextBlocks(m_MaxContextBlocks);
		m_job->SetMaxWordsToRead(m_MaxWordsToRead);

		// Set context words/paragraphs
		if (m_ParagraphsOfContext)
			m_job->SetContextParagraphs(m_ParagraphsOfContext);
		else if (m_WordsOfContext)
			m_job->SetContextWords(m_WordsOfContext, false);
		else
			m_job->SetContextWords(m_WordsOfContextExact, true);
	    
		// Set flags
		long flags = m_job->GetFlags();
		flags |= m_Flags;
		m_job->SetFlags(flags);
	    
		// Set timeout
		m_job->SetTimeout (m_TimeoutSeconds);

		// Run the job and retrieve any errors
		m_job->Execute();
		m_Errors->Copy(m_job->getErrors());

		// Store result into OutputString if OutputToString is true
		if (m_OutputToString)
		{
			m_OutputString = m_job->outputString.allocNetString();
		}
	}
	void SearchReportJob::DisposeUnmanagedItems() {
		if (m_job) {
			DSearchReportJob *toDelete = m_job;
			m_job = 0;
			delete toDelete;
			}
		}

}}
