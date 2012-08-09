/*
========================================================================
 Name        : StarDictContainerView.cpp
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <gdi.h>
#include <eikgted.h>
#include <eikenv.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <StarDict.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "StarDict.hrh"
#include "StarDictContainerView.h"
#include "StarDictContainer.hrh"
#include "myComRes.hrh"
#include "StarDictContainer.h"
// ]]] end generated region [Generated User Includes]

#include "StarDictAppUi.h"
#include <aknmessagequerydialog.h>

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CStarDictContainerView::CStarDictContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iStarDictContainer = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CStarDictContainerView::~CStarDictContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iStarDictContainer;
	iStarDictContainer = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CStarDictContainerView
 */
CStarDictContainerView* CStarDictContainerView::NewL()
	{
	CStarDictContainerView* self = CStarDictContainerView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CStarDictContainerView
 */
CStarDictContainerView* CStarDictContainerView::NewLC()
	{
	CStarDictContainerView* self = new ( ELeave ) CStarDictContainerView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CStarDictContainerView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_STAR_DICT_CONTAINER_STAR_DICT_CONTAINER_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CStarDictContainerView::Id() const
	{
	return TUid::Uid( EStarDictContainerViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CStarDictContainerView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case EStarDictContainerViewIControlPaneRightId:
			commandHandled = HandleControlPaneRightSoftKeyPressedL( aCommand );
			break;
		case EStarDictContainerViewIChangeDictionaryMenuItemCommand:
			commandHandled = HandleIChangeDictionaryMenuItemSelectedL( aCommand );
			break;
		case EStarDictContainerViewIGoMenuItemCommand:
			commandHandled = HandleIGoMenuItemSelectedL( aCommand );
			break;
		case EStarDictContainerViewISettingMenuItemCommand:
			commandHandled = HandleISettingMenuItemSelectedL( aCommand );
			break;
		case EStarDictContainerViewIAboutMenuItemCommand:
			commandHandled = HandleAboutMenuItemSelectedL( aCommand );
			break;
		case EStarDictContainerViewIExitMenuItemCommand:
			commandHandled = HandleExitMenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EStarDictContainerViewIControlPaneRightId )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CStarDictContainerView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iStarDictContainer == NULL )
	{
		iStarDictContainer = CreateContainerL();
		iStarDictContainer->SetMopParent( this );
		AppUi()->AddToStackL( *this, iStarDictContainer );
		iStarDictContainer->SetDictionaryName( iDictionaryName );
		iStarDictContainer->SetWordToSearch( iWordToSearch );
	} 
	// ]]] end generated region [Generated Contents]
	
}

/**
 */
void CStarDictContainerView::DoDeactivate()
{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iStarDictContainer != NULL )
	{
		iStarDictContainer->GetWordToSearch(iWordToSearch);
		AppUi()->RemoveFromViewStack( *this, iStarDictContainer );
		delete iStarDictContainer;
		iStarDictContainer = NULL;
	}
	// ]]] end generated region [Generated Contents]
	
}

/** 
 * Handle status pane size change for this view (override)
 */
void CStarDictContainerView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

// [[[ begin generated function: do not modify
void CStarDictContainerView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* iTitle = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_STAR_DICT_CONTAINER_I_TITLE_RESOURCE );
		iTitle->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CStarDictContainerView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CStarDictContainer::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CStarDictContainer
 */
CStarDictContainer* CStarDictContainerView::CreateContainerL()
	{
	return CStarDictContainer::NewL( ClientRect(), NULL, this );
	}

/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleControlPaneRightSoftKeyPressedL( TInt aCommand )
	{
	// TODO: implement rightSoftKeyPressed event handler
	//return EFalse;
	HandleIGoMenuItemSelectedL( aCommand );
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleIChangeDictionaryMenuItemSelectedL( TInt aCommand )
	{
	// TODO: implement selected event handler
	CStarDictAppUi * appui = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	appui->ShowDictionaryListL();
	
	//iStarDictContainer->HandleChangeDictionaryButtonStateChanged();
	return ETrue;
	}


void CStarDictContainerView::SetDictionaryName(const TDesC &aName)
{
	iDictionaryName.Copy(aName.Left(KMaxDictionaryNameSize));
	if ( NULL != iStarDictContainer )
	{
		iStarDictContainer->SetDictionaryName(iDictionaryName);
	}
}


void CStarDictContainerView::HandleGoMenuItemSelected()
{
	iStarDictContainer->HandleListBoxItemClicked();
}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleIGoMenuItemSelectedL( TInt aCommand )
	{
	// TODO: implement selected event handler
	HandleGoMenuItemSelected();
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleISettingMenuItemSelectedL( TInt aCommand )
	{
	// TODO: implement selected event handler
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleAboutMenuItemSelectedL( TInt aCommand )
	{
	// TODO: implement selected event handler
	CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
	dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
	HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
	dlg->QueryHeading()->SetTextL(*title);
	CleanupStack::PopAndDestroy(); //title
	HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
	dlg->SetMessageTextL(*msg);
	CleanupStack::PopAndDestroy(); //msg
	dlg->RunLD();

	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CStarDictContainerView::HandleExitMenuItemSelectedL( TInt aCommand )
{
	// TODO: implement selected event handler
	AppUi()->HandleCommandL(EEikCmdExit);
	return ETrue;
}
				
				
void CStarDictContainerView::UpdateListBox(void)
{
	if ( NULL != iStarDictContainer )
	{
		iStarDictContainer->UpdateListBox();
	}
}