#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dcpixel/kduRepresentationParameter.h"

kduRepresentationParameter::kduRepresentationParameter(int aQuality)
: DcmRepresentationParameter()
, quality(aQuality)
{
}

kduRepresentationParameter::kduRepresentationParameter(const kduRepresentationParameter& arg)
: DcmRepresentationParameter(arg)
, quality(arg.quality)
{
}

kduRepresentationParameter::~kduRepresentationParameter()
{
}  

DcmRepresentationParameter *kduRepresentationParameter::clone() const
{
  return new kduRepresentationParameter(*this);
}

const char *kduRepresentationParameter::className() const
{
  return "kduRepresentationParameter";
}

OFBool kduRepresentationParameter::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const kduRepresentationParameter& argll = OFstatic_cast(const kduRepresentationParameter&, arg);
      if (quality == argll.quality) return OFTrue;
    }
  }
  return OFFalse;
}
