/*  This file is a part of Dict. Dict is a Series 60 application for browsing
	dict databases (http:\\www.dict.org) stored in the phone's file system.
	Copyright (C) 2005, Kostas Giannakakis
	Web page: http://users.forthnet.gr/ath/kgiannak/

	Dict is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Dict is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dict; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _DICT_TEXT_DECORATOR_H_
#define _DICT_TEXT_DECORATOR_H_

#include <e32std.h>
#include <e32def.h>

/** Class for decorating meaning text to be displayed nicer on the phone's
	screen. In a future version it could be replaced by decorator objects,
	different for every dict database.
*/
class DictTextDecorator
{
public:
	/** Decorates the meaning text.
		@param aText The text to decorate.
	*/
	inline static void Decorate(TDes& aText)
	{
		_LIT(KTextToFind, "  ");
		TInt pos, length;

		pos = aText.Find(KTextToFind);

		while(pos != KErrNotFound)
		{
			length = 2;
			if (pos + length < aText.Length() &&
				aText[pos + length] == ' ')
			{
				length++;
			}
			aText.Replace(pos, length, _L(" "));
			pos = aText.Find(KTextToFind);
		}
	};
};


#endif //_DICT_TEXT_DECORATOR_H_