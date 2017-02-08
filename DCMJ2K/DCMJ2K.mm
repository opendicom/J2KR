#include <Foundation/Foundation.h>
#include "openjpeg.h"

//subsampling
typedef struct raw_comp_cparameters {
    int dx;
    int dy;
} raw_comp_cparameters_t;

//raw
typedef struct raw_cparameters {
    int rawWidth;
    int rawHeight;
    int rawComp;
    int rawBitDepth;
    OPJ_BOOL rawSigned;
    raw_comp_cparameters_t *rawComps;
} raw_cparameters_t;



static opj_image_t* rawtoimage_common(
                                      const char *filename,
                                      opj_cparameters_t *parameters,
                                      raw_cparameters_t *raw_cp,
                                      OPJ_BOOL big_endian)
{
    NSLog(@"rawtoimge_common");
    int subsampling_dx = parameters->subsampling_dx;
    int subsampling_dy = parameters->subsampling_dy;
    
    FILE *f = NULL;
    int i, compno, numcomps, w, h;
    OPJ_COLOR_SPACE color_space;
    opj_image_cmptparm_t *cmptparm;
    opj_image_t * image = NULL;
    unsigned short ch;
    
    if((! (raw_cp->rawWidth & raw_cp->rawHeight & raw_cp->rawComp & raw_cp->rawBitDepth)) == 0)
    {
        fprintf(stderr,"\nError: invalid raw image parameters\n");
        fprintf(stderr,"Please use the Format option -F:\n");
        fprintf(stderr,"-F <width>,<height>,<ncomp>,<bitdepth>,{s,u}@<dx1>x<dy1>:...:<dxn>x<dyn>\n");
        fprintf(stderr,"If subsampling is omitted, 1x1 is assumed for all components\n");
        fprintf(stderr,"Example: -i image.raw -o image.j2k -F 512,512,3,8,u@1x1:2x2:2x2\n");
        fprintf(stderr,"         for raw 512x512 image with 4:2:0 subsampling\n");
        fprintf(stderr,"Aborting.\n");
        return NULL;
    }
    
    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open %s for reading !!\n", filename);
        fprintf(stderr,"Aborting\n");
        return NULL;
    }
    numcomps = raw_cp->rawComp;
    
    //FIXME ADE at this point, tcp_mct has not been properly set in calling function
    if (numcomps == 1) {
        color_space = OPJ_CLRSPC_GRAY;
    } else if ((numcomps >= 3) && (parameters->tcp_mct == 0)) {
        color_space = OPJ_CLRSPC_SYCC;
    } else if ((numcomps >= 3) && (parameters->tcp_mct != 2)) {
        color_space = OPJ_CLRSPC_SRGB;
    } else {
        color_space = OPJ_CLRSPC_UNKNOWN;
    }
    w = raw_cp->rawWidth;
    h = raw_cp->rawHeight;
    cmptparm = (opj_image_cmptparm_t*) calloc((OPJ_UINT32)numcomps,sizeof(opj_image_cmptparm_t));
    if (!cmptparm) {
        fprintf(stderr, "Failed to allocate image components parameters !!\n");
        fprintf(stderr,"Aborting\n");
        fclose(f);
        return NULL;
    }
    /* initialize image components */
    for(i = 0; i < numcomps; i++) {
        cmptparm[i].prec = (OPJ_UINT32)raw_cp->rawBitDepth;
        cmptparm[i].bpp = (OPJ_UINT32)raw_cp->rawBitDepth;
        cmptparm[i].sgnd = (OPJ_UINT32)raw_cp->rawSigned;
        cmptparm[i].dx = (OPJ_UINT32)(subsampling_dx * raw_cp->rawComps[i].dx);
        cmptparm[i].dy = (OPJ_UINT32)(subsampling_dy * raw_cp->rawComps[i].dy);
        cmptparm[i].w = (OPJ_UINT32)w;
        cmptparm[i].h = (OPJ_UINT32)h;
    }
    /* create the image */
    image = opj_image_create((OPJ_UINT32)numcomps, &cmptparm[0], color_space);
    free(cmptparm);
    if(!image) {
        fclose(f);
        return NULL;
    }
    /* set image offset and reference grid */
    image->x0 = (OPJ_UINT32)parameters->image_offset_x0;
    image->y0 = (OPJ_UINT32)parameters->image_offset_y0;
    image->x1 = (OPJ_UINT32)parameters->image_offset_x0 + (OPJ_UINT32)(w - 1) *	(OPJ_UINT32)subsampling_dx + 1;
    image->y1 = (OPJ_UINT32)parameters->image_offset_y0 + (OPJ_UINT32)(h - 1) * (OPJ_UINT32)subsampling_dy + 1;
    
    if(raw_cp->rawBitDepth <= 8)
    {
        unsigned char value = 0;
        for(compno = 0; compno < numcomps; compno++) {
            int nloop = (w*h)/(raw_cp->rawComps[compno].dx*raw_cp->rawComps[compno].dy);
            for (i = 0; i < nloop; i++) {
                if (!fread(&value, 1, 1, f)) {
                    fprintf(stderr,"Error reading raw file. End of file probably reached.\n");
                    opj_image_destroy(image);
                    fclose(f);
                    return NULL;
                }
                image->comps[compno].data[i] = raw_cp->rawSigned?(char)value:value;
            }
        }
    }
    else if(raw_cp->rawBitDepth <= 16)
    {
        unsigned short value;
        for(compno = 0; compno < numcomps; compno++) {
            int nloop = (w*h)/(raw_cp->rawComps[compno].dx*raw_cp->rawComps[compno].dy);
            for (i = 0; i < nloop; i++) {
                unsigned char temp1;
                unsigned char temp2;
                if (!fread(&temp1, 1, 1, f)) {
                    fprintf(stderr,"Error reading raw file. End of file probably reached.\n");
                    opj_image_destroy(image);
                    fclose(f);
                    return NULL;
                }
                if (!fread(&temp2, 1, 1, f)) {
                    fprintf(stderr,"Error reading raw file. End of file probably reached.\n");
                    opj_image_destroy(image);
                    fclose(f);
                    return NULL;
                }
                if( big_endian )
                {
                    value = (unsigned short)((temp1 << 8) + temp2);
                }
                else
                {
                    value = (unsigned short)((temp2 << 8) + temp1);
                }
                image->comps[compno].data[i] = raw_cp->rawSigned?(short)value:value;
            }
        }
    }
    else {
        fprintf(stderr,"OpenJPEG cannot encode raw components with bit depth higher than 16 bits.\n");
        opj_image_destroy(image);
        fclose(f);
        return NULL;
    }
    
    if (fread(&ch, 1, 1, f)) {
        fprintf(stderr,"Warning. End of raw file not reached... processing anyway\n");
    }
    fclose(f);
    
    return image;
}


