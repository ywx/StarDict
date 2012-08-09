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

#include <eikenv.h>
#include <utf.h> // charconv.lib
#include "DictGz.h"
#include "dictzip.h"
#include "DictFactory.h"
#include "DictTextDecorator.h"

#define GET_CHUNK(i) (*((TUint16 *) (iChunks->Ptr((i) << 1).Ptr())))
#define GET_OFFSET(i) (*((TInt *) (iOffsets->Ptr((i) << 2).Ptr())))

CDictGzReader::CDictGzReader()
{
	iOpen = EFalse;
	iStamp = 0;

	iChunks = NULL;
	iOffsets = NULL;
	iOutBuffer = NULL;
	iMeaningBuffer = NULL;

	for (TInt j = 0; j < DICT_CACHE_SIZE; j++)
	{
		cache[j].inBuffer = NULL;
	}
}

CDictGzReader::~CDictGzReader()
{
	CloseStream();
	delete iMeaningBuffer;
	iMeaningBuffer = NULL;
	delete iChunks;
	delete iOffsets;
	delete [] iOutBuffer;

	for (TInt j = 0; j < DICT_CACHE_SIZE; j++)
	{
		delete [] cache[j].inBuffer;
	}

	if (iOpen)
	{
		iFile.Close();
	}

	LOGGING_STOP;
}

CDictGzReader *CDictGzReader::NewL(const TDesC& aFileName)
{
	CDictGzReader *self = new (ELeave) CDictGzReader;
	CleanupStack::PushL(self);
	self->ConstructL(aFileName);
   	CleanupStack::Pop(self);
   	return self;
}

void CDictGzReader::ConstructL(const TDesC& aFileName)
{
	LOGGING_START(_L("DictLog"),_L("LogFileDictGz"));

	iFileSession = CEikonEnv::Static()->FsSession();
	TInt length = aFileName.Length();

	if (length == 0 || length > KFileNameSizeMax)
	{
		LOG_WRITE(_L("Invalid file name!"));
		return;
	}

	iFileName.Copy(aFileName);
	iInitialized = 0;

	if (ReadHeaderL())
	{
		TInt err = iFile.Open(iFileSession,
							  aFileName,
							  EFileRead);
		if (err != KErrNone)
		{
			return;
		}
		iOpen = ETrue;
		for (TInt j = 0; j < DICT_CACHE_SIZE; j++)
		{
			cache[j].chunk = -1;
			cache[j].stamp = -1;
			cache[j].inBuffer = new (ELeave) TUint8[IN_BUFFER_SIZE];
			cache[j].count = 0;
		}
		iOutBuffer = new (ELeave) TUint8[OUT_BUFFER_SIZE];
		LOG_WRITE(_L("File open for reading!"));
	}
}

