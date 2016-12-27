#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmrk2j/rk2jRegister.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */

#include "dcmtk/dcmrk2j/rudk/rudk.h"

// initialization of static members
OFBool rk2jRegister::registered                       = OFFalse;
rk2jParams *rk2jRegister::cp                           = NULL;
rudk *rk2jRegister::dec2KLoL                           = NULL;

void rk2jRegister::registerCodecs(
    E_DecompressionColorSpaceConversion pDecompressionCSConversion,
    E_UIDCreation pCreateSOPInstanceUID,
    E_PlanarConfiguration pPlanarConfiguration,
    OFBool predictor6WorkaroundEnable)
{
  if (! registered)
  {
    cp = new rk2jParams(
      ECC_lossyYCbCr, // ignored, compression only
      pDecompressionCSConversion, 
      pCreateSOPInstanceUID, 
      pPlanarConfiguration,
      predictor6WorkaroundEnable);
    if (cp)
    {
      dec2KLoL = new rudk();
      if (dec2KLoL) DcmCodecList::registerCodec(dec2KLoL, NULL, cp);

      registered = OFTrue;
    }
  }
}

void rk2jRegister::cleanup()
{
  if (registered)
  {
    delete cp;
    DcmCodecList::deregisterCodec(dec2KLoL);
    delete dec2KLoL;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    dec2KLol = NULL;
    cp     = NULL;
#endif

  }
}