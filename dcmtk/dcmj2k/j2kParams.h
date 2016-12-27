#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dccodec.h" /* for DcmCodecParameter */
#include "djutils.h" /* for enums */

class j2kParams: public DcmCodecParameter
{
public:
  j2kParams();

  /// copy constructor
  j2kParams(const j2kParams& arg);

  /// destructor
  virtual ~j2kParams();

  /** this methods creates a copy of type DcmCodecParameter   *  it must be overweritten in every subclass.
   *  @return copy of this object
   */
  virtual DcmCodecParameter *clone() const;

  // returns the class name as string. Can be used as poor man's RTTI replacement.
  virtual const char *className() const;

    // returns forced bit depth for image compression, 0 (auto) or 8/12/16
    int getForcedBitDepth() const
    {
        return 0;
    }
    
    /** returns maximum fragment size (in kbytes) for compression, 0 for unlimited.*/
    Uint32 getFragmentSize() const
    {
        return 0;
    }
    
    /** returns offset table creation flag*/
    OFBool getCreateOffsetTable() const
    {
        return OFTrue;
    }
    
    /** returns flag indicating whether a compressed YBR color stream should be marked
     *  as YBR_FULL or YBR_FULL_422 on DICOM level*/
    OFBool getWriteYBR422() const
    {
        return OFFalse;
    }

    
    // returns mode for VOI transformation of monochrome images. Mode [0..6]
    //removed 7
    /*
     0  no VOI transformation: disable correction of polarity or any other presentation LUT transformation
     1  use the n-th VOI window from the image file
     2  use the n-th VOI look up table from the image file
     3  compute VOI window using min-max algorithm
     4  compute VOI window using Histogram algorithm, ignoring n percent
     5  compute VOI window using center r and width s
     6  compute VOI window using min-max algorithm ignoring extremes
     */
    size_t getWindowType() const
    {
        return 0;
    }

    
    // returns parameter for VOI transform of monochrome images, used in VOI modes 1, 2, 4, 6
    size_t getWindowParameter() const
    {
        return 0;
    }

    
    // returns VOI window center and width for VOI mode 5
    void getVOIWindow(double& center, double& width) const
    {
        center = 0.0;
        width = 0.0;
    }


    
    // returns color conversion mode for compression
    E_CompressionColorSpaceConversion getCompressionColorSpaceConversion() const
    {
        return ECC_monochrome;//[ECC_lossyYCbCr,ECC_lossyRGB,ECC_monochrome]
    }
    
    
    // returns flag indicating if compression to be optimized for real pixel values
    //OFBool opt_usePixelValues Check smallest, largest pixel value and optimize, mode 0 only
    OFBool getUsePixelValues() const
    {
        return OFTrue;
    }

    
    // returns flag indicating if compression should compute Rescale Slope/Intercept
    //OFBool opt_useModalityRescale to scale back, to original pixel range, mode 0 only
    OFBool getUseModalityRescale() const
    {
        return OFFalse;
    }

    
    // Returns flag indicating if incorrect palette attribute tags should be accepted
    OFBool getAcceptWrongPaletteTags() const
    {
        return OFFalse;
    }

private:

  /// private undefined copy assignment operator
  j2kParams& operator=(const j2kParams&);

};