int CDictGzReader::ReadHeaderL()
{
	TUint8 id1, id2, si1, si2;
	TUint16 extraLength, subLength;
	TInt i, aPos;
	TBuf8<4> aBuf;

	// Open file
	TInt err = iFile.Open(iFileSession,
						  iFileName,
						  EFileRead);
	if (err != KErrNone)
	{
		LOG_WRITE(_L("Can't open file!\n"));
		return EFalse;
	}

	// Read file's size
	err = iFile.Size(iSize);
	if (err != KErrNone)
	{
		return EFalse;
	}

	iHeaderLength = GZ_XLEN - 1;
	err = iFile.Read(aBuf, 2);
	id1 = aBuf[0];
	id2 = aBuf[1];

	LOG_WRITE2(_L("id1 %02X, id2 %02X"), id1, id2);

	if (id1 != GZ_MAGIC1 || id2 != GZ_MAGIC2)
	{
		iType = DICT_TEXT;
		iCompressedLength = iLength = iSize;
		iOrigFileName.Copy(iFileName);
		iFile.Close();
		return ETrue;
	}
	iType = DICT_GZIP;

	err = iFile.Read(aBuf, 2);
	iMethod = aBuf[0];
	iFlags = aBuf[1];
	err = iFile.Read(aBuf, 4);
	iMTime = *((TInt32 *) aBuf.Ptr());
	err = iFile.Read(aBuf, 2);
	iExtraFlags = aBuf[0];
	iOs = aBuf[1];

	LOG_WRITE2(_L("method %02X, iFlags %02X"), iMethod, iFlags);
	LOG_WRITE1(_L("mtime %u"), iMTime);
	LOG_WRITE2(_L("extra flags %02X, os %02X"), iExtraFlags, iOs);

	if (iFlags & GZ_FEXTRA)
	{
		err = iFile.Read(aBuf, 2);
		extraLength = *((TUint16 *) aBuf.Ptr());
		iHeaderLength += extraLength + 2;

		LOG_WRITE2(_L("Extra length %d, Header length %d"), extraLength, iHeaderLength);

		err = iFile.Read(aBuf, 2);
		si1 = aBuf[0];
		si2 = aBuf[1];

		if (si1 == GZ_RND_S1 && si2 == GZ_RND_S2)
		{
			err = iFile.Read(aBuf, 2);
			subLength = *((TUint16 *) aBuf.Ptr());
			err = iFile.Read(aBuf, 2);
			iVersion = *((TUint16 *) aBuf.Ptr());

			LOG_WRITE2(_L("Sublength %d, Version %d"), subLength, iVersion);

			if (iVersion != 1)
			{
				iFile.Close();
				return EFalse;
			}
			err = iFile.Read(aBuf, 2);
			iChunkLength = *((TUint16 *) aBuf.Ptr());
			err = iFile.Read(aBuf, 2);
			iChunkCount = *((TUint16 *) aBuf.Ptr());
			LOG_WRITE2(_L("Chunk Length %u, Chunk Count %u"),
					  (iChunkLength & 0xFFFF), iChunkCount);
			if (iChunkCount <= 0)
			{
				iFile.Close();
				return EFalse;
			}

			iChunks = CBufFlat::NewL(iChunkCount * 2);
			iChunks->SetReserveL(iChunkCount * 2);
			for (i=0, aPos=0; i<iChunkCount; i++)
			{
				err = iFile.Read(aBuf, 2);
				iChunks->InsertL(aPos, ((TUint16 *) aBuf.Ptr()), 2);
				aPos+=2;
			}
			iType = DICT_DZIP;
		}
		else
		{
			iFile.Seek(ESeekStart, iHeaderLength);
		}
	}

	if (iFlags & GZ_FNAME)
	{
		i = 0;
		err = iFile.Read(aBuf, 1);
		while (err == KErrNone && aBuf[0] != 0x00)
		{
			iOrigFileName.Append( ((TChar) aBuf[0]));
			i++;
			if (i > KFileNameSizeMax - 2)
			{
				iFile.Close();
				return EFalse;
			}
			err = iFile.Read(aBuf, 1);
		}
		LOG_WRITE(iOrigFileName);
		iHeaderLength += i + 1;
		LOG_WRITE1(_L("Header length %d"), iHeaderLength);
	}
	else
	{
		iOrigFileName.Copy(_L(""));
	}

	if (iFlags & GZ_COMMENT)
	{
		i = 0;
		err = iFile.Read(aBuf, 1);
		while (err == KErrNone && aBuf[0] != 0x00)
		{
			i++;
			err = iFile.Read(aBuf, 1);
		}
		// Ignore comment
		iHeaderLength += i + 1;
	}

	if (iFlags & GZ_FHCRC)
	{
		err = iFile.Read(aBuf, 2);
		iHeaderLength += 2;
	}

	aPos = 0;
	iFile.Seek(ESeekCurrent, aPos);
	if (aPos != iHeaderLength + 1)
	{
		LOG_WRITE2(_L("File position (%u) != header length + 1 (%u)\n"),
				   aPos,
				   iHeaderLength + 1);
		iFile.Close();
		return EFalse;
	}
	aPos = -8;
	err = iFile.Seek(ESeekEnd , aPos);
	err = iFile.Read(aBuf, 4);	// crc
	err = iFile.Read(aBuf, 4);
	iLength = *((TInt32 *) aBuf.Ptr());
	aPos = 0;
	iFile.Seek(ESeekCurrent, aPos);
	iCompressedLength = aPos;
	LOG_WRITE1(_L("Compressed Length %u"), iCompressedLength);

	// Compute offsets
	iOffsets = CBufFlat::NewL(iChunkCount * sizeof(TInt));
	iOffsets->SetReserveL(iChunkCount * sizeof(TInt));
	for(TInt aOffset = iHeaderLength + 1, i=0, aPos = 0;
		i < iChunkCount;
		i++)
	{
		iOffsets->InsertL(aPos ,&aOffset, sizeof(TInt));
		aPos += sizeof(TInt);
		aOffset +=  GET_CHUNK(i);
	}

	iFile.Close();
	return ETrue;
}


