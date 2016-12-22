#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/j2kDecoderRegistration.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2k/dccodec/dec/djdec2k.h"
#include "dcmtk/dcmj2k/dccodec/param.h"

// initialization of static members
OFBool j2kDecoderRegistration::registered                  = OFFalse;
j2kCodecParameter *j2kDecoderRegistration::cp               = NULL;
DJDecoderJP2k *j2kDecoderRegistration::dec2k               = NULL;
DJDecoderJP2kLossLess *j2kDecoderRegistration::dec2kLossLess    = NULL;

void j2kDecoderRegistration::registerCodecs(
    E_DecompressionColorSpaceConversion pDecompressionCSConversion,
    E_UIDCreation pCreateSOPInstanceUID,
    E_PlanarConfiguration pPlanarConfiguration,
    OFBool predictor6WorkaroundEnable)
{
  if (! registered)
  {
    cp = new j2kCodecParameter(
      ECC_lossyYCbCr, // ignored, compression only
      pDecompressionCSConversion, 
      pCreateSOPInstanceUID, 
      pPlanarConfiguration,
      predictor6WorkaroundEnable);
    if (cp)
    {
      // JPEG 2K Lossy
      dec2k = new DJDecoderJP2k();
      if (dec2k) DcmCodecList::registerCodec(dec2k, NULL, cp);
        
      // JPEG 2K
      dec2kLossLess = new DJDecoderJP2kLossLess();
      if (dec2kLossLess) DcmCodecList::registerCodec(dec2kLossLess, NULL, cp);

      registered = OFTrue;
    }
  }
}

void j2kDecoderRegistration::cleanup()
{
  if (registered)
  {
    delete cp;
    DcmCodecList::deregisterCodec(dec2k);
    delete dec2k;
    DcmCodecList::deregisterCodec(dec2kLossLess);
    delete dec2kLossLess;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    dec2K  = NULL;
    dec2KLoL = NULL;
    cp     = NULL;
#endif

  }
}
