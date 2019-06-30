#pragma once

#include "Constants2.h"

namespace dtSearch {

namespace Engine {

		/// <summary>
		/// Values that CheckForAbort (IIndexStatusHandler or
		/// ISearchStatusHandler) can return
		/// to indicate whether a job should continue.
		/// </summary>
	public enum class AbortValue {

		/// <summary>
		/// Continue the search or index update
		/// </summary>
		Continue = 0,

		/// <summary>
		/// Cancel the search or index update.  For index updates,
		/// documents indexed so far are saved, and the indexer
		/// will stop after completing the current document.
		/// </summary>
		Cancel = -1,

		/// <summary>
		/// For index updates only, cancels the update immediately,
		/// without saving any changes to the index.
		/// </summary>
		CancelImmediately = -2
	};




    	/// <summary>
    	/// Flags for Server.ConvertPath
    	/// </summary>
    [System::Flags]
    public enum class ConvertPathFlags {
		None = 0,

		/// <summary>
		/// Convert a local path to a virtual path
		/// </summary>
        dtsCpMakeVirtual = 1,

        /// <summary>
        /// Convert a virtual path to a local path
        /// </summary>
        dtsCpMakeLocal = 2,

        /// <summary>
        /// Convert a mapped path to a UNC path
        /// </summary>
        dtsCpMakeUNC = 0x0008,

        /// <summary>
        /// Use the Metabase to update the table of local-to-virtual mappings that the dtSearch engine uses to perform these conversions
        /// </summary>
        dtsCpRefreshMappings = 0x0010,

        /// <summary>
        /// Store the updated table of mappings in the registry.
        /// </summary>
        dtsCpStoreInRegistry = 0x0020,

        /// <summary>
        /// Read the latest table of mappings from the registry.
        /// </summary>
        dtsCpReadFromRegistry = 0x0040
        };

    	/// <summary>
    	///  Output file formats for conversion of documents and generation of search reports
    	/// </summary>
    public enum class OutputFormats {

		/// <summary>
		/// HTML
		/// </summary>
        itHTML = 226,

        /// <summary>
        /// Ansi text
        /// </summary>
        itAnsi = 202,

        /// <summary>
        /// Microsoft Rich Text Format
        /// </summary>
        itRTF = 212,

        /// <summary>
        /// Unicode
        /// </summary>
        itUTF8 = 238,

		/// <summary>
		/// XML
		/// </summary>
        itXML = 234,

		/// <summary>
		/// HTML, with all formatting removed
		/// </summary>
        itUnformattedHTML = 256,

		/// <summary>
  		/// Output format for FileConverter that organizes document content, metadata, and attachments into a standard XML format        
		/// </summary>
        it_ContentAsXml		= 108


        };



		/// <summary>
		/// Flags for Server.SetDebugLogging
		/// </summary>
	[System::Flags]
    public enum class DebugLogFlags {

		/// <summary>
		/// Each line in the log will include the time, in hundredths of a second, since the start of execution.
		/// </summary>
        dtsLogTime                  = 0x0001,

        /// <summary>
        /// The log will be committed to disk after each line is written. This slows execution considerably but ensures that the log will survive a system crash.
        /// </summary>
        dtsLogCommit                = 0x0002,

        /// <summary>
        /// Log data is appended to the file if it already exists.
        /// </summary>
        dtsLogAppend                = 0x0004,

        /// <summary>
        /// Generate a stack trace in the logFile if an unhandled exception occurs.
        /// </summary>
        /// <remarks>
        /// dtsCrashLog should not be combined with the other flags and the filename provided
        /// should be different from the filename used for logging.
        /// </remarks>
        dtsCrashLog                 = 0x1000
        };


		/// <summary>
		/// Values for Options.Hyphens, specifying how hyphens should be
		/// indexed.
		/// </summary>
		/// <remarks>
		/// See <see cref="Hyphens" /> in the <see cref="Options Overview" />
		/// section of dtSearchApiRef.chm for information on what each
		/// value means.
		/// </remarks>
    public enum class  HyphenSettings {

		///<summary>
		/// index "first-class" as "firstclass"
		///</summary>
		dtsoHyphenAsIgnore           = 1,

		///<summary>
		/// index "first-class" as "first-class"
		///</summary>
		dtsoHyphenAsHyphen           = 2,

		///<summary>
		/// index "first-class" as "first" and "class"
		///</summary>
		dtsoHyphenAsSpace            = 3,

		///<summary>
		/// index "first-class" all three ways
		///</summary>
		dtsoHyphenAll                = 4
        };






}

}