TInt CDictGzReader::Read(TInt aStart,
		 				 TInt aSize,
		 				 TPtr8 &aTextBufferPtr)
{
	TInt i, j;
	TInt err;
	TInt aEnd = aStart + aSize;
	TInt aFirstChunk, aLastChunk;
	TInt aFirstOffset, aLastOffset;
	TInt aFound, aTarget, aLastStamp;
	TInt aCount;
	TUint8 *aInBuffer = NULL;

	switch (iType)
	{
		case DICT_GZIP:
			LOG_WRITE(_L("Cannot seek on pure gzip format files."));
			return 0;
			break;
		case DICT_TEXT:
			err = iFile.Seek(ESeekStart, aStart);
			iFile.Read(aTextBufferPtr, aSize);
			break;
		case DICT_DZIP:
			if (!iInitialized)
			{
				++iInitialized;
				zStream.zalloc    = NULL;
				zStream.zfree     = NULL;
				zStream.opaque    = NULL;
				zStream.next_in   = 0;
				zStream.avail_in  = 0;
				zStream.next_out  = NULL;
				zStream.avail_out = 0;
				if (inflateInit2( &zStream, -15 ) != Z_OK)
				{
					LOG_WRITE1(_L("Cannot initialize inflation engine: %s"),
						   zStream.msg );
					return 0;
				}
			}
			aFirstChunk  = aStart / iChunkLength;
			aFirstOffset = aStart - aFirstChunk * iChunkLength;
			aLastChunk   = aEnd / iChunkLength;
			aLastOffset  = aEnd - aLastChunk * iChunkLength;
			LOG_WRITE2(_L("start = %d, end = %d"), aStart, aEnd);
			LOG_WRITE2(_L("firstChunk = %d, firstOffset = %d"), aFirstChunk, aFirstOffset);
			LOG_WRITE2(_L("lastChunk = %d, lastOffset = %d"), aLastChunk, aLastOffset);
			for (i = aFirstChunk; i <= aLastChunk; i++)
			{
				// Access cache
				aFound  = 0;
				aTarget = 0;
				aLastStamp = 0xFFFFFFFF;//INT_MAX;
				for (j = 0; j < DICT_CACHE_SIZE; j++)
				{
					if (cache[j].chunk == i)
					{
						aFound  = 1;
						aTarget = j;
						break;
					}
					if (cache[j].stamp < aLastStamp)
					{
						aLastStamp = cache[j].stamp;
						aTarget = j;
					}
				}
				cache[aTarget].stamp = ++iStamp;
				if (aFound)
				{
					LOG_WRITE1(_L("Using cache %d"), aTarget);
					aCount = cache[aTarget].count;
					aInBuffer = cache[aTarget].inBuffer;
				}
				else
				{
					cache[aTarget].chunk = i;
					aInBuffer = cache[aTarget].inBuffer;

					if (GET_CHUNK(i) >= OUT_BUFFER_SIZE )
					{
						LOG_WRITE3(_L("chunks[%d] = %d >= %ld (OUT_BUFFER_SIZE)"),
							   i, GET_CHUNK(i), OUT_BUFFER_SIZE );
						return 0;
					}
					TInt aPos = GET_OFFSET(i);
					TPtr8 aOutBufferPtr(iOutBuffer, OUT_BUFFER_SIZE);
					LOG_WRITE2(_L("Reading file at %d, length %d"), aPos, GET_CHUNK(i));
					err = iFile.Seek(ESeekStart, aPos);
					err = iFile.Read(aOutBufferPtr, GET_CHUNK(i));
					if (err!=KErrNone)
					{
						LOG_WRITE(_L("Error reading file!"));
						return 0;
					}
					LOG_WRITE4(_L("%02X%02X%02X%02X"), aOutBufferPtr[0],
													   aOutBufferPtr[1],
													   aOutBufferPtr[2],
													   aOutBufferPtr[3]);
					LOG_WRITE4(_L("%02X%02X%02X%02X"), iOutBuffer[0],
													   iOutBuffer[1],
													   iOutBuffer[2],
													   iOutBuffer[3]);
					//memcpy(outBuffer, h->start + h->offsets[i], h->chunks[i]);

					zStream.next_in   = iOutBuffer;
					zStream.avail_in  = GET_CHUNK(i);
					zStream.next_out  = aInBuffer;
					zStream.avail_out = IN_BUFFER_SIZE;
					int errCode = inflate( &zStream,  Z_PARTIAL_FLUSH );
					if (errCode != Z_OK)
					{
						LOG_WRITE2(_L( "error: %d, inflate: %s"), errCode, zStream.msg);
						return 0;
					}
					if (zStream.avail_in)
					{
						LOG_WRITE2(_L("inflate did not flush (%d pending, %d avail)"),
							   zStream.avail_in, zStream.avail_out);
						return 0;
					}

					aCount = IN_BUFFER_SIZE - zStream.avail_out;
					cache[aTarget].count = aCount;
				}

				if (i == aFirstChunk)
				{
					if (i == aLastChunk)
					{
						aTextBufferPtr.Append(aInBuffer + aFirstOffset,
											  aLastOffset - aFirstOffset);
						//memcpy(pt, inBuffer + firstOffset, lastOffset-firstOffset);
						//pt += aLastOffset - aFirstOffset;
					}
					else
					{
						if (aCount != iChunkLength )
						{
							LOG_WRITE2(_L("Length = %d instead of %d\n"),
								   aCount, iChunkLength);
							return 0;
						}
						aTextBufferPtr.Append(aInBuffer + aFirstOffset,
											  iChunkLength - aFirstOffset);
						/*memcpy(pt,
							   inBuffer + firstOffset,
							   h->chunkLength - firstOffset);
						pt += h->chunkLength - firstOffset;*/
					}
				}
				else if (i == aLastChunk)
				{
					aTextBufferPtr.Append(aInBuffer, aLastOffset);
					//memcpy( pt, inBuffer, lastOffset );
					//pt += aLastOffset;
				}
				else
				{
					aTextBufferPtr.Append(aInBuffer, iChunkLength);
					//assert( count == h->chunkLength );
					//memcpy( pt, inBuffer, h->chunkLength );
					//pt += iChunkLength;
				}
			}
			//*pt = '\0';
			break;
	default:
		LOG_WRITE(_L("Cannot read unknown file type!"));
		return 0;
		break;
	}
	CloseStream();
	LOG_WRITE(aTextBufferPtr);
	return 1;
}

