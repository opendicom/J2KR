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

#include "osconfig.h"
#include "djeijg2k.h"
#include "dcmtk/dcmdataImplementation/dccodec/djcparam.h"
#include "ofconsol.h"
#include "ofconsol.h"
#include "ofstdinc.h"

// These two macros are re-defined in the IJG header files.
// We undefine them here and hope that IJG's configure has
// come to the same conclusion that we have...
#ifdef HAVE_STDLIB_H
#undef HAVE_STDLIB_H
#endif
#ifdef HAVE_STDDEF_H
#undef HAVE_STDDEF_H
#endif

#include "OPJSupport.h"
#include "openjpeg.h"

//#include "options.h"

// use 16K blocks for temporary storage of compressed JPEG data
#define IJGE12_BLOCKSIZE 16384

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


//buffer 16 bits
OFCondition DJCompressJP2K::encode(
  Uint16  columns ,
  Uint16  rows ,
  EP_Interpretation  interpr ,
  Uint16  samplesPerPixel ,
  Uint16 *  image_buffer ,
  Uint8 *&  to ,
  Uint32 &  length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16, pixelRepresentation, minUsed, maxUsed);
}

//buffer 8 bits
OFCondition DJCompressJP2K::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 * image_buffer,
  Uint8 * & to,
  Uint32 & length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed
)
{
	return encode( columns, rows, colorSpace, samplesPerPixel, (Uint8*) image_buffer, to, length, 8, pixelRepresentation, minUsed, maxUsed);
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
  double minUsed,
  double maxUsed
)
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
