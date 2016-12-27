#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/kdu/kduParams.h"


kduParams::kduParams(int aPrediction, int aPt)
: DcmRepresentationParameter()
, prediction(aPrediction)
, pt(aPt)
{
}

kduParams::kduParams(const kduParams& arg)
: DcmRepresentationParameter(arg)
, prediction(arg.prediction)
, pt(arg.pt)
{
}

kduParams::~kduParams()
{
}  

DcmRepresentationParameter *kduParams::clone() const
{
  return new kduParams(*this);
}

const char *kduParams::className() const
{
  return "kduParams";
}

OFBool kduParams::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const kduParams& argll = OFstatic_cast(const kduParams&, arg);
      if ((prediction == argll.prediction) && (pt == argll.pt)) return OFTrue;
    }
  }
  return OFFalse;
}
