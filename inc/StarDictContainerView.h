/*
========================================================================
 Name        : StarDictContainerView.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef STARDICTCONTAINERVIEW_H
#define STARDICTCONTAINERVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]

#define KMaxSearchWordSize		30
#define KMaxDictionaryNameSize	20
// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CStarDictContainer;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for StarDictContainerView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CStarDictContainerView StarDictContainerView.h
 */						
			
class CStarDictContainerView : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CStarDictContainerView();
	static CStarDictContainerView* NewL();
	static CStarDictContainerView* NewLC();        
	void ConstructL();
	virtual ~CStarDictContainerView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CStarDictContainer* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	void HandleGoMenuItemSelected();
	
	/**
	  Sets the name of the active dictionary.
	  @param aName The name of the new dictionary.
	*/
	void SetDictionaryName(const TDesC& aName);
	void UpdateListBox(void);
	
	// ]]] end [Public Section]
	
	
	// [[[ begin [Protected Section]
protected:
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();
	
	// [[[ begin generated region: do not modify [Overridden Methods]
	// ]]] end generated region [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
	TBool HandleControlPaneRightSoftKeyPressedL( TInt aCommand );
	TBool HandleIChangeDictionaryMenuItemSelectedL( TInt aCommand );
	TBool HandleIGoMenuItemSelectedL( TInt aCommand );
	TBool HandleISettingMenuItemSelectedL( TInt aCommand );
	TBool HandleAboutMenuItemSelectedL( TInt aCommand );
	TBool HandleExitMenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CStarDictContainer* iStarDictContainer;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]

	/** Stores the word to search for. */
	TBuf<KMaxSearchWordSize+1> iWordToSearch;

	/** Stores the name of the active dictionary. */
	TBuf<KMaxDictionaryNameSize> iDictionaryName;
	
	};

#endif // STARDICTCONTAINERVIEW_H
