#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/opjr/opjrParams.h"


opjrParams::opjrParams(int aPrediction, int aPt)
: DcmRepresentationParameter()
, prediction(aPrediction)
, pt(aPt)
{
}

opjrParams::opjrParams(const opjrParams& arg)
: DcmRepresentationParameter(arg)
, prediction(arg.prediction)
, pt(arg.pt)
{
}

opjrParams::~opjrParams()
{
}  

DcmRepresentationParameter *opjrParams::clone() const
{
  return new opjrParams(*this);
}

const char *opjrParams::className() const
{
  return "opjrParams";
}

OFBool opjrParams::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const opjrParams& argll = OFstatic_cast(const opjrParams&, arg);
      if ((prediction == argll.prediction) && (pt == argll.pt)) return OFTrue;
    }
  }
  return OFFalse;
}
