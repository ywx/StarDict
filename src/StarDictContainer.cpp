/*
========================================================================
 Name        : StarDictContainer.cpp
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <gdi.h>
#include <eikgted.h>
#include <eikenv.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <txtfrmat.h>
#include <gulcolor.h>
#include <StarDict.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "StarDictContainer.h"
#include "StarDictContainerView.h"
#include "StarDict.hrh"
#include "StarDictContainer.hrh"
#include "myComRes.hrh"
// ]]] end generated region [Generated User Includes]

#include <aknsdrawutils.h>
#include <AknUtils.h>
#include <aknlists.h>

#include "StarDictAppUi.h"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CStarDictContainer::CStarDictContainer()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	iIChangeDictionaryButton = NULL;
	iISearchTextEditor = NULL;
	// ]]] end generated region [Generated Contents]
	iResultsListBox = NULL;
	iBgContext = NULL;

}
/** 
 * Destroy child controls.
 */
CStarDictContainer::~CStarDictContainer()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iIChangeDictionaryButton;
	iIChangeDictionaryButton = NULL;
	iControlEventDispatch.Close();		
	delete iISearchTextEditor;
	iISearchTextEditor = NULL;
	iEdwinEventDispatch.Close();		
	// ]]] end generated region [Generated Contents]
	iListBoxEventDispatch.Close();
	delete iResultsListBox;
	iResultsListBox = NULL;
	
	delete iBgContext;
	iBgContext = NULL;
	
}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CStarDictContainer
 */
CStarDictContainer* CStarDictContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CStarDictContainer* self = CStarDictContainer::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CStarDictContainer
 */
CStarDictContainer* CStarDictContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CStarDictContainer* self = new ( ELeave ) CStarDictContainer();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver );
	return self;
	}
			
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CStarDictContainer::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
{
	if ( aParent == NULL )
	{
		CreateWindowL();
	}
	else
	{
	    SetContainerWindowL( *aParent );
	}
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	InitializeControlsL();

	// Create a control to display a list of messages
	iResultsListBox = new ( ELeave ) CAknSingleStyleListBox;
	iResultsListBox->SetContainerWindowL( *this );
	iResultsListBox->ConstructL( this, 0 );
	
	SetRect( aRect );
	
	iBgContext = CAknsBasicBackgroundControlContext::NewL( KAknsIIDQsnBgAreaMain,aRect,ETrue);
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb labelColor;
	AknsUtils::GetCachedColor(skin, labelColor, KAknsIIDQsnTextColors,
			EAknsCIQsnTextColorsCG6);

	ActivateL();
	// [[[ begin generated region: do not modify [Post-ActivateL initializations]
	SetISearchTextEditorFormattingL();	
	// ]]] end generated region [Post-ActivateL initializations]
	

	CStarDictAppUi * appUi = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	
	CTextListBoxModel* model = iResultsListBox->Model();
	model->SetItemTextArray(appUi->GetHeadList());
	model->SetOwnershipType(ELbmDoesNotOwnItemArray);
	iResultsListBox->HandleItemAdditionL();
	
	iResultsListBox->SetListBoxObserver( this );
	AddListBoxEventHandlerL( 
			iResultsListBox, 
			EEventItemClicked,//EEventEnterKeyPressed, 
			&CStarDictContainer::HandleListBoxItemClickedL );

}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CStarDictContainer::CountComponentControls() const
	{
	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CStarDictContainer::ComponentControl( TInt aIndex ) const
{
	// [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
		case EIChangeDictionaryButton:
			return iIChangeDictionaryButton;
		case EISearchTextEditor:
			return iISearchTextEditor;
		}
	// ]]] end generated region [Generated Contents]
	
	// handle any user controls here...
	switch( aIndex )
	{
		case EResultsListBox :
			return iResultsListBox;
	}

	
	return NULL;
}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CStarDictContainer::SizeChanged()
{
	CCoeControl::SizeChanged();
	LayoutControls();
	// [[[ begin generated region: do not modify [Generated Contents]
			
	// ]]] end generated region [Generated Contents]
	
	if( iBgContext )
	{
		iBgContext->SetRect(Rect());
		if( &Window() )
		{
			iBgContext->SetParentPos(PositionRelativeToScreen());
		}
	}	
}
				
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CStarDictContainer::LayoutControls()
{
	iISearchTextEditor->SetExtent( TPoint( 0, 445 ), TSize( 360, 40 ) );
	iResultsListBox->SetExtent( TPoint ( 0, 40 ) , TSize ( 360, 403 ) );
}
// ]]] end generated function

