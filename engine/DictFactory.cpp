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

#include <eikenv.h>
#include <utf.h> // charconv.lib
#include "DictFactory.h"
#include "DictGz.h"
#include "DictIndexSearcher.h"

_LIT( KPath, "E:\\S60Dict\\" );

CDictFactory::CDictFactory()
{
	iCurrentDataBase = -1;

	iDataBaseList = NULL;
	iDictGzReader = NULL;
	iDictIndexSearcher = NULL;
}

CDictFactory::~CDictFactory()
{
	LOGGING_STOP;

	SaveSettings();

	delete iDataBaseList;
	iDataBaseList = NULL;

	delete iDictGzReader;
	iDictGzReader = NULL;
	
	delete iDictIndexSearcher;
	iDictIndexSearcher = NULL;
}

CDictFactory *CDictFactory::NewL()
{
	CDictFactory *self = new (ELeave) CDictFactory;
	CleanupStack::PushL(self);
	self->ConstructL();
   	CleanupStack::Pop(self);
   	return self;
}

void CDictFactory::ConstructL()
{
	LOGGING_START(_L("DictLog"),_L("LogFileDictFactory"));

	iDictIndexSearcher = DictIndexSearcher::NewL();
	
	LoadSettings();
	ConstructDataBaseListL();
}

void CDictFactory::ForAllMatchingFilesL(RFs& aSession, const TDesC& aWildName, const TDesC& aScanDir)
{
	TFindFile file_finder(aSession);
	CDir* file_LITist;
	TInt err = file_finder.FindWildByDir(aWildName, aScanDir, file_LITist);
	while (err==KErrNone)
	{
		LOG_WRITE1(_L("Files %d"), file_LITist->Count());
		TInt i;
		for (i=0; i<file_LITist->Count(); i++)
		{
			TParse fullentry;
			fullentry.Set((*file_LITist)[i].iName,
						  &file_finder.File(),
						  NULL);

			if (fullentry.Name().Length() < KDataBaseNameSizeMax &&
				fullentry.DriveAndPath().Length() < KDataBasePathSizeMax)
			{
				// Check if dict.dz file also exists
				TBuf<KDataBasePathSizeMax + KDataBaseNameSizeMax + 10> dzFileName;
				dzFileName.Append(fullentry.DriveAndPath());
				dzFileName.Append(fullentry.Name());
				dzFileName.Append(_L(".dict.dz"));
				
				TBuf<KDataBasePathSizeMax + KDataBaseNameSizeMax + 10> ifoFileName;
				ifoFileName.Append(fullentry.DriveAndPath());
				ifoFileName.Append(fullentry.Name());
				ifoFileName.Append(_L(".ifo"));
					
				LOG_WRITE(dzFileName);

				RFile file;
				TInt err = file.Open(aSession,
									 dzFileName,
									 EFileRead);
				if (err == KErrNone)
				{
					file.Close();
					TDictDataBase dictDataBase;
					RFileReadStream iFileStream;
					dictDataBase.iName.Copy(fullentry.Name());
					dictDataBase.iDicName.Copy(fullentry.Name());
					if(iFileStream.Open(aSession, ifoFileName, EFileRead) == KErrNone)
						{
						TBuf8<100> infBuf;
						TBuf8<8> iBookName(_L8("bookname"));
						TBuf8<16> iSameTypeSequence(_L8("sametypesequence"));
						while(1)
							{
							TRAPD(eofErr, 
								iFileStream.ReadL(infBuf, TChar('\n'));
								);
							if(!eofErr)
								{
								if(infBuf.FindF(iBookName) != KErrNotFound)
									{
									CnvUtfConverter::ConvertToUnicodeFromUtf8(dictDataBase.iDicName, infBuf.Mid(9,infBuf.Length()-10));
									}
								if(infBuf.FindF(iSameTypeSequence) != KErrNotFound)
									{
									dictDataBase.iSameTypeSequence.Copy(infBuf.Mid(17,infBuf.Length()-18));
									}
								}
							else
								{
								break;
								}
							}
						iFileStream.Close();
						}

					dictDataBase.iPath.Copy(fullentry.DriveAndPath());

					LOG_WRITE(_L("Appended database:"));
					LOG_WRITE(fullentry.Name());
					iDataBaseList->AppendL(dictDataBase);
				}
			}
		}
		delete file_LITist;
		err=file_finder.FindWild(file_LITist);
	}
}

