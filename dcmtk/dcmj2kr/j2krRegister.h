#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oftypes.h"  /* for OFBool */
#include "dcmtk/dcmdata/dctypes.h"  /* for Uint32 */
#include "djutils.h"
#include "dcmtk/dcmj2kr/j2krParams.h"

class j2krParams;
class kdur;
class kdurParams;
class opjr;
class opjrParams;

// singleton class registering all j2kr processes.

class j2krRegister
{
public:
  static void registerCodecs(
    //posibility of defining params
    );

  /** deregisters encoders.
   *  Attention: Must not be called while other threads might still use
   *  the registered codecs, e.g. because they are currently encoding
   *  DICOM data sets through dcmdata.
   */
  static void cleanup();

//JF private:

  /// flag indicating whether the encoders are already registered.
  static OFBool registered;

  /// pointer to codec parameter shared by all encoders
  static j2krParams *tsParams;
    
  /// pointer to encoder for JPEG 2K LossLess
  static kdur *kakaduReversible;
  static kdurParams *kakaduParams;
  static opjr *openjpegReversible;
  static opjrParams *openjpegParams;
};
