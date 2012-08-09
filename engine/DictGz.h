/*  This file is a part of Dict. Dict is a Series 60 application for browsing
	dict databases (http:\\www.dict.org) stored in the phone's file system.

	This code has been adapted from dictd-1.9.15
	(ftp://ftp.dict.org/pub/dict/dictd-1.9.15.tar.gz)
	Copyright Rickard E. Faith (faith@dict.org)

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

/** @file DictGz.h
	This code has been adapted from
	<A HREF="ftp://ftp.dict.org/pub/dict/dictd-1.9.15.tar.gz">dictd-1.9.15</A>.
	Copyright Rickard E. Faith (faith@dict.org)
*/

#ifndef _DICTGZ_H_
#define _DICTGZ_H_

#include <e32std.h>
#include <e32def.h>
#include <f32file.h>
#include <zlib.h>
#include "LogMacro.h"

#define DICT_UNKNOWN    0
#define DICT_TEXT       1
#define DICT_GZIP       2
#define DICT_DZIP       3

#define DICT_CACHE_SIZE 	5
#define KFileNameSizeMax	256

/** This structure defines a cache for reading from .dz files.
*/
typedef struct dictCache {
	TInt chunk;
	TUint8 *inBuffer;
	TInt stamp;
	TInt count;
} dictCache;

/** An CDictGzReader object is used for reading from .dz files. The
	implementation is based on dictd-1.9.15.
*/
class CDictGzReader : public CBase
{
public:
	/** Constructor. */
	CDictGzReader();

	/** Destructor. */
	~CDictGzReader();

	/** Constructs a CDictGzReader object.
		@param aFileName The full path and name of the .dz file to open.
		@result A pointer to the created instance of CDictGzReader.
	*/
	static CDictGzReader *NewL(const TDesC& aFileName);

	/** Performs the second phase construction of a CDictFactory object.
		@param aFileName The full path and name of the .dz file to open.
	*/
	void ConstructL(const TDesC& aFileName);

	/** Reads from the compressed .dz file.
		@param aStart The position to start reading from.
		@param aSize Number of bytes to read.
		@param aTextBufferPtr The buffer to place the read bytes.
		@result A result code, 1 for success.*/
	TInt Read(TInt aStart,
			  TInt aSize,
			  TPtr8 &aTextBufferPtr);

	/** Tests if the .dz file opened successfully.
		@result True if the .dz file is open for reading.*/
	TBool IsOpen() {return iOpen;};
	
	TInt GetMeaningTextL(TInt aOffset, TInt aLength, TDesC8& aSameTypeSequence, HBufC **aMeaningText);

	enum {NO_ERROR = 0,
		  ERROR_ASSERT,
		  ERROR_NOT_ENOUGH_MEMORY,
		  ERROR_CANT_READ_FROM_FILE,
		  ERROR_LENGTH_IS_ZERO};
private:

	/** Reads the header of the .dz file.
		@result True if the header is of the correct format.
	*/
	TBool ReadHeaderL();

	/** Closed the zstream.
	*/
	void CloseStream();
	
	void PowerWordParse(TUint8* powerword, TInt& length);

private:
	/** This flag has a value of True when the .dz file is open for reading. */
	TBool iOpen;

	/** The handle of the .dz file. */
	RFile iFile;

	/** The File Session used for file operations. */
	RFs	iFileSession;

	/** Full path and and name of the .dz file. */
	TBuf<KFileNameSizeMax> iFileName;

	/** The name of the database stored in the header of the .dz file. */
	TBuf<KFileNameSizeMax> iOrigFileName;
	
	TInt iSize;
	TInt iType;
	TInt iInitialized;
	TInt iLength;
	TInt iCompressedLength;
	TInt iHeaderLength;
	TUint8 iMethod;
	TUint8 iFlags;
	TInt32 iMTime;
	TUint8 iExtraFlags;
	TUint8 iOs;
	TUint16 iVersion;
	TUint16 iChunkLength;
	TUint16 iChunkCount;
	z_stream zStream;
	CBufFlat *iChunks;
	CBufFlat *iOffsets;	// Sum-scan of chunks
	TUint8* iOutBuffer;
	TInt iStamp;
	dictCache cache[DICT_CACHE_SIZE];

	HBufC16 *iMeaningBuffer;
	LOG_DECLARATION
};


#endif //_DICTGZ_H_
