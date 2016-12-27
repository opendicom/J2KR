#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/j2kParams.h"
#include "dcmtk/dcmj2k/kdu/kduParams.h"
#include "dcmtk/dcmj2k/kdu/kdu.h"
#include "dcmtk/dcmj2k/kdu/kduInstance.h"

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
  const j2kParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kduParams defaultRP;
  const kduParams *rp = toRepParam ? (const kduParams *)toRepParam : &defaultRP ;
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
    const j2kParams *cp,
    Uint8 bitsPerSample) const
{
  kduParams defaultRP;
  const kduParams *rp = toRepParam ? (const kduParams *)toRepParam : &defaultRP ;

  DJEncoder * result = NULL;
  result = new kduInstance(*cp, EJM_JP2K_lossy, rp->getQuality(), bitsPerSample);
  return result;
}
