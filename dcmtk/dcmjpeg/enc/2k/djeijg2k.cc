/*
 *  Copyright (C) 1997-2005, OFFIS
 *  This software and supporting documentation were developed by
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *  Module:  dcmjpeg
 *  Author:  Marco Eichelberg, Norbert Olges
 *  Purpose: compression routines of the IJG JPEG library configured for 12 bits/sample.
 *  Last Update:      $Author: lpysher $
 *  Update Date:      $Date: 2006/03/01 20:15:44 $
 *  Source File:      $Source: /cvsroot/osirix/osirix/Binaries/dcmtk-source/dcmjpeg/djeijg12.cc,v $
 *  CVS/RCS Revision: $Revision: 1.1 $
 *  Status:           $State: Exp $
 *  CVS/RCS Log at end of file
 */

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmjpeg/enc/2k/djeijg2k.h"
#include "dcmtk/dcmdataImplementation/dccodec/djcparam.h"
#include "dcmtk/ofstd/ofconsol.h"
#include "dcmtk/ofstd/ofstdinc.h"

//kdu_osirix59 32bit compressed lib
extern "C" void* kdu_compressJPEG2K( void *data, int samplesPerPixel, int rows, int columns, int precision, bool sign, int rate, long *compressedDataSize, int num_threads);

//openjpeg 2
#include "OPJSupport.h"
#include "openjpeg.h"


// These two macros are re-defined in the IJG header files.
// We undefine them here and hope that IJG's configure has
// come to the same conclusion that we have...
//#ifdef HAVE_STDLIB_H
//#undef HAVE_STDLIB_H
//#endif
//#ifdef HAVE_STDDEF_H
//#undef HAVE_STDDEF_H
//#endif

// use 16K blocks for temporary storage of compressed JPEG data
#define IJGE12_BLOCKSIZE 16384


DJCompressJP2K::DJCompressJP2K(const DJCodecParameter& cp, EJ_Mode mode, Uint8 quality,
    Uint8 bitsPerSample)
                                : DJEncoder()
                                , cparam(&cp)
                                , quality(quality)
                                , bitsPerSampleValue(bitsPerSample)
                                , modeofOperation(mode)
{}
DJCompressJP2K::~DJCompressJP2K()
{}


//imageBuffer 16bit - abstract class DJEncoder
OFCondition DJCompressJP2K::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: imageBuffer 16bit\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint16*) image_buffer, to, length, 0, 0.0L, 0.0L);
    //pixel representation 0000H unsigned integer, 0001H 2's complement
}

//imageBuffer 16bit - used
OFCondition DJCompressJP2K::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to ,
  Uint32 &length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    fprintf(stdout,"D: pixelRepresentation:0000H (unsigned) minUsed:0.0(fixed) maxUsed:0.0(fixed)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16, pixelRepresentation, minUsed, maxUsed);
}


//imageBuffer 8bit - abstract class DJEncoder
OFCondition DJCompressJP2K::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: imageBuffer 8bit\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 0, 0.0L, 0.0L);
}

//imageBuffer 8bit
OFCondition DJCompressJP2K::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    fprintf(stdout,"D: pixelRepresentation:0000H (unsigned) minUsed:0.0(fixed) maxUsed:0.0(fixed)\r\n");
    return encode( columns, rows, colorSpace, samplesPerPixel, (Uint8*) image_buffer, to, length, 8, pixelRepresentation, minUsed, maxUsed);
}

//abstract class DJEncoder
Uint16 DJCompressJP2K::bytesPerSample() const
{
	if( bitsPerSampleValue <= 8) return 1;
	else return 2;
}

//abstract class DJEncoder
Uint16 DJCompressJP2K::bitsPerSample() const
{
	return bitsPerSampleValue;
}

OFCondition DJCompressJP2K::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 bitsAllocated,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    //if ([args[codec] isEqualToString:@"KDU"])
    //{
    
    //to = (Uint8 *) results in segmantation fault
    long compressedLength = 0;
    void *outBuffer = (Uint8 *)kdu_compressJPEG2K(
       (void*)image_buffer,
       samplesPerPixel,
       rows,
       columns,
       bitsAllocated,
       false,
       0,
       &compressedLength,
       0);
    //[7] rate (0=lossless), [9] processors

    if( outBuffer)
    {
        to = new Uint8[ compressedLength];
        memcpy( to, outBuffer, compressedLength);
        length = compressedLength;
    
        free( outBuffer);
    }
    return EC_Normal;
    
    /*} else if ([args[codec] isEqualToString:@"OPJ"])
     {
     long compressedLength = 0;//type mismatch
     OPJSupport opj;
     to = (Uint8 *)opj.compressJPEG2K(
     (void*) image_buffer,
     samplesPerPixel,
     rows,
     columns,
     bitsAllocated,
     bitsAllocated,
     false,
     0,
     &compressedLength);
     //[5] bitsstored, [7] signed, [8] rate=0=reversible
     length = compressedLength;
     
     return EC_Normal;
     }
     */
    return EC_NoEncodingLibrary;
}
