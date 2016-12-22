#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcpixel.h" /* for class DcmRepresentationParameter */
#include "djdefine.h"

class kdurRepresentationParameter : public DcmRepresentationParameter
{
public:

  /** constructor
   *  @param aPrediction prediction value
   *  @param aPt point transform value
   */
  kdurRepresentationParameter(int aPrediction=1, int aPt=0);

  /// copy constructor
  kdurRepresentationParameter(const kdurRepresentationParameter& arg);

  /// destructor
  virtual ~kdurRepresentationParameter();
  
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

  /** returns the prediction value
   *  @return prediction value
   */
  int getPrediction() const
  {
    return prediction;
  }

  /** returns the point transform 
   *  @return point transform 
   */
  int getPointTransformation() const 
  {
    return pt;
  }

private:

  /// prediction value
  int prediction; 

  /// point transform value
  int pt;
};