/**
 *	Handle key events.
 */				
TKeyResponse CStarDictContainer::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	
	// ]]] end generated region [Generated Contents]
	
	if ( iFocusControl != NULL
		&& iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
		{
		return EKeyWasConsumed;
		}
	return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
	}
				
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CStarDictContainer::InitializeControlsL()
	{
	
	iIChangeDictionaryButton = static_cast< CAknButton* >
					( EikControlFactory::CreateByTypeL( EAknCtButton ).iControl );
	iIChangeDictionaryButton->ConstructFromResourceL ( R_STAR_DICT_CONTAINER_I_CHANGE_DICTIONARY_BUTTON );
	
	iIChangeDictionaryButton->SetContainerWindowL ( *this );
	iIChangeDictionaryButton->SetRect ( TRect ( 
								TPoint ( 0, 0 ) , 
								TSize ( 360, 40 ) )  
								 ) ; 
	iIChangeDictionaryButton->SetObserver( ( MCoeControlObserver* ) iCommandObserver ); 
	
	
	
	
	
	
	iIChangeDictionaryButton->SetObserver( this );
	AddControlEventHandlerL( 
			iIChangeDictionaryButton, 
			EEventStateChanged, 
			&CStarDictContainer::HandleChangeDictionaryButtonStateChangedL );
	iISearchTextEditor = new ( ELeave ) CEikGlobalTextEditor;
	iISearchTextEditor->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_STAR_DICT_CONTAINER_I_SEARCH_TEXT_EDITOR );
		iISearchTextEditor->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
		{
		HBufC* text = StringLoader::LoadLC( R_STAR_DICT_CONTAINER_I_SEARCH_TEXT_EDITOR_2 );
		iISearchTextEditor->SetTextL( text );
		CleanupStack::PopAndDestroy( text );
		}
	iISearchTextEditor->AddEdwinObserverL( this );
	AddEdwinEventHandlerL( 
			iISearchTextEditor, 
			EEventTextUpdate, 
			&CStarDictContainer::HandleISearchTextEditorTextUpdatedL );
	
	iISearchTextEditor->SetFocus( ETrue );
	iFocusControl = iISearchTextEditor;
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CStarDictContainer::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( EStarDictContainerViewId ) )->ClientRect() );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 *	Draw container contents.
 */				
void CStarDictContainer::Draw( const TRect& aRect ) const
{
	// [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
	// ]]] end generated region [Generated Contents]
	
	MAknsSkinInstance* skinInstance = AknsUtils::SkinInstance();
	MAknsControlContext* iCtrlContext = AknsDrawUtils::ControlContext( this );
	AknsDrawUtils::Background( skinInstance, iCtrlContext, this, gc, aRect );
	iIChangeDictionaryButton->OverrideColorL(EColorButtonText, KRgbRed);
}

TTypeUid::Ptr CStarDictContainer::MopSupplyObject( TTypeUid aId )
{
	if( iBgContext )
	{
		return MAknsControlContext::SupplyMopObject(aId, iBgContext);
	}

	return CCoeControl::MopSupplyObject(aId);
}

/** 
 * Override of the HandleControlEventL virtual function
 */
