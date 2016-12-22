#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dcpixel/kdurRepresentationParameter.h"


kdurRepresentationParameter::kdurRepresentationParameter(int aPrediction, int aPt)
: DcmRepresentationParameter()
, prediction(aPrediction)
, pt(aPt)
{
}

kdurRepresentationParameter::kdurRepresentationParameter(const kdurRepresentationParameter& arg)
: DcmRepresentationParameter(arg)
, prediction(arg.prediction)
, pt(arg.pt)
{
}

kdurRepresentationParameter::~kdurRepresentationParameter()
{
}  

DcmRepresentationParameter *kdurRepresentationParameter::clone() const
{
  return new kdurRepresentationParameter(*this);
}

const char *kdurRepresentationParameter::className() const
{
  return "kdurRepresentationParameter";
}

OFBool kdurRepresentationParameter::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const kdurRepresentationParameter& argll = OFstatic_cast(const kdurRepresentationParameter&, arg);
      if ((prediction == argll.prediction) && (pt == argll.pt)) return OFTrue;
    }
  }
  return OFFalse;
}
