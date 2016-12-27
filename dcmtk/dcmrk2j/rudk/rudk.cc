#include "osconfig.h"
#include "dcmtk/dcmrk2j/rudk/rudk.h"
#include "dcmtk/dcmrk2j/rk2jParams.h"
#include "dcmtk/dcmrk2j/rudk/rudkInstance.h"
#include "dcpixseq.h"  /* for class DcmPixelSequence */

rudk::rudk()
: rk2jCoder()
{}
rudk::~rudk()
{}

OFBool rudk::canChangeCoding(
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


OFCondition rudk::encode(
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
E_TransferSyntax rudk::supportedTransferSyntax() const
{
  return EXS_JPEG2000;
}

#pragma mark additional public
OFBool rudk::isJPEG2000() const
{
    return OFTrue;
}


#pragma mark -
#pragma mark private

#pragma mark abstract implemented
DJDecoder *rudk::createDecoderInstance(
    const DcmRepresentationParameter *toRepParam,
    const rk2jParams *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const
{
    DJDecoder * result = NULL;
    result =  new rudkInstance(*cp, isYBR);
    return result;
}
