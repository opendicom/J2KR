/*
 *  Copyright (C) 2001-2010, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *  This software and supporting documentation were developed by
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *  Module:  dcmjpeg
 *  Author:  Marco Eichelberg, Norbert Olges
 *  Purpose: Codec class for decoding JPEG Progressive (lossy, 8/12-bit)
 */

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmjpeg/dec/ijg/pro/djdecpro.h"
#include "dcmtk/dcmjpeg/dccodec/djcparam.h"
#include "dcmtk/dcmjpeg/dcpixel/jpegParams.h"
#include "dcmtk/dcmjpeg/dec/ijg/djdijg8.h"
#include "dcmtk/dcmjpeg/dec/ijg/djdijg12.h"


DJDecoderProgressive::DJDecoderProgressive()
: DJCodecDecoder()
{
}


DJDecoderProgressive::~DJDecoderProgressive()
{
}


E_TransferSyntax DJDecoderProgressive::supportedTransferSyntax() const
{
  return EXS_JPEGProcess10_12;
}


DJDecoder *DJDecoderProgressive::createDecoderInstance(
    const DcmRepresentationParameter * /* toRepParam */,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const
{
  if (bitsPerSample > 8) return new DJDecompressIJG12Bit(*cp, isYBR);
  else return new DJDecompressIJG8Bit(*cp, isYBR);
}
