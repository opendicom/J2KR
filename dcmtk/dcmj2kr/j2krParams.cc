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


void opj_set_default_encoder_parameters(opj_cparameters_t *parameters) {
    if(parameters) {
        memset(parameters, 0, sizeof(opj_cparameters_t));
        /* default coding parameters */
        parameters->cp_cinema = OPJ_OFF; /* DEPRECATED */
        parameters->rsiz = OPJ_PROFILE_NONE;
        parameters->max_comp_size = 0;
        parameters->numresolution = 6;
        parameters->cp_rsiz = OPJ_STD_RSIZ; /* DEPRECATED */
        parameters->cblockw_init = 64;
        parameters->cblockh_init = 64;
        parameters->prog_order = OPJ_LRCP;
        parameters->roi_compno = -1;		/* no ROI */
        parameters->subsampling_dx = 1;
        parameters->subsampling_dy = 1;
        parameters->tp_on = 0;
        parameters->decod_format = -1;
        parameters->cod_format = -1;
        parameters->tcp_rates[0] = 0;
        parameters->tcp_numlayers = 0;
        parameters->cp_disto_alloc = 0;
        parameters->cp_fixed_alloc = 0;
        parameters->cp_fixed_quality = 0;
        parameters->jpip_on = OPJ_FALSE;
    }
}
