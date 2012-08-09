/*  This file is a part of Dict. Dict is a Series 60 application for browsing
	dict databases (http:\\www.dict.org) stored in the phone's file system.
	Copyright (C) 2005, Kostas Giannakakis
	Web page: http://users.forthnet.gr/ath/kgiannak/

	Dict is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Dict is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dict; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//#include <collate.h >
#include <eikenv.h>
#include <utf.h> // charconv.lib
#include "DictIndexSearcher.h"
#include "DictFactory.h"

DictIndexSearcher::DictIndexSearcher()
{
	iIsOpen = EFalse;
	iHeadwordList = NULL;
	iOffsetList = NULL;
	iLengthList = NULL;
	iTitleBuffer = NULL;
}

DictIndexSearcher::~DictIndexSearcher()
{
	if (iIsOpen)
	{
		iFile.Close();
		iFilePos.Close();
		iIsOpen = EFalse;
	}
	
	delete iHeadwordList;
	iHeadwordList = NULL;
	delete iOffsetList;
	iOffsetList = NULL;
	delete iLengthList;
	iLengthList = NULL;
	
	delete iTitleBuffer;
	iTitleBuffer = NULL;
		
	LOGGING_STOP;
}

DictIndexSearcher *DictIndexSearcher::NewL()
{
DictIndexSearcher *self = new (ELeave) DictIndexSearcher;
	CleanupStack::PushL(self);
	self->ConstructL();
   	CleanupStack::Pop(self);
   	return self;
}

void DictIndexSearcher::ConstructL()
{
	LOGGING_START(_L("DictLog"),_L("LogFileDictEngine"));
	iHeadwordList = new (ELeave) CDesCArrayFlat(KListBoxSize);
	iOffsetList = new (ELeave) CDesC8ArrayFlat(KListBoxSize);
	iLengthList = new (ELeave) CDesC8ArrayFlat(KListBoxSize);
}

TBool DictIndexSearcher::Open(const TDesC& aFileName)
{
	LOGGING_START(_L("DictLog"), _L("LogFileDictSearcher"));
	
	iFile.Close();
	iFilePos.Close();
	iFileSession = CEikonEnv::Static()->FsSession();
	TInt err = iFile.Open(iFileSession,
						  aFileName,
						  EFileRead);
	if (err != KErrNone)
	{
		iIsOpen = EFalse;
		return EFalse;
	}
	
	if (iFile.Size(iFileSize) != KErrNone)
	{
		iIsOpen = EFalse;
		return EFalse;
	}
	
	iIsOpen = ETrue;
	
	TParse fullentry;
	fullentry.Set(aFileName, NULL, NULL);
	TBuf<120> posFileName;
	posFileName.Append(fullentry.DriveAndPath());
	posFileName.Append(fullentry.Name());
	posFileName.Append(_L(".pos"));
	err = iFilePos.Open(iFileSession, posFileName, EFileRead);
	if(err != KErrNone)
		{
		iFile.Close();
		RFileWriteStream iFilePosStream;
		RFileReadStream iFileStream;
		iFilePosStream.Create(iFileSession, posFileName, EFileWrite);
		iFileStream.Open(iFileSession, aFileName, EFileRead);
		TInt pos = 0;
		TBuf8<300> word;
		TBuf8<8> offset;
		
		while(pos < iFileSize)
			{
			//TBuf8<4> numBuf((TUint8 *)pos);
			//numBuf.AppendNum(pos);
			iFilePosStream.WriteInt32L(pos);
			iFileStream.ReadL(word, TChar(0));
			pos = pos + word.Length();
			iFileStream.ReadL(offset, 8);
			pos = pos + 8;
			}
		iFileStream.Close();
		iFilePosStream.Close();
		err = iFilePos.Open(iFileSession, posFileName, EFileRead);
		err = iFile.Open(iFileSession, aFileName, EFileRead);
		}
	iCurrentPos = 0;
	iFilePos.Size(iMaxPos);
	iMaxPos = iMaxPos/4 - 1;
	//iSupportList->Reset();
	
	
	return ETrue;
}


TInt DictIndexSearcher::FindWord(const TDesC& aSearchWord)
{
	if (!iIsOpen || aSearchWord.Length() <= 0)
		return EFalse;

	TBuf8<KSearchWordSizeMax> searchWord;//(aSearchWord);
	TBuf8<10> wordByte;
	TBuf8<KIndexEntryWordSizeMax> word;
	TBuf<KIndexEntryWordSizeMax> wordL;
	CnvUtfConverter::ConvertFromUnicodeToUtf8(searchWord, aSearchWord);
	searchWord.LowerCase();

	TInt result = -1;
	TInt wordsFoundCount = 0;
	TInt pos=0;
	TBuf8<4> offset;
	TBuf8<4> length;
	
	//TInt t;
	
	
	TInt iBegin = -1;
	TInt iEnd = iMaxPos;
	TInt iCurrent;
	TInt iCurrent4;
	TBuf8<4> indexPos;
	TInt indexOffset;
	TBuf8<KIndexEntryWordSizeMax> word1;
	
	while(iEnd-iBegin > 1)
	{
		iCurrent = (iBegin+iEnd)/2;
		iCurrent4 = iCurrent*4;
		iFilePos.Seek(ESeekStart, iCurrent4);
		iFilePos.Read(indexPos, 4);
		indexOffset = indexPos[3]*256*256*256 + indexPos[2]*256*256 + indexPos[1]*256 + indexPos[0];
		iFile.Seek(ESeekStart, indexOffset);
		iFile.Read(word, 20);
		word.LowerCase();
		if(searchWord.Compare(word) < 0)
		{
			iEnd = iCurrent;
		}
		else
		{
			iBegin = iCurrent;
		}
	}
	iCurrentPos = iEnd;
	
	iHeadwordList->Reset();
	iOffsetList->Reset();
	iLengthList->Reset();
	
	if(iCurrentPos+KListBoxSize >= iMaxPos)
	{
		wordsFoundCount = iCurrentPos+KListBoxSize - iMaxPos - 1;
		iCurrentPos = iMaxPos - KListBoxSize + 1;
	}
	
	//iFile.Seek(ESeekStart, pos);
	for(int i = iCurrentPos; i < iCurrentPos+KListBoxSize; i++)
	{
		iCurrent4 = i*4;
		iFilePos.Seek(ESeekStart, iCurrent4);
		iFilePos.Read(indexPos, 4);
		indexOffset = indexPos[3]*256*256*256 + indexPos[2]*256*256 + indexPos[1]*256 + indexPos[0];
		iFile.Seek(ESeekStart, indexOffset);
		iFile.Read(word, 200);
		
		TInt wordLength = word.Locate(TChar(0));
		offset = word.Mid(wordLength+1, 4);
		length = word.Mid(wordLength+5, 4);

		CnvUtfConverter::ConvertToUnicodeFromUtf8(wordL, word.Mid(0,wordLength));
		HBufC* buffer = HBufC::NewL(wordL.Length()+1);
		buffer->Des().Append('\t');
		buffer->Des().Append(wordL);
		iHeadwordList->AppendL(*buffer);
		delete buffer;
		
		iOffsetList->AppendL(offset);
		iLengthList->AppendL(length);
	}
	//iFileStream.Close();

	return wordsFoundCount;
}

void DictIndexSearcher::UpWord()
{
	TInt iCurrent4;
	TBuf8<4> indexPos;
	TInt indexOffset;
	TBuf8<KIndexEntryWordSizeMax> word;
	TBuf<KIndexEntryWordSizeMax> wordL;
	TBuf8<4> offset;
	TBuf8<4> length;
		
	if(iCurrentPos>0)
	{
		iCurrentPos--;
		iCurrent4 = iCurrentPos*4;
		iFilePos.Seek(ESeekStart, iCurrent4);
		iFilePos.Read(indexPos, 4);
		indexOffset = indexPos[3]*256*256*256 + indexPos[2]*256*256 + indexPos[1]*256 + indexPos[0];
		iFile.Seek(ESeekStart, indexOffset);
		iFile.Read(word, 200);
				
		TInt wordLength = word.Locate(TChar(0));
		offset = word.Mid(wordLength+1, 4);
		length = word.Mid(wordLength+5, 4);

		CnvUtfConverter::ConvertToUnicodeFromUtf8(wordL, word.Mid(0,wordLength));
		iHeadwordList->Delete(KListBoxSize-1);
		iOffsetList->Delete(KListBoxSize-1);
		iLengthList->Delete(KListBoxSize-1);
		HBufC* buffer = HBufC::NewL(wordL.Length()+1);
		buffer->Des().Append('\t');
		buffer->Des().Append(wordL);
		iHeadwordList->InsertL(0, *buffer);
		delete buffer;
		iOffsetList->InsertL(0, offset);
		iLengthList->InsertL(0, length);
	}
}

void DictIndexSearcher::DownWord()
	{
	TInt iCurrent4;
	TBuf8<4> indexPos;
	TInt indexOffset;
	TBuf8<KIndexEntryWordSizeMax> word;
	TBuf<KIndexEntryWordSizeMax> wordL;
	TBuf8<4> offset;
	TBuf8<4> length;
		
	if(iCurrentPos<iMaxPos-KListBoxSize)
		{
		iCurrentPos++;
		iCurrent4 = (iCurrentPos+KListBoxSize-1)*4;
		iFilePos.Seek(ESeekStart, iCurrent4);
		iFilePos.Read(indexPos, 4);
		indexOffset = indexPos[3]*256*256*256 + indexPos[2]*256*256 + indexPos[1]*256 + indexPos[0];
		iFile.Seek(ESeekStart, indexOffset);
		iFile.Read(word, 200);
				
		TInt wordLength = word.Locate(TChar(0));
		offset = word.Mid(wordLength+1, 4);
		length = word.Mid(wordLength+5, 4);
	
		CnvUtfConverter::ConvertToUnicodeFromUtf8(wordL, word.Mid(0,wordLength));
		iHeadwordList->Delete(0);
		iOffsetList->Delete(0);
		iLengthList->Delete(0);
		HBufC* buffer = HBufC::NewL(wordL.Length()+1);
		buffer->Des().Append('\t');
		buffer->Des().Append(wordL);
		iHeadwordList->AppendL(*buffer);
		delete buffer;
		iOffsetList->AppendL(offset);
		iLengthList->AppendL(length);
		}
	}

CDesCArrayFlat* DictIndexSearcher::GetHeadList(void)
	{
	return iHeadwordList;
	}

void DictIndexSearcher::GetMeaningPosition(const TInt aIndex, TInt& aOffset, TInt& aLength, HBufC **aTitleText)
{
	TBuf8<4> offset;
	TBuf8<4> length;
	TInt titleLength;
	
	if (iTitleBuffer == NULL)
	{
	iTitleBuffer = HBufC::NewL(200);
	}
	
	offset = iOffsetList->operator [](aIndex);
	length = iLengthList->operator [](aIndex);
	
	aOffset = offset[0]*256*256*256 + offset[1]*256*256 + offset[2]*256 + offset[3];
	aLength = length[0]*256*256*256 + length[1]*256*256 + length[2]*256 + length[3];

	iTitleBuffer->Des().Copy(iHeadwordList->operator [](aIndex));
	*aTitleText = iTitleBuffer;
}



