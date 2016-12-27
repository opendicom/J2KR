#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krRegister.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2kr/kdur/kdur.h"

// initialization of static members
OFBool j2krRegister::registered      = OFFalse;
j2krParams *j2krRegister::cp          = NULL;
kdur *j2krRegister::enc2KLoL	        = NULL;

void j2krRegister::registerCodecs(
    E_CompressionColorSpaceConversion pCompressionCSConversion,
    E_UIDCreation pCreateSOPInstanceUID,
    OFBool pOptimizeHuffman,
    int pSmoothingFactor,
    int pForcedBitDepth,
    Uint32 pFragmentSize,
    OFBool pCreateOffsetTable,
    E_SubSampling pSampleFactors,
    OFBool pWriteYBR422,
    OFBool pConvertToSC,
    unsigned long pWindowType,
    unsigned long pWindowParameter,
    double pVoiCenter,
    double pVoiWidth,
    unsigned long pRoiLeft,
    unsigned long pRoiTop,
    unsigned long pRoiWidth,
    unsigned long pRoiHeight,
    OFBool pUsePixelValues,
    OFBool pUseModalityRescale,
    OFBool pAcceptWrongPaletteTags,
    OFBool pAcrNemaCompatibility,
    OFBool pRealLossless)
{
  if (! registered)
  {
    cp = new j2krParams();
    if (cp)
    {
      // JPEG 2K LossLess
      enc2KLoL = new kdur();
      if (enc2KLoL) DcmCodecList::registerCodec(enc2KLoL, NULL, cp);

      registered = OFTrue;
    }
  }
}

void j2krRegister::cleanup()
{
  if (registered)
  {
    DcmCodecList::deregisterCodec(enc2KLoL);
    delete enc2KLoL;
    delete cp;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    enc2KLoL = NULL;
    cp     = NULL;
#endif

  }
}
