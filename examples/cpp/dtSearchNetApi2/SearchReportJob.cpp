#include "stdafx.h"

#include "SearchReportJob.h"
#include "SearchResults.h"


using namespace System;

namespace dtSearch { namespace Engine 
{
	SearchReportJob::SearchReportJob ()
	{	debugLog("dtSearch.Engine.SearchReportJob::SearchReportJob");
		OutputFormat = OutputFormats::itHTML;
		m_pObjParentResults = nullptr;
		m_pResults = nullptr;
		m_job = nullptr;
		Errors = gcnew JobErrorInfo;
		
		CountCreate(otSearchReportJob);
	}

	SearchReportJob::~SearchReportJob ()
	{	debugLog("dtSearch.Engine.SearchReportJob::~SearchReportJob");
		CountDelete(otSearchReportJob);
	}

	void SearchReportJob::SetResults(SearchResults^ parent)
	{
		m_pObjParentResults = parent;
		m_pResults = parent->getImplementation();
		if (m_job) {
			delete m_job;
			m_job = 0;
			}
		m_job = new DSearchReportJob ();
		m_job->AttachResults(m_pResults, false);
	}

	void SearchReportJob::ClearSelections ()
	{
		if (m_job != nullptr)
			m_job->ClearSelections ();
	}

	void SearchReportJob::SelectAll ()
	{
		if (m_job != nullptr)
			m_job->SelectAll ();
	}

	void SearchReportJob::SelectItems (long iFirstItem, long iLastItem)
	{
		if (m_job != nullptr)
			m_job->SelectRange (iFirstItem, iLastItem);
	}

	void SearchReportJob::Execute ()
	{
		// Set output destination
		if (OutputToString)
			m_job->SetOutputToString(OutputStringMaxSize);
		else
			m_job->OutputFile.set(OutputFile);

		// Set properties
		m_job->SetOutputFormat((int) OutputFormat);
		m_job->BeforeHit.set((BeforeHit));
		m_job->AfterHit.set((AfterHit));
		m_job->Footer.set((Footer));
		m_job->Header.set(Header);
		m_job->ContextHeader.set(ContextHeader);
		m_job->ContextFooter.set(ContextFooter);
		m_job->ContextSeparator.set(ContextSeparator);
		m_job->FileHeader.set(FileHeader);
		m_job->FileFooter.set(FileFooter);
		m_job->SetMaxContextBlocks(MaxContextBlocks);
		m_job->SetMaxWordsToRead(MaxWordsToRead);

		// Set context words/paragraphs
		if (ParagraphsOfContext)
			m_job->SetContextParagraphs(ParagraphsOfContext);
		else if (WordsOfContext)
			m_job->SetContextWords(WordsOfContext, false);
		else
			m_job->SetContextWords(WordsOfContextExact, true);
	    
		// Set flags
		long flags = m_job->GetFlags();
		flags |= (long) Flags;
		m_job->SetFlags(flags);
	    
		// Set timeout
		m_job->SetTimeout (TimeoutSeconds);

		// Run the job and retrieve any errors
		m_job->Execute();
		Errors->Copy(m_job->getErrors());

		// Store result into OutputString if OutputToString is true
		if (OutputToString)
		{
			OutputString = m_job->outputString.allocNetString();
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
