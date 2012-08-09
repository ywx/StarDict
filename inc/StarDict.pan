
#ifndef STARDICT_PAN_H
#define STARDICT_PAN_H

/** StarDict application panic codes */
enum TStarDictPanics
	{
	EStarDictUi = 1
	// add further panics here
	};

inline void Panic(TStarDictPanics aReason)
	{
	_LIT(applicationName,"StarDict");
	User::Panic(applicationName, aReason);
	}

#endif // STARDICT_PAN_H