void CDictGzReader::CloseStream()
{
	if (iInitialized)
	{
		iInitialized = 0;
		if (inflateEnd(&zStream))
		{
			LOG_WRITE1(_L("Cannot shut down inflation engine: %s"),
					   zStream.msg);
		}
	}
}

TInt CDictGzReader::GetMeaningTextL(TInt aOffset, TInt aLength, TDesC8& aSameTypeSequence, HBufC **aMeaningText)
{
	if (iMeaningBuffer == NULL)
	{
		iMeaningBuffer = HBufC::NewL(KMeaningInitialSize);
	}

	TInt returnCode = NO_ERROR;
	TBuf8<1> lf(_L8("\n"));
	TBuf8<1> breaklines(_L8("\f"));

	//offset = iOffsetList->operator [](aIndex);
	//length = iLengthList->operator [](aIndex);
	
	//aOffset = offset[0]*256*256*256 + offset[1]*256*256 + offset[2]*256 + offset[3];
	//aLength = length[0]*256*256*256 + length[1]*256*256 + length[2]*256 + length[3];

	LOG_WRITE2(_L("Offset=%d, Length=%d"), aOffset, aLength);

	if (aLength > 0)
	{
		TUint8 *temp = new TUint8[aLength];
		TPtr8 aTextBufferPtr(temp, aLength);
		if (Read(aOffset, aLength, aTextBufferPtr))
		{
			// replace all \n to \f
			TInt i;
			for(i=0; i<aSameTypeSequence.Length(); i++)
			{
				switch(aSameTypeSequence.operator [](i))
				{
					case 'y':
					{
						TInt aPos = aTextBufferPtr.Locate(TChar(0));
						if(aPos != KErrNotFound)
							{
							aTextBufferPtr.Replace(aPos, 1, breaklines);
							}
						break;
					}
					case 'k':
					{
						PowerWordParse(temp, aLength);
						break;
					}
					default:
					{
						break;
					}
				}
			}
			aTextBufferPtr.SetLength(aLength);
			while(1)
			{
				TInt aPos = aTextBufferPtr.FindC(lf);
				if(aPos != KErrNotFound)
					{
					aTextBufferPtr.Replace(aPos, 1, breaklines);
					}
				else
					{
					break;
					}
			}
		
			if (aLength > iMeaningBuffer->Length())
			{
				iMeaningBuffer = iMeaningBuffer->ReAllocL(aLength);
			}

			if (iMeaningBuffer != NULL)
			{
				TPtr16 bufptr(iMeaningBuffer->Des());
				CnvUtfConverter::ConvertToUnicodeFromUtf8(bufptr, aTextBufferPtr);
				DictTextDecorator::Decorate(bufptr);		
			}
			else
			{
				returnCode = ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		else
		{
			returnCode = ERROR_CANT_READ_FROM_FILE;
		}
		delete [] temp;
	}
	else
	{
		returnCode = ERROR_LENGTH_IS_ZERO;
	}
	LOG_WRITE(iMeaningBuffer->Des());

	*aMeaningText = iMeaningBuffer;
	return returnCode;
}

void CDictGzReader::PowerWordParse(TUint8* powerword, TInt& length)
{
	_LIT8(KCidianyinbiao, "词典音标");
	TUint8 *temp = new TUint8[length];
	TPtr8 aTempPtr(temp, length);
	TPtr8 aPowerwordPtr(powerword, length);
	aTempPtr.Copy(powerword, length);
	
	
	TBuf8<9> CData(_L8("<![CDATA["));
	TBuf8<3> CDataEnd(_L8("]]>"));

	TInt xmlStart = 0;
	TInt dataStart, dataEnd;
	
	aPowerwordPtr.Delete(0, length);
	while(1)
	{
		dataStart = aTempPtr.Mid(xmlStart).FindF(CData);
		dataEnd   = aTempPtr.Mid(xmlStart).FindF(CDataEnd);
		if(dataStart != KErrNotFound && dataEnd != KErrNotFound)
		{
                        TPtrC8 dataPtr = aTempPtr.Mid(xmlStart+dataStart+CData.Length(), dataEnd-dataStart-CData.Length());
			xmlStart = xmlStart + dataEnd + CDataEnd.Length();
			TInt p1 = aTempPtr.Mid(xmlStart).Locate(TChar('<'));
			TInt p2 = aTempPtr.Mid(xmlStart).Locate(TChar('>'));
			TPtrC8 flagPtr = aTempPtr.Mid(xmlStart+p1+1, p2-p1-1);
			if(flagPtr.FindF(KCidianyinbiao) != KErrNotFound)
			{
			  TInt i;
			  aPowerwordPtr.Append(TChar('['));
			  for(i=0; i<dataPtr.Length(); i++)
			    {
			      TChar c = dataPtr.operator [](i);
			      switch(c)
			      {
			        case 'A':
			          aPowerwordPtr.Append(_L8("æ"));
			          break;
			        case 'B':
			          aPowerwordPtr.Append(_L8("ɑ"));
			          break;
                                case 'C':
                                  aPowerwordPtr.Append(_L8("ɔ"));
                                  break;
                                case 'Q':
                                  aPowerwordPtr.Append(_L8("ʌ"));
                                  break;
                                case 'E':
                                  aPowerwordPtr.Append(_L8("ə"));
                                  break;
                                case 'Z':
                                  aPowerwordPtr.Append(_L8("ε"));
                                  break;
                                case 'N':
                                  aPowerwordPtr.Append(_L8("ŋ"));
                                  break;
                                case 'W':
                                  aPowerwordPtr.Append(_L8("θ"));
                                  break;
                                case 'T':
                                  aPowerwordPtr.Append(_L8("ð"));
                                  break;
                                case 'F':
                                  aPowerwordPtr.Append(_L8("∫"));
                                  break;
                                case 'V':
                                  aPowerwordPtr.Append(_L8("ӡ"));
                                  break;
                                case 'L':
                                  aPowerwordPtr.Append(_L8("ɚ"));
                                  break;
                                case 'I':
                                  aPowerwordPtr.Append(_L8("ɪ"));
                                  break;
                                case '^':
                                  aPowerwordPtr.Append(_L8("ɡ"));
                                  break;
                                case '9':
                                  aPowerwordPtr.Append(_L8(","));
                                  break;
			        case '5':
			          aPowerwordPtr.Append(TChar('\''));
			          break;
			        default:
			          aPowerwordPtr.Append(c);
			          break;
			      }
			    }
			  aPowerwordPtr.Append(TChar(']'));
			}
			else
			{
			  if(dataPtr.Locate(TChar('&'))!= KErrNotFound)
			    {
			      // exact link
			      TInt linkPos = 0;
                              while(1)
                                {
                                  TPtrC8 linkPtr = dataPtr.Mid(linkPos, dataPtr.Length()-linkPos);
                                  TInt p1 = linkPtr.Locate(TChar('{'));
                                  TInt p2 = linkPtr.Locate(TChar('}'));
                                  if(p1 != KErrNotFound && p2 != KErrNotFound)
                                    {
                                      aPowerwordPtr.Append(linkPtr.Mid(p1+1, p2-p1-1));
                                      aPowerwordPtr.Append(_L8(" "));
                                      linkPos = linkPos+p2+1;
                                    }
                                  else
                                    {
                                      break;
                                    }
                                }
			    }
			  else
			    {
			      aPowerwordPtr.Append(dataPtr);
			    }
			}
			aPowerwordPtr.Append(TChar('\n'));
		}
		else
		{
			break;
		}
	}
	delete temp;
	length = aPowerwordPtr.Length();
}

