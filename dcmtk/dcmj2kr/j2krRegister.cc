#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krRegister.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2kr/kdur/kdur.h"

// initialization of static members
OFBool j2krRegister::registered      = OFFalse;
j2krParams *j2krRegister::cp          = NULL;
kdur *j2krRegister::kakaduReversible	        = NULL;

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
      kakaduReversible = new kdur();
      if (kakaduReversible) DcmCodecList::registerCodec(kakaduReversible, NULL, cp);

      registered = OFTrue;
    }
  }
}

void j2krRegister::cleanup()
{
  if (registered)
  {
    DcmCodecList::deregisterCodec(kakaduReversible);
    delete kakaduReversible;
    delete cp;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    kakaduReversible = NULL;
    cp     = NULL;
#endif

  }
}