#pragma mark-

static void error_callback(const char *msg, void *client_data) {
    (void)client_data;
    fprintf(stdout, "[ERROR] %s", msg);
}
static void warning_callback(const char *msg, void *client_data) {
    (void)client_data;
    fprintf(stdout, "[WARNING] %s", msg);
}
static void info_callback(const char *msg, void *client_data) {
    (void)client_data;
    fprintf(stdout, "[INFO] %s", msg);
}

int main(int argc, char **argv) {

#pragma mark params
    opj_cparameters_t parameters;
    opj_set_default_encoder_parameters(&parameters);

    /* arguments of -F
     <width>,
     <height>,
     <ncomp>,
     <bitdepth>,
     {s,u},
     [@<dx1>x<dy1>:...:<dxn>x<dyn>]
     
     If subsampling is omitted, 1x1 is assumed for all components
     
     Example: -i image.raw -o image.j2k -F 512,512,3,8,u@1x1:2x2:2x2
     for raw 512x512 image with 4:2:0 subsampling
     */

    raw_cparameters_t raw_cp;

    raw_cp.rawBitDepth = 16;
    raw_cp.rawWidth = 240;
    raw_cp.rawHeight = 512;
    raw_cp.rawSigned  = OPJ_FALSE;//OPJ_TRUE

    raw_cp.rawComp = 1;
    raw_cp.rawComps = (raw_comp_cparameters_t*) malloc(((OPJ_UINT32)(raw_cp.rawComp))*sizeof(raw_comp_cparameters_t));
    
    raw_cp.rawComps[0].dx = 1;
    raw_cp.rawComps[0].dy = 1;
    
    //if 3 comp as in the example
    /*
     raw_cp.rawComps[1].dx = 2;
     raw_cp.rawComps[1].dy = 2;
     raw_cp.rawComps[2].dx = 2;
     raw_cp.rawComps[2].dy = 2;
     */
    
#pragma mark -optional
    
    
    //rates/distorsion
    /*
     -r <compression ratio>,<compression ratio>,...
     Different compression ratios for successive layers.
     The rate specified for each quality level is the desired
     compression factor.
     Decreasing ratios required.
     Example: -r 20,10,1 means
     quality layer 1: compress 20x,
     quality layer 2: compress 10x
     quality layer 3: compress lossless
     */
    char *s = "20,10,1";
    parameters.tcp_numlayers = 0;
    while (sscanf(s, "%f", &parameters.tcp_rates[parameters.tcp_numlayers]) == 1) {
        parameters.tcp_numlayers++;
        while (*s && *s != ',') {
            s++;
        }
        if (!*s)
            break;
        s++;
    }
    parameters.cp_disto_alloc = 1;
    /* if no rate entered, lossless by default */
    if (parameters.tcp_numlayers == 0) {
        parameters.tcp_rates[0] = 0;	/* MOD antonin : losslessbug */
        parameters.tcp_numlayers++;
        parameters.cp_disto_alloc = 1;
    }
    
    
    if ((parameters.cp_disto_alloc || parameters.cp_fixed_alloc || parameters.cp_fixed_quality)
        && (!(parameters.cp_disto_alloc ^ parameters.cp_fixed_alloc ^ parameters.cp_fixed_quality))) {
        fprintf(stderr, "[ERROR] options -r -q and -f cannot be used together !!\n");
        return 1;
    }				/* mod fixed_quality */
    
    
    if((parameters.cp_tx0 > parameters.image_offset_x0) || (parameters.cp_ty0 > parameters.image_offset_y0)) {
        fprintf(stderr,
                "[ERROR] Tile offset dimension is unnappropriate --> TX0(%d)<=IMG_X0(%d) TYO(%d)<=IMG_Y0(%d) \n",
                parameters.cp_tx0, parameters.image_offset_x0, parameters.cp_ty0, parameters.image_offset_y0);
        return 1;
    }
    
    for (int i = 0; i < parameters.numpocs; i++) {
        if (parameters.POC[i].prg == -1) {
            fprintf(stderr,
                    "Unrecognized progression order in option -P (POC n %d) [LRCP, RLCP, RPCL, PCRL, CPRL] !!\n",
                    i + 1);
        }
    }
    
    


#pragma mark Encoding image

    opj_image_t *image = NULL;

    image = rawtoimage_common("/Users/jacquesfauquex/Downloads/1", &parameters, &raw_cp,OPJ_FALSE);
    if (!image) {
            fprintf(stderr, "Unable to load raw file\n");
            return 1;
    }

    // Decide if MCT should be used
    parameters.tcp_mct = (char) 255;
    parameters.tcp_mct = (image->numcomps >= 3) ? 1 : 0;

    
    //encode the destination image
    opj_codec_t* l_codec = opj_create_compress(OPJ_CODEC_J2K);//OPJ_CODEC_JP2

    
    /* catch events using our callbacks and give a local context */
    opj_set_info_handler(l_codec, info_callback,00);
    opj_set_warning_handler(l_codec, warning_callback,00);
    opj_set_error_handler(l_codec, error_callback,00);

    
    
    if (! opj_setup_encoder(l_codec, &parameters, image)) {
        fprintf(stderr, "failed to encode image: opj_setup_encoder\n");
        opj_destroy_codec(l_codec);
        opj_image_destroy(image);
        return 1;
    }

#pragma mark stream to encode the image
    opj_stream_t *l_stream = 00;
    l_stream = opj_stream_create_default_file_stream("/Users/jacquesfauquex/Downloads/1.j2k",OPJ_FALSE);
    if (l_stream)
    {
        NSDate *start=[NSDate date];
        if (!opj_start_compress(l_codec,image,l_stream))
            fprintf(stderr, "failed to encode image: opj_start_compress\n");
        else if (!opj_encode(l_codec, l_stream))
            fprintf(stderr, "failed to encode image: opj_encode\n");
        else if (!opj_end_compress(l_codec, l_stream))
        {
            fprintf(stderr, "failed to encode image: opj_end_compress\n");
            fprintf(stderr, "failed to encode image\n");
            remove(parameters.outfile);
            return 1;
        }
        else fprintf(stdout, "encode time: %f s\n", [[NSDate date]timeIntervalSinceDate:start]);

        opj_stream_destroy(l_stream);
    }
    opj_destroy_codec(l_codec);
    opj_image_destroy(image);


    /* free user parameters structure */
    if(parameters.cp_comment)   free(parameters.cp_comment);
    if(parameters.cp_matrice)   free(parameters.cp_matrice);
    if(raw_cp.rawComps) free(raw_cp.rawComps);
    return 0;
}
