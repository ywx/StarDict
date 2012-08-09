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

#ifndef _DICT_INDEX_SEARCHER_H_
#define _DICT_INDEX_SEARCHER_H_

#include <e32std.h>
#include <e32def.h>
#include <s32file.h>
#include "LogMacro.h"


/** A DictIndexSearcher object is used for searching a dict index file.*/
class DictIndexSearcher : public CBase
{
public:
	/** Constructor. */
	DictIndexSearcher();

	/** Destructor. */
	~DictIndexSearcher();

	/** Constructs a CDictEngine object.
		@result A pointer to the created instance of CDictEngine.
	*/
	static DictIndexSearcher *NewL();

	/** Performs the second phase construction of a CDictEngine object.
	*/
	void ConstructL();
	
	/** Opens a dict index file.
		@param aFileName The full path and name of the file to open.
		@result True if the file opened successfully.*/
	TBool Open(const TDesC& aFileName);

	/** Searches for a word in the index file.
		@param aSearchWord The word to search for.
		@param aPos Position at the file, where the searching stopped.
				OUT parameter.
		@result Number of entries found.
		*/
	TInt FindWord(const TDesC& aSearchWord);

	void UpWord(void);
	
	void DownWord(void);
	
	CDesCArrayFlat* GetHeadList(void);
	
	void GetMeaningPosition(const TInt aIndex, TInt& aOffset, TInt& aLength, HBufC **aTitleText);

	/** Gets an entry to a specific location.
		@param aPos Position of the entry. When the method exits, it is
				updated with the position of the next entry in the file.
				IN/OUT parameter.
		@param aWord The headword of the entry. OUT parameter.
		@param aDzFileOffset The offset of the entry. OUT parameter.
		@param aDzFileLength The length of the entry. OUT parameter.
		@result True if the operation was successful.
	*/
	/*
	TBool GetEntryAt(TInt& aPos,
					 TDes& aWord,
					 TDes8& aDzFileOffset,
					 TDes8& aDzFileLength);
					 */
public:
	/** Maximum size of any headword. */
	static const TInt KIndexEntryWordSizeMax = 200;

private:
	/** Retrieves the index entry at a specific position. The entry is taken from
		the line at which the given position belongs to. The function updates the
		iLastEntryWord variable.
		@param aPos The position at which an entry is looked for.
		@result A DictIndexEntry struct object, with the length and the offset of
				the entry.
	*/
	//DictIndexEntry GetIndexEntryAt(TInt aPos);

private:
	/** Maximum size of word to search for. */
	static const TInt KSearchWordSizeMax = 20;
	
	/** File handle for random access of the index file. */
	RFile iFile;
	
	RFile iFilePos;

	/** File session*/
	RFs	iFileSession;

	/** Boolean variable that is True if the index file is open.*/
	TBool iIsOpen;

	/** Index file size.*/
	TInt iFileSize;
	
	TInt iCurrentPos;
	
	TInt iMaxPos;

	/** Array for storing the found headwords.*/
	CDesCArrayFlat* iHeadwordList;

	/** Array for storing the offsets of the found headwords. The offset is
		used by GetMeaningTextL to retrieve the meaning text.*/
	CDesC8ArrayFlat* iOffsetList;

	/** Array for storing the lengths of the found headwords. The length is
		used by GetMeaningTextL to retrieve the meaning text.*/
	CDesC8ArrayFlat* iLengthList;
	
	HBufC16 *iTitleBuffer;
	
	LOG_DECLARATION
};


#endif //_DICT_INDEX_SEARCHER_H_
