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
	public __gc class SearchResults;

	/// <summary>
	/// Generates a report showing each hit in one or more documents, with a specified amount of context
	/// </summary>
	/// <remarks>
	///<para>
	/// Use BeforeHit and AfterHit to insert hit highlight markings (for example, &lt;b&gt;
	/// and &lt;/b&gt; in HTML files).
	///</para>
	///</remarks>
	public __gc class SearchReportJob : public DisposableObject
	{
	public:

		SearchReportJob ();
		~SearchReportJob ();

		/// <summary>
		/// File to write search report to.
		/// </summary>
		IMPLEMENT_STRING (OutputFile);

		/// <summary>
		/// Text to be inserted before each hit in the report to highlight hits.
		/// </summary>
		IMPLEMENT_STRING (BeforeHit);
		/// <summary>
		/// Text to be inserted after each hit in the report.
		/// </summary>
		IMPLEMENT_STRING (AfterHit);
		/// <summary>
		/// Text to appear at the top of the report.
		/// </summary>
		IMPLEMENT_STRING (Header);
		/// <summary>
		/// Text to appear after the end of the report.
		/// </summary>
		IMPLEMENT_STRING (Footer);

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
		IMPLEMENT_STRING (FileHeader);

		/// <summary>
		/// Text to appear after each document in the report.
		/// </summary>
		IMPLEMENT_STRING (FileFooter);

		/// <summary>
		/// Text to appear at the start of each block of context in the report.
		/// </summary>
		/// <remarks>
		/// Use the following symbols to insert file information into the context header.
		///<list type="table">
		///<ListHeader>
		///	<term>Symbol</term><description>Meaning</description>
		///</ListHeader>
		///<item>
		///	<term>%%Filename%%</term>
		///	<description>The name of the file (without path information). For PDF and HTML files, this will be the Title.</description>
		///</item>
		///<item>
		///	<term>%%Page%%</term>
		///	<description>
		///Page number where the hit occurs</description>
		///</item>
		///<item>
		///	<term>%%Paragraph%%</term>
		///	<description>Paragraph number where the hit occurs (relative to the start of the page)</description>
		///</item>
		///<item>
		///	<term>%%Word%%</term>
		///	<description>Word offset of the block of context from the beginning of the file.</description>
		///</item>
		///<item>
		///	<term>%%FirstHit%%</term>
		///	<description>Word offset of the first hit in this block of context.</description>
		///</item>
		///</list>
		///</remarks>
		IMPLEMENT_STRING (ContextHeader);

		/// <summary>
		/// Text to appear after each block of context in the report.
		/// </summary>
		IMPLEMENT_STRING (ContextFooter);

		/// <summary>
		/// Text to appear between blocks of context in the report (after one ContextFooter, before the next ContextHeader)
		/// </summary>
		IMPLEMENT_STRING (ContextSeparator);


		/// <summary>
		/// Number of words to scan in each document looking for blocks of context to include in the report.
		/// </summary>
		/// <remarks>
		/// For example, if maxWordsToRead=5000, then the search report will only include hits that occur in the first 5000 words in each document. This setting can be used to generate faster (but not necessarily complete) reports by limiting the search report to hits near the beginning of each document
		/// </remarks>
		IMPLEMENT_INT (MaxWordsToRead)

		/// <summary>
		/// Number of blocks of context to include in the report for each document.
		/// </summary>
		IMPLEMENT_INT (MaxContextBlocks)

		/// <summary>
		/// Approximate number of words of context to include around each hit.
		/// </summary>
		/// <remarks>
		/// Use setWordsOfContextExact to get exactly the requested number of words,
		/// and use setWordsOfContext to get whole paragraphs with at least the requested number of words
		/// </remarks>
		IMPLEMENT_INT (WordsOfContext)

		/// <summary>
		///  Number of words of context to include around each hit.
		/// </summary>
		/// <remarks>
		/// Use setWordsOfContextExact to get exactly the requested number of words,
		/// and use setWordsOfContext to get whole paragraphs with at least the requested number of words
		/// </remarks>
		IMPLEMENT_INT (WordsOfContextExact)

		/// <summary>
		/// Number of paragraphs of context to include around each hit.
		/// </summary>
		IMPLEMENT_INT (ParagraphsOfContext)

		/// <summary>
		/// The file format of the output.
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::OutputFormats, OutputFormat)

		/// <summary>
		/// Maximum number of characters to generate in the OutputString, when OutputToString is true.
		/// </summary>
		IMPLEMENT_INT (OutputStringMaxSize)

		/// <summary>
		/// Set to a non-zero value to make the job terminate automatically after a specified number of seconds.
		/// </summary>
		IMPLEMENT_INT (TimeoutSeconds)

		/// <summary>
		/// Flags controlling generation of the report.
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::ReportFlags, Flags)

		/// <summary>
		/// Generate output to a string (OutputString), rather than to a file (OutputFile).
		/// </summary>
		IMPLEMENT_BOOLEAN (OutputToString)

		/// <summary>
		/// Result of the search report, if OutputToString was set to true
		/// </summary>
		IMPLEMENT_STRING_GET (OutputString)

		/// <summary>
		/// Contains any errors that occurred during generation of the search report.
		/// </summary>
		IMPLEMENT_TYPE_GET (JobErrorInfo*, Errors);

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
		void SetResults(SearchResults* parent);
			//M:SetResults(dtSearch.Engine.SearchResults)

	protected:

		FIELD_STRING (OutputFile);
		FIELD_STRING (BeforeHit);
		FIELD_STRING (AfterHit);
		FIELD_STRING (Footer);
		FIELD_STRING (Header);
		FIELD_STRING (FileHeader);
		FIELD_STRING (FileFooter);
		FIELD_STRING (ContextHeader);
		FIELD_STRING (ContextFooter);
		FIELD_STRING (ContextSeparator);
		FIELD_INT (MaxWordsToRead)
		FIELD_INT (MaxContextBlocks)
		FIELD_INT (WordsOfContext)
		FIELD_INT (WordsOfContextExact)
		FIELD_INT (ParagraphsOfContext)
		FIELD_TYPE (dtSearch::Engine::OutputFormats, OutputFormat)
		FIELD_INT (OutputStringMaxSize)
		FIELD_INT (TimeoutSeconds)
		FIELD_TYPE (dtSearch::Engine::ReportFlags, Flags)

		FIELD_BOOLEAN (OutputToString)
		FIELD_STRING_GET (OutputString)
		FIELD_TYPE_GET (JobErrorInfo*, Errors);

	protected:
		DSearchReportJob* m_job;
		DSearchResults* m_pResults;
		SearchResults*	m_pObjParentResults;

		virtual void DisposeUnmanagedItems();

	};
}
}