void CStarDictContainer::HandleControlEventL( 
		CCoeControl* aControl, 
		TCoeEvent anEventType )
	{
	for (int i = 0; i < iControlEventDispatch.Count(); i++)
		{
		const TControlEventDispatch& currEntry = iControlEventDispatch[i];
		if ( currEntry.src == aControl && currEntry.event == anEventType )
			{
			( this->*currEntry.handler )( aControl, anEventType );
			break;
			}
		}
	}
/** 
 * Helper function to register MCoeControlObserver event handlers
 */
void CStarDictContainer::AddControlEventHandlerL( 
		CCoeControl* aControl, 
		TCoeEvent anEvent, 
		ControlEventHandler aHandler )
	{
	TControlEventDispatch entry;
	entry.src = aControl;
	entry.event = anEvent;
	entry.handler = aHandler;
	TInt err = iControlEventDispatch.Append( entry );
	User::LeaveIfError( err );
	}


void CStarDictContainer::HandleChangeDictionaryButtonStateChanged()
{
	static TInt i = 0;
	//++i;
	TBuf<64> tmpTxt;
	tmpTxt.Format( _L("You Have Select Dict: %d"), ++i );
	iIChangeDictionaryButton->State( 0 )->SetTextL( tmpTxt );
	DrawDeferred();
}


void CStarDictContainer::SetDictionaryName(const TDesC& aName)
{
	if (iIChangeDictionaryButton)
	{
		if ( aName.Length() > 0)
			iIChangeDictionaryButton->State( 0 )->SetTextL( aName );
		else
			iIChangeDictionaryButton->State( 0 )->SetTextL(_L("Click to Select Dictionary"));
		DrawDeferred();
	}
}

/** 
 * Handle the EEventStateChanged event.
 */
void CStarDictContainer::HandleChangeDictionaryButtonStateChangedL( 
		CCoeControl* /* aControl */, 
		TCoeEvent /* anEvent */ )
{
	// TODO: implement stateChanged event handler
	//HandleChangeDictionaryButtonStateChanged();
	CStarDictAppUi * appui = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	appui->ShowDictionaryListL();
}
				
void CStarDictContainer::SetISearchTextEditorFormattingL()
	{
	// [[[ begin generated region: do not modify [Generated GlobalText formatting]
	
	// ]]] end generated region [Generated GlobalText formatting]
	
	}


void CStarDictContainer::SetWordToSearch(const TDesC& aWord)
{
	if (iISearchTextEditor)
	{
		iISearchTextEditor->SetTextL(&aWord);
		//iISearchTextEditor->ClearSelectionL();
		//iISearchTextEditor->SetCursorPosL( 0, EFalse );
		DrawDeferred();
	}
}


void CStarDictContainer::GetWordToSearch(TDes& aDes)
{
	iISearchTextEditor->GetText(aDes);
}


void CStarDictContainer::UpdateListBox(void)
{
	TBuf<30> word;
	TInt iSelected;
	CStarDictAppUi * appUi = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	iISearchTextEditor->GetText(word);
	//CEikonEnv::Static()->InfoWinL(_L("iISearchTextEditor::GetText()"), word);
	iSelected = appUi->FindWord(word);
	//TBuf<50> msg;
	//msg.Format(_L("FindWord(): %d\n"), iSelected);
	//msg.Append(word);
	//CEikonEnv::Static()->InfoWinL(_L("appUi->FindWord(word)"), msg);
	
	CTextListBoxModel* model = iResultsListBox->Model();
	model->SetItemTextArray(appUi->GetHeadList());
	model->SetOwnershipType(ELbmDoesNotOwnItemArray);

	if (iResultsListBox->Model()->NumberOfItems() > 0)
	{
		iResultsListBox->HandleItemAdditionL();
		iResultsListBox->SetCurrentItemIndex(iSelected);
	}
	DrawDeferred();
}
/** 
 * Override of the HandleEdwinEventL virtual function
 */
