/*
========================================================================
 Name        : DictMeaningContainerView.cpp
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

#include "StarDictAppUi.h"
// [[[ begin generated region: do not modify [Generated User Includes]

#include "StarDict.hrh"
#include "DictMeaningContainerView.h"
#include "StarDictContainer.hrh"
#include "myComRes.hrh"
#include "DictMeaningContainer.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CDictMeaningContainerView::CDictMeaningContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iDictMeaningContainer = NULL;
	// ]]] end generated region [Generated Contents]
	iMeaningText = NULL;
	iTitleText = NULL;
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CDictMeaningContainerView::~CDictMeaningContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iDictMeaningContainer;
	iDictMeaningContainer = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CDictMeaningContainerView
 */
CDictMeaningContainerView* CDictMeaningContainerView::NewL()
	{
	CDictMeaningContainerView* self = CDictMeaningContainerView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CDictMeaningContainerView
 */
CDictMeaningContainerView* CDictMeaningContainerView::NewLC()
	{
	CDictMeaningContainerView* self = new ( ELeave ) CDictMeaningContainerView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CDictMeaningContainerView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_DICT_MEANING_CONTAINER_DICT_MEANING_CONTAINER_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CDictMeaningContainerView::Id() const
	{
	return TUid::Uid( EDictMeaningContainerViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CDictMeaningContainerView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case EAknSoftkeyBack:
			commandHandled = HandleIControlPaneRightSoftKeyPressedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyBack )
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
void CDictMeaningContainerView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iDictMeaningContainer == NULL )
	{
		iDictMeaningContainer = CreateContainerL();
		iDictMeaningContainer->SetMopParent( this );
		AppUi()->AddToStackL( *this, iDictMeaningContainer );
		iDictMeaningContainer->SetMeaningText( iMeaningText, iTitleText );
	} 
	// ]]] end generated region [Generated Contents]
	
	}

/**
 */
void CDictMeaningContainerView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iDictMeaningContainer != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iDictMeaningContainer );
		delete iDictMeaningContainer;
		iDictMeaningContainer = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CDictMeaningContainerView::HandleStatusPaneSizeChange()
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
void CDictMeaningContainerView::SetupStatusPaneL()
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
		iEikonEnv->CreateResourceReaderLC( reader, R_DICT_MEANING_CONTAINER_I_TITLE_RESOURCE );
		iTitle->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CDictMeaningContainerView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CDictMeaningContainerView::HandleIControlPaneRightSoftKeyPressedL( TInt aCommand )
{
	// TODO: implement rightSoftKeyPressed event handler
	CStarDictAppUi * appui = static_cast<CStarDictAppUi*> ( CEikonEnv::Static()->AppUi() );
	appui->SetActiveMainView();

	return ETrue;;
}
				
/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CDictMeaningContainer::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CDictMeaningContainer
 */
CDictMeaningContainer* CDictMeaningContainerView::CreateContainerL()
	{
	return CDictMeaningContainer::NewL( ClientRect(), NULL, this );
	}
				
// ---------------------------------------------------------
// CDictMeaningContainer::SetMeaningText
// Show dynamic text;
// ---------------------------------------------------------
//
void CDictMeaningContainerView::SetMeaningText( const TDesC * aMeaningText, const TDesC * aTitleText )
{
	iMeaningText = aMeaningText;
	iTitleText = aTitleText;
}