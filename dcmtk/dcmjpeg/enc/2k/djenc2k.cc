/*  Copyright (C) 1997-2005, OFFIS
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
 *  Purpose: Codec class for encoding JP2K
 *  Last Update:      $Author: lpysher $
 *  Update Date:      $Date: 2006/03/01 20:15:44 $
 */

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmjpeg/djcparam.h"
#include "djrploss.h"
#include "djenc2k.h"
#include "djeijg2k.h" //createEncoderInstance requiere DJCompressJP2K (djeijg2k...)


#pragma mark DJEncoder2K

DJEncoder2K::DJEncoder2K()
: DJCodecEncoder()
{
}


DJEncoder2K::~DJEncoder2K()
{
}


E_TransferSyntax DJEncoder2K::supportedTransferSyntax() const
{
  return EXS_JPEG2000;
}


OFBool DJEncoder2K::isLosslessProcess() const
{
  return OFFalse;
}


void DJEncoder2K::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const DJCodecParameter * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  DJ_RPLossy defaultRP;
  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
  char buf[64];
 
  derivationDescription =  "Lossy compression with JPEG 2K ";
  derivationDescription += ", quality factor ";
  sprintf(buf, "%u", rp->getQuality());
  derivationDescription += buf;
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *DJEncoder2K::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample) const
{
  DJ_RPLossy defaultRP;
  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;

  DJEncoder * result = NULL;
  result = new DJCompressJP2K(*cp, EJM_JP2K_lossy, rp->getQuality(), bitsPerSample);
  return result;
}

#pragma mark -
#pragma mark DJEncoder2KLossLess

DJEncoder2KLossLess::DJEncoder2KLossLess()
: DJCodecEncoder()
{
}


DJEncoder2KLossLess::~DJEncoder2KLossLess()
{
}


E_TransferSyntax DJEncoder2KLossLess::supportedTransferSyntax() const
{
  return EXS_JPEG2000LosslessOnly;
}


OFBool DJEncoder2KLossLess::isLosslessProcess() const
{
  return OFTrue;
}


void DJEncoder2KLossLess::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const DJCodecParameter * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  DJ_RPLossy defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "LossLess compression with JPEG 2K";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *DJEncoder2KLossLess::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample) const
{
  DJ_RPLossy defaultRP;
  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;

  //DJCompressJP2K
  DJEncoder * result = NULL;
  result = new DJCompressJP2K(*cp, EJM_JP2K_lossless, rp->getQuality(), bitsPerSample);
  return result;  
}


