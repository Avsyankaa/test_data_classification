#include "StdAfx.h"
#include "netjobbase.h"

using namespace System;
using namespace System::Threading;

using namespace dtSearch::Engine;

// Any DJobBase-based class (DIndexJob, DSearchJob, etc.) can be executed by calling Execute, and
// will return errors from GetErrors(), so these common aspects of executing a job can be
// handled in the base class.
bool NetJobBase::ExecuteJob(DJobBase *aJob)
{	bDone = false;
	if (!SetExecuting(true)) {
		reportError("Error:  Job is already running");
		return false;
		}
	
	freeJob();
	pJob = aJob;
	
	
	
	pJob->Execute();
	
	if (pErrorInfo->getCount()) {
		const dtsErrorInfo *jobErrors = pJob->GetErrors();
		pJob->GetErrors()->forwardTo(*pErrorInfo);
		Errors->Copy(pErrorInfo);
		}
	else
		Errors->Copy(pJob->GetErrors());
	
	pErrorInfo->clear();
	
	bool ret = (pJob->Failed() == FALSE);
	
	SetExecuting(false);
	bDone = true;
	return ret;
}

bool NetJobBase::CallExecuteThroughDelegate() {
	if (pOptions) {
		// Makes sure options are consistent with thread called from
		DOptions thisThreadOptions;
		if (!thisThreadOptions.equals(*pOptions)) {
			debugLog("Conforming options to those of calling thread");
			pOptions->Save();
			}
		else
			debugLog("Thread options are consistent");
		}
	return Execute();
}

	
// This invokes the Execute() method of the derived class through a delegate.
// The Execute() method will create a DIndexJob, DSearchJob, or other DJobBase-derived class,
// and call ExecuteJob() to execute it.	
void NetJobBase::ExecuteInThreadImp()
{	debugLog("<ExecuteInThreadImp", name, ">");
	bDone = false;		
	if ((asrExecutingResult != nullptr) || (executingDelegate != nullptr))
	{
		reportError("SearchJob is already running -- cannot execute the same SearchJob twice");
		return;
	}
	Monitor::Enter(this);	
			
	if (pOptions) 
		delete pOptions;
	pOptions = new DOptions;

	executingDelegate = gcnew ExecuteDelegate(this, &NetJobBase::CallExecuteThroughDelegate);
	
	AsyncCallback ^acb = gcnew AsyncCallback(this, &NetJobBase::OnThreadDone);
		
	asrExecutingResult = executingDelegate->BeginInvoke(acb, nullptr);
	Monitor::Exit(this);
	debugLog("</ExecuteInThreadImp", name, ">");
}

// AbortThread sets the cancel flag for the job and returns immediately.  The job will then
// exit as soon as possible and return.
void NetJobBase::AbortThread(AbortValue v)
{	debugLog("AbortThread", name);
	if (!pJob)
		debugLog("No job executing");
	else if (v == AbortValue::CancelImmediately)
		pJob->CancelImmediate();
	else
		pJob->Cancel();
}

// Ensures that EndInvoke is always called
void NetJobBase::OnThreadDone(IAsyncResult ^ar)
{	debugLog("NetJobBase::OnThreadDone");
	if (asrExecutingResult != nullptr)
		EndThread();
}

// Calls EndInvoke on the thread that was started by ExecuteInThreadImp.  The
// saved delegate and AsyncResult pointers are set to null to ensure this will be
// done exactly once for each thread invocation.
void NetJobBase::EndThread()
{	debugLog("NetJobBase::EndThread", name);

	if (executingDelegate == nullptr) {
		debugLog("No delegate");
		return;
		}

	Monitor::Enter(this);
	ExecuteDelegate ^del = executingDelegate;
	executingDelegate = nullptr;
	IAsyncResult ^asr = asrExecutingResult;
	asrExecutingResult = nullptr;
	Monitor::Exit(this);
	if ((del != nullptr) && (asr != nullptr)) {
		debugLog("EndInvoke", name);
		del->EndInvoke(asr);
		debugLog("EndInvoke done", name);
		}
}
	

bool NetJobBase::IsThreadDoneWaitImp(long millisecondsToWait)
{	debugLog("NetJobBase::IsThreadDoneWaitImp", millisecondsToWait);
	if (bDone)
		return true;
		
	Monitor::Enter(this);
	IAsyncResult ^asr = asrExecutingResult;
	Monitor::Exit(this);

	if (!asr)
		return bDone;

	bool ret = asr->AsyncWaitHandle->WaitOne(millisecondsToWait, false);
	debugLog("IsDone: ", (ret? "Yes" : "No"));
	return ret;
}	
	
bool NetJobBase::IsThreadDoneImp()
{	
	return IsThreadDoneWaitImp(0);
}

bool NetJobBase::SetExecuting(bool v) {
	bool ret = true;
	Monitor::Enter(this);
	if (v && bExecuting)
		ret = false;
	else 
		bExecuting = v;
	Monitor::Exit(this);
	return ret;
	}
