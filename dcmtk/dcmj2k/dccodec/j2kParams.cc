#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kParams.h"

j2kParams::j2kParams()
: DcmCodecParameter()
{
}


j2kParams::j2kParams(const j2kParams& arg)
: DcmCodecParameter(arg)
{
}

j2kParams::~j2kParams()
{
}

DcmCodecParameter *j2kParams::clone() const
{
  return new j2kParams(*this);
}

const char *j2kParams::className() const
{
  return "j2kParams";
}
