/*
========================================================================
 Name        : StarDictAppUi.cpp
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <StarDict.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "StarDictAppUi.h"
#include "StarDict.hrh"
#include "StarDictContainer.hrh"
#include "myComRes.hrh"
#include "StarDictContainerView.h"
#include "DictMeaningContainerView.h"
// ]]] end generated region [Generated User Includes]

#include <aknnotewrappers.h>
#include "StarDictApplication.h"
#include <f32file.h>
#include <s32file.h>

#include "DictGz.h"
#include "DictFactory.h"

_LIT( KFileName, "C:\\private\\ED0C820B\\StarDict.txt" );
_LIT( KText, "StarDict for S60V5");
// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CStarDictAppUi instance
 */ 
CStarDictAppUi::CStarDictAppUi()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	iDictFactory = NULL;
}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CStarDictAppUi::~CStarDictAppUi()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	delete iDictFactory;
	iDictFactory = NULL;

}

// [[[ begin generated function: do not modify
void CStarDictAppUi::InitializeContainersL()
	{
	iStarDictContainerView = CStarDictContainerView::NewL();
	AddViewL( iStarDictContainerView );
	SetDefaultViewL( *iStarDictContainerView );
	iDictMeaningContainerView = CDictMeaningContainerView::NewL();
	AddViewL( iDictMeaningContainerView );
	}
// ]]] end generated function

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CStarDictAppUi::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
	// ]]] end generated region [Generated Code]
	
	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CStarDictAppUi::HandleResourceChangeL( TInt aType )
	{
	CAknViewAppUi::HandleResourceChangeL( aType );
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
				
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CStarDictAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
	// The inherited HandleKeyEventL is private and cannot be called
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CStarDictAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
	CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CStarDictAppUi::ConstructL()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	
	//BaseConstructL( EAknEnableSkin  | EAknEnableMSK ); 
	BaseConstructL( EAknEnableSkin );

	iDictFactory = CDictFactory::NewL();
	CleanupStack::PushL(iDictFactory);
	CleanupStack::Pop(iDictFactory);
	InitializeContainersL();
	// ]]] end generated region [Generated Contents]
	

	if (iDictFactory->IsDictionaryPresent())
	{
		TDesC *name, *path;

		name = path = NULL;
		TInt currentDatabase = iDictFactory->GetCurrentDataBase();
		if (iDictFactory->GetDataBaseName(currentDatabase, &name, &path))
		{
			if (name)
			{
				iStarDictContainerView->SetDictionaryName(*name);
			}
		}
	}

	iViewId1 = iStarDictContainerView->Id();
	iViewId2 = iDictMeaningContainerView->Id();

	// Create a file to write the text to
	TInt err = CCoeEnv::Static()->FsSession().MkDirAll( KFileName );
	if( ( KErrNone != err ) && ( KErrAlreadyExists != err ) )
	{
		return;
	}

	RFile file;
	err = file.Replace( CCoeEnv::Static()->FsSession(), KFileName, EFileWrite );
	CleanupClosePushL( file );
	if( KErrNone != err )
	{
		CleanupStack::PopAndDestroy( 1 ); // file
		return;
	}

	RFileWriteStream outputFileStream( file );
	CleanupClosePushL( outputFileStream );
	outputFileStream << KText;

	CleanupStack::PopAndDestroy( 2 ); // outputFileStream, file
}

void CStarDictAppUi::SetActiveMainView()
{
	ActivateLocalViewL(iViewId1);
}

void CStarDictAppUi::SetActiveMeaningView()
{
	ActivateLocalViewL(iViewId2);
}


// CDictMeaningContainer::SetMeaningTextL
// Show dynamic text;
void CStarDictAppUi::ShowItemL(TInt aIndex)
{
	HBufC *meaningText = NULL;
	HBufC *titleText = NULL;

	TInt returnCode = iDictFactory->GetMeaningTextL( aIndex, &meaningText, &titleText );
	if( returnCode == CDictGzReader::NO_ERROR )
	{
		iDictMeaningContainerView->SetMeaningText( meaningText, titleText );
		SetActiveMeaningView();
	}
	else
	{
		TBuf<100> resourceString;

		switch( returnCode )
		{
			case CDictGzReader::ERROR_NOT_ENOUGH_MEMORY :
				CEikonEnv::Static()->ReadResource( resourceString, R_NOT_ENOUGH_MEMORY_STRING );
			break;
			case CDictGzReader::ERROR_CANT_READ_FROM_FILE :
				CEikonEnv::Static()->ReadResource( resourceString, R_CANT_READ_FROM_FILE_STRING );
			break;
			case CDictGzReader::ERROR_LENGTH_IS_ZERO :
				CEikonEnv::Static()->ReadResource( resourceString, R_LENGTH_IS_ZERO_STRING );
			break;
			default :
			case CDictGzReader::ERROR_ASSERT :
				CEikonEnv::Static()->ReadResource( resourceString, R_INTERNAL_ERROR );
			break;
		}
		User::InfoPrint( resourceString );
	}
}


TInt CStarDictAppUi::FindWord(const TDesC& aWord)
{
	if( iDictFactory != NULL && iDictFactory->IsDictionaryPresent() )
	{
		return iDictFactory->FindWord(aWord);
	}
	else
	{
		TBuf<100> resourceString;
		CEikonEnv::Static()->ReadResource(resourceString,
										  R_NO_DICTIONARIES_FOUND_STRING);
		CAknWarningNote* warningNote;
		warningNote = new (ELeave) CAknWarningNote;
		warningNote->ExecuteLD(resourceString);
		return 0;
	}
}


CDesCArrayFlat* CStarDictAppUi::GetHeadList(void)
{
	return iDictFactory->GetHeadList();
}



void CStarDictAppUi::ShowDictionaryListL()
{
	TDesC *name, *path;
	TBuf<KDataBaseNameSizeMax> str;

	CDesCArray *itemTextArray = new (ELeave) CDesCArrayFlat(16);
	CleanupStack::PushL(itemTextArray);

	for(TInt i=0; i<iDictFactory->GetDataBaseCount(); i++)
	{
		name = path = NULL;
		if (iDictFactory->GetDataBaseName(i, &name, &path) && name)
		{
			str.Copy(*name);
		}
		else
		{
			TBuf<100> resourceString;
			CEikonEnv::Static()->ReadResource(resourceString,
											  R_CORRUPT_STRING);
			str.Copy(resourceString);
		}

		itemTextArray->AppendL(str);
	}

	TInt itemSelected(0), prevSelection;
	CAknListQueryDialog* listQuery = new (ELeave) CAknListQueryDialog(&itemSelected);
	listQuery->PrepareLC(R_DICTIONARY_LIST_DIALOG);
	listQuery->SetItemTextArray(itemTextArray);
	listQuery->SetOwnershipType(ELbmDoesNotOwnItemArray);

	prevSelection = iDictFactory->GetCurrentDataBase();
	if (prevSelection >= 0 && prevSelection < itemTextArray->Count())
	{
		listQuery->ListBox()->SetCurrentItemIndex(prevSelection);
	}
	if(listQuery->RunLD())
	{
		iDictFactory->SetCurrentDataBase(itemSelected);
		iStarDictContainerView->SetDictionaryName((*itemTextArray)[itemSelected]);
		iStarDictContainerView->UpdateListBox();
	}
	CleanupStack::PopAndDestroy();
}
