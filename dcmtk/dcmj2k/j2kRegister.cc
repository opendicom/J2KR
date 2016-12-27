#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/j2kRegister.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2k/kdu/kdu.h"

// initialization of static members
OFBool j2kRegister::registered      = OFFalse;
j2kParams *j2kRegister::cp          = NULL;
kdu *j2kRegister::kakadu             = NULL;

void j2kRegister::registerCodecs(
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
    cp = new j2kParams();
    if (cp)
    {
      // JPEG 2K Lossy
      kakadu = new kdu();
      if (kakadu) DcmCodecList::registerCodec(kakadu, NULL, cp);
        
      registered = OFTrue;
    }
  }
}

void j2kRegister::cleanup()
{
  if (registered)
  {
    DcmCodecList::deregisterCodec(kakadu);
    delete kakadu;
    delete cp;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    kakadu = NULL;
    cp     = NULL;
#endif

  }
}
