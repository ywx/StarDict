/*
========================================================================
 Name        : DictMeaningContainer.cpp
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
#include <eikrted.h>
#include <txtrich.h>
#include <gulcolor.h>
#include <StarDict.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "DictMeaningContainer.h"
#include "DictMeaningContainerView.h"
#include "StarDict.hrh"
#include "StarDictContainer.hrh"
#include "myComRes.hrh"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CDictMeaningContainer::CDictMeaningContainer()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iIDictMeaningText = NULL;
	// ]]] end generated region [Generated Contents]
	
	}
/** 
 * Destroy child controls.
 */
CDictMeaningContainer::~CDictMeaningContainer()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iIDictMeaningText;
	iIDictMeaningText = NULL;
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CDictMeaningContainer
 */
CDictMeaningContainer* CDictMeaningContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CDictMeaningContainer* self = CDictMeaningContainer::NewLC( 
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
 * @return new instance of CDictMeaningContainer
 */
CDictMeaningContainer* CDictMeaningContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CDictMeaningContainer* self = new ( ELeave ) CDictMeaningContainer();
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
void CDictMeaningContainer::ConstructL( 
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
	SetRect( aRect );
	ActivateL();
	// [[[ begin generated region: do not modify [Post-ActivateL initializations]
	SetIDictMeaningTextFormattingL();	
	// ]]] end generated region [Post-ActivateL initializations]
	
	}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CDictMeaningContainer::CountComponentControls() const
	{
	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CDictMeaningContainer::ComponentControl( TInt aIndex ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
		case EIDictMeaningText:
			return iIDictMeaningText;
		}
	// ]]] end generated region [Generated Contents]
	
	// handle any user controls here...
	
	return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CDictMeaningContainer::SizeChanged()
	{
	CCoeControl::SizeChanged();
	LayoutControls();
	// [[[ begin generated region: do not modify [Generated Contents]
			
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CDictMeaningContainer::LayoutControls()
	{
	iIDictMeaningText->SetExtent( TPoint( 0, 0 ), TSize( 340, 485 ) );
	}
// ]]] end generated function

/**
 *	Handle key events.
 */				
TKeyResponse CDictMeaningContainer::OfferKeyEventL( 
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
void CDictMeaningContainer::InitializeControlsL()
	{
	iIDictMeaningText = new ( ELeave ) CEikRichTextEditor;
	iIDictMeaningText->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_DICT_MEANING_CONTAINER_I_DICT_MEANING_TEXT );
		iIDictMeaningText->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
		{
		HBufC* text = StringLoader::LoadLC( R_DICT_MEANING_CONTAINER_I_DICT_MEANING_TEXT_2 );
		iIDictMeaningText->SetTextL( text );
		CleanupStack::PopAndDestroy( text );
		}
	
	iIDictMeaningText->SetFocus( ETrue );
	iFocusControl = iIDictMeaningText;
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CDictMeaningContainer::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( EDictMeaningContainerViewId ) )->ClientRect() );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 *	Draw container contents.
 */				
void CDictMeaningContainer::Draw( const TRect& aRect ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
	// ]]] end generated region [Generated Contents]
	
	}
				
void CDictMeaningContainer::SetIDictMeaningTextFormattingL()
	{
	// [[[ begin generated region: do not modify [Generated GlobalText formatting]
	
	// ]]] end generated region [Generated GlobalText formatting]
	
	}

// ---------------------------------------------------------
// CDictMeaningContainer::SetMeaningTextL
// Show dynamic text;
// ---------------------------------------------------------
//
void CDictMeaningContainer::SetMeaningText( const TDesC * aMeaningText, const TDesC * aTitleText )
{
	//iIDictMeaningText->SetTextL(aMeaningText);
	//SizeChanged();
	//DrawDeferred();
	if( iIDictMeaningText )
	{
		TInt aPos;
		CRichText *richText = iIDictMeaningText->RichText();
		TCharFormat charFormat;
		TCharFormatMask charFormatMask;
		const CFont* editorFont=iEikonEnv->AnnotationFont();
		charFormat.iFontSpec=editorFont->FontSpecInTwips();
		
		aPos = richText->DocumentLength();

		if( aTitleText )
		{
			richText->InsertL( aPos, aTitleText->Right( aTitleText->Length() - 1 ) );
			richText->AppendParagraphL();
			
			charFormatMask.ClearAll();
			charFormatMask.SetAttrib(EAttColor);
			charFormatMask.SetAttrib(EAttFontTypeface);
			charFormatMask.SetAttrib(EAttFontStrokeWeight);
			charFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
			richText->ApplyCharFormatL(charFormat, charFormatMask, aPos, aTitleText->Length());
			
			aPos = richText->DocumentLength();
		}

		if( aMeaningText )
		{
			richText->InsertL( aPos, *aMeaningText );

			richText->AppendParagraphL();
			charFormatMask.ClearAll();
			charFormatMask.SetAttrib( EAttColor );
			charFormatMask.SetAttrib( EAttFontTypeface );
			charFormatMask.SetAttrib( EAttFontStrokeWeight );
			charFormatMask.SetAttrib( EAttFontHeight );
			//charFormat.iFontSpec.iHeight += iFontSize * 20;
			charFormat.iFontSpec.iHeight *= 12 / 10;
			charFormat.iFontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightNormal );
			richText->ApplyCharFormatL( charFormat, charFormatMask, aPos, aMeaningText->Length() );
		}
		
		//iIDictMeaningText->ClearSelectionL();
		//iIDictMeaningText->SetCursorPosL( 0, EFalse );
		//DrawDeferred();
		iIDictMeaningText->HandleTextChangedL();
	}
}
