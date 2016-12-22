#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/j2kEncoderRegistration.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2k/dccodec/enc/kdu.h"
#include "dcmtk/dcmj2k/dccodec/enc/kdur.h"
#include "dcmtk/dcmj2k/dccodec/param.h"

// initialization of static members
OFBool j2kEncoderRegistration::registered                  = OFFalse;
j2kCodecParameter *j2kEncoderRegistration::cp               = NULL;
kdu *j2kEncoderRegistration::enc2K				  = NULL;
kdur *j2kEncoderRegistration::enc2KLoL	  = NULL;

void j2kEncoderRegistration::registerCodecs(
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
    cp = new j2kCodecParameter(
      pCompressionCSConversion,
      EDC_photometricInterpretation,  // not relevant, used for decompression only
      pCreateSOPInstanceUID,
      EPC_default, // not relevant, used for decompression only
      OFFalse, // not relevant, used for decompression only
      pOptimizeHuffman,
      pSmoothingFactor,
      pForcedBitDepth,
      pFragmentSize,
      pCreateOffsetTable,
      pSampleFactors,
      pWriteYBR422,
      pConvertToSC,
      pWindowType,
      pWindowParameter,
      pVoiCenter,
      pVoiWidth,
      pRoiLeft,
      pRoiTop,
      pRoiWidth,
      pRoiHeight,
      pUsePixelValues,
      pUseModalityRescale,
      pAcceptWrongPaletteTags,
      pAcrNemaCompatibility,
      pRealLossless);
    if (cp)
    {
      // JPEG 2K
      enc2K = new kdu();
      if (enc2K) DcmCodecList::registerCodec(enc2K, NULL, cp);
        
      // JPEG 2K Lossy
      enc2KLoL = new kdur();
      if (enc2KLoL) DcmCodecList::registerCodec(enc2KLoL, NULL, cp);

      registered = OFTrue;
    }
  }
}

void j2kEncoderRegistration::cleanup()
{
  if (registered)
  {
    DcmCodecList::deregisterCodec(enc2K);
    delete enc2K;
    DcmCodecList::deregisterCodec(enc2KLoL);
    delete enc2KLoL;
    delete cp;
    registered = OFFalse;
#ifdef DEBUG
    // not needed but useful for debugging purposes
    enc2K = NULL;
    enc2KLoL = NULL;
    cp     = NULL;
#endif

  }
}
