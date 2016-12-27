#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/kdu90/kdu90Params.h"

kdu90Params::kdu90Params(int aQuality)
: DcmRepresentationParameter()
, quality(aQuality)
{
}

kdu90Params::kdu90Params(const kdu90Params& arg)
: DcmRepresentationParameter(arg)
, quality(arg.quality)
{
}

kdu90Params::~kdu90Params()
{
}  

DcmRepresentationParameter *kdu90Params::clone() const
{
  return new kdu90Params(*this);
}

const char *kdu90Params::className() const
{
  return "kdu90Params";
}

OFBool kdu90Params::operator==(const DcmRepresentationParameter &arg) const
{
  const char *argname = arg.className();
  if (argname)
  {
    OFString argstring(argname);
    if (argstring == className())
    {
      const kdu90Params& argll = OFstatic_cast(const kdu90Params&, arg);
      if (quality == argll.quality) return OFTrue;
    }
  }
  return OFFalse;
}
