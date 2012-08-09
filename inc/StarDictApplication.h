/*
========================================================================
 Name        : StarDictApplication.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef STARDICTAPPLICATION_H
#define STARDICTAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidStarDictApplication = { 0xED0C820B };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CStarDictApplication StarDictApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CStarDictApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // STARDICTAPPLICATION_H		
