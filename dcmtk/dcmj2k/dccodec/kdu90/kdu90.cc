#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kParams.h"
#include "dcmtk/dcmj2k/dccodec/kdu90/kdu90Params.h"
#include "dcmtk/dcmj2k/dccodec/kdu90/kdu90.h"
#include "dcmtk/dcmj2k/dccodec/kdu90/kdu90Instance.h"

kdu90::kdu90()
: j2kCoder()
{}

kdu90::~kdu90()
{}

E_TransferSyntax kdu90::supportedTransferSyntax() const
{
  return EXS_JPEG2000;
}


OFBool kdu90::isLosslessProcess() const
{
  return OFFalse;
}


void kdu90::createDerivationDescription(
  const DcmRepresentationParameter * toRepParam,
  const j2kParams * /* cp */ ,
  Uint8 /* bitsPerSample */ ,
  double ratio,
  OFString& derivationDescription) const
{
  kdu90Params defaultRP;
  const kdu90Params *rp = toRepParam ? (const kdu90Params *)toRepParam : &defaultRP ;
  char buf[64];
 
  derivationDescription =  "Lossy compression with JPEG 2K ";
  derivationDescription += ", quality factor ";
  sprintf(buf, "%u", rp->getQuality());
  derivationDescription += buf;
  derivationDescription += ", compression ratio ";
  appendCompressionRatio(derivationDescription, ratio);
}


DJEncoder *kdu90::createEncoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const j2kParams *cp,
    Uint8 bitsPerSample) const
{
  kdu90Params defaultRP;
  const kdu90Params *rp = toRepParam ? (const kdu90Params *)toRepParam : &defaultRP ;

  DJEncoder * result = NULL;
  result = new kdu90Instance(*cp, EJM_JP2K_lossy, rp->getQuality(), bitsPerSample);
  return result;
}
