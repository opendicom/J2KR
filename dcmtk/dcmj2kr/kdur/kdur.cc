#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krParams.h"
#include "dcmtk/dcmj2kr/kdur/kdurParams.h"
#include "dcmtk/dcmj2kr/kdur/kdur.h"
#include "dcmtk/dcmj2kr/kdur/kdurInstance.h"

kdur::kdur()
: j2krCoder()
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
  const j2krParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kdurParams defaultRP;
//  const DJ_RPLossy *rp = toRepParam ? (const DJ_RPLossy *)toRepParam : &defaultRP ;
//  char buf[64];
 
  derivationDescription =  "LossLess compression with JPEG 2K";
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdur::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2krParams *cp,
    Uint8 bitsPerSample) const
{
  DJEncoder * result = NULL;
  result = new kdurInstance(
                            *cp,
                            EJM_lossless,
                            bitsPerSample
                            );
  return result;  
}


