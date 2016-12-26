#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kCodecParameter.h"
#include "dcmtk/dcmj2k/dcpixel/kduRepresentationParameter.h"
#include "dcmtk/dcmj2k/dcpixel/kdurRepresentationParameter.h"
#include "dcmtk/dcmj2k/dccodec/enc/kdu.h"
#include "dcmtk/dcmj2k/dccodec/enc/kduInstance.h"

#pragma mark kdu

kdu::kdu()
: j2kCoder()
{}

kdu::~kdu()
{}

E_TransferSyntax kdu::supportedTransferSyntax() const
{
  return EXS_JPEG2000;
}


OFBool kdu::isLosslessProcess() const
{
  return OFFalse;
}


void kdu::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const j2kCodecParameter * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kduRepresentationParameter defaultRP;
  const kduRepresentationParameter *rp = toRepParam ? (const kduRepresentationParameter *)toRepParam : &defaultRP ;
  char buf[64];
 
  derivationDescription =  "Lossy compression with JPEG 2K ";
  derivationDescription += ", quality factor ";
  sprintf(buf, "%u", rp->getQuality());
  derivationDescription += buf;
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdu::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2kCodecParameter *cp,
    Uint8 bitsPerSample) const
{
  kduRepresentationParameter defaultRP;
  const kduRepresentationParameter *rp = toRepParam ? (const kduRepresentationParameter *)toRepParam : &defaultRP ;

  DJEncoder * result = NULL;
  result = new kduInstance(*cp, EJM_JP2K_lossy, rp->getQuality(), bitsPerSample);
  return result;
}
