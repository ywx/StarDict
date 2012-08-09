/*
========================================================================
 Name        : StarDictAppUi.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef STARDICTAPPUI_H
#define STARDICTAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CStarDictContainerView;
class CDictMeaningContainerView;
// ]]] end generated region [Generated Forward Declarations]
class CDictFactory;

/**
 * @class	CStarDictAppUi StarDictAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CStarDictAppUi : public CAknViewAppUi
	{
public: 
	// constructor and destructor
	CStarDictAppUi();
	virtual ~CStarDictAppUi();
	void ConstructL();

public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	void SetActiveMainView();
	void SetActiveMeaningView();

	/**
	 * Show Meaning text message on screen
	 */
	void ShowItemL(TInt aIndex);

	/**
	  Initiates the searching of a word.
	  @param aWord The word to search for.
	  */
	TInt FindWord(const TDesC& aWord);

	CDesCArrayFlat* GetHeadList(void);
        
        /**
          Displays a list of the available dictionaries. Allows the user
          to change the active dictionary.
          */
        void ShowDictionaryListL();
    
	/** A pointer to the object that constructs and switches between
	    Dict Engine objects.*/
	CDictFactory *iDictFactory;
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CStarDictContainerView* iStarDictContainerView;
	CDictMeaningContainerView* iDictMeaningContainerView;
	// ]]] end generated region [Generated Instance Variables]
	
	TUid	iViewId1;
	TUid	iViewId2;
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
	};

#endif // STARDICTAPPUI_H			
