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
#include "dcmtk/dcmk2j/dccodec/udk/udkInstance.h"
#include "dcmtk/dcmdataImplementation/dccodec/djcparam.h"
#include "ofconsol.h"
#include "OPJSupport.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#define INCLUDE_CSTDIO
#define INCLUDE_CSETJMP
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

BEGIN_EXTERN_C
#define boolean ijg_boolean
#include "jpeglib16.h"
#include "jerror16.h"
#undef boolean

// disable any preprocessor magic the IJG library might be doing with the "const" keyword
#ifdef const
#undef const
#endif


DJDecompressJP2k::DJDecompressJP2k(const DJCodecParameter& cp, OFBool isYBR)
: DJDecoder()
, cparam(&cp)
, cinfo(NULL)
, suspension(0)
, jsampBuffer(NULL)
, dicomPhotometricInterpretationIsYCbCr(isYBR)
, decompressedColorModel(EPI_Unknown)
{
}

DJDecompressJP2k::~DJDecompressJP2k()
{
  cleanup();
}


OFCondition DJDecompressJP2k::init()
{
  // everything OK
  return EC_Normal;
}


void DJDecompressJP2k::cleanup()
{
}


OFCondition DJDecompressJP2k::decode(
  Uint8 *compressedFrameBuffer,
  Uint32 compressedFrameBufferSize,
  Uint8 *uncompressedFrameBuffer,
  Uint32 uncompressedFrameBufferSize,
  OFBool isSigned)
{
    int processors = 0;
    
    if( compressedFrameBufferSize > 512*1024)
    {
        int mib[2] = {CTL_HW, HW_NCPU};
        size_t dataLen = sizeof(int); // 'num' is an 'int'
        int result = sysctl(mib, 2, &processors, &dataLen, NULL, 0);
        if (result == -1)
            processors = 1;
        if( processors > 8)
            processors = 8;
    }
    
    long decompressedBufferSize = 0;
    int colorModel;
    
    OPJSupport opj;
    opj.decompressJPEG2KWithBuffer( uncompressedFrameBuffer,
                                   compressedFrameBuffer,
                                   compressedFrameBufferSize,
                                   &decompressedBufferSize,
                                   &colorModel);
    
    if( colorModel == 1)
        decompressedColorModel = (EP_Interpretation) EPI_RGB;
	return EC_Normal;
}

void DJDecompressJP2k::outputMessage() const
{

}
}