void CStarDictContainer::HandleEdwinEventL( 
		CEikEdwin* anEdwin, 
		TEdwinEvent anEventType )
{
	for (int i = 0; i < iEdwinEventDispatch.Count(); i++)
	{
		const TEdwinEventDispatch& currEntry = iEdwinEventDispatch[i];
		if ( currEntry.src == anEdwin && currEntry.event == anEventType )
		{
			( this->*currEntry.handler )( anEdwin, anEventType );
			break;
		}
	}
//	switch( anEventType )
//	{
//		case EEventTextUpdate :
//		{
//			UpdateListBox();
//			break;
//		}
//		default :
//		break;
//	}
}


/** 
 * Helper function to register MEikEdwinObserver event handlers
 */
void CStarDictContainer::AddEdwinEventHandlerL( 
		CEikEdwin* anEdwin, 
		TEdwinEvent anEvent, 
		EdwinEventHandler aHandler )
	{
	TEdwinEventDispatch entry;
	entry.src = anEdwin;
	entry.event = anEvent;
	entry.handler = aHandler;
	TInt err = iEdwinEventDispatch.Append( entry );
	User::LeaveIfError( err );
	}
/** 
 * Handle the EEventTextUpdate event.
 */
void CStarDictContainer::HandleISearchTextEditorTextUpdatedL( 
		CEikEdwin* /* anEdwin */, 
		TEdwinEvent /* anEventType */ )
{
	// TODO: implement textUpdated (FEP only) event handler
	UpdateListBox();
}


/** 
 * Override of the HandleListBoxEventL virtual function
 */
void CStarDictContainer::HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType )
{
	for( int i = 0; i < iListBoxEventDispatch.Count(); i++ )
	{
		const TListBoxEventDispatch& currEntry = iListBoxEventDispatch[i];
		if( currEntry.src == aListBox && currEntry.event == aEventType )
		{
			( this->*currEntry.handler )( aListBox, aEventType );
			break;
		}
	}
//	if (aEventType == EEventPenDownOnItem) 
//	{ 
//	//action to take when the stylus is set down on a list item; 
//	} 
//	else if (aEventType == EEventItemClicked) 
//	{ 
//	//action to take when the stylus is set down on a list item and lifted; 
//	} 
//	else if (aEventType == EEventItemDoubleClicked) 
//	{ 
//	//action to take when a focused list item is tapped; 
//	} 
//	else if (aEventType == EEventEnterKeyPressed) 
//	{          
//	//action to take when the selection key is pressed on the keypad; 
//		switch( iResultsListBox->CurrentItemIndex() )
//		{
//			case 1 :
//				// Handle keypress event here¡­
//
//			break;
//			default :
//				// Do default handling here¡­
//
//			break;
//		}
//	}         
//	else if (aEventType == EEventItemDraggingActioned) 
//	{ 
//	//action to take when a list item is dragged with the stylus; 
//	} 
//	else 
//	{ 
//	//do something; 
//	}       
}

/** 
 * Helper function to register MEikListBoxObserver event handlers
 */
void CStarDictContainer::AddListBoxEventHandlerL(CEikListBox *aListBox, TListBoxEvent anEvent,
        ListBoxEventHandler aHandler)
{
	TListBoxEventDispatch entry;
	entry.src = aListBox;
	entry.event = anEvent;
	entry.handler = aHandler;
	iListBoxEventDispatch.AppendL( entry );
}

void CStarDictContainer::HandleListBoxItemClicked()
{
	TInt iCurSel = iResultsListBox->CurrentItemIndex();

	CStarDictAppUi * appui = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	if( iCurSel >= 0 )
	{
		appui->ShowItemL( iCurSel );
	}
}

/** 
 * Handle the EEventEnterKeyPressed event.
 */
void CStarDictContainer::HandleListBoxItemClickedL(CEikListBox* /* aListBox */, TListBoxEvent /* anEventType */)
{
	HandleListBoxItemClicked();
}
