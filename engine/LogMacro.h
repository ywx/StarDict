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

#ifndef __LOG_MACRO_H__
#define __LOG_MACRO_H__

/** @file LogMacro.h
	Macro definitions for easily adding logging capabilities to a class.
*/

#define _LOGGING_ENABLED

#ifdef LOGGING_ENABLED

#include <flogger.h>

#define LOG_DECLARATION				;//RFileLogger iLog;

#define LOGGING_START(app, file)	;//iLog.Connect();	\
									iLog.CreateLog(app,	\
												   file,	\
												   EFileLoggingModeOverwrite); \
									iLog.SetDateAndTime(EFalse, EFalse); \
									iLog.WriteFormat(_L("Created log file"));

#define LOGGING_STOP				//iLog.CloseLog();	\
									iLog.Close();

#define LOG_WRITE(str)				RDebug::Print(str);//			iLog.WriteFormat(str);
#define LOG_WRITE1(str, param)			RDebug::Print(str, param);//		iLog.WriteFormat(str, param);
#define LOG_WRITE2(str, p1, p2)			RDebug::Print(str, p1, p2);//		iLog.WriteFormat(str, p1, p2);
#define LOG_WRITE3(str, p1, p2, p3)		RDebug::Print(str, p1, p2, p3);//		iLog.WriteFormat(str, p1, p2, p3);
#define LOG_WRITE4(str, p1, p2, p3, p4)		RDebug::Print(str, p1, p2, p3, p4);//	iLog.WriteFormat(str, p1, p2, p3, p4);
#define LOG_WRITE5(str, p1, p2, p3, p4, p5)	RDebug::Print(str, p1, p2, p3, p4, p5);//	iLog.WriteFormat(str, p1, p2, p3, p4, p5);
#define LOG_WRITE6(str, p1, p2, p3, p4, p5, p6)	RDebug::Print(str, p1, p2, p3, p4, p5, p6);//iLog.WriteFormat(str, p1, p2, p3, p4, p5, p6);

#else

#define LOG_DECLARATION
#define LOGGING_START(app, file)	;
#define LOGGING_STOP				;
#define LOG_WRITE(str)				;
#define LOG_WRITE1(str, param)		;
#define LOG_WRITE2(str, p1, p2)		;
#define LOG_WRITE3(str, p1, p2, p3)					;
#define LOG_WRITE4(str, p1, p2, p3, p4)				;
#define LOG_WRITE5(str, p1, p2, p3, p4, p5)			;
#define LOG_WRITE6(str, p1, p2, p3, p4, p5, p6)		;

#endif	//LOGGING_ENABLED

#endif // __LOG_MACRO_H__

