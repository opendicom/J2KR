#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kParams.h"
#include "dcmtk/dcmj2k/dcpixel/kdurRepresentationParameter.h"
#include "dcmtk/dcmj2k/dccodec/kdu/kdur.h"
#include "dcmtk/dcmj2k/dccodec/kdu/kdurInstance.h"

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
  const j2kParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kdurRepresentationParameter defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "LossLess compression with JPEG 2K";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdur::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2kParams *cp,
    Uint8 bitsPerSample) const
{
  DJEncoder * result = NULL;
  result = new kdurInstance(
                            *cp,
                            EJM_JP2K_lossless,
                            bitsPerSample
                            );
  return result;  
}


