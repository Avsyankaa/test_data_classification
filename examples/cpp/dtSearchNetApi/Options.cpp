#include "stdafx.h"

#include "Options.h"
#include "Constants.h"

using namespace System;

namespace dtSearch { namespace Engine
{
	Options::Options ()
	{	CountCreate(otOptions);
		DHeapPtr<DOptions> pOpts = new DOptions;
		m_AlphabetFile = pOpts->AlphabetFile.allocNetString();
		m_BinaryFiles = (dtSearch::Engine::BinaryFilesSettings) pOpts->binaryFiles;
		m_Hyphens = (dtSearch::Engine::HyphenSettings) pOpts->hyphens;
		m_IndexNumbers = (pOpts->indexNumbers != FALSE);
		m_MaxStoredFieldSize = pOpts->maxStoredFieldSize;
		m_MaxFieldNesting = pOpts->maxFieldNesting;
		m_MaxWordsToRetrieve = pOpts->maxWordsToRetrieve;
		m_NoiseWordFile =  (pOpts->NoiseWordFile.allocNetString());
		m_StemmingRulesFile =  (pOpts->StemmingRulesFile.allocNetString());
		m_TitleSize = pOpts->titleSize;
		m_XmlIgnoreTags =  (pOpts->XmlIgnoreTags.allocNetString());
		m_MaxWordLength = pOpts->maxWordLength;

		m_SegmentationRulesFile =  (pOpts->SegmentationRulesFile.allocNetString());
		m_TextFieldsFile =  (pOpts->TextFieldsFile.allocNetString());
		m_UserThesaurusFile =  (pOpts->UserThesaurusFile.allocNetString());
		m_HomeDir =  (pOpts->HomeDir.allocNetString());
		m_PrivateDir =  (pOpts->PrivateDir.allocNetString());
		m_BooleanConnectors =  (pOpts->BooleanConnectors.allocNetString());
		m_FileTypeTableFile =  (pOpts->FileTypeTableFile.allocNetString());
		m_BinaryFilterTextChars = (pOpts->BinaryFilterTextChars.allocNetString());

		m_UpdateFiles = (pOpts->updateFiles != FALSE);
		m_LZWEnableCode = pOpts->lzwEnableCode;
		m_FieldFlags = (dtSearch::Engine::FieldFlags) pOpts->fieldFlags;
		m_TextFlags = (dtSearch::Engine::TextFlags) pOpts->textFlags;

		m_MacroChar = Char::ToString (pOpts->macroChar);
		m_FuzzyChar = Char::ToString (pOpts->fuzzyChar);
		m_PhonicChar = Char::ToString (pOpts->phonicChar);
		m_StemmingChar = Char::ToString (pOpts->stemmingChar);
		m_SynonymChar = Char::ToString (pOpts->synonymChar);
		m_WeightChar = Char::ToString (pOpts->weightChar);
		m_MatchDigitChar = Char::ToString (pOpts->matchDigitChar);
		m_StoredFieldDelimiterChar = Char::ToString(pOpts->storedFieldDelimiterChar);

		m_UnicodeFilterFlags = (dtSearch::Engine::UnicodeFilterFlags) pOpts->unicodeFilterFlags;
		m_UnicodeFilterBlockSize = pOpts->unicodeFilterBlockSize;
		m_UnicodeFilterMinTextSize = pOpts->unicodeFilterMinTextSize;
		m_UnicodeFilterWordOverlapAmount = pOpts->unicodeFilterWordOverlapAmount;

		DString ranges;
		for (int i = 0; i < 256; ++i) {
			if (pOpts->unicodeFilterRanges[i])
				ranges << i << " ";
			}
		m_UnicodeFilterRanges =  (ranges.allocNetString());
	}

	Options::~Options ()
	{	CountDelete(otOptions);
	}

	void Options::Save ()
	{
		DHeapPtr<DOptions> pOpts = new DOptions;
		pOpts->AlphabetFile.set(m_AlphabetFile);
		pOpts->binaryFiles = m_BinaryFiles;
		pOpts->hyphens = m_Hyphens;
		pOpts->indexNumbers = m_IndexNumbers;
		pOpts->maxStoredFieldSize = m_MaxStoredFieldSize;
		pOpts->maxFieldNesting = m_MaxFieldNesting;
		pOpts->maxWordsToRetrieve = m_MaxWordsToRetrieve;
		pOpts->NoiseWordFile.set(m_NoiseWordFile);
		pOpts->StemmingRulesFile.set(m_StemmingRulesFile);
		pOpts->titleSize = m_TitleSize;
		pOpts->XmlIgnoreTags.set(m_XmlIgnoreTags);
		pOpts->maxWordLength = m_MaxWordLength;

		pOpts->SegmentationRulesFile.set(m_SegmentationRulesFile);
		pOpts->TextFieldsFile.set(m_TextFieldsFile);
		pOpts->UserThesaurusFile.set(m_UserThesaurusFile);
		pOpts->HomeDir.set(m_HomeDir);
		pOpts->PrivateDir.set(m_PrivateDir);
		pOpts->BooleanConnectors.set(m_BooleanConnectors);
		pOpts->FileTypeTableFile.set(m_FileTypeTableFile);
		pOpts->BinaryFilterTextChars.set(m_BinaryFilterTextChars);
		pOpts->updateFiles = m_UpdateFiles;
		pOpts->lzwEnableCode = m_LZWEnableCode;
		pOpts->fieldFlags = m_FieldFlags;
		pOpts->textFlags = m_TextFlags;

		pOpts->macroChar = (char) m_MacroChar->Chars[0];
		pOpts->fuzzyChar = (char) m_FuzzyChar->Chars[0];
		pOpts->phonicChar = (char) m_PhonicChar->Chars[0];
		pOpts->stemmingChar = (char) m_StemmingChar->Chars[0];
		pOpts->synonymChar = (char) m_SynonymChar->Chars[0];
		pOpts->weightChar = (char) m_WeightChar->Chars[0];
		pOpts->matchDigitChar = (char) m_MatchDigitChar->Chars[0];
		pOpts->storedFieldDelimiterChar = (char) m_StoredFieldDelimiterChar->Chars[0];

		pOpts->unicodeFilterFlags = m_UnicodeFilterFlags;
		pOpts->unicodeFilterBlockSize = m_UnicodeFilterBlockSize;
		pOpts->unicodeFilterMinTextSize = m_UnicodeFilterMinTextSize;
		pOpts->unicodeFilterWordOverlapAmount = m_UnicodeFilterWordOverlapAmount;

		DStringProperty r;
		r.set(m_UnicodeFilterRanges);
		DStringSet ranges;
		ranges.tokenize(r, " ,;", false);
		memset(&pOpts->unicodeFilterRanges, 0, sizeof pOpts->unicodeFilterRanges);
		for (int i = 0; i < ranges.getCount(); ++i) {
			int iRange = ranges.getInt(i);
			if ((iRange >= 0) && (iRange < 256))
				pOpts->unicodeFilterRanges[iRange] = true;
			}

	pOpts->Save();

	}
}}
