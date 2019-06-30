#pragma once

#include "stdafx.h"
#include "DataSource.h"
#include "Server.h"
#include "JobErrorInfo.h"
#include <ApiIndexJob.h>
#include "NetJobBase.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

// ------------------ Unmanaged code -----------------------
#pragma unmanaged

class DIndexJobThread;
class CApiIndexJobForNet;



// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch {
namespace Engine {



	/// <summary>
	/// Information about a document being indexed (part of IndexProgressInfo)
	/// </summary>
	public ref class IndexFileInfo {
		public:
		/// <summary>
		/// Name of the file 
		/// </summary>
		String ^Name;
		/// <summary>
		/// Location of the file
		/// </summary>
		String ^Location;
		/// <summary>
		/// File format expressed as a string
		/// </summary>
		String ^Type;
		/// <summary>
		/// If the file could not be opened, a message indicating the reason
		/// </summary>
		String ^OpenFailMessage;
		/// <summary>
		/// Size of the file in bytes
		/// </summary>
		Int32 Size;
		/// <summary>
		/// Number of bytes of the file that have been read so far
		/// </summary>
		Int32 BytesRead;
		/// <summary>
		/// Number of words parsed from the file
		/// </summary>
		Int32 WordCount;
		/// <summary>
		/// Percent of the file that has been processed so far
		/// </summary>
		Int32 PercentDone;
		/// <summary>
		/// File format expressed as a TypeId
		/// </summary>
		TypeId TypeId;
		/// <summary>
		/// DocId that will be assigned to this document in the index
		/// </summary>
		Int32 DocId;
		/// <summary>
		/// Alternative user-friendly name for this document (usually the "Title")
		/// </summary>
		String ^DisplayName;
		/// <summary>
		/// Full path and filename of the document
		/// </summary>
		String ^Fullname;
		/// <summary>
		/// Information about how this document was indexed.
		/// </summary>
		FileInfoFlags Flags;
		/// <summary>
		/// Stored fields in the document (sent with dtsnIndexFileDone only)
		/// <summary>
		String ^UserFields;
		};


	/// <summary>
	/// State of an index update
	/// </summary>
	public ref class IndexProgressInfo {
		public:
		IndexProgressInfo() {
			File = gcnew IndexFileInfo;
			}
		/// <summary>
		/// A MessageCode indicating the reason for the update message
		/// </summary>
		MessageCode UpdateType;
		/// <summary>
		/// An IndexingStep value indicating what the indexer is currently doing
		/// </summary>
		IndexingStep Step;
		/// <summary>
		/// Location of the index being updated
		/// </summary>
		String ^IndexPath;
		/// <summary>
		/// The percentage of the files that have been indexed
		/// </summary>
		Int32 PercentDone;

		/// <summary>
		/// Kilobytes of document data that have been read so far
		/// </summary>
		UInt32 DocBytesReadKB;
		/// <summary>
		/// Total size of the files to be indexed, in kilobytes
		/// </summary>
		UInt32 BytesToIndexKB;
		/// <summary>
		/// Total size of the files indexed so far, in kilobytes
		/// </summary>
		UInt32 BytesReadKB;
		/// <summary>
		/// Bytes of document data that have been read so far
		/// </summary>
		UInt64 DocBytesRead64;
		/// <summary>
		/// Total size of the files to be indexed, in bytes
		/// </summary>
		UInt64 BytesToIndex64;
		/// <summary>
		/// Total size of the files indexed so far, in bytes
		/// </summary>
		UInt64 BytesRead64;
		/// <summary>
		/// Percentage of the maximum possible size of this index
		/// </summary>
		Int32 IndexPercentFull;
		/// <summary>
		/// Number of files not indexed because they were determined to be binary. (See Options.BinaryFiles)
		/// </summary>
		UInt32 BinaryCount;
		/// <summary>
		/// Number of files not indexed because they could not be accessed
		/// </summary>
		UInt32 OpenFailures;
		/// <summary>
		/// Number of files not indexed because they were encrypted.
		/// </summary>
		UInt32 EncryptedCount;
		/// <summary>
		/// Number of unique words in the index
		/// </summary>
		UInt32 WordsInIndex;
		/// <summary>
		/// Number of documents in the index
		/// </summary>
		UInt32 DocsInIndex;
		/// <summary>
		/// Information about the file currently being indexed
		/// </summary>
		IndexFileInfo ^File;
		/// <summary>
		/// Number of files to index
		/// </summary>
		UInt32 FilesToIndex;
		/// <summary>
		/// Number of files read so far
		/// </summary>
		UInt32 FilesRead;
		/// <summary>
		/// Number of documents read so far (a single file may include more than one document)
		/// </summary>
		UInt32 DocsRead;
		/// <summary>
		/// Number of files in the selected folders to check to see if they have been indexed
		/// </summary>
		UInt32 FilesToCheck;
		/// <summary>
		/// Number of files in the selected folders that have been checked to see if they need to be indexed
		/// </summary>
		UInt32 FilesChecked;
		/// <summary>
		/// Seconds since the index job started
		/// </summary>
		UInt32 ElapsedSeconds;
		/// <summary>
		/// Estimated number of seconds remaining in the index job
		/// </summary>
		UInt32 EstRemainingSeconds;
		/// <summary>
		/// Percentage of the current merge process that has been completed
		/// </summary>
		UInt32 CurrMergePercent;
		/// <summary>
		/// Number of documents that were partially encrypted 
		/// </summary>
		UInt32 PartiallyEncryptedCount;
		/// <summary>
		/// Number of documents that were partially corrupt
		/// </summary>
		UInt32 PartiallyCorruptCount;

		};


