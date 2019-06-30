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
	public __gc class IndexFileInfo {
		public:
		String *Name;
		String *Location;
		String *Type;
		String *OpenFailMessage;
		Int32 Size;
		Int32 BytesRead;
		Int32 WordCount;
		Int32 PercentDone;
		TypeId TypeId;
		Int32 DocId;
		String *DisplayName;
		String *Fullname;
		};


	/// <summary>
	/// State of an index update
	/// </summary>
	public __gc class IndexProgressInfo {
		public:
		IndexProgressInfo() {
			File = new IndexFileInfo;
			}
		MessageCode UpdateType;
		IndexingStep Step;
		String *IndexPath;
		Int32 PercentDone;

		UInt32 DocBytesReadKB;
		UInt32 BytesToIndexKB;
		UInt32 BytesReadKB;
		Int32 IndexPercentFull;
		UInt32 BinaryCount;
		UInt32 OpenFailures;
		UInt32 EncryptedCount;
		UInt32 WordsInIndex;
		UInt32 DocsInIndex;
		IndexFileInfo *File;
		};


	/// <summary>
	/// Information about the current state of an index
	/// </summary>
	public __gc class IndexInfo {
		public:
		UInt32 DocCount;
		UInt64 WordCount;
		UInt32 Fragmentation;
		UInt32 ObsoleteCount;
		DateTime CreatedDate;
		DateTime UpdatedDate;
		DateTime CompressedDate;
		UInt64 IndexSize;
		UInt32 PercentFull;
		IndexingFlags Flags;
		UInt32 StructureVersion;
		};

		/// <summary>
		/// Provides a way for the caller to receive updates as the IndexJob
		/// progresses.
		/// </summary>
		/// <remarks>
		/// Use IIndexStatusHandler if your application needs to receive a
		/// notification each time a document is indexed.
		/// </remarks>
	public __gc __interface IIndexStatusHandler	{
	public:

		void OnProgressUpdate(IndexProgressInfo *info);
		AbortValue CheckForAbort ();
	};

		/// <summary>
		///  Create or update indexes
		/// </summary>
		/// <remarks>
		///<para>
		///  For more information on creating,
		///  updating, and maintaining indexes, see
		///  <see href="dtSearchApiRef.chm::/Building_and_Maintaining_Indexes.html">Building and Maintaining Indexes</see>
		///   in dtSearchApiRef.chm.
		///</para>
		///<para>
		/// To create or update an index, make an IndexJob, use the properties to describe the indexing task you
		/// want the engine to perform, and call the Execute method.
		///</para>
		///<para>
		/// IndexJob provides two ways to specify
		/// the text to be indexed: (1) the FoldersToIndex, Include Filters, and ExcludeFilters members
		/// let you specify directories and filename filters
		/// identifying a set of disk files to index, and (2) the DataSourceToIndex member lets you supply
		/// text data directly to the dtSearch Engine for indexing, using an object that implements
		/// the <see cref="T:dtSearch.Engine.DataSource">DataSource</see> interface.  DataSourceToIndex is useful for indexing data
		/// from non-file sources such as message stores, SQL databases, dynamically-generated data, or any non-file
		/// data accessible to your program.
		///</para>
		///<para>
		/// The Action flags specify the actions you want the engine to perform. If more than one action is specified,
		/// the engine will perform the actions in the following order: create, removeDeleted, removeListed, add,
		/// compress, merge, verify.
		///</para>
		/// </remarks>
		/// <seealso cref="T:dtSearch.Engine.DataSource"/>
		/// <seealso href="dtengine.chm::/Building_and_Maintaining_Indexes.htm">"Building and Maintaining Indexes"</seealso>
	public __gc class IndexJob : public NetJobBase
	{
	public:

		IndexJob ();
		~IndexJob ();

			/// <summary>
			/// Add documents to an existing index.  If ActionCreate is not set, the index
			/// must already exist or the IndexJob will fail.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionAdd)
			/// <summary>
			/// Remove obsolete information from the index.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionCompress)
			/// <summary>
			/// Create a new index. If an index already exists in the specified directory, the index will be
			/// destroyed and replaced with a new, empty index.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionCreate)
			/// <summary>
			/// Check that each file in the index still exists on disk and remove from the index any files that no longer exist.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionRemoveDeleted)
			/// <summary>
			/// Remove the files listed in the text file identified by ToRemoveListName from the index.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionRemoveListed)
			/// <summary>
			/// Scan all index structures in the index to verify that the index is not damaged.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionVerify)
			/// <summary>
			/// Merge the indexes listed in IndexesToMerge into the index in IndexPath.
			/// </summary>
		IMPLEMENT_BOOLEAN (ActionMerge)

			/// <summary>
			/// Create accent sensitive index, so accents on letters will be treated as significant when searching
			/// </summary>
		IMPLEMENT_BOOLEAN (CreateAccentSensitive)
			/// <summary>
			///  Create case sensitive index, so apple and Apple will be two different words.
			/// </summary>
		IMPLEMENT_BOOLEAN (CreateCaseSensitive)
			/// <summary>
			/// Use relative rather than absolute paths when storing document locations.
			/// </summary>

		IMPLEMENT_BOOLEAN (CreateRelativePaths)
			/// <summary>
			/// DataSource object to index.
			/// </summary>
		IMPLEMENT_TYPE_SET (DataSource*, DataSourceToIndex)


			/// <summary>
			/// Full path of the folder where the index is located.
			/// </summary>
		IMPLEMENT_STRING (IndexPath)
			/// <summary>
			/// Object to receive status update notifications during indexing.  Can be used to
			/// provide a progress display and to provide an opportunity to cancel an index update.
			/// </summary>
		IMPLEMENT_TYPE_SET (IIndexStatusHandler*, StatusHandler)
			/// <summary>
			/// Fields to be stored in the index for each document and returned as document properties in SearchResults.
			/// </summary>
			/// <remarks>
			///<para>
			/// When dtSearch indexes a document that contains fields, all fields are automatically
			/// made searchable.
			/// </para>
			/// <para>
			/// Additionally, you can use StoredFields to designate certain fields as document properties
			/// that should be returned in search results.  For example, if you make "Subject" a stored field,
			/// then the
			/// <see cref="T:dtSearch.Engine.SearchResultsItem">SearchResultsItem</see>
			/// for each email message indexed will include the message Subject in
			/// <see cref="F:dtSearch.Engine.SearchResultsItem.UserFields">SearchResultsItem.UserFields</see>.
			///</para>
			///<para>
			/// StoredFields expressions can include the wildcards * and ?.
			///</para>
			///<para>
			/// The maximum amount of data to store for a field is controlled by Options.MaxStoredFieldSize.
			///</para>
			/// </remarks>
			/// <seealso cref="T:dtSearch.Engine.SearchResultsItem"/>
		IMPLEMENT_STRCOL(StoredFields)

			/// <summary>
			/// List of folders (or files) to index (add &lt;+&gt; after a folder name to include subfolders).
			/// </summary>
			/// <remarks>
			/// <para>
			/// Use FoldersToIndex to specify the folders that should be scanned to
			/// locate documents to be indexed.  Add &lt;+&gt; after a folder name to include subfolders,
			/// like this:  "c:\docs&lt;+&gt;".
			/// </para>
			/// <para>A file will be indexed if it matches one of the IncludeFilters and does not match
			/// any of the ExcludeFilters.</para>
			/// <para>
			/// Filenames can also be added to FoldersToIndex, so to add c:\docs\sample.doc to the
			/// index, add "c:\docs\sample.doc" to the FoldersToIndex collection (this is more efficient
			/// than adding "c:\docs" to FoldersToIndex and setting IncludeFilters to "sample.doc").
			/// </para>
			/// </remarks>
		IMPLEMENT_STRCOL(FoldersToIndex)

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
		IMPLEMENT_STRCOL (ExcludeFilters)

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
		IMPLEMENT_STRCOL (IncludeFilters)

			/// <summary>
			/// Name of file containing list of files to remove from the index.
			/// </summary>
		IMPLEMENT_STRING (ToRemoveListName)

			/// <summary>
			/// Contains any error messages generated during the indexing job.
			/// </summary>
		IMPLEMENT_TYPE_GET (JobErrorInfo*, Errors)

			/// <summary>
			/// Maximum amount of memory that the indexer will use for workspace (in megabytes). By default, the indexer will calculate how much memory to use based on the size of the document collection being indexed and the amount of physical RAM available (no more than 50% of the amount of physical RAM installed will be used). At least 10 Mb is needed.
			/// </summary>
		IMPLEMENT_INT (MaxMemToUseMB)

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
		IMPLEMENT_STRING (TempFileDir)

			/// <summary>
			/// Flags controlling the indexing of documents.
			/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::IndexingFlags, IndexingFlags)

			/// <summary>
			/// Number of megabytes to index before committing the index update.
			/// </summary>
			/// <remarks>
			/// By default, an index update will commit after all documents have been indexed.
			/// With version 7 indexes, an index can commit periodically during an update, so
			/// newly-indexed documents will be searchable before the index update is complete.
			/// </remarks>
		IMPLEMENT_INT (AutoCommitIntervalMB)
			/// <summary>
			/// Indexes to merge into the target index, which is IndexPath.   (ActionMerge must be true for the merge to occur.)  For each index, provide the full path to the index.
			/// </summary>
		IMPLEMENT_STRCOL(IndexesToMerge)


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
		bool IsThreadDone (int waitTimeMillis, IndexProgressInfo *status);
			//M:IsThreadDone(System.Int32,dtSearch.Engine.IndexProgressInfo)

			/// <summary>
			/// Executes the IndexJob and returns when the job is complete.
			/// </summary>
		bool Execute ();
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
		static IndexInfo *GetIndexInfo(String *indexPath);
			//M:GetIndexInfo(System.String)

	protected:

		FIELD_BOOLEAN (ActionAdd)
		FIELD_BOOLEAN (ActionCompress)
		FIELD_BOOLEAN (ActionCreate)
		FIELD_BOOLEAN (ActionRemoveDeleted)
		FIELD_BOOLEAN (ActionRemoveListed)
		FIELD_BOOLEAN (ActionVerify)
		FIELD_BOOLEAN (ActionMerge)
		FIELD_BOOLEAN (CreateAccentSensitive)
		FIELD_BOOLEAN (CreateCaseSensitive)

		FIELD_TYPE_SET (DataSource*, DataSourceToIndex)

		FIELD_STRING (IndexName)
		FIELD_STRING (IndexPath)

		FIELD_TYPE_SET (IIndexStatusHandler*, StatusHandler)

		FIELD_STRCOL (StoredFields)
		FIELD_STRCOL (FoldersToIndex)
		FIELD_STRCOL (ExcludeFilters)
		FIELD_STRCOL (IncludeFilters)
		FIELD_STRING (ToRemoveListName)
		FIELD_STRING (TempFileDir)


		FIELD_TYPE_GET (dtSearch::Engine::IndexingStep, StatusIndexingStep)
		FIELD_INT_GET (StatusPercentDone)
		FIELD_STRING_GET (StatusCurrentFileLocation)
		FIELD_STRING_GET (StatusCurrentFileName)
		FIELD_STRING_GET (StatusCurrentFileType)
		FIELD_INT_GET (StatusDiskAvailKB)
		FIELD_INT_GET (StatusWordsInIndex)
		FIELD_INT_GET (StatusDocsInIndex)
		FIELD_INT_GET (StatusBytesIndexedKB)
		FIELD_INT_GET (StatusBytesToIndexKB)
		FIELD_INT_GET (StatusCurrentFilePercentDone)

		FIELD_BOOLEAN (CreateAddToLibrary)
		FIELD_BOOLEAN (CreateRelativePaths)

		FIELD_INT (MaxMemToUseMB)
		FIELD_TYPE (dtSearch::Engine::IndexingFlags, IndexingFlags)
		FIELD_INT (AutoCommitIntervalMB)

		FIELD_STRCOL( IndexesToMerge)

	protected:
		CApiIndexJobForNet *indexJob;

	    void makeIndexProgressInfo(IndexProgressInfo *netStatus);


		void makeJob(CApiIndexJobForNet& netjob);

		DateTime ConvertToDateTime (dtsFileDate fileDate);

		void DisposeUnmanagedItems();

	};
}
}
