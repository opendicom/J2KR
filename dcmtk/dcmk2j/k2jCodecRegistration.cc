#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmk2j/k2jCodecRegistration.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2k/dccodec/dec/djdec2k.h"

// initialization of static members
OFBool k2jCodecRegistration::registered                       = OFFalse;
k2jCodecParameter *k2jCodecRegistration::cp                   = NULL;
DJDecoderJP2k *k2jCodecRegistration::dec2k                    = NULL;
DJDecoderJP2kLossLess *k2jCodecRegistration::dec2kLossLess    = NULL;

void k2jCodecRegistration::registerCodecs(
    E_DecompressionColorSpaceConversion pDecompressionCSConversion,
    E_UIDCreation pCreateSOPInstanceUID,
    E_PlanarConfiguration pPlanarConfiguration,
    OFBool predictor6WorkaroundEnable)
{
  if (! registered)
  {
    cp = new k2jCodecParameter(
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

void k2jCodecRegistration::cleanup()
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
