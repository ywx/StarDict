/*
========================================================================
 Name        : DictMeaningContainerView.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef DICTMEANINGCONTAINERVIEW_H
#define DICTMEANINGCONTAINERVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CDictMeaningContainer;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for DictMeaningContainerView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CDictMeaningContainerView DictMeaningContainerView.h
 */						
			
class CDictMeaningContainerView : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CDictMeaningContainerView();
	static CDictMeaningContainerView* NewL();
	static CDictMeaningContainerView* NewLC();        
	void ConstructL();
	virtual ~CDictMeaningContainerView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CDictMeaningContainer* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Public Section]
	
	// New functions
	/**
	 * Show Meaning text message on screen
	 */
	void SetMeaningText(const TDesC * aMeaningText,const TDesC * aTitleText);
	
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
	TBool HandleIControlPaneRightSoftKeyPressedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CDictMeaningContainer* iDictMeaningContainer;
	// ]]] end generated region [Generated Instance Variables]
	const TDesC * iMeaningText;
	const TDesC * iTitleText;
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
	};

#endif // DICTMEANINGCONTAINERVIEW_H