void CDictFactory::ConstructDataBaseListL()
{
	iDataBaseList = new (ELeave) CArrayFixFlat<TDictDataBase>(10);
	iDataBaseList->SetReserveL(10);

	RFs	iFileSession;
	iFileSession = CEikonEnv::Static()->FsSession();

	ForAllMatchingFilesL(iFileSession, _L("*.idx"), KPath);

	if (!iDataBaseList || iDataBaseList->Count() == 0)
	{
		iCurrentDataBase = -1;
		LOG_WRITE(_L("No database found!"));
	}
	else
	{
		LOG_WRITE1(_L("Current database = %d"), iCurrentDataBase);
		if (iCurrentDataBase >= 0 &&
			iCurrentDataBase < iDataBaseList->Count() &&
			OpenDataBase(iCurrentDataBase))
		{
			// Use database from previous run
			LOG_WRITE1(_L("Using database %d"), iCurrentDataBase);
		}
		else
		{
			// Use first database that opens correctly
			iCurrentDataBase = 0;
			while(!OpenDataBase(iCurrentDataBase) &&
				iCurrentDataBase < iDataBaseList->Count())
			{
				iCurrentDataBase++;
			}
		}
	}
}

TBool CDictFactory::OpenDataBase(TInt aIndex)
{
	if (aIndex < 0 || !iDataBaseList || aIndex >= iDataBaseList->Count())
	{
		return EFalse;
	}

	TBuf<KDataBasePathSizeMax + KDataBaseNameSizeMax + 10> fileName;
	fileName.Append((*iDataBaseList)[aIndex].iPath);
	fileName.Append((*iDataBaseList)[aIndex].iName);
	fileName.Append(_L(".dict.dz"));

	delete iDictGzReader;
	iDictGzReader = NULL;
	iDictGzReader = CDictGzReader::NewL(fileName);

	fileName.Delete(0, KDataBasePathSizeMax + KDataBaseNameSizeMax + 10);
	fileName.Append((*iDataBaseList)[aIndex].iPath);
	fileName.Append((*iDataBaseList)[aIndex].iName);
	fileName.Append(_L(".idx"));

	if (iDictIndexSearcher->Open(fileName))
	{
		LOG_WRITE(_L("Opened database:"));
		LOG_WRITE((*iDataBaseList)[aIndex].iName);
		return ETrue;
	}

	return EFalse;
}

TBool CDictFactory::GetDataBaseName(TInt aIndex,
								   TDesC **aName,
								   TDesC **aPath)
{
	if (aIndex < 0 || !iDataBaseList || aIndex >= iDataBaseList->Count())
	{
		return EFalse;
	}

	*aName = &(*iDataBaseList)[aIndex].iDicName;
	*aPath = &(*iDataBaseList)[aIndex].iPath;

	return ETrue;
}

TBool CDictFactory::IsDictionaryPresent()
{
	if (!iDataBaseList)
		return EFalse;

	return iDataBaseList->Count() > 0 && iDictGzReader && iDictIndexSearcher;
}

TInt CDictFactory::GetDataBaseCount()
{
	if (!iDataBaseList)
		return 0;

	return iDataBaseList->Count();
}

TInt CDictFactory::GetCurrentDataBase()
{
	return iCurrentDataBase;
}

TBool CDictFactory::SetCurrentDataBase(TInt aIndex)
{
	if (aIndex >= 0 && aIndex < iDataBaseList->Count())
	{
		if (OpenDataBase(aIndex))
		{
			iCurrentDataBase = aIndex;
			return ETrue;
		}
	}
	return EFalse;
}

TBool CDictFactory::FindIniFilePath()
{
	TBool ok = EFalse;
	TFindFile file_finder(CEikonEnv::Static()->FsSession());
	CDir* file_LITist;
	TInt err = file_finder.FindWildByDir(_L("dict.ini"),
										 _L("\\system\\apps\\S60Dict\\"),
										 file_LITist);
	if (err==KErrNone)
	{
		TParse fullentry;
		fullentry.Set((*file_LITist)[0].iName,
					  &file_finder.File(),
					  NULL);
		if (fullentry.FullName().Length() < KDataBasePathSizeMax)
		{
			iIniFilePathAndName.Copy(fullentry.FullName());
			LOG_WRITE(_L("Ini File"));
			LOG_WRITE(iIniFilePathAndName);
			ok = ETrue;
		}
		delete file_LITist;
	}
	return ok;
}


