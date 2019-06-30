#include "stdafx.h"
#include "Server.h"
#include "FileConverter.h"
#include "SearchResults.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;
using namespace dtSearch::Engine;




namespace dtSearch { namespace Engine 
{
	FileConverter::FileConverter ()
	{	CountCreate(otFileConverter);
		// Setup error handling
		m_convertJob = 0;
		m_Errors = new JobErrorInfo();

		// Setup default values
		m_OutputFormat = OutputFormats::itHTML;
		m_OutputToString = 0;
		m_OutputStringMaxSize = 0;
		m_TimeoutSeconds = 0;
		m_Flags = (ConvertFlags) ConvertFlags::dtsConvertSkipHiddenHits;
		m_InputTypeId = 0;
		m_InputDocId = 0;
		m_InputFields = "";
		m_InputText = "";
		m_Hits = new int __gc[0];
	}

	FileConverter::~FileConverter ()
	{	LogFunction("FileConverter::~FileConverter", "");
				
		m_Errors = 0;
		
		m_Hits = 0;
		
		Dispose(false);
		
		CountDelete(otFileConverter);
	}
	
	void FileConverter::DisposeUnmanagedItems()
	{	freeJob();
	}

	void FileConverter::SetInputItem (SearchResults* results, Int32 whichItem)
	{	if (!results)
			return;
			
		DSearchResults *imp = results->getImplementation();
		if (!imp)
			return;
		
		makeJob();
		
		m_convertJob->SetInputItem(*imp, whichItem);
		m_InputFile = m_convertJob->InputFile;
		m_AlphabetLocation = m_convertJob->AlphabetLocation;
		m_IndexRetrievedFrom = m_convertJob->IndexRetrievedFrom;
		m_InputDocId = m_convertJob->GetInputDocId();
		m_InputTypeId = m_convertJob->GetTypeId();
		
		results->GetNthDoc(whichItem);
		m_Hits = results->DocHits;
	}
	
	void FileConverter::ExecuteFromInputStream(CInternalApiConvertJob& job)
	{	LogFunction("FileConverter::ExecuteFromInputStream", "");
		DInputSourceBase *is = 0;
		if (m_DocBytes && (m_DocBytes->Length > 0)) {
			debugLog("Accessing docBytes");
			int len = m_DocBytes->Length;
			if (len > 0) {
				debugLog("Got bytes, size = ", len);
				// TODO: do this without creating extra copy
				DString bytes;
				bytes.extend(len);
				Marshal::Copy(m_DocBytes, 0, bytes.str(), len);
				job.BuildInputStream(job.InputFile, bytes.str(), len);
				}
			}
		else if (!job.InputFile.isBlank() && !DFilename::isUrl(job.InputFile)) 
			job.BuildInputStream(job.InputFile);
		DStringSet fieldSet;
		DString sFields;
		sFields.set(InputFields);
		debugLog("InputFields: ", sFields);
		debugLog("InputText:   ", job.InputText);
		fieldSet.tokenize(sFields, '\t', true);
		job.InputFields.copy(fieldSet);
		job.InputText.set(InputText);
		debugLog("InputFields length", InputFields->Length);
		job.Execute();
	}

	void FileConverter::Execute ()
	{	makeJob();
	
		m_convertJob->InputFile.set ( (m_InputFile));

		if (m_OutputToString)
			m_convertJob->SetOutputToString(m_OutputStringMaxSize);
		else
			m_convertJob->OutputFile.set( (m_OutputFile));

		m_convertJob->SetOutputFormat(m_OutputFormat);

		m_convertJob->BeforeHit.set( ( m_BeforeHit));
		m_convertJob->AfterHit.set( ( m_AfterHit));
		m_convertJob->Footer.set( ( m_Footer));
		m_convertJob->Header.set( ( m_Header));
		m_convertJob->HtmlBaseHref.set(m_BaseHRef);
		m_convertJob->HtmlHead.set(m_HtmlHead);
		m_convertJob->AlphabetLocation.set(m_AlphabetLocation);
		m_convertJob->SetTypeId( m_InputTypeId );
		m_convertJob->IndexRetrievedFrom.set(m_IndexRetrievedFrom);
		m_convertJob->SetInputDocId( m_InputDocId );

		DArrayAsVector<long> hits;
		Converters::GetNetArray(&hits, m_Hits);
		if (hits.getCount())
			m_convertJob->SetHits(hits.c_vector(), hits.getCount());

		m_convertJob->SetTimeout (m_TimeoutSeconds);
		m_convertJob->SetFlags(m_Flags);

		if (m_InputFields->Length || m_InputText->Length || (m_DocBytes && m_DocBytes->Length))
			ExecuteFromInputStream(*m_convertJob);
		else                        
			m_convertJob->Execute();

		m_Errors->Copy(m_convertJob->getErrors());

		if (m_OutputToString) 
			m_OutputString = m_convertJob->outputString.allocNetString();
		if (m_convertJob->GetDetectedTypeId() > 0)
			m_DetectedTypeId = (dtSearch::Engine::TypeId) (m_convertJob->GetDetectedTypeId());
		
		freeJob();
	}
	
	void FileConverter::makeJob() {
		if (!m_convertJob)
			m_convertJob = new CInternalApiConvertJob;
		}
	void FileConverter::freeJob() {
		if (m_convertJob) {
			delete m_convertJob;
			m_convertJob = 0;
			}
		}	

}}
