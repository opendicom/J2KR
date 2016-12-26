#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2k/dccodec/j2kCodecParameter.h"

j2kCodecParameter::j2kCodecParameter()
: DcmCodecParameter()
{
}


j2kCodecParameter::j2kCodecParameter(const j2kCodecParameter& arg)
: DcmCodecParameter(arg)
{
}

j2kCodecParameter::~j2kCodecParameter()
{
}

DcmCodecParameter *j2kCodecParameter::clone() const
{
  return new j2kCodecParameter(*this);
}

const char *j2kCodecParameter::className() const
{
  return "j2kCodecParameter";
}
