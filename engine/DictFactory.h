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

#ifndef _DICT_FACTORY_H_
#define _DICT_FACTORY_H_

#include <e32std.h>
#include <e32def.h>

#include "LogMacro.h"

class CDictEngine;
class CDictGzReader;
class DictIndexSearcher;

#define KDataBaseNameSizeMax		32
#define KDataBasePathSizeMax		80
#define KListBoxSize  6
#define KMeaningInitialSize 200

/** This class represents a dict database.*/
class TDictDataBase
{
public:
	/** The name of the dict database. */
	TBuf<KDataBaseNameSizeMax> iName;

	/** The path where the .dz and .index files are stored. */
	TBuf<KDataBasePathSizeMax> iPath;
	
	TBuf<KDataBaseNameSizeMax> iDicName;
	
	TBuf8<4>	iSameTypeSequence;
};

/** TDictSettings define the user's settings. A single instance of this
	class is read from/written to a ini file. CDictFactory is initialised
	with these settings.
*/
class TDictSettings
{
public:
	/** The number of the database to use.*/
	TInt iCurrentDataBase;

	/** First path location to look for dict databases. */
	TInt iFontSize;

	/** Second path location to look for dict databases. */
	//TBuf<KDataBasePathSizeMax> iPath2;

	/** Third path location to look for dict databases. */
	//TBuf<KDataBasePathSizeMax> iPath3;

	/** Fourth path location to look for dict databases. */
	//TBuf<KDataBasePathSizeMax> iPath4;
};

/** CDictFactory is responsible for locating, loading and switching
	between dict databases.
*/
class CDictFactory : public CBase
{
public:
	/** Constructor. */
	CDictFactory();

	/** Destructor. */
	~CDictFactory();

	/** Constructs a CDictFactory object.
		@result A pointer to the created instance of CDictFactory.
	*/
	static CDictFactory *NewL();

	/** Performs the second phase construction of a CDictFactory object.
	*/
	void ConstructL();

	/** Tests if dict databases were found.
		@result Returns true if one or more dict databases were found.
	*/
	TBool IsDictionaryPresent();

	/** Retrieves the number of databases present in the system.
		@result Number of dict databases.
	*/
	TInt GetDataBaseCount();

	/** Gets the name and path for a given database.
		@param aIndex The index of the database. Must be lower than
					  GetDataBaseCount().
		@param aName A pointer to a pointer to the name of the database.
		@param aPath A pointer to a pointer to the path of the database.
		@result True if the operation was successful.
	*/
	TBool GetDataBaseName(TInt aIndex,
						  TDesC **aName,
						  TDesC **aPath);

	/** Gets the index of the active database.
		@result The index of the active database.
	*/
	TInt GetCurrentDataBase();

	/** Switches to a new database.
		@param aIndex The index of the new database.
		@result True if the operation was successful.
	*/
	TBool SetCurrentDataBase(TInt aIndex);

	/** Gets the full path, where database files are looked for.
		@param aPath The full path, where database files are looked for.
		@param aMaxLength The maximum number of characters to write on aPath
						string reference.
	*/
	void GetPath(TDes& aPath, TInt aMaxLength);

	/** Sets the full path, where database files are looked for.
		@param aPath The full path, where database files are looked for.
		@result True if the operation was successful.
	*/
	TBool SetPath(const TDesC& aPath);
	
	TInt FindWord(const TDesC& aWord);

	void UpWord(void);
		
	void DownWord(void);
	
	CDesCArrayFlat* GetHeadList(void);
	
	TInt GetMeaningTextL(TInt iIndex, HBufC **aMeaningText, HBufC **aTitleText);
	
	void SaveFontSize(TInt aFontSize);
	
	TInt GetFontSize(void);
private:

	/** Constucts the iDataBaseList list. It searches for database files
		at the directories contained in the path.
	*/
	void ConstructDataBaseListL();

	/** Searches for dict database files in a specific directory. The databases
		found are placed in the iDataBaseList list.
		@param aSession The file session.
		@param aWildName The name with a wild character to look for.
		@param aScanDir The scan directory.
	*/
	void ForAllMatchingFilesL(RFs& aSession,
							  const TDesC& aWildName,
							  const TDesC& aScanDir);

	/** Opens a database.
		@param aIndex The index of the database. Must be lower than
					  GetDataBaseCount().
		@result True if the operation was successful.
	*/
	TBool OpenDataBase(TInt aIndex);

	/** Finds the directory where the ini file is stored. This directory
		can be located at either the phone's memory or at the memory card,
		depending on where the application was installed.
		@result True if the ini file directory is found.
	*/
	TBool FindIniFilePath();

	/** Loads the settings from the ini file.
	*/
	void LoadSettings();

	/** Stores the settings in the ini file.
	*/
	void SaveSettings();


private:

	/** A pointer to the instance of CDictGzReader used by iDictEngine.
		The class is responsible for creating and destroying this object.
	*/
	CDictGzReader *iDictGzReader;

	/** A pointer to the instance of DictIndexSearcher used by iDictEngine.
		The class is responsible for creating and destroying this object.
	*/
	DictIndexSearcher *iDictIndexSearcher;

	/** Number of directories contained in the path. */
	static const TInt KPathListSize = 4;

	/** User's settings. They are read from/written to a ini file. */
	TDictSettings iSettings;

	/** A list of the databases present in the system. */
	CArrayFixFlat<TDictDataBase> *iDataBaseList;

	/** The index of the active database. */
	TInt iCurrentDataBase;

	/** The path of the directory where the ini file is stored.*/
	TBuf<KDataBasePathSizeMax> iIniFilePathAndName;

	LOG_DECLARATION
};


#endif //_DICT_FACTORY_H_
