#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oftypes.h"  /* for OFBool */
#include "djutils.h"
#include "dcmtk/dcmk2j/dccodec/k2jParams.h"

class k2jParams;
class udk;

/** singleton class that registers decoders for all supported j2 processes.
 */
class k2jRegister
{
public:
  /** registers decoders for all supported j2 processes.
   *  If already registered, call is ignored unless cleanup() has
   *  been performed before.
   *  @param pCreateSOPInstanceUID flag indicating whether or not
   *    a new SOP Instance UID should be assigned upon decompression.
   *  @param pPlanarConfiguration flag indicating how planar configuration
   *    of color images should be encoded upon decompression.
   *  @param predictor6WorkaroundEnable enable workaround for buggy lossless compressed images with
   *           overflow in predictor 6 for images with 16 bits/pixel
   */
  static void registerCodecs(
    E_DecompressionColorSpaceConversion pDecompressionCSConversion = EDC_photometricInterpretation,
    E_UIDCreation pCreateSOPInstanceUID = EUC_default,
    E_PlanarConfiguration pPlanarConfiguration = EPC_default,
    OFBool predictor6WorkaroundEnable = OFFalse);

  /** deregisters decoders.
   *  Attention: Must not be called while other threads might still use
   *  the registered codecs, e.g. because they are currently decoding
   *  DICOM data sets through dcmdata.
   */
  static void cleanup();

//jf private:

  /// flag indicating whether the decoders are already registered.
  static OFBool registered;

  /// pointer to codec parameter shared by all decoders
  static k2jParams *cp;
    
  /// pointer to decoder for lossless JPEG
  static udk *dec2KLoL;

};
