#include "stdafx.h"
#include "Server.h"
#include "FileConverter.h"
#include "SearchResults.h"
#include "DataSource.h"


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
		m_convertJob = nullptr;
		Errors = gcnew JobErrorInfo;

		// Setup default values
		OutputFormat = OutputFormats::itHTML;
		OutputToString = 0;
		OutputStringMaxSize = 0;
		TimeoutSeconds = 0;
		Flags = (ConvertFlags) ConvertFlags::dtsConvertSkipHiddenHits;
		InputTypeId = 0;
		InputDocId = 0;
		InputFields = "";
		InputText = "";
		Hits = gcnew array<int>(0);
	}

	FileConverter::~FileConverter ()
	{	LogFunction("FileConverter::~FileConverter", "");

		Errors = nullptr;

		Hits = nullptr;

		CountDelete(otFileConverter);
	}

	void FileConverter::DisposeUnmanagedItems()
	{	freeJob();
	}

	void FileConverter::SetInputItem (SearchResults^ results, Int32 whichItem)
	{	if (!results)
			return;

		DSearchResults *imp = results->getImplementation();
		if (!imp)
			return;

		makeJob();

		m_convertJob->SetInputItem(*imp, whichItem);
		InputFile = m_convertJob->InputFile.allocNetString();
		AlphabetLocation = m_convertJob->AlphabetLocation.allocNetString();
		IndexRetrievedFrom = m_convertJob->IndexRetrievedFrom.allocNetString();
		InputDocId = m_convertJob->GetInputDocId();
		InputTypeId = m_convertJob->GetTypeId();

		results->GetNthDoc(whichItem);
		Hits = results->DocHits;
	}

	void FileConverter::ExecuteFromInputStream(DFileConvertJob& job)
	{	LogFunction("FileConverter::ExecuteFromInputStream", "");

	    InternalApiIndexJob::CInputStreamMaker inputStreamMaker;
		inputStreamMaker.Text.set(InputText);
		inputStreamMaker.Name.set(InputFile);
	    DStringProperty fields;
		fields.set(InputFields);
    	inputStreamMaker.SetFields(fields);

		Stream ^toConvert = InputStream;
		
		if (InputBytes && (InputBytes->Length > 0)) 
			toConvert = gcnew MemoryStream(InputBytes);
				
		CNetInputStream netInputStream;		
		if (toConvert) {
			netInputStream.attach(toConvert);
			inputStreamMaker.MakeInputStream(&netInputStream);
			job.SetInputStream(*netInputStream.getInterface());
			}		
		else {
			if (!job.InputFile.isBlank() && !DFilename::isUrl(job.InputFile)) 
				inputStreamMaker.DocIsFile = true;
			inputStreamMaker.MakeInputStream();
			job.SetInputStream(inputStreamMaker.GetInterface());
			}
		job.Execute();
	}

	void FileConverter::Execute ()
	{	makeJob();

		m_convertJob->InputFile.set (InputFile);

		if (OutputToString)
			m_convertJob->SetOutputToString(OutputStringMaxSize);
		else
			m_convertJob->OutputFile.set(OutputFile);

		m_convertJob->SetOutputFormat((int) OutputFormat);

		m_convertJob->BeforeHit.set(BeforeHit);
		m_convertJob->AfterHit.set(AfterHit);
		m_convertJob->Footer.set(Footer);
		m_convertJob->Header.set(Header);
		m_convertJob->HtmlBaseHref.set(BaseHRef);
		m_convertJob->HtmlHead.set(HtmlHead);
		m_convertJob->DocTypeTag.set(DocTypeTag);
		m_convertJob->AlphabetLocation.set(AlphabetLocation);
		m_convertJob->SetTypeId(InputTypeId );
		m_convertJob->IndexRetrievedFrom.set(IndexRetrievedFrom);
		m_convertJob->SetInputDocId( InputDocId );
		if (HitsByWord)
			m_convertJob->HitsByWord.copy(HitsByWord);

		DArrayAsVector<long> hits;
		Converters::GetNetArray(&hits, Hits);
		if (hits.getCount())
			m_convertJob->SetHits(hits.c_vector(), hits.getCount());

		m_convertJob->SetTimeout (TimeoutSeconds);
		m_convertJob->SetFlags((long) Flags);
		
		if (ExtractionOptions != nullptr) {
			DExtractionOptions extractionOptions;
			extractionOptions.AllowedExtensions.tokenize(ExtractionOptions->AllowedExtensions);
			extractionOptions.DefaultExtension.set(ExtractionOptions->DefaultExtension);
			extractionOptions.FilenamePrefix.set(ExtractionOptions->FilenamePrefix);
			extractionOptions.SetFlags((int) ExtractionOptions->Flags);
			extractionOptions.OutputFileListName.set(ExtractionOptions->OutputFileListName);
			extractionOptions.OutputLocation.set(ExtractionOptions->OutputLocation);
			extractionOptions.OutputReference.set(ExtractionOptions->OutputReference);
			extractionOptions.UnnamedAttachmentLinkText.set(ExtractionOptions->UnnamedAttachmentLinkText);
			m_convertJob->setExtractionOptions(extractionOptions);
			}

		if (InputFields->Length || InputText->Length || (InputBytes && InputBytes->Length) || InputStream)
			ExecuteFromInputStream(*m_convertJob);
		else
			m_convertJob->Execute();

		Errors->Copy(m_convertJob->getErrors());

		if (OutputToString)
			OutputString = m_convertJob->outputString.allocNetString();
		if (m_convertJob->GetDetectedTypeId() > 0)
			DetectedTypeId = (dtSearch::Engine::TypeId) (m_convertJob->GetDetectedTypeId());

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
