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
	public __gc class Server
	{
	public:
		Server() ;
		
		~Server();

		/// <summary>
		/// dtSearch Engine major version number
		/// </summary>
		__property int get_MajorVersion() {
			//P:MajorVersion
			return m_majorVersion;
			}

		/// <summary>
		/// dtSearch Engine minor version number
		/// </summary>
		__property int get_MinorVersion() {
			//P:MinorVersion
			return m_minorVersion;
			}

		/// <summary>
		/// dtSearch Engine build number
		/// </summary>
		__property int get_Build() {
			//P:Build
			return m_build;
			}

		/// <summary>
		/// Start recording a debug log in the specified file
		/// </summary>
		/// <param name="logName">Name of the log file to create</param>
		/// <param name="flags">Flags controlling optional logging behavior</param>
		static void SetDebugLogging (String* logName, DebugLogFlags flags);
			//M:SetDebugLogging(System.String,dtSearch.Engine.DebugLogFlags)

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
		static String* ConvertPath (String* path, long siteId, ConvertPathFlags flags, String* tableLocation);
			//M:ConvertPath(System.String,System.Int32,dtSearch.Engine.ConvertPathFlags,System.String)

		static System::Collections::Specialized::StringCollection *Tokenize(String *input);
		
		static String *AllocatedObjectList();

	private:
		int m_majorVersion;
		int m_minorVersion;
		int m_build;
	};
}
}
