#include "stdafx.h"

#include "Options.h"
#include "Constants.h"

using namespace System;

namespace dtSearch { namespace Engine
{
	Options::Options ()
	{	CountCreate(otOptions);
		DHeapPtr<DOptions> pOpts = new DOptions;
		AlphabetFile = pOpts->AlphabetFile.allocNetString();
		BinaryFiles = (dtSearch::Engine::BinaryFilesSettings) pOpts->binaryFiles;
		Hyphens = (dtSearch::Engine::HyphenSettings) pOpts->hyphens;
		IndexNumbers = (pOpts->indexNumbers != FALSE);
		MaxStoredFieldSize = pOpts->maxStoredFieldSize;
		MaxFieldNesting = pOpts->maxFieldNesting;
		MaxWordsToRetrieve = pOpts->maxWordsToRetrieve;
		NoiseWordFile =  (pOpts->NoiseWordFile.allocNetString());
		StemmingRulesFile =  (pOpts->StemmingRulesFile.allocNetString());
		TitleSize = pOpts->titleSize;
		XmlIgnoreTags =  (pOpts->XmlIgnoreTags.allocNetString());
		MaxWordLength = pOpts->maxWordLength;

		SegmentationRulesFile =  (pOpts->SegmentationRulesFile.allocNetString());
		TextFieldsFile =  (pOpts->TextFieldsFile.allocNetString());
		UserThesaurusFile =  (pOpts->UserThesaurusFile.allocNetString());
		HomeDir =  (pOpts->HomeDir.allocNetString());
		PrivateDir =  (pOpts->PrivateDir.allocNetString());
		TempFileDir = (pOpts->TempFileDir.allocNetString());
		BooleanConnectors =  (pOpts->BooleanConnectors.allocNetString());
		FileTypeTableFile =  (pOpts->FileTypeTableFile.allocNetString());
		BinaryFilterTextChars = (pOpts->BinaryFilterTextChars.allocNetString());

		UpdateFiles = (pOpts->updateFiles != FALSE);

		FieldFlags = (dtSearch::Engine::FieldFlags) pOpts->fieldFlags;
		TextFlags = (dtSearch::Engine::TextFlags) pOpts->textFlags;

		MacroChar = Char::ToString (pOpts->macroChar);
		FuzzyChar = Char::ToString (pOpts->fuzzyChar);
		PhonicChar = Char::ToString (pOpts->phonicChar);
		StemmingChar = Char::ToString (pOpts->stemmingChar);
		SynonymChar = Char::ToString (pOpts->synonymChar);
		WeightChar = Char::ToString (pOpts->weightChar);
		MatchDigitChar = Char::ToString (pOpts->matchDigitChar);
		StoredFieldDelimiterChar = Char::ToString(pOpts->storedFieldDelimiterChar);

		UnicodeFilterFlags = (dtSearch::Engine::UnicodeFilterFlags) pOpts->unicodeFilterFlags;
		UnicodeFilterBlockSize = pOpts->unicodeFilterBlockSize;
		UnicodeFilterMinTextSize = pOpts->unicodeFilterMinTextSize;
		UnicodeFilterWordOverlapAmount = pOpts->unicodeFilterWordOverlapAmount;

		DString ranges;
		for (int i = 0; i < 256; ++i) {
			if (pOpts->unicodeFilterRanges[i])
				ranges << i << " ";
			}
		UnicodeFilterRanges =  (ranges.allocNetString());
	}

	Options::~Options ()
	{	CountDelete(otOptions);
	}

	void Options::Save ()
	{
		DHeapPtr<DOptions> pOpts = new DOptions;
		pOpts->AlphabetFile.set(AlphabetFile);
		pOpts->binaryFiles = (long) BinaryFiles;
		pOpts->hyphens = (long) Hyphens;
		pOpts->indexNumbers = IndexNumbers;
		pOpts->maxStoredFieldSize = MaxStoredFieldSize;
		pOpts->maxFieldNesting = MaxFieldNesting;
		pOpts->maxWordsToRetrieve = MaxWordsToRetrieve;
		pOpts->NoiseWordFile.set(NoiseWordFile);
		pOpts->StemmingRulesFile.set(StemmingRulesFile);
		pOpts->titleSize = TitleSize;
		pOpts->XmlIgnoreTags.set(XmlIgnoreTags);
		pOpts->maxWordLength = MaxWordLength;

		pOpts->SegmentationRulesFile.set(SegmentationRulesFile);
		pOpts->TextFieldsFile.set(TextFieldsFile);
		pOpts->UserThesaurusFile.set(UserThesaurusFile);
		pOpts->HomeDir.set(HomeDir);
		pOpts->PrivateDir.set(PrivateDir);
		pOpts->TempFileDir.set(TempFileDir);
		pOpts->BooleanConnectors.set(BooleanConnectors);
		pOpts->FileTypeTableFile.set(FileTypeTableFile);
		pOpts->BinaryFilterTextChars.set(BinaryFilterTextChars);
		pOpts->updateFiles = UpdateFiles;
		pOpts->fieldFlags = (long) FieldFlags;
		pOpts->textFlags = (long) TextFlags;

		pOpts->macroChar = (char) MacroChar[0];
		pOpts->fuzzyChar = (char) FuzzyChar[0];
		pOpts->phonicChar = (char) PhonicChar[0];
		pOpts->stemmingChar = (char) StemmingChar[0];
		pOpts->synonymChar = (char) SynonymChar[0];
		pOpts->weightChar = (char) WeightChar[0];
		pOpts->matchDigitChar = (char) MatchDigitChar[0];
		pOpts->storedFieldDelimiterChar = (char) StoredFieldDelimiterChar[0];

		pOpts->unicodeFilterFlags = (long) UnicodeFilterFlags;
		pOpts->unicodeFilterBlockSize = UnicodeFilterBlockSize;
		pOpts->unicodeFilterMinTextSize = UnicodeFilterMinTextSize;
		pOpts->unicodeFilterWordOverlapAmount = UnicodeFilterWordOverlapAmount;

		DStringProperty r;
		r.set(UnicodeFilterRanges);
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
