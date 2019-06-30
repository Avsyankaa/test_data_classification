#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch { namespace Engine
{
		/// <summary>
		/// Sets indexing and searching option settings.
		/// </summary>
		/// <remarks>
		///  For more information about how these options affect indexing and searching, see the
		///  <see href="dtengine.chm::/Options_Overview.htm">"Options Overview"</see>
		///   section of the dtSearch Engine help file
		/// </remarks>
	public __gc class Options
	{
	public:

		Options ();
		~Options ();


		/// <summary>
		/// Flags that control text-processing options
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::TextFlags, TextFlags);

		/// <summary>
		/// Minimum length of a run of text when applying the Unicode Filtering algorithm.
		/// </summary>
		/// <remarks>
		/// This option specifies how many text characters must occur consecutively for a block of text to be included.
		/// At the default value, 6, a series of 5 text characters surrounded by non-text data would be filtered out.
		/// </remarks>
		IMPLEMENT_INT (UnicodeFilterMinTextSize);

		/// <summary>
		/// Amount of overlap when automatically breaking words when applying the Unicode Filtering algorithm.
		/// </summary>
		/// <remarks>
		/// <para>
		/// Unicode Filtering can automatically break long runs of letters into words each time more than
		/// Options.MaxWordLength consecutive letters are found.   By default, a word break is inserted
		/// and the next word starts with the following character.  Set UnicodeFilterWordOverlapAmount
		/// and also set the dtsoUfAutoWordBreakOverlapWords flag in UnicodeFilterFlags
		/// to start the next word before the end of the previous word.
		/// </para>
		/// <para>
		/// For example, suppose the maximum word length is set to 8, and
		/// the following run of letters is found:  aaaaahiddenaaaaa.
		/// By default, this would be indexed as aaaaahid and denaaaa, which
		/// means that a search for *hidden* would not find it.   With a word overlap
		/// of 4, this would be indexed as:  aaaaahid, ahiddena, denaaaaa
		/// which would allow the embedded word "hidden" to be found in a search for
		/// *hidden*.
		/// </para>
		/// </remarks>
		IMPLEMENT_INT (UnicodeFilterWordOverlapAmount);

		/// <summary>
		/// Flags controlling the behavior of the Unicode Filtering algorithm.
		/// </summary>
		IMPLEMENT_TYPE(dtSearch::Engine::UnicodeFilterFlags, UnicodeFilterFlags);

		/// <summary>
 		/// Specifies how each input file is divided into blocks before being filtered.
		/// </summary>
		IMPLEMENT_INT (UnicodeFilterBlockSize);

		/// <summary>
		/// Indicates Unicode ranges that are of interest when filtering.
		/// </summary>
		/// <remarks>
		/// This is used to help the filtering algorithm to distinguish text from non-text data.
		/// It is only used as a hint in the algorithm, so if the text extraction algorithm detects text in another language with a sufficient level of confidence, it will return that text even if the language was not selected.
		/// </remarks>
		IMPLEMENT_STRING (UnicodeFilterRanges);

		/// <summary>
		/// Name of dtSearch alphabet file to use when parsing text into words.
		/// </summary>
		/// <remarks>
		///<para>
		/// The alphabet file determines which characters are letters and what the rules are for handling capitalization and punctuation. If blank, a default alphabet will be created based on information supplied by Windows
		///</para>
		///<para>
		/// The alphabet file only controls the indexing of characters in the range 32-127.  Unicode character properties
		/// control the treatment of other characters.
		///</para>
		/// </remarks>
		IMPLEMENT_STRING (AlphabetFile);

		/// <summary>
		/// Wildcard character that matches a single digit (default: "=").
		/// </summary>
		IMPLEMENT_STRING (MatchDigitChar);

		/// <summary>
		/// Character used to indicate term weighting (example: <i>apple:5</i>)
		/// </summary>
		/// <remarks>
		/// The prefix used to add field name in front of a word in an xfilter expression
		/// is a doubling of the WeightChar (default "::").  For example, if you change
		/// the WeightChar to !, then an xfilter expression with a field would
		/// look like this:
		/// <code>xfilter(word "FieldName!!Something")</code>
		/// </remarks>
		IMPLEMENT_STRING (WeightChar);

		/// <summary>
		/// Character that enables synonym searching for a search term.
		/// </summary>
		IMPLEMENT_STRING (SynonymChar);

		/// <summary>
		/// Character that enables stemming for a search term (default: "~").
		/// </summary>
		/// <remarks>
		/// The expression used for range searching (default: "~~") a doubling of the
		/// StemmingChar.
		/// </remarks>
		IMPLEMENT_STRING (StemmingChar);


		/// <summary>
		/// Character that enables phonic searching for a search term (default "#")
		/// </summary>
		/// <remarks>
		/// The regular expression mark ("##") is a doubling of the PhonicChar.
		/// </remarks>
		IMPLEMENT_STRING (PhonicChar);

		/// <summary>
		/// Character that enables fuzzy searching for a search term (default: "%")
		/// </summary>
		IMPLEMENT_STRING (FuzzyChar);

		/// <summary>
		/// Character that indicates that a search term is a macro (default: "@")
		/// </summary>
		IMPLEMENT_STRING (MacroChar);

		/// <summary>
		/// Flags that control indexing of metadata.
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::FieldFlags, FieldFlags);


		/// <summary>
		/// Set to true to force all configuration files to be re-read when Save() is called.
		/// </summary>
		/// <remarks>
		/// If the contents of a configuration file such as TextFieldsFile changes, but
		/// the filename is not changed, set UpdateFiles=true to indicate that dtSearch should
		/// discard any internally-cached copies of configuration files and re-read them
		/// from disk.
		/// </remarks>
		IMPLEMENT_BOOLEAN (UpdateFiles);

		/// <summary>
		/// Name of the file containing rules for extraction of field data from text files based on markers in the next
		/// </summary>
		/// <remarks>
		/// The TextFieldsFile is an XML file.  To create the file, start dtSearch Desktop,
		/// click <b>Options &gt; Preferences &gt; Text Fields</b>
		/// and use the dialog box to set up the text field definitions.   The XML file will be saved as fields.xml
		/// in your dtSearch UserData folder.
		/// See <see href="http://support.dtsearch.com/webhelp/dtsearch/text_fields.htm">Define Text Fields</see> in the dtSearch Desktop help for information on this setting.
		/// </remarks>
		IMPLEMENT_STRING (TextFieldsFile);

		/// <summary>
		/// Name of the file containing a table of filename patterns for file formats that dtSearch cannot detect automatically, such as older versions of WordStar
		/// </summary>
		/// <remarks>
		/// The FileTypeTableFile is an XML file.  To create the file, start dtSearch Desktop,
		/// click <b>Options &gt; Preferences &gt; File Types</b>
		/// and use the dialog box to set up the file type definitions.   The XML file will be saved as filetype.xml
		/// in your dtSearch UserData folder.
		/// See <see href="http://support.dtsearch.com/webhelp/dtsearch/file_types.htm">File Types</see> in the dtSearch
		/// Desktop help for information on this setting.
		/// </remarks>
		IMPLEMENT_STRING (FileTypeTableFile);

		/// <summary>
		/// Use to replace the default connectors used in search requests.
		/// </summary>
		/// <remarks><para>
		/// The BooleanConnectors option setting lets you change the connectors that dtSearch uses in boolean searches (and, or, not, etc.)
		/// </para><para>
		/// The default connectors are: AND, OR, NOT, W/, CONTAINS, and TO.
		/// </para><para>
		/// To replace the default connectors with alternative connectors (i.e., in French) set BooleanConnectors to a series of items each of which consists of a connector string and the equivalent English connector, in this format:
		/// </para><code>
		///      {new connector=connector name}
		/// </code><para>For example:</para><code>
		/// {et=and} {ou=or} {sauf=not} {m/=w/} {contient=contains} {and=and} {or=or} {not=not} {contains=contains} {w/=w/} {to=to}
		/// </code>
		/// <para>
		/// The standard English connectors are included as well as the French connectors in this example.
		/// If BooleanConnectors is not blank, the connectors it defines will replace the defaults, so dtSearch will
		/// not recognize and, or, not, etc.  To restore the default English connectors, add an item for each connector
		/// to the list, as in the above example. It is possible to have more than one way of expressing each type of
		/// connector so, for example, "and" or "et" both are equivalent to the default "AND" connector.
		/// </para><para>
		/// Text in connectors must consist entirely of searchable letters. Punctuation characters that are not searchable letters cannot be used as part of a connector. The only exception to this rule is the w/N syntax for proximity searches, which allows w/ to be redefined as any single letter (a-z only) followed by a /.
		/// </para>
		/// </remarks>
		IMPLEMENT_STRING (BooleanConnectors);

		/// <summary>
		/// A directory that the dtSearch Engine can use to store temporary files.  Most applications should not set PrivateDir.
		/// </summary>
		/// <remarks>
		///<para>
		/// PrivateDir, if non-blank, must be unique for each concurrent user of the dtSearch Engine.
		/// </para>
		/// <para>
		/// If the privateDir is blank, no directory will be reserved for temporary files, and dtSearch will use
		/// the Windows GetTempPath() call to get a directory for temporary files.
		/// </para>
		/// </remarks>
		IMPLEMENT_STRING (PrivateDir);

		/// <summary>
		/// Directory where the dtSearch Engine and support files are located.
		/// </summary>
		/// <remarks>
		///<para>
		/// This may be a shared, read-only network directory, and is usually the folder where dten600.dll is located.
		///</para>
		///<para>
		/// The dtSearch Engine automatically loads external file parser DLLs from the <b>viewers</b>
		/// subdirectory of the HomeDir directory.  If no Home directory is supplied, the dtSearch Engine will
		/// attempt to determine the location of its home directory using Windows system calls. Often this will work, but it is better to supply a specific Home directory.
		///</para>
		/// </remarks>

		IMPLEMENT_STRING (HomeDir);

		/// <summary>
		/// User-defined synonym sets.
		/// </summary>
		/// <remarks>
		/// The UserThesaurusFile is an XML file.  To create the file, start dtSearch Desktop,
		/// click <b>Options &gt; Preferences &gt; User Thesaurus</b>
		/// and use the dialog box to set up the synonym definitions.   The XML file will be saved as thesaur.xml
		/// in your dtSearch UserData folder.
		/// See <see href="http://support.dtsearch.com/webhelp/dtsearch/user_thesaurus.htm">User Thesaurus</see> in the dtSearch
		/// Desktop help for information on this setting.
		/// </remarks>
		IMPLEMENT_STRING (UserThesaurusFile);

		/// <summary>
		/// File segmentation rules, used to split up long text files into logical subdocuments during indexing.
		/// </summary>
		/// <remarks>
		/// The SegmentationRulesFile is an XML file.  To create the file, start dtSearch Desktop,
		/// click <b>Options &gt; Preferences &gt; File Segmentation Rules</b>
		/// and use the dialog box to set up the rules.   The XML file will be saved as fileseg.xml
		/// in your dtSearch UserData folder.
		/// See <see href="http://support.dtsearch.com/webhelp/dtsearch/file_segmentation.htm">File Segmentation Rules</see> in the dtSearch
		/// Desktop help for information on this setting.
		/// </remarks>
		IMPLEMENT_STRING (SegmentationRulesFile);


		/// <summary>
		/// Words longer than the maxWordLength will be truncated when indexing. The default maxWordLength is 32.
		/// The maximum value is 128.
		/// </summary>
		IMPLEMENT_INT (MaxWordLength);

		/// <summary>
		/// Comma-separated list of tags to ignore when indexing XML
		/// </summary>
		/// <remarks>
		/// If your XML data includes HTML tags like &lt;P&gt;, &lt;B&gt;, &lt;I&gt;, etc., use XmlIgnoreTags
		/// to prevent these tags from affecting the field structure of XML.  Example:
		/// <code>
		/// options.XmlIgnoreTags = "b,p,i,u,h1,h2,h3,br,hr"
		/// </code>
		/// </remarks>
		IMPLEMENT_STRING (XmlIgnoreTags);

		/// <summary>
		/// By default, the dtSearch Engine collects the first 80 characters of text from a file for the title associated with each document. Use this option to change the number of characters stored, up to a maximum of 512.
		/// </summary>
		IMPLEMENT_INT (TitleSize);

		/// <summary>
		/// Maximum size of a single stored field.  Stored fields are field data collected during indexing that is returned in search results.
		/// </summary>
		IMPLEMENT_INT (MaxStoredFieldSize);

		///<summary>
		/// Maximum depth of nested fields (value must be between 1 and 32)
		/// </summary>
		IMPLEMENT_INT (MaxFieldNesting)

		/// <summary>
		/// Maximum number of words that can be matched in a search. This can be any value from 16 to 256k. The default is 64k. If a search matches more unique words than the maxWordsToRetrieve limit, the error code dtsErMaxWords (137) will be returned.
		/// </summary>
		IMPLEMENT_INT (MaxWordsToRetrieve);

		/// <summary>
		/// List of noise words to skip during indexing (default: "noise.dat")
		/// </summary>
		/// <remarks>
		///<para>
		/// A noise word is a word such as the or if that is so common that it is not useful in searches. To save time, noise words are not indexed and are ignored in index searches.
		///</para>
		///<para>
		/// When an index is created, dtSearch copies the list of words from noise.dat into the index directory and also builds the word list into other index files. After an index is created, subsequent changes to the noise word list will not affect indexing for that index
		///</para>
		/// </remarks>
		IMPLEMENT_STRING (NoiseWordFile);

		/// <summary>
		/// Stemming rules for stemming searches (default: "stemming.dat")
		/// </summary>
		/// <remarks>
		/// The stemming.dat file uses a plain text file and includes comments in the file
		/// that describe the file format.
		/// </remarks>
		IMPLEMENT_STRING (StemmingRulesFile);

		/// <summary>
		///  Define characters considered to be text if Options.BinaryFiles is set ot dtsoFilterBinary
		/// </summary>
		IMPLEMENT_STRING (BinaryFilterTextChars);

		/// <summary>
		/// If false, any word that begins with a digit will not be indexed.
		/// </summary>
		IMPLEMENT_BOOLEAN (IndexNumbers);

		/// <summary>
		/// Controls the treatment of hyphens
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::HyphenSettings, Hyphens);

		/// <summary>
		/// Controls the treatment of binary files.
		/// </summary>
		/// <remarks>
		/// Binary files are files that are not in a format that dtSearch recognizes and that do not appear to be plain text files.
		/// </remarks>
		IMPLEMENT_TYPE (dtSearch::Engine::BinaryFilesSettings, BinaryFiles);

		/// <summary>
		/// Delimiter between multiple instances of a stored field in a single file (default: " ")
		/// </summary>
		/// <remarks>
		/// This must be a single character in the range 1-127.
		/// </remarks>
		IMPLEMENT_STRING (StoredFieldDelimiterChar);


		/// <summary>
		/// Apply changes to Options.
		/// </summary>
		/// <remarks>
		/// Changes to options are applied when you call Save().   They are not really saved anywhere,
		/// so options must be set up each time your program runs.
		/// </remarks>
		void Save();
			//M:Save

	protected:
		FIELD_TYPE (dtSearch::Engine::TextFlags, TextFlags);
		FIELD_INT (UnicodeFilterMinTextSize);
		FIELD_INT (UnicodeFilterWordOverlapAmount);
		FIELD_TYPE (dtSearch::Engine::UnicodeFilterFlags, UnicodeFilterFlags);
		FIELD_INT (UnicodeFilterBlockSize);
		FIELD_STRING (UnicodeFilterRanges);
		FIELD_STRING (AlphabetFile);
		FIELD_STRING (MatchDigitChar);
		FIELD_STRING (WeightChar);
		FIELD_STRING (SynonymChar);
		FIELD_STRING (StemmingChar);
		FIELD_STRING (PhonicChar);
		FIELD_STRING (FuzzyChar);
		FIELD_STRING (MacroChar);
		FIELD_TYPE (dtSearch::Engine::FieldFlags, FieldFlags);
		FIELD_INT (LZWEnableCode);
		FIELD_BOOLEAN (UpdateFiles);
		FIELD_STRING (FileTypeTableFile);
		FIELD_STRING (BooleanConnectors);
		FIELD_STRING (PrivateDir);
		FIELD_STRING (HomeDir);
		FIELD_STRING (UserThesaurusFile);
		FIELD_STRING (TextFieldsFile);
		FIELD_STRING (SegmentationRulesFile);
		FIELD_STRING(StoredFieldDelimiterChar);
		FIELD_INT (MaxWordLength);
		FIELD_STRING (XmlIgnoreTags);
		FIELD_INT (TitleSize);
		FIELD_INT (MaxStoredFieldSize);
		FIELD_INT (MaxFieldNesting);
		FIELD_INT (MaxWordsToRetrieve);
		FIELD_STRING (NoiseWordFile);
		FIELD_STRING (StemmingRulesFile);
		FIELD_STRING (BinaryFilterTextChars);
		FIELD_BOOLEAN (IndexNumbers);
		FIELD_TYPE (dtSearch::Engine::HyphenSettings, Hyphens);
		FIELD_TYPE (dtSearch::Engine::BinaryFilesSettings, BinaryFiles);

	protected:
	};
}
}