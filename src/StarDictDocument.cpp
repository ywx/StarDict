/*
========================================================================
 Name        : StarDictDocument.cpp
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "StarDictDocument.h"
#include "StarDictAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CStarDictDocument::CStarDictDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CStarDictDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CStarDictDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CStarDictDocument
 */
CStarDictDocument* CStarDictDocument::NewL( CEikApplication& aApp )
	{
	CStarDictDocument* self = new ( ELeave ) CStarDictDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CStarDictDocument::CreateAppUiL()
	{
	return new ( ELeave ) CStarDictAppUi;
	}
				
