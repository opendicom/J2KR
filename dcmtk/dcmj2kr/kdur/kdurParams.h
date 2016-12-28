#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcpixel.h" /* for class DcmRepresentationParameter */
#include "djdefine.h"

class kdurParams : public DcmRepresentationParameter
{
public:

  /** constructor
   *  @param aPrediction prediction value
   *  @param aPt point transform value
   */
  kdurParams(int aPrediction=1, int aPt=0);

  /// copy constructor
  kdurParams(const kdurParams& arg);

  /// destructor
  virtual ~kdurParams();
  
  // this methods creates a copy of abstract type DcmRepresentationParameter*.
  // every subclasses must be overweritten

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

  int getPrediction() const
  {
    return prediction;
  }
   */
    
  /** returns the point transform 
   *  @return point transform 

  int getPointTransformation() const 
  {
    return pt;
  }
   */
private:

  /// prediction value
  int prediction;

  /// point transform value
  int pt;
};
