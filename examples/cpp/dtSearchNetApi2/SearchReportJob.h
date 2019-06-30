#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"


using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch
{

namespace Engine
{
	ref class SearchResults;

	/// <summary>
	/// Generates a report showing each hit in one or more documents,
	/// with a specified amount of context
	/// </summary>
	/// <remarks>
	/// To generate a search report,
	/// 
	/// (1) Start with a SearchResults object representing the
	/// results of a search.
	/// 
	/// (2) Call SearchJob.NewSearchReportJob to make a
	/// SearchReportJob
	/// 
	/// (3) Select the items to include in the search report using
	/// the Select*() methods in SearchReportJob
	/// 
	/// (4) Specify the amount of context to include using
	/// WordsOfContextExact, ParagraphsOfContext, or WordsOfContext
	/// 
	/// (5) Set the output format for the report using ContextFooter,
	/// ContextHeader, etc.
	/// 
	/// (6) Call Execute() to generate the report
	/// 
	/// <b>Format</b>
	/// 
	/// A search report lists the hits found in one or more
	/// documents, with each hit surrounded by a specified amount of
	/// context. Each block of context starts with a ContextHeader
	/// and ends with the ContextFooter. Contiguous or overlapping
	/// blocks of context will be combined. The amount of context
	/// included in the report can be specified by words or by
	/// paragraphs.
	/// 
	/// Each block of context is constructed as follows:
	/// <code>
	///     [ContextHeader]
	///     ...text... [BeforeHit] hit [AfterHit] ...text...
	///     [ContextFooter]
	/// </code>
	/// The report as a whole is constructed as follows:
	/// <code>
	///     [Header]
	///         [FileHeader]
	///             [ContextHeader]
	///             ...text... [BeforeHit] hit [AfterHit] ...text...
	///             [ContextFooter]
	///             [ContextSeparator]
	///             [ContextHeader]
	///             ...text... [BeforeHit] hit [AfterHit] ...text...
	///             [ContextFooter]
	///             ... more blocks of context, if present
	///         [FileFooter]
	///         ... more files ...
	///     [Footer]
	/// </code>
	/// 
	/// Use the following symbols to insert file information into the
	/// FileHeader and FileFooter:
	/// <xtable>
	/// Symbol               Meaning
	/// -------------------  -------------------------------------------
	/// Filename             The name of the file (without path
	///                       information). For PDF and HTML files, this
	///                       will be the Title.
	/// Location             The location of the file
	/// Fullname             The path and filename of the file.
	/// Size                 \File size in bytes
	/// SizeK                \File size in kilobytes
	/// Date                 Modification date of the file when indexed
	/// Hits                 Number of hits in the file
	/// Title                The first 80 characters of the file
	/// DocId                The docId of the file
	/// Type                 The file type (Microsoft Word, PDF, HTML,
	///                       etc.)
	/// Ordinal              The 1-based ordinal of this item in the
	///                       SearchResults from which it was generated
	/// IndexRetrievedFrom   The index where the file was found
	/// </xtable>
	/// Use %% around each symbol, like this: %%FullName%%
	/// 
	/// Use the following symbols to insert context information in
	/// the ContextHeader, which appears in front of each block of
	/// context:
	/// <xtable>
	/// Symbol      Meaning
	/// ----------  ------------------------------------------------------
	/// Page        Page number where the hit occurs
	/// Paragraph   Paragraph number where the hit occurs (relative to
	///              the start of the page)
	/// Word        Word offset of the block of context from the
	///              beginning of the file.
	/// FirstHit    Word offset of the first hit in the block of context.
	/// </xtable>
	/// 
	/// </remarks>                                                        
	public ref class SearchReportJob : public DisposableObject
	{
	public:

		SearchReportJob ();
		~SearchReportJob ();

		/// <summary>
		/// File to write search report to.
		/// </summary>
		property String^ OutputFile;

		/// <summary>
		/// Text to be inserted before each hit in the report to highlight hits.
		/// </summary>
		property String^ BeforeHit;
		/// <summary>
		/// Text to be inserted after each hit in the report.
		/// </summary>
		property String^ AfterHit;
		/// <summary>
		/// Text to appear at the top of the report.
		/// </summary>
		property String^ Header;
		/// <summary>
		/// Text to appear after the end of the report.
		/// </summary>
		property String^ Footer;

		/// <summary>
		/// Text to appear at the start of each document in the report.
		/// </summary>
		/// <remarks>
		/// Use the following symbols to insert file information into the header.
		///		<list type="table">
		///		<ListHeader>
		///			<term>Symbol</term><description>Meaning</description>
		///		</ListHeader>
		///		<item>
		///			<term>%%Filename%%</term>
		///			<description>The name of the file (without path information). For PDF and HTML files, this will be the Title.</description>
		///		</item>
		///
		///		<item>
		///			<term>%%Location%%</term>
		///			<description>The location of the file</description>
		///		</item>
		///		<item>
		///			<term>%%Fullname%%</term>
		///			<description>The path and filename of the file.</description></item>
		///		<item>
		///			<term>%%Size%%</term>
		///			<description>File size in bytes</description></item>
		///		<item><term>%%Date%%</term>
		///			<description>Modification date of the file when indexed</description>
		///		</item>
		///		<item>
		///			<term>%%Hits%%</term>
		///			<description>Number of hits in the file</description>
		///		</item>
		///		<item>
		///			<term>%%Title%%</term>
		///			<description>The first 80 characters of the file</description>
		///		</item>
		///		</list>
		/// </remarks>
		property String^ FileHeader;

		/// <summary>
		/// Text to appear after each document in the report.
		/// </summary>
		property String^ FileFooter;

		/// <summary>
		/// Text to appear at the start of each block of context in the
		/// report.
		/// </summary>                                                 
		property String^ ContextHeader;

		/// <summary>
		/// Text to appear after each block of context in the report.
		/// </summary>
		property String^ ContextFooter;

		/// <summary>
		/// Text to appear between blocks of context in the report (after one ContextFooter, before the next ContextHeader)
		/// </summary>
		property String^ ContextSeparator;


		/// <summary>
		/// Number of words to scan in each document looking for blocks of context to include in the report.
		/// </summary>
		/// <remarks>
		/// For example, if maxWordsToRead=5000, then the search report will only include hits that occur in the first 5000 words in each document. This setting can be used to generate faster (but not necessarily complete) reports by limiting the search report to hits near the beginning of each document
		/// </remarks>
		property int MaxWordsToRead;

		/// <summary>
		/// Number of blocks of context to include in the report for each document.
		/// </summary>
		property int MaxContextBlocks;

		/// <summary>
		/// Approximate number of words of context to include around each hit.
		/// </summary>
		/// <remarks>
		/// Use setWordsOfContextExact to get exactly the requested number of words,
		/// and use setWordsOfContext to get whole paragraphs with at least the requested number of words
		/// </remarks>
		property int WordsOfContext;

		/// <summary>
		///  Number of words of context to include around each hit.
		/// </summary>
		/// <remarks>
		/// Use setWordsOfContextExact to get exactly the requested number of words,
		/// and use setWordsOfContext to get whole paragraphs with at least the requested number of words
		/// </remarks>
		property int WordsOfContextExact;

		/// <summary>
		/// Number of paragraphs of context to include around each hit.
		/// </summary>
		property int ParagraphsOfContext;

		/// <summary>
		/// The file format of the output.
		/// </summary>
		property dtSearch::Engine::OutputFormats OutputFormat;

		/// <summary>
		/// Maximum number of characters to generate in the OutputString, when OutputToString is true.
		/// </summary>
		property int OutputStringMaxSize;

		/// <summary>
		/// Set to a non-zero value to make the job terminate automatically after a specified number of seconds.
		/// </summary>
		property int TimeoutSeconds;

		/// <summary>
		/// Flags controlling generation of the report.
		/// </summary>
		dtSearch::Engine::ReportFlags Flags;

		/// <summary>
		/// Generate output to a string (OutputString), rather than to a file (OutputFile).
		/// </summary>
		property bool OutputToString;

		/// <summary>
		/// Result of the search report, if OutputToString was set to true
		/// </summary>
		property String^ OutputString;

		/// <summary>
		/// Contains any errors that occurred during generation of the search report.
		/// </summary>
		property JobErrorInfo^ Errors;

		/// <summary>
		/// Select all items in the SearchResults.
		/// </summary>
		void SelectAll ();
			//M:SelectAll

		/// <summary>
		/// Select no items in the SearchResults.
		/// </summary>
		void ClearSelections ();
			//M:ClearSelections

		/// <summary>
		/// Select a range of items in the SearchResults.
		/// </summary>
		/// <remarks>
		/// iFirstItem and iLastItem are 0-based indexes into the SearchResults list, so
		/// the range of permissible values is 0 to Count-1.
		/// </remarks>
		void SelectItems (long iFirstItem, long iLastItem);
			//M:SelectItems(System.Int32,System.Int32)

		/// <summary>
		/// Generate the report.
		/// </summary>
		void Execute ();
			//M:Execute

		/// <summary>
		/// The search results list that this SearchReportJob will use.
		/// </summary>
		/// <remarks>
		/// The SearchResults will be attached automatically if you used its NewSearchReportJob method to create this job.
		/// </remarks>
		void SetResults(SearchResults^ parent);
			//M:SetResults(dtSearch.Engine.SearchResults)

	protected:


	protected:
		DSearchReportJob* m_job;
		DSearchResults* m_pResults;
		SearchResults^	m_pObjParentResults;

		virtual void DisposeUnmanagedItems() override;

	};
}
}
