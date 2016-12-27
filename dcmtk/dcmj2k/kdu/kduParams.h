#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcpixel.h" /* for class DcmRepresentationParameter */
#include "djdefine.h"

class kduParams: public DcmRepresentationParameter
{
public:

  /** constructor
   *  @param aQuality quality factor
   */
  kduParams(int aQuality=90);

  /// copy constructor
  kduParams(const kduParams& arg);

  /// destructor
  virtual ~kduParams();
  
  /** this methods creates a copy of type DcmRepresentationParameter   *  it must be overweritten in every subclass.
   *  @return copy of this object
   */
  virtual DcmRepresentationParameter *clone() const;

  /** returns the class name as string.
   *  can be used in operator== as poor man's RTTI replacement.
   */
  virtual const char *className() const;

  /** compares an object to another DcmRepresentationParameter.
   *  Implementation must make sure that classes are comparable.
   *  @param arg representation parameter to compare with
   *  @return true if equal, false otherwise.
   */
  virtual OFBool operator==(const DcmRepresentationParameter &arg) const;

  /** returns the compression quality factor
   *  @return compression quality factor
   */
  int getQuality() const
  {
    return quality;
  }

private:

  /// compression quality factor
  int quality;

};
