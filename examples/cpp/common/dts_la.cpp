#include <stdafx.h>
#include <dtconfig.h>
#include <darray.h>
#include <dstring.h>
#include <dts_la.h>

const int LanguageAnalyzerId = 0x1234;
const int AnalyzerInstanceId = 0x5678;


CLanguageAnalyzerBase::CLanguageAnalyzerBase()
{	m_id = LanguageAnalyzerId;
}

void CLanguageAnalyzerBase::makeInterface(dtsLanguageAnalyzerInterface& la)
{	la.structSize = sizeof(dtsLanguageAnalyzerInterface);
	la.name = "";
	la.pInitializeJob = initializeJobCB;
	la.pAnalyze = analyzeCB;
	la.pDestroyJob = destroyJobCB;
	la.pCreateAnalyzer = 0;
	la.pDestroyAnalyzer = destroyAnalyzerCB;
}

CLanguageAnalyzerBase *CLanguageAnalyzerBase::safeCast(dtsAnalyzerHandle hAnalyzer)
{	if (hAnalyzer) {
		CLanguageAnalyzerBase *ret = (CLanguageAnalyzerBase *) hAnalyzer;
		if (ret->m_id == LanguageAnalyzerId)
			return ret;
		}
	return 0;
}

int CLanguageAnalyzerBase::initializeJobCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job)
{	CLanguageAnalyzerBase *la = safeCast(hAnalyzer);
	if (la)
		return la->initializeJob(job);
	return FAIL;
}

void CLanguageAnalyzerBase::destroyJobCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job)
{	CLanguageAnalyzerBase *la = safeCast(hAnalyzer);
	if (la)
		la->destroyJob(job);
}

bool CLanguageAnalyzerBase::analyzeCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job)
{	CLanguageAnalyzerBase *la = safeCast(hAnalyzer);
	if (la)
		return la->analyze(job);
	return false;
}

void CLanguageAnalyzerBase:: destroyAnalyzerCB(dtsAnalyzerHandle hAnalyzer)
{	CLanguageAnalyzerBase *la = safeCast(hAnalyzer);
	if (la)
		delete la;
}

int CLanguageAnalyzerBase::initializeJob(dtsLaJob& job)
{	CLanguageAnalyzerJobBase *instance = makeAnalyzerInstance();
	if (!instance)
		return FAIL;
	job.pData = instance;
	instance->initialize(job);
	return SUCCESS;
}

void CLanguageAnalyzerBase::destroyJob(dtsLaJob& job)
{	CLanguageAnalyzerJobBase *instance = CLanguageAnalyzerJobBase::safeCast(job.pData);
	if (instance)
		delete instance;
	job.clear();
}

bool CLanguageAnalyzerBase::analyze(dtsLaJob& job)
{
	CLanguageAnalyzerJobBase *instance = CLanguageAnalyzerJobBase::safeCast(job.pData);
	if (instance)
		return instance->analyze(job);
	return false;

}

CLanguageAnalyzerJobBase::CLanguageAnalyzerJobBase()
{
	m_id = AnalyzerInstanceId;  // for safeCast
}

CLanguageAnalyzerJobBase *CLanguageAnalyzerJobBase::safeCast(void *pData)
{	CLanguageAnalyzerJobBase *ret = (CLanguageAnalyzerJobBase *) pData;
	if (ret->m_id == AnalyzerInstanceId)
		return ret;
	return 0;
}

int CLanguageAnalyzerJobBase::initialize(dtsLaJob& job)
{	job.outputBuffer = 0;
	job.outputTextLength = 0;
	job.wordCount = 0;
	job.wordTable = 0;
	job.pData = this;
	return SUCCESS;
}

bool CLanguageAnalyzerJobBase::analyze(dtsLaJob& job)
{	// This virtual function should be overridden to provide language-specific processing
	// of the input text.
	// After the processing is completed, linkBuffers() MUST be called to
	// set up the pointers in the dtsLaJob structure

	linkBuffers(job);
	return false;
}

// This must always be called at the end of analyze()
void CLanguageAnalyzerJobBase::linkBuffers(dtsLaJob& job)
{
	job.outputTextLength = ToInt(m_outputBuf.getLength());
	job.outputBuffer = m_outputBuf.str();
	job.wordCount = m_wordTable.getCount();
	job.wordTable = m_wordTable.c_vector();
}


