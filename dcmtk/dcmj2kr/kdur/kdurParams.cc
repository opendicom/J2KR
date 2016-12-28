#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/kdur/kdurParams.h"


kdurParams::kdurParams(int aPrediction, int aPt)
: DcmRepresentationParameter()
, prediction(aPrediction)
, pt(aPt)
{
}

kdurParams::kdurParams(const kdurParams& arg)
: DcmRepresentationParameter(arg)
, prediction(arg.prediction)
, pt(arg.pt)
{
}

kdurParams::~kdurParams()
{
}  

DcmRepresentationParameter *kdurParams::clone() const
{
  return new kdurParams(*this);
}

const char *kdurParams::className() const
{
  return "kdurParams";
}

OFBool kdurParams::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const kdurParams& argll = OFstatic_cast(const kdurParams&, arg);
      if ((prediction == argll.prediction) && (pt == argll.pt)) return OFTrue;
    }
  }
  return OFFalse;
}
