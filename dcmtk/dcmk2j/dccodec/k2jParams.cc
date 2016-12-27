#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmk2j/dccodec/k2jParams.h"

k2jParams::k2jParams(
    E_CompressionColorSpaceConversion pCompressionCSConversion,
    E_DecompressionColorSpaceConversion pDecompressionCSConversion,
    E_UIDCreation pCreateSOPInstanceUID,
    E_PlanarConfiguration pPlanarConfiguration,
    OFBool predictor6WorkaroundEnable,
    OFBool pOptimizeHuffman,
    int pSmoothingFactor,
    int pForcedBitDepth,
    Uint32 pFragmentSize,
    OFBool pCreateOffsetTable,
    E_SubSampling pSampleFactors,
    OFBool pWriteYBR422,
    OFBool pConvertToSC,
    size_t pWindowType,
    size_t pWindowParameter,
    double pVoiCenter,
    double pVoiWidth,
    size_t pRoiLeft,
    size_t pRoiTop,
    size_t pRoiWidth,
    size_t pRoiHeight,
    OFBool pUsePixelValues,
    OFBool pUseModalityRescale,
    OFBool pAcceptWrongPaletteTags,
    OFBool pAcrNemaCompatibility,
    OFBool pTrueLosslessMode)
: DcmCodecParameter()
, compressionCSConversion(pCompressionCSConversion)
, decompressionCSConversion(pDecompressionCSConversion)
, planarConfiguration(pPlanarConfiguration)
, optimizeHuffman(pOptimizeHuffman)
, smoothingFactor(pSmoothingFactor)
, forcedBitDepth(pForcedBitDepth)
, fragmentSize(pFragmentSize)
, createOffsetTable(pCreateOffsetTable)
, sampleFactors(pSampleFactors)
, writeYBR422(pWriteYBR422)
, convertToSC(pConvertToSC)
, uidCreation(pCreateSOPInstanceUID)
, windowType(pWindowType)
, windowParameter(pWindowParameter)
, voiCenter(pVoiCenter)
, voiWidth(pVoiWidth)
, roiLeft(pRoiLeft)
, roiTop(pRoiTop)
, roiWidth(pRoiWidth)
, roiHeight(pRoiHeight)
, usePixelValues(pUsePixelValues)
, useModalityRescale(pUseModalityRescale)
, acceptWrongPaletteTags(pAcceptWrongPaletteTags)
, acrNemaCompatibility(pAcrNemaCompatibility)
, trueLosslessMode(pTrueLosslessMode)
, predictor6WorkaroundEnabled_(predictor6WorkaroundEnable)
{
}


k2jParams::k2jParams(const k2jParams& arg)
: DcmCodecParameter(arg)
, compressionCSConversion(arg.compressionCSConversion)
, decompressionCSConversion(arg.decompressionCSConversion)
, planarConfiguration(arg.planarConfiguration)
, optimizeHuffman(arg.optimizeHuffman)
, smoothingFactor(arg.smoothingFactor)
, forcedBitDepth(arg.forcedBitDepth)
, fragmentSize(arg.fragmentSize)
, createOffsetTable(arg.createOffsetTable)
, sampleFactors(arg.sampleFactors)
, writeYBR422(arg.writeYBR422)
, convertToSC(arg.convertToSC)
, uidCreation(arg.uidCreation)
, windowType(arg.windowType)
, windowParameter(arg.windowParameter)
, voiCenter(arg.voiCenter)
, voiWidth(arg.voiWidth)
, roiLeft(arg.roiLeft)
, roiTop(arg.roiTop)
, roiWidth(arg.roiWidth)
, roiHeight(arg.roiHeight)
, usePixelValues(arg.usePixelValues)
, useModalityRescale(arg.useModalityRescale)
, acceptWrongPaletteTags(arg.acceptWrongPaletteTags)
, acrNemaCompatibility(arg.acrNemaCompatibility)
, trueLosslessMode(arg.trueLosslessMode)
, predictor6WorkaroundEnabled_(arg.predictor6WorkaroundEnabled_)
{
}

k2jParams::~k2jParams()
{
}

DcmCodecParameter *k2jParams::clone() const
{
  return new k2jParams(*this);
}

const char *k2jParams::className() const
{
  return "k2jParams";
}