	/// <summary>
	/// Information about the current state of an index
	/// </summary>
	public ref class IndexInfo {
		public:
		/// <summary>
		/// Number of documents in the index.
		/// </summary>
		UInt32 DocCount;
		/// <summary>
		/// Number of unique words in the index.
		/// </summary>
		UInt64 WordCount;
		/// <summary>
		/// Measures the extent to which an index is fragmented by being
		/// built in multiple indexing jobs.
		/// </summary>
		/// <remarks>
		/// Compressing an index eliminates fragmentation. Fragmentation
		/// \of an index increases the size of the index and slows
		/// searching, but the effect is generally not noticeable unless
		/// the fragmentation is severe. Indexing documents in very small
		/// batches can produce a severely fragmented index, but
		/// \otherwise index fragmentation can be disregarded.
		/// * See also: *
		/// <see cref="Compressing (optimizing) an index" />
		/// 
		/// <see cref="dtSearch::Engine::IndexInfo::Fragmentation" text="IndexInfo::Fragmentation Data Member" />
		/// 
		/// </remarks>                                                                                           
		UInt32 Fragmentation;
		/// <summary>
		/// ObsoleteCount is the number of documents for which obsolete
		/// information is stored in the index.
		/// </summary>
		/// <remarks>
		/// When a document is indexed, modified, then indexed again,
		/// information about the document in the index is marked as
		/// \obsolete and new information about the document is appended
		/// to the index. Compressing an index removes all obsolete
		/// information.
		/// * See also: *
		/// <see cref="Compressing (optimizing) an index" />
		/// 
		/// 
		/// </remarks>                                                  
		UInt32 ObsoleteCount;
		/// <summary>
		/// When index was created
		/// </summary>
		DateTime CreatedDate;
		/// <summary>
		/// When index was last updated
		/// </summary>
		DateTime UpdatedDate;
		/// <summary>
		/// The date the index was compressed.
		/// </summary>
		DateTime CompressedDate;
		/// <summary>
		/// Size of the index, in bytes
		/// </summary>
		UInt64 IndexSize;
		/// <summary>
		/// Indicates how much of the maximum capacity of the index has been used  (This 
		/// is a percentage of the maximum index capacity and does not consider the amount of disk space actually available.
		/// </summary>
		UInt32 PercentFull;
		/// <summary>
		/// IndexingFlags used when this index was created.
		/// </summary>
		IndexingFlags Flags;
		/// <summary>
		/// dtSearch index format version, expressed as minorVersion + majorVersion * 100.  701, for example, means 7.01
		/// </summary>
		UInt32 StructureVersion;
		
		/// <summary>
		/// Last document id assigned in this index
		/// </summary>
		UInt32 LastDocId;
		
		/// <summary>
		/// First document id assigned, or to be assigned, in this index (will be 1 for an empty index)
		/// </summary>
		UInt32 StartingDocId;

		/// <summary>
		/// The total size of all documents added to this index (including documents indexed multiple times)
		/// </summary>
		UInt64 TotalDataSize;


		};

		/// <summary>
		/// Provides a way for the caller to receive updates as the
		/// IndexJob progresses.
		/// </summary>
		/// <remarks>
		/// Use IIndexStatusHandler if your application needs to receive
		/// a notification each time a document is indexed.
		/// * See also *
		/// <see cref="Monitoring Index Jobs in .NET" /> 
		/// </remarks>                                                  
	public interface class IIndexStatusHandler	{
	public:

		void OnProgressUpdate(IndexProgressInfo ^info);
		AbortValue CheckForAbort ();
	};

