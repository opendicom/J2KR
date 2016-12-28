#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krParams.h"

j2krParams::j2krParams()
: DcmCodecParameter()
{
}


j2krParams::j2krParams(const j2krParams& arg)
: DcmCodecParameter(arg)
{
}

j2krParams::~j2krParams()
{
}

DcmCodecParameter *j2krParams::clone() const
{
  return new j2krParams(*this);
}

const char *j2krParams::className() const
{
  return "j2krParams";
}
