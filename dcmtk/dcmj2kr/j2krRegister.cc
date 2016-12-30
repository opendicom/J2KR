#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/j2krRegister.h"

#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmtk/dcmj2kr/kdur/kdur.h"
#include "dcmtk/dcmj2kr/kdur/kdurParams.h"
#include "dcmtk/dcmj2kr/opjr/opjr.h"
#include "dcmtk/dcmj2kr/opjr/opjrParams.h"

// initialization of static members
OFBool j2krRegister::registered           = OFFalse;
j2krParams *j2krRegister::tsParams        = NULL;
kdur *j2krRegister::kakaduReversible      = NULL;
opjr *j2krRegister::openjpegReversible    = NULL;
kdurParams *j2krRegister::kakaduParams    = NULL;
opjrParams *j2krRegister::openjpegParams  = NULL;

void j2krRegister::registerCodecs()
{
  if (! registered)
  {
    tsParams = new j2krParams();
    if (tsParams)
    {
        openjpegReversible = new opjr();
        openjpegParams = new opjrParams();
        if (openjpegReversible) DcmCodecList::registerCodec(openjpegReversible, openjpegParams, tsParams);

        kakaduReversible = new kdur();
        kakaduParams = new kdurParams();
        if (kakaduReversible) DcmCodecList::registerCodec(kakaduReversible, kakaduParams, tsParams);
        
      registered = OFTrue;
    }
  }
}

void j2krRegister::cleanup()
{
  if (registered)
  {
    DcmCodecList::deregisterCodec(kakaduReversible);
    delete kakaduReversible;
    kakaduReversible = NULL;
    delete kakaduParams;
    kakaduParams = NULL;
      
    DcmCodecList::deregisterCodec(openjpegReversible);
    delete openjpegReversible;
    openjpegReversible = NULL;
    delete openjpegParams;
    openjpegParams = NULL;
      
    delete tsParams;
    tsParams     = NULL;

    registered = OFFalse;
  }
}
