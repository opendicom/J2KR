/*
 *
 *  Copyright (C) 1997-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  dcmjpeg
 *
 *  Author:  Marco Eichelberg, Norbert Olges
 *
 *  Purpose: compression routines of the IJG JPEG library configured for 12 bits/sample. 
 *
 *  Last Update:      $Author: lpysher $
 *  Update Date:      $Date: 2006/03/01 20:15:44 $
 *  Source File:      $Source: /cvsroot/osirix/osirix/Binaries/dcmtk-source/dcmjpeg/djeijg12.cc,v $
 *  CVS/RCS Revision: $Revision: 1.1 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#include "osconfig.h"
#include "djeijg2k.h"
#include "djcparam.h"
#include "ofconsol.h"
#import "Accelerate/Accelerate.h"
#include "ofconsol.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#define INCLUDE_CSTDIO
#define INCLUDE_CSETJMP
#include "ofstdinc.h"

extern "C" void* kdu_compressJPEG2K( void *data, int samplesPerPixel, int rows, int columns, int precision, bool sign, int rate, long *compressedDataSize, int num_threads);

// These two macros are re-defined in the IJG header files.
// We undefine them here and hope that IJG's configure has
// come to the same conclusion that we have...
#ifdef HAVE_STDLIB_H
#undef HAVE_STDLIB_H
#endif
#ifdef HAVE_STDDEF_H
#undef HAVE_STDDEF_H
#endif

// use 16K blocks for temporary storage of compressed JPEG data
#define IJGE12_BLOCKSIZE 16384

//#include "openjpeg.h"
/**
sample error callback expecting a FILE* client object
JF removed
static void error_callback(const char *msg, void *a)
{
	printf( "%s", msg);
}
*/

/**
sample warning callback expecting a FILE* client object
JF removed
static void warning_callback(const char *msg, void *a)
{
	printf( "%s", msg);
}
*/

/**
sample debug callback expecting no client object
JF removed
static void info_callback(const char *msg, void *a)
{
//	NSLog( @"%s", msg);
}
*/

/*JF removed
static inline int int_ceildivpow2(int a, int b)
{
	return (a + (1 << b) - 1) >> b;
}
*/

DJCompressJP2K::DJCompressJP2K(const DJCodecParameter& cp, EJ_Mode mode, Uint8 theQuality, Uint8 theBitsPerSample)
: DJEncoder()
, cparam(&cp)
, quality(theQuality)
, bitsPerSampleValue(theBitsPerSample)
, modeofOperation(mode)
{

}

DJCompressJP2K::~DJCompressJP2K()
{

}


void DJCompressJP2K::findMinMax( int &_min, int &_max, char *bytes, long length, OFBool isSigned, int rows, int columns, int bitsAllocated)
{
    _min = 0;
    _max = 0;
    
	float max = 0,  min = 0;
	
	if (bitsAllocated <= 8) 
		length = length;
	else if (bitsAllocated <= 16)
		length = length/2;
	else
		length = length/4;
		
	float *fBuffer = (float*) malloc(length * 4);
	if( fBuffer)
	{
		vImage_Buffer src, dstf;
		dstf.height = src.height = rows;
		dstf.width = src.width = columns;
		dstf.rowBytes = columns*sizeof(float);
		dstf.data = fBuffer;
		src.data = (void*) bytes;
		
		if (bitsAllocated <= 8)
		{
			src.rowBytes = columns;
			vImageConvert_Planar8toPlanarF( &src, &dstf, 0, 256, 0);
		}
		else if (bitsAllocated <= 16)
		{
			src.rowBytes = columns * 2;
			
			if( isSigned)
				vImageConvert_16SToF( &src, &dstf, 0, 1, 0);
			else
				vImageConvert_16UToF( &src, &dstf, 0, 1, 0);
		}
		
		vDSP_minv( fBuffer, 1, &min, length);
		vDSP_maxv( fBuffer, 1, &max, length);
        
		_min = min;
		_max = max;
		
		
		free(fBuffer);
	}
    else
        printf( "\r**** DJCompressJP2K::findMinMax malloc failed\r");
}

OFCondition DJCompressJP2K::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 * image_buffer,
  Uint8 * & to,
  Uint32 & length,
  Uint8 pixelRepresentation,
  double minUsed, double maxUsed)
{
	return encode( columns, rows, colorSpace, samplesPerPixel, (Uint8*) image_buffer, to, length, 8, pixelRepresentation, minUsed, maxUsed);
}

OFCondition DJCompressJP2K::encode(
    Uint16  columns ,
    Uint16  rows ,
    EP_Interpretation  interpr ,
    Uint16  samplesPerPixel ,
    Uint16 *  image_buffer ,
    Uint8 *&  to ,
    Uint32 &  length,
	Uint8 pixelRepresentation,
	double minUsed, double maxUsed)
{
	return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16, pixelRepresentation, minUsed, maxUsed);
}

Uint16 DJCompressJP2K::bytesPerSample() const
{
	if( bitsPerSampleValue <= 8)
		return 1;
	else
		return 2;
}

Uint16 DJCompressJP2K::bitsPerSample() const
{
	return bitsPerSampleValue;
}

OFCondition DJCompressJP2K::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 * image_buffer,
  Uint8 * & to,
  Uint32 & length,
  Uint8 bitsAllocated,
  Uint8 pixelRepresentation,
  double minUsed, double maxUsed)
{
	int bitsstored = bitsAllocated;
	
    if( samplesPerPixel > 1)
        bitsstored = bitsAllocated = 8;
    
	OFBool isSigned = 0;
	
	if( bitsAllocated >= 16)
	{
		if( minUsed == 0 && maxUsed == 0)
		{
			int _min = 0, _max = 0;
			findMinMax( _min, _max, (char*) image_buffer, columns*rows*samplesPerPixel*bitsAllocated/8, isSigned, rows, columns, bitsAllocated);
			
//			if( minUsed != _min || maxUsed != _max)
//				printf("\r******* ( minUsed != _min || maxUsed != _max) ********\r");
			
			minUsed = _min;
			maxUsed = _max;
		}
		
		int amplitude = maxUsed;
		
		if( minUsed < 0)
			amplitude -= minUsed;
		
		int bits = 1, value = 2;
		
		while( value < amplitude && bits <= 16)
		{
			value *= 2;
			bits++;
		}
		
		if( minUsed < 0) // K A10009536850 22.06.12
			bits++;
		
		if( bits < 9)
			bits = 9;
		
		// avoid the artifacts... switch to lossless
		if( (maxUsed >= 32000 && minUsed <= -32000) || maxUsed >= 65000 || bits > 16)
			quality = 0;
		
		if( bits > 16) bits = 16;
		
		bitsstored = bits;
	}
	
//	int precision = bitsstored;
    int rate = 0;
    
    long compressedLength = 0;
    
    int processors = 0;
    
    if( rows*columns > 256*1024) // 512 * 512
    {
        int mib[2] = {CTL_HW, HW_NCPU};
        size_t dataLen = sizeof(int); // 'num' is an 'int'
        int result = sysctl(mib, 2, &processors, &dataLen, NULL, 0);
        if (result == -1)
            processors = 1;
        if( processors > 8)
            processors = 8;
    }
    
    void *outBuffer = kdu_compressJPEG2K( (void*) image_buffer, samplesPerPixel, rows, columns, bitsstored, false, rate, &compressedLength, processors);
    
    if( outBuffer)
    {
        to = new Uint8[ compressedLength];
        memcpy( to, outBuffer, compressedLength);
        length = compressedLength;
    
        free( outBuffer);
    }

    return EC_Normal;
}
