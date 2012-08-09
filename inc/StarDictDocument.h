/*
========================================================================
 Name        : StarDictDocument.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef STARDICTDOCUMENT_H
#define STARDICTDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CStarDictDocument StarDictDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CStarDictDocument : public CAknDocument
	{
public: 
	// constructor
	static CStarDictDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CStarDictDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // STARDICTDOCUMENT_H
