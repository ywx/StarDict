/*
========================================================================
 Name        : StarDictContainer.h
 Author      : YWX
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef STARDICTCONTAINER_H
#define STARDICTCONTAINER_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
#include <aknbutton.h>
#include <stringloader.h>
#include <coecobs.h>
// ]]] end generated region [Generated Includes]

#include <aknsbasicbackgroundcontrolcontext.h>
#include <eiklbo.h>


// [[[ begin [Event Handler Includes]
#include <eikedwob.h>
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknButton;
class CEikGlobalTextEditor;
// ]]] end generated region [Generated Forward Declarations]
class CAknSingleStyleListBox;

/**
 * Container class for StarDictContainer
 * 
 * @class	CStarDictContainer StarDictContainer.h
 */
class CStarDictContainer : public CCoeControl
	,MCoeControlObserver	,MEikEdwinObserver, MEikListBoxObserver	{
public:
	// constructors and destructor
	CStarDictContainer();
	static CStarDictContainer* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CStarDictContainer* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CStarDictContainer();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	void InitializeControlsL();
	void LayoutControls();
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
	void HandleChangeDictionaryButtonStateChanged();
	void SetDictionaryName(const TDesC& aName);
	void SetWordToSearch(const TDesC& aWord);
	void GetWordToSearch(TDes& aDes);
	void UpdateListBox(void);
	void HandleListBoxItemClicked();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknButton* iIChangeDictionaryButton;
	CEikGlobalTextEditor* iISearchTextEditor;
	// ]]] end generated region [Generated Instance Variables]
	CAknSingleStyleListBox* iResultsListBox;
	
	CAknsBasicBackgroundControlContext* iBgContext;
	
	// [[[ begin [Overridden Methods]
protected: 
	void HandleControlEventL( 
			CCoeControl* aControl, 
			TCoeEvent anEventType );
	void HandleEdwinEventL( 
			CEikEdwin* anEdwin, 
			TEdwinEvent anEventType );
	// ]]] end [Overridden Methods]

	//From MEikListBoxObserver
	virtual void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType ); 
	
	// [[[ begin [User Handlers]
protected: 
	void HandleChangeDictionaryButtonStateChangedL( 
			CCoeControl* aControl, 
			TCoeEvent anEvent );
	void SetISearchTextEditorFormattingL();
	void HandleISearchTextEditorTextUpdatedL( 
			CEikEdwin* anEdwin, 
			TEdwinEvent anEventType );
	// ]]] end [User Handlers]
	void HandleListBoxItemClickedL(CEikListBox* aListBox,
			TListBoxEvent anEventType);
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
		EIChangeDictionaryButton,
		EISearchTextEditor,
		
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		EResultsListBox,
		
		ELastControl
		};
	
	// [[[ begin [MCoeControlObserver support]
private: 
	typedef void ( CStarDictContainer::*ControlEventHandler )( 
			CCoeControl* aControl, TCoeEvent anEvent );
	
	void AddControlEventHandlerL( 
			CCoeControl* aControl, 
			TCoeEvent anEvent, 
			ControlEventHandler aHandler );
	
	class TControlEventDispatch 
		{
	public: 
		CCoeControl* src; 
		TCoeEvent event; 
		ControlEventHandler handler;
		};
		
	RArray< TControlEventDispatch > iControlEventDispatch;
	// ]]] end [MCoeControlObserver support]
	
	
	// [[[ begin [MEdwinObserver support]
private: 
	typedef void ( CStarDictContainer::*EdwinEventHandler )( 
			CEikEdwin* anEdwin, 
			TEdwinEvent event );
	
	void AddEdwinEventHandlerL( 
			CEikEdwin* anEdwin, 
			TEdwinEvent anEvent, 
			EdwinEventHandler aHandler );
	
	class TEdwinEventDispatch 
		{
	public: 
		CEikEdwin* src; 
		TEdwinEvent event; 
		EdwinEventHandler handler;
		};
		
	RArray< TEdwinEventDispatch > iEdwinEventDispatch;
	// ]]] end [MEdwinObserver support]
private: 
	typedef void (CStarDictContainer::*ListBoxEventHandler)(CEikListBox *aListBox,
			TListBoxEvent anEvent);

	void AddListBoxEventHandlerL(CEikListBox *aListBox, TListBoxEvent anEvent,
			ListBoxEventHandler aHandler);

	struct TListBoxEventDispatch
		{
		CEikListBox *src;
		TListBoxEvent event;
		ListBoxEventHandler handler;
		};

	RArray<TListBoxEventDispatch> iListBoxEventDispatch;
	
	};
				
#endif // STARDICTCONTAINER_H
