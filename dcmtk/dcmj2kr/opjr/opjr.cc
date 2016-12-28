#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krParams.h"
#include "dcmtk/dcmj2kr/opjr/opjrParams.h"
#include "dcmtk/dcmj2kr/opjr/opjr.h"
#include "dcmtk/dcmj2kr/opjr/opjrInstance.h"

opjr::opjr()
: j2krCoder()
{}

opjr::~opjr()
{}

E_TransferSyntax opjr::supportedTransferSyntax() const
{
  return EXS_JPEG2000LosslessOnly;
}


OFBool opjr::isLosslessProcess() const
{
  return OFTrue;
}


void opjr::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const j2krParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  opjrParams defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "compression openjpeg reversible";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *opjr::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2krParams *cp,
    Uint8 bitsPerSample) const
{
  DJEncoder * result = NULL;
  result = new opjrInstance(
                            *cp,
                            EJM_lossless,
                            bitsPerSample
                            );
  return result;  
}


