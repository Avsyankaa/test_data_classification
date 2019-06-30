#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"
#include "Constants.h"
#include "NetJobBase.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch { namespace Engine
{


	/// <summary>
	/// List words, fields, or filenames in an index to a file or to a memory buffer
	/// </summary>
	public ref class ListIndexJob : public NetJobBase
	{
	public:

		ListIndexJob();
		~ListIndexJob();

		/// <summary>
		/// Index to list
		/// </summary>
		property String^ IndexPath;

		/// <summary>
		/// Name of file to create
		/// </summary>
		property String^ OutputFile;

		/// <summary>
		/// Destination for list, if OutputToString is true
		/// </summary>
		property String^ OutputString;

		/// <summary>
		/// Create list in OutputString instead of filename specified by OutputFile
		/// </summary>
		property bool OutputToString;

		/// <summary>
		/// ToMatch is an optional search expression specifying the text to match against items being listed.
		/// </summary>
		/// <remarks>
		/// For example, to list all field names starting with "A", you would set ListFlags to dtsListIndexFields and set ToMatch to "A*".
		/// </remarks>
		property String^ ToMatch;

		/// <summary>
		/// ListIndexFlags values specifying the type of list to create
		/// </summary>
		property ListIndexFlags ListIndexFlags;

		/// <summary>
		/// Maximum size of the output string to create (zero = no limit)
		/// </summary>
		property int OutputStringMaxSize;


		/// <summary>
		/// Generates the list
		/// </summary>
		virtual bool Execute () override;


		/// <summary>
		/// Starts a separate thread to execute the job and returns immediately.
		/// </summary>
		void ExecuteInThread ();

		/// <summary>
		/// Returns true immediately if the thread is done; otherwise
		/// waits up to millisecondsToWait for the thread to finish,
		/// then returns false if the thread is not done yet.
		/// </summary>
		/// <param name="millisecondsToWait">Number of milliseconds to wait, if the thread is not done yet.</param>
		bool IsThreadDoneWait (long millisecondsToWait);


		/// <summary>
		/// Aborts a job started using ExecuteInThread.
		/// </summary>
		void AbortThread ();

	protected:

		virtual void DisposeUnmanagedItems() override;
	};
}
}
