#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kParams.h"
#include "dcmtk/dcmj2k/dccodec/kdu/kduParams.h"
#include "dcmtk/dcmj2k/dccodec/kdu/kdu.h"
#include "dcmtk/dcmj2k/dccodec/kdu/kduInstance.h"

kdu::kdu()
: j2kCoder()
{}

kdu::~kdu()
{}

E_TransferSyntax kdu::supportedTransferSyntax() const
{
  return EXS_JPEG2000LosslessOnly;
}


OFBool kdu::isLosslessProcess() const
{
  return OFTrue;
}


void kdu::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const j2kParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kduParams defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "LossLess compression with JPEG 2K";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdu::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2kParams *cp,
    Uint8 bitsPerSample) const
{
  DJEncoder * result = NULL;
  result = new kduInstance(
                            *cp,
                            EJM_JP2K_lossless,
                            bitsPerSample
                            );
  return result;  
}