void CDictFactory::LoadSettings()
{
	RFile file;
	TInt length = 0;

	if (!FindIniFilePath())
	{
		
		CEikonEnv::Static()->FsSession().PrivatePath(iIniFilePathAndName);
		CEikonEnv::Static()->FsSession().MkDirAll(iIniFilePathAndName);
		iIniFilePathAndName.Append(_L("dict.ini"));

		//iIniFilePathAndName.Copy(_L("c:\\system\\apps\\dict\\dict.ini"));			
		LOG_WRITE(_L("Default Ini File"));
		LOG_WRITE(iIniFilePathAndName);
	}

	TInt err = file.Open(CEikonEnv::Static()->FsSession(),
						 iIniFilePathAndName,
						 EFileRead);
	if (err == KErrNone)
	{
		file.Size(length);
	}

	if (length > 0)
	{
		TInt	version;
		TPckg<TInt>	versionPkg(version);
		err = file.Read(versionPkg);
		if (err == KErrNone)
		{
			if (version == 1)
			{
				TPckg<TDictSettings> settingsPkg(iSettings);
				err = file.Read(settingsPkg);
			}
		}
		file.Close();

		iCurrentDataBase = iSettings.iCurrentDataBase;
		if(iSettings.iFontSize > 5)
		  iSettings.iFontSize = 5;
		if(iSettings.iFontSize < -5)
		  iSettings.iFontSize = -5;
		/*
		if (iSettings.iPath1.Length() == 0)
		{
			// Default settings
			iCurrentDataBase = -1;
			iSettings.iPath1.Copy(_L("E:\\S60Dict\\"));
		}
		*/
	}
	else
	{
		if (err == KErrNone)
		{
			file.Close();
		}

		// Default settings
		iCurrentDataBase = -1;
		//iSettings.iPath1.Copy(_L("E:\\S60Dict\\"));
	}
}

void CDictFactory::SaveSettings()
{
	iSettings.iCurrentDataBase = iCurrentDataBase;

	RFile file;
	TInt err = file.Replace(CEikonEnv::Static()->FsSession(),
							iIniFilePathAndName,
							EFileRead);
	if (err != KErrNone)
	{
		err = file.Create(CEikonEnv::Static()->FsSession(),
						  iIniFilePathAndName,
						  EFileRead);
	}
	if (err == KErrNone)
	{
		TInt version = 1;
		TPckg<TInt>	versionPkg(version);
		err = file.Write(versionPkg);
		if (err == KErrNone)
		{
			TPckg<TDictSettings> settingsPkg(iSettings);
			err = file.Write(settingsPkg);
			file.Close();
		}
	}

	if (err != KErrNone)
		User::InfoPrint(_L("Could not save settings"));
}

void CDictFactory::GetPath(TDes& aPath, TInt aMaxLength)
{
	TInt length = 0;
	aPath.Zero();
	_LIT(KSeparator, ";");
	const TInt KSeparatorLength = 1;

	aPath.Append(KPath);
	aPath.Append(KSeparator);

	LOG_WRITE(aPath);
}

TBool CDictFactory::SetPath(const TDesC& aPath)
{
  /*
	TInt start, length;
	_LIT(KSeparator, ";");

	// Path 1
	start = 0;
	length = aPath.Find(KSeparator);
	if (length != KErrNotFound && length < KDataBasePathSizeMax &&
	    length > 0)
	{
		iSettings.iPath1.Copy(aPath.Left(length));
		LOG_WRITE(iSettings.iPath1);
		start = length + 1;
	}
	else
	{
		LOG_WRITE(_L("Invalid path!"));
		return EFalse;
	}
*/
	return ETrue;
}

TInt CDictFactory::FindWord(const TDesC& aWord)
	{
	return iDictIndexSearcher->FindWord(aWord);
	}

void CDictFactory::UpWord(void)
	{
	iDictIndexSearcher->UpWord();
	}
	
void CDictFactory::DownWord(void)
	{
	iDictIndexSearcher->DownWord();
	}

CDesCArrayFlat* CDictFactory::GetHeadList(void)
	{
	return iDictIndexSearcher->GetHeadList();
	}

TInt CDictFactory::GetMeaningTextL(TInt iIndex, HBufC **aMeaningText, HBufC **aTitleText)
	{
	TInt offset;
	TInt length;
	iDictIndexSearcher->GetMeaningPosition(iIndex, offset, length, aTitleText);
	return iDictGzReader->GetMeaningTextL(offset, length, (*iDataBaseList)[iCurrentDataBase].iSameTypeSequence, aMeaningText);
	}

void CDictFactory::SaveFontSize(TInt aFontSize)
{
  iSettings.iFontSize = aFontSize;
}

TInt CDictFactory::GetFontSize(void)
{
  return iSettings.iFontSize;
}