		/// <summary>
		/// Create or update indexes
		/// </summary>
		/// <remarks>
		/// For more information on creating, updating, and maintaining
		/// indexes, see <see cref="Building and Maintaining Indexes" />
		/// in dtSearchApiRef.chm.
		/// 
		/// To create or update an index, make an IndexJob, use the
		/// properties to describe the indexing task you want the engine
		/// to perform, and call the Execute method.
		/// 
		/// IndexJob provides two ways to specify the text to be indexed:
		/// (1) the FoldersToIndex, Include Filters, and ExcludeFilters
		/// \members let you specify directories and filename filters
		/// identifying a set of disk files to index, and (2) the
		/// DataSourceToIndex member lets you supply text data directly
		/// to the dtSearch Engine for indexing, using an object that
		/// implements the DataSource interface. DataSourceToIndex is
		/// useful for indexing data from non-file sources such as
		/// message stores, SQL databases, dynamically-generated data, or
		/// any non-file data accessible to your program.
		/// 
		/// The Action flags specify the actions you want the engine to
		/// perform. If more than one action is specified, the engine
		/// will perform the actions in the following order: create,
		/// removeDeleted, removeListed, add, compress, merge, verify.
		/// * See also: *
		/// <see cref="Building and Maintaining Indexes" /> in
		/// dtSearchApiRef.chm.
		/// </remarks>                                                   
	public ref class IndexJob : public NetJobBase
	{
	public:

		IndexJob ();
		~IndexJob ();

			/// <summary>
			/// Add documents to an existing index.  If ActionCreate is not set, the index
			/// must already exist or the IndexJob will fail.
			/// </summary>
		property bool ActionAdd;

			/// <summary>
			/// Remove obsolete information from the index.
			/// </summary>
		property bool ActionCompress;
			/// <summary>
			/// Create a new index. If an index already exists in the specified directory, the index will be
			/// destroyed and replaced with a new, empty index.
			/// </summary>
		property bool ActionCreate;
			/// <summary>
			/// Check that each file in the index still exists on disk and remove from the index any files that no longer exist.
			/// </summary>
		property bool ActionRemoveDeleted;
			/// <summary>
			/// Remove the files listed in the text file identified by ToRemoveListName from the index.
			/// </summary>
		property bool ActionRemoveListed;
			/// <summary>
			/// Scan all index structures in the index to verify that the index is not damaged.
			/// </summary>
		property bool ActionVerify;
			/// <summary>
			/// Merge the indexes listed in IndexesToMerge into the index in IndexPath.
			/// </summary>
		property bool ActionMerge;

			/// <summary>
			/// Create accent sensitive index, so accents on letters will be treated as significant when searching
			/// </summary>
		property bool CreateAccentSensitive;
			/// <summary>
			///  Create case sensitive index, so apple and Apple will be two different words.
			/// </summary>
		property bool CreateCaseSensitive;
			/// <summary>
			/// Use relative rather than absolute paths when storing document locations.
			/// </summary>

		property bool CreateRelativePaths;
			/// <summary>
			/// DataSource object to index.
			/// </summary>
		property DataSource^ DataSourceToIndex;


			/// <summary>
			/// Full path of the folder where the index is located.
			/// </summary>
		property String^ IndexPath;
			/// <summary>
			/// Object to receive status update notifications during indexing.  Can be used to
			/// provide a progress display and to provide an opportunity to cancel an index update.
			/// </summary>
		property IIndexStatusHandler ^StatusHandler;

			/// <summary>
			/// A set of fields to be stored in the index
			/// for each document and returned as document properties in
			/// SearchResults.
			/// </summary>
			/// <remarks>
			/// When dtSearch indexes a document that contains fields, all
			/// fields are automatically made searchable.
			/// 
			/// Additionally, you can use StoredFields to designate certain
			/// fields as document properties that should be returned in
			/// search results. For example, if you make &quot;Subject&quot;
			/// a stored field, then the SearchResultsItem for each email
			/// message indexed will include the message Subject in
			/// SearchResultsItem.UserFields.
			/// 
			/// StoredFields expressions can include the wildcards * and ?.
			/// 
			/// The maximum amount of data to store for a field is controlled
			/// by Options.MaxStoredFieldSize.
			/// </remarks>
			/// <see cref="Retrieving Fields in Search Results" />           
		property System::Collections::Specialized::StringCollection ^StoredFields;

			/// <summary>
			/// A set of fields to be stored in the index for each document
			/// in a way that lets them be enumerated by
			/// WordListBuilder.ListFieldValues
			/// </summary>
			/// <remarks>
			/// When a field is listed in EnumerableFields, all of the values
			/// \of that field are stored in the index in a way that permits
			/// the values to be enumerated using
			/// WordListBuilder.ListFieldValues. The field names in the list
			/// can contain wildcards (* and ?). A set containing a single
			/// entry &quot;*&quot; would match all fields.
			/// 
			/// The maximum amount of text that can be stored in each stored
			/// field is the lesser of the maxStoredFieldSize option setting
			/// \or 128 characters.
			/// 
			/// All enumerable fields are also automatically designated as
			/// stored fields (see StoredFields).
			/// 
			/// \See also:
			/// 
			/// <see cref="dtSearch::Engine::WordListBuilder::ListFieldValues@String^@String^@long" text="WordListBuilder::ListFieldValues Method" />
			/// 
			/// 
			/// </remarks>                                                                                                                           
		property System::Collections::Specialized::StringCollection ^EnumerableFields;


			/// <summary>
			/// List of folders (or files) to index (add &lt;+&gt; after a
			/// folder name to include subfolders).
			/// </summary>
			/// <remarks>
			/// Use FoldersToIndex to specify the folders that should be
			/// scanned to locate documents to be indexed. Add &lt;+&gt;
			/// after a folder name to include subfolders, like this:
			/// &quot;c:\\docs&lt;+&gt;&quot;.
			/// 
			/// A file will be indexed if it matches one of the
			/// IncludeFilters and does not match any of the ExcludeFilters.
			/// 
			/// Filenames can also be added to FoldersToIndex, so to add
			/// c:\\docs\\sample.doc to the index, add &quot;c:\\docs\\sample.doc&quot;
			/// to the FoldersToIndex collection (this is more efficient than
			/// adding &quot;c:\\docs&quot; to FoldersToIndex and setting
			/// IncludeFilters to &quot;sample.doc&quot;).
			/// 
			/// 
			/// </remarks>                                                             
		property System::Collections::Specialized::StringCollection ^FoldersToIndex;

			/// <summary>
			/// Filters specifying documents not to index in the selected folders.
			/// </summary>
			/// <remarks>
			/// <para>Filename filters apply to the FoldersToIndex.  Documents returned by a DataSource are
			/// not filtered.</para>
			/// <para>A file will be indexed if it matches one of the IncludeFilters and does not match
			/// any of the ExcludeFilters.</para>
			/// <para>A filename filter that does not contain a slash is compared
			/// to the name of each file. A filename filter that contains a slash is compared to the
			/// fully-qualified pathname of each file.</para>
			/// </remarks>
		property System::Collections::Specialized::StringCollection ^ExcludeFilters;

			/// <summary>
			/// Filters specifying documents to index in the FoldersToIndex.
			/// </summary>
			/// <remarks>
			/// <para>Filename filters apply to the FoldersToIndex.  Documents returned by a DataSource are
			/// not filtered.</para>
			/// <para>A file will be indexed if it matches one of the IncludeFilters and does not match
			/// any of the ExcludeFilters.</para>
			/// <para>A filename filter that does not contain a slash is compared
			/// to the name of each file. A filename filter that contains a slash is compared to the
			/// fully-qualified pathname of each file.</para>
			/// </remarks>
		property System::Collections::Specialized::StringCollection ^IncludeFilters;

			/// <summary>
			/// Name of file containing list of files to remove from the index.
			/// </summary>
			/// <remarks>
			///  The file should be a plain text file, in UTF-8 or Unicode text format, with one
			///  filename per line.
			/// </remarks>
		property String^ ToRemoveListName;

			/// <summary>
			/// Name of file containing list of files to add to the index.
			/// </summary>
			/// <remarks>
			///  The file should be a plain text file, in UTF-8 or Unicode text format, with one
			///  filename per line.
			/// </remarks>
		property String^ ToAddFileListName;

			/// <summary>
			/// MaxMemToUseMB controls the size of the memory buffers that
			/// dtSearch can use to sort words during indexing.
			/// </summary>
			/// <remarks>
			/// If MaxMemToUseMB is zero, dtSearch will decide the amount of
			/// memory to use based on the estimated amount of text to be
			/// indexed and the amount of system memory available.
			/// 
			/// If possible, dtSearch will use memory for all sorting
			/// \operations; otherwise, some disk-based buffers will be used.
			/// For large updates, some disk-based sort buffers are always
			/// necessary and there is little benefit to MaxMemToUseMB values
			/// above 512.
			/// 
			/// MaxMemToUseMB does not affect other memory that may be used
			/// during indexing for other purposes such as parsing document
			/// formats.
			/// </remarks>                                                   
		property int MaxMemToUseMB;

			/// <summary>
			/// Flags controlling the indexing of documents.
			/// </summary>
		property dtSearch::Engine::IndexingFlags IndexingFlags;

			/// <summary>
			/// Number of megabytes to index before committing the index update.
			/// </summary>
			/// <remarks>
			/// By default, an index update will commit after all documents have been indexed.
			/// With version 7 indexes, an index can commit periodically during an update, so
			/// newly-indexed documents will be searchable before the index update is complete.
			/// </remarks>
		property int AutoCommitIntervalMB;


			/// <summary>
			/// Folder to use instead of the index folder for temporary files during indexing.
			/// </summary>
			/// <remarks>
			/// <para>
			/// During indexing, the dtSearch Engine may need to create temporary files to store
			/// word lists that are too large to fit into memory.  By default, these files will be
			/// placed in the index folder.  Use this setting to specify a different location
			/// for the word lists.  The indexer will automatically delete the word list files
			/// when the index update completes.
			/// </para>
			/// </remarks>
		property String^ TempFileDir;
		
			/// <summary>
			/// If non-zero, the first doc id to assign to documents in this index.
			/// </summary>
			///
			/// <remarks> 
			/// <para>
			/// By default, the dtSearch indexer assigns each document a numerical id starting with 1 in each 
			// index.  These ids are used in SearchFilters and in other places in the dtSearch Engine API
			/// to identify documents within an index.
			/// </para>
			/// <para>
			/// When an index is created, startingDocId can be used to specify that doc ids for the index
			/// should start with a value other than 1.  This can be useful in situations where
			/// it is important that doc ids not change when indexes are merged.
			/// </para>
			/// </remarks>
			
		property int StartingDocId;
		

			/// <summary>
			/// Indexes to merge into the target index, which is IndexPath.   (ActionMerge must be true for the merge to occur.)  For each index, provide the full path to the index.
			/// </summary>
		property System::Collections::Specialized::StringCollection ^IndexesToMerge;


			/// <summary>
			/// Checks whether an IndexJob started using ExecuteInThread is done, and also
			/// gets information on the state of the index update.
			/// </summary>
			/// <param name="waitTimeMillis">Number of milliseconds to wait of if the thread is not done.</param>
			/// <param name="status">Structure to update with the current
			/// state of the index update (can be null).</param>
			/// <returns>
			/// True if the thread is done, or false if the thread is still running.
			/// </returns>
			/// <remarks>
			///<para>
			/// An application that loops calling IsThreadDone() should call Sleep() between
			/// calls to allow the indexing thread to run.  Otherwise a high percentage
			/// if CPU resources will be spent just processing IsThreadDone calls.
			///</para>
			///<para>
			/// If waitTimeMillis is greater than zero, IsThreadDone will automatically
			/// wait up to waitTimeMillis milliseconds for the thread to complete before
			/// returning false.
			///</para>
			/// </remarks>
		bool IsThreadDone (int waitTimeMillis, IndexProgressInfo ^ status);
			//M:IsThreadDone(System.Int32,dtSearch.Engine.IndexProgressInfo)

			/// <summary>
			/// Executes the IndexJob and returns when the job is complete.
			/// </summary>
		virtual bool Execute () override;
			//M:Execute

			/// <summary>
			/// Starts a separate thread to execute the IndexJob and returns immediately.
			/// </summary>
		void ExecuteInThread ();
			//M:ExecuteInThread

			/// <summary>
			/// Aborts an IndexJob started using ExecuteInThread.
			/// </summary>
		void AbortThread ();
			//M:AbortThread

			/// <summary>
			/// Aborts an IndexJob started using ExecuteInThread, without saving anything indexed so far.
			/// </summary>
		void AbortThreadImmediate ();
			//M:AbortThreadImmediate


		/// <summary>
		/// Get information about the current state of an index
		/// </summary>
		/// <param name="indexPath">Location of the index</param>
		/// <returns>
		/// IndexInfo containing the current state of the index.  IndexInfo is
		/// returned even if the index could not be accessed.
		/// </returns>
		static IndexInfo ^GetIndexInfo(String ^indexPath);
			//M:GetIndexInfo(System.String)

	protected:
		CApiIndexJobForNet *indexJob;

	    void makeIndexProgressInfo(IndexProgressInfo ^ netStatus);


		void makeJob(CApiIndexJobForNet& netjob);

		DateTime ConvertToDateTime (dtsFileDate fileDate);

		virtual void DisposeUnmanagedItems() override;

	};
}
}
