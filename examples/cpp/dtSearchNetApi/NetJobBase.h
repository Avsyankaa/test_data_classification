#pragma once
#include "utils.h"
#include "JobErrorInfo.h"
#pragma managed

namespace dtSearch { namespace Engine {

public __gc class NetJobBase :
	public DisposableObject
{
	public:
		NetJobBase(const char *aName);
		~NetJobBase() {
			debugLog("NetJobBase::~NetJobBase...");
			Dispose(false);
			debugLog("NetJobBase::~NetJobBase...done");
			}
		__delegate bool ExecuteDelegate();
			
		virtual bool Execute() = 0;
		

	private:
		const char *name;
		DJobBase *pJob;
		DOptions *pOptions;
		bool bExecuting;
		bool bDone;
		IAsyncResult *asrExecutingResult;
		ExecuteDelegate *executingDelegate;
		
		bool SetExecuting(bool v); 
		void OnThreadDone(IAsyncResult *ar);
		void EndThread();
		void freeJob();
		bool CallExecuteThroughDelegate();
	protected:
		JobErrorInfo *m_Errors;
		void reportError(const char *s) {
			debugLog("NetJobBase::reportError", s);
			if (m_Errors)
				m_Errors->AppendError(-1, s);
			}
		void DisposeUnmanagedItems();
		
		// Derived classes call these to implement Execute, ExecuteInThread, etc.
		void ExecuteInThreadImp();
		void AbortThread(AbortValue v);
		bool IsThreadDoneWaitImp(long millisecondsToWait);
		bool IsThreadDoneImp();

		bool ExecuteJob(DJobBase *);
	};

} }
