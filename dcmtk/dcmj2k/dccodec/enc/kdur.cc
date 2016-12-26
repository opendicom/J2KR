#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kCodecParameter.h"
#include "dcmtk/dcmj2k/dcpixel/kduRepresentationParameter.h"
#include "dcmtk/dcmj2k/dcpixel/kdurRepresentationParameter.h"
#include "dcmtk/dcmj2k/dccodec/enc/kdur.h"
#include "dcmtk/dcmj2k/dccodec/enc/kdurInstance.h"

kdur::kdur()
: j2kCoder()
{}

kdur::~kdur()
{}

E_TransferSyntax kdur::supportedTransferSyntax() const
{
  return EXS_JPEG2000LosslessOnly;
}


OFBool kdur::isLosslessProcess() const
{
  return OFTrue;
}


void kdur::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const j2kCodecParameter * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kduRepresentationParameter defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "LossLess compression with JPEG 2K";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdur::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2kCodecParameter *cp,
    Uint8 bitsPerSample) const
{
  kduRepresentationParameter defaultRP;
  const kduRepresentationParameter *rp = toRepParam ? (const kduRepresentationParameter *)toRepParam : &defaultRP ;

  //DJCompressJP2K
  DJEncoder * result = NULL;
  result = new kdurInstance(*cp, EJM_JP2K_lossless, rp->getQuality(), bitsPerSample);
  return result;  
}


