#pragma once

#include "stdafx.h"
#include "Utils.h"
#include <dtsearch.h>

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed


namespace dtSearch { namespace Engine {
	/// <summary>
	/// dtSearch Engine version information, diagnostic logging, and utility functions.
	/// </summary>
	public ref class Server
	{
	public:
		Server() ;
		
		~Server();

		/// <summary>
		/// dtSearch Engine major version number
		/// </summary>
		property int MajorVersion {
			int get() {
				return m_majorVersion;
			}
		}
			//P:MajorVersion

		/// <summary>
		/// dtSearch Engine minor version number
		/// </summary>
		property int MinorVersion {
			int get() {
				return m_minorVersion;
			}
		}
			//P:MinorVersion

		/// <summary>
		/// dtSearch Engine build number
		/// </summary>
		property int Build {
			int get() {
				return m_build;
			}
		}
			//P:Build

		/// <summary>
		/// Start recording a debug log in the specified file
		/// </summary>
		/// <param name="logName">Name of the log file to create</param>
		/// <param name="flags">DebugLogFlags controlling optional
		///                     logging behavior</param>                
		static void SetDebugLogging (String^ logName, DebugLogFlags flags);

		/// <summary>
		/// Add a line of text to the debug log
		/// </summary>
		/// <param name="text">Text to log</param>
		static void AddToLog(String^ text);



		/// <summary>
		/// If a crash occurs, record a log with a stack trace and other diagnostic information.
		/// </summary>
		/// <param name="logName">Name of the log file to create</param>
		static void SetCrashLogging (String^ logName);

		/// <summary>
		/// Convert between virtual and local paths.
		/// </summary>
		/// <param name="path">Path to convert, either a virtual or a local path, depending on the flags</param>
		/// <param name="siteId">The id of the web site in IIS (the INSTANCE_ID server variable can be used)</param>
		/// <param name="flags">Flags indicating the type of conversion to perform</param>
		/// <param name="tableLocation">Registry key where the virtual path mappings are stored</param>
		/// <remarks>
		///<para>
		/// After a search, the SearchResults object contains a list of the files that were retrieved. These files are expressed as local paths, such as c:\docs\sample.doc. For web-based applications, it is usually preferable to report document locations using virtual directory names, such as /docs/sample.doc. Also, when highlighting hits in PDF files or creating direct links to retrieved documents, a virtual directory name is required.
		///</para>
		///<para>
		/// Internet Information Server stores its mapping between virtual and local paths in the Metabase.
		/// Reading this information from the Metabase is time-consuming and requires access rights that may not
		/// be available to a process running in the web server context.  Therefore, the dtSearch Engine
		/// caches this information in the registry, under the key HKEY_LOCAL_MACHINE\Software\dtSearch Corp.\dtSearch\Virtual Roots.
		/// By default, ConvertPath will use the cached table values.  Call ConvertPath with the dtsCpRefreshMappings + dtsCpStoreInRegistry flags to update the cached information. (Running dtWebSetup.exe, the dtSearch Web Setup program, will also update this information.)
		///</para>
		///<para>
		/// If ConvertPath returns a blank string, it means that either (1) the folder passed in does not have a
		/// virtual directory associated with it, or (2) the registry tables have not been refreshed since the
		/// virtual directory was created.
		///</para>
		/// </remarks>
		static String^ ConvertPath (String^ path, long siteId, ConvertPathFlags flags, String^ tableLocation);
			//M:ConvertPath(System.String,System.Int32,dtSearch.Engine.ConvertPathFlags,System.String)

		static System::Collections::Specialized::StringCollection ^Tokenize(String ^input);
		
		static String ^AllocatedObjectList();

		/// \ \ 
		/// <summary>
		/// Specify the folder location of the dtSearch Engine DLL
		/// </summary>
		/// <remarks>
		/// This method provides a way to specify where the dtSearch
		/// Engine is located. If used, it must be called before anything
		/// else is done using the dtSearch .NET API.
		/// * When to use SetEnginePath *
		/// SetEnginePath is only needed when you cannot ensure that the
		/// dtSearch Engine DLL will be in the same folder as the .NET
		/// API wrapper when your application executes. Currently this
		/// \occurs in ASP.NET applications and Azure deployments.
		/// 
		/// In ASP.NET applications, if you have administrative rights on
		/// the server, you can avoid the need to use SetEnginePath by
		/// registering the dtSearch Engine DLL using regsvr32, or by
		/// copying it into a folder that is on the system PATH.
		/// * Why SetEnginePath is needed *
		/// The dtSearch .NET API wrapper (dtSearchNetApi2.dll,
		/// dtSearchNetApi3.dll, or dtSearchNetApi4.dll) depends on the
		/// dtSearch Engine DLL (dten600.dll or dtengine64.dll). In .NET
		/// applications, an unmanaged DLL dependency can be separated
		/// from the managed assembly when it executes. This occurs in
		/// ASP.NET applications when the application is copied to a
		/// temporary folder for execution. A DLL dependency of an
		/// assembly is not copied in this process.
		/// 
		/// If the dtSearch Engine DLL is not on the system PATH, and if
		/// its type library is not registered, then it will not be found
		/// when the .NET application executes, and the application will
		/// fail with an ApiInitializer exception or a FileNotFound
		/// exception.
		/// 
		/// SetEnginePath provides a way to prevent this even if you do
		/// not have administrative rights on the server.
		/// * How SetEnginePath works *
		/// The .NET API wrapper depends on the dtSearch Engine, but the
		/// dtSearch Engine is linked using delayed loading, so it is not
		/// needed until you actually create a API object such as
		/// SearchJob or IndexJob. If you call Server.SetEnginePath
		/// before creating any API objects, then the API wrapper can use
		/// the provided directory to load the dtSearch Engine DLL using
		/// LoadLibrary, eliminating the need for the dtSearch Engine DLL
		/// to be located using the usual DLL search path.
		/// 
		/// 
		/// </remarks>                                                   
		static void SetEnginePath(String^ p);

	private:
		int m_majorVersion;
		int m_minorVersion;
		int m_build;
	};
}
}
