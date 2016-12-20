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
 *  Purpose: Codec class for decoding JP2K
 *  Last Update:      $Author: lpysher $
 *  Update Date:      $Date: 2006/03/01 20:15:44 $
 */
#include "osconfig.h"
#include "djdec2k.h"
#include "dcmtk/dcmdataImplementation/dccodec/djcparam.h"
#include "djdijp2k.h"
#include "dcpixseq.h"  /* for class DcmPixelSequence */

DJDecoderJP2k::DJDecoderJP2k()
: DJCodecDecoder()
{}
DJDecoderJP2k::~DJDecoderJP2k()
{}

OFBool DJDecoderJP2k::canChangeCoding(
    const E_TransferSyntax oldRepType,
    const E_TransferSyntax newRepType) const
{
  E_TransferSyntax myXfer = supportedTransferSyntax();//EXS_JPEG2000
  DcmXfer newRep(newRepType);
  if (newRep.isNotEncapsulated() && (oldRepType == myXfer))
	return OFTrue; // decompress requested

  if (newRep.getXfer() == EXS_JPEG2000LosslessOnly && (oldRepType == myXfer))
	return OFTrue;//JP2K lossy toJP2K lossless...

  // we don't support re-coding for now.
  return OFFalse;
}


/*djcodecd
 OFCondition DJCodecDecoder::decode(
 const DcmRepresentationParameter * fromRepParam,
 DcmPixelSequence * pixSeq,
 DcmPolymorphOBOW& uncompressedPixelData,
 const DcmCodecParameter * cp,
 const DcmStack& objStack) const
 {}
 */

/*djcodecd
 OFCondition DJCodecDecoder::decodeFrame(
 const DcmRepresentationParameter *fromParam,
 DcmPixelSequence *fromPixSeq,
 const DcmCodecParameter *cp,
 DcmItem *dataset,
 Uint32 frameNo,
 Uint32& startFragment,
 void *buffer,
 Uint32 bufSize,
 OFString& decompressedColorModel) const
 {}
 */

/*djcodecd
 OFCondition DJCodecDecoder::encode(
 const Uint16 * pixelData,
 const Uint32 length,
 const DcmRepresentationParameter * toRepParam,
 DcmPixelSequence * & ixSeq,
 const DcmCodecParameter *cp,
 DcmStack & objStack) const
 {}
 */


OFCondition DJDecoderJP2k::encode(
    const E_TransferSyntax fromRepType,
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence *fromPixSeq,
    const DcmRepresentationParameter *toRepParam,
    DcmPixelSequence * & toPixSeq,
    const DcmCodecParameter * cp,
    DcmStack & objStack) const
{
  if( fromRepType == EXS_JPEG2000)
  {
	 toPixSeq = new DcmPixelSequence( *fromPixSeq);
	 toPixSeq->changeXfer( EXS_JPEG2000LosslessOnly);
	 
	 return EC_Normal;
  }
  
  // we don't support re-coding for now.
  return EC_IllegalCall;
}

/*djcodecd
 OFCondition DJCodecDecoder::determineDecompressedColorModel(
 const DcmRepresentationParameter *fromParam,
 DcmPixelSequence *fromPixSeq,
 const DcmCodecParameter *cp,
 DcmItem *dataset,
 OFString &decompressedColorModel) const
 {}
 */

#pragma mark abstract implemented
E_TransferSyntax DJDecoderJP2k::supportedTransferSyntax() const
{
  return EXS_JPEG2000;
}

#pragma mark additional public
OFBool DJDecoderJP2k::isJPEG2000() const
{
    return OFTrue;
}


#pragma mark -
#pragma mark private

#pragma mark abstract implemented
DJDecoder *DJDecoderJP2k::createDecoderInstance(
    const DcmRepresentationParameter * /* toRepParam */,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const
{
	return new DJDecompressJP2k(*cp, isYBR);
}


#pragma mark -
#pragma mark -
#pragma mark Decoder class for JPEG 2K Lossless
DJDecoderJP2kLossLess::DJDecoderJP2kLossLess()
: DJCodecDecoder()
{}
DJDecoderJP2kLossLess::~DJDecoderJP2kLossLess()
{}

OFBool DJDecoderJP2kLossLess::canChangeCoding(
    const E_TransferSyntax oldRepType,
    const E_TransferSyntax newRepType) const
{
  E_TransferSyntax myXfer = supportedTransferSyntax();
  DcmXfer newRep(newRepType);
  if (newRep.isNotEncapsulated() && (oldRepType == myXfer))
	return OFTrue; // decompress requested

  if (newRep.getXfer() == EXS_JPEG2000 && (oldRepType == myXfer))
	return OFTrue;

  // we don't support re-coding for now.
  return OFFalse;
}

OFCondition DJDecoderJP2kLossLess::encode(
    const E_TransferSyntax fromRepType,
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence *fromPixSeq,
    const DcmRepresentationParameter *toRepParam,
    DcmPixelSequence * & toPixSeq,
    const DcmCodecParameter * cp,
    DcmStack & objStack) const
{
  if( fromRepType == EXS_JPEG2000LosslessOnly)
  {
	toPixSeq = new DcmPixelSequence( *fromPixSeq);
	toPixSeq->changeXfer( EXS_JPEG2000);
	
	return EC_Normal;
  }
  
  // we don't support re-coding for now.
  return EC_IllegalCall;
}

E_TransferSyntax DJDecoderJP2kLossLess::supportedTransferSyntax() const
{
  return EXS_JPEG2000LosslessOnly;
}


#pragma mark -
#pragma mark private

#pragma mark abstract implemented

DJDecoder *DJDecoderJP2kLossLess::createDecoderInstance(
    const DcmRepresentationParameter * /* toRepParam */,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const
{
	return new DJDecompressJP2k(*cp, isYBR);
}

OFBool DJDecoderJP2kLossLess::isJPEG2000() const
{
	return OFTrue;
}
