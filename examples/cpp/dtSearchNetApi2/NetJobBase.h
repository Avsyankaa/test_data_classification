#pragma once
#include "utils.h"
#include "JobErrorInfo.h"
#pragma managed

namespace dtSearch { namespace Engine {

public
/// <summary>
/// Base class for SearchJob, IndexJob, FileConverter, etc. 
/// </summary>                                              
ref class NetJobBase abstract :
	public DisposableObject
{
	public:
		NetJobBase(const char *aName) :
			bExecuting(0)
			{	name = aName;
				Errors = gcnew JobErrorInfo();
				pJob = 0;
				pOptions = 0;
				pErrorInfo = new dtsErrorInfo;
			};
		~NetJobBase() {
			}
		delegate bool ExecuteDelegate();

		virtual bool Execute() = 0;

		/// <summary>
		/// Contains any error messages generated during the job.
		/// </summary>
		property JobErrorInfo ^Errors;
			//F:Errors

	private:
		const char *name;
		DJobBase *pJob;
		DOptions *pOptions;
		bool bExecuting;
		bool bDone;
		dtsErrorInfo *pErrorInfo;
		IAsyncResult ^asrExecutingResult;
		ExecuteDelegate ^executingDelegate;

		bool SetExecuting(bool v);
		void OnThreadDone(IAsyncResult ^ar);
		void EndThread();
		void freeJob() {
			DJobBase *toDelete = pJob;
			pJob = 0;
			if (toDelete)
				delete toDelete;
			}
		bool CallExecuteThroughDelegate();
	protected:

		void reportError(const char *s) {
			if (pErrorInfo)
				pErrorInfo->handleMessage(-1, s);
			}
		virtual void DisposeUnmanagedItems() override {

			freeJob();
			if (pOptions) {
				delete pOptions;
				pOptions = 0;
				}
			if (pErrorInfo) {
				delete pErrorInfo;
				pErrorInfo = 0;
				}
			}

		// Derived classes call these to implement Execute, ExecuteInThread, etc.
		void ExecuteInThreadImp();
		/// Abort execution of a thread started by ExecuteInThread
		void AbortThread(AbortValue v);
		bool IsThreadDoneWaitImp(long millisecondsToWait);
		bool IsThreadDoneImp();

		bool ExecuteJob(DJobBase *);
	};

} }
