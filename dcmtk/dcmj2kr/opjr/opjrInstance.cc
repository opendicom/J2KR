#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/opjr/opjrInstance.h"
#include "dcmtk/dcmj2kr/j2krParams.h"
#include "dcmtk/ofstd/ofconsol.h"
#include "dcmtk/ofstd/ofstdinc.h"

//JF#include "openjpeg.h"
//JF#include "convert.h"
/*JF
 added within openjpeg.h
 https://groups.google.com/forum/#!topic/openjpeg/Ggvuvw1mggE
 
 typedef struct opj_buffer_info {
 OPJ_BYTE* buf;
 OPJ_BYTE* cur;
 OPJ_SIZE_T len;
 } opj_buffer_info_t;
*/

#include "format_defs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>



typedef struct decode_info
{
    opj_codec_t *codec;
    opj_stream_t *stream;
    opj_image_t *image;
    opj_codestream_info_v2_t* cstr_info;
    opj_codestream_index_t* cstr_index;
    OPJ_BOOL deleteImage;
    
} decode_info_t;

#define JP2_RFC3745_MAGIC "\x00\x00\x00\x0c\x6a\x50\x20\x20\x0d\x0a\x87\x0a"
#define JP2_MAGIC "\x0d\x0a\x87\x0a"
/* position 45: "\xff\x52" */
#define J2K_CODESTREAM_MAGIC "\xff\x4f\xff\x51"

// Adapted from infile_format() in /src/bin/jp2/opj_decompress.c
static int buffer_format(void * buf)
{
    int magic_format;
    
    if (memcmp(buf, JP2_RFC3745_MAGIC, 12) == 0 || memcmp(buf, JP2_MAGIC, 4) == 0)
    {
        magic_format = JP2_CFMT;
    }
    else if (memcmp(buf, J2K_CODESTREAM_MAGIC, 4) == 0)
    {
        magic_format = J2K_CFMT;
    }
    else
    {
        return -1;
    }
    
    return magic_format;
}

const char *clr_space(OPJ_COLOR_SPACE i)
{
    if(i == OPJ_CLRSPC_SRGB) return "OPJ_CLRSPC_SRGB";
    if(i == OPJ_CLRSPC_GRAY) return "OPJ_CLRSPC_GRAY";
    if(i == OPJ_CLRSPC_SYCC) return "OPJ_CLRSPC_SYCC";
    if(i == OPJ_CLRSPC_UNKNOWN) return "OPJ_CLRSPC_UNKNOWN";
    return "CLRSPC_UNDEFINED";
}

void release(decode_info_t *decodeInfo)
{
    //*JF
    if(decodeInfo->codec) {
        opj_destroy_codec(decodeInfo->codec);
        decodeInfo->codec = NULL;
    }
    
    if(decodeInfo->stream) {
        opj_stream_destroy(decodeInfo->stream);
        decodeInfo->stream = NULL;
    }
    
    if(decodeInfo->deleteImage && decodeInfo->image) {
        opj_image_destroy(decodeInfo->image);
        decodeInfo->image = NULL;
    }
     //*/
}

// from "src/bin/jp2/opj_dump.c"
static void error_callback(const char *msg, void *client_data)
{
    (void)client_data;
    fprintf(stdout, "E: %s", msg);
}
static void warning_callback(const char *msg, void *client_data)
{
    (void)client_data;
    fprintf(stdout, "W: %s", msg);
}
static void info_callback(const char *msg, void *client_data)
{
    (void)client_data;
    fprintf(stdout, "D: %s", msg);
}

//---------------------------------------------------------------------
#pragma mark -

opjrInstance::opjrInstance(
                           const j2krParams& cp,
                           EJ_Mode mode,
                           Uint8 bitsPerSample
                           )
                            : DJEncoder()
                            , cparam(&cp)
                            , bitsPerSampleValue(bitsPerSample)
                            , modeofOperation(mode)
{}

opjrInstance::~opjrInstance()
{}


//imageBuffer 16bit - abstract class DJEncoder
OFCondition opjrInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: opjrInstance (16)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16);
}


//imageBuffer 8bit - abstract class DJEncoder
OFCondition opjrInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: opjrInstance (8)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 8);
}

//abstract class DJEncoder
Uint16 opjrInstance::bytesPerSample() const
{
	if( bitsPerSampleValue <= 8) return 1;
	else return 2;
}

//abstract class DJEncoder
Uint16 opjrInstance::bitsPerSample() const
{
	return bitsPerSampleValue;
}


E_TransferSyntax opjrInstance::supportedTransferSyntax()
{
    return EXS_JPEG2000LosslessOnly;
}

#pragma mark -
#pragma mark connection to openjpeg

template<typename T>
void rawtoimage_fill(T *inputbuffer, int w, int h, int numcomps, opj_image_t *image, int pc)
{
    T *p = inputbuffer;
    if( pc )
    {
        for(int compno = 0; compno < numcomps; compno++)
        {
            for (int i = 0; i < w * h; i++)
            {
                /* compno : 0 = GREY, (0, 1, 2) = (R, G, B) */
                image->comps[compno].data[i] = *p;
                ++p;
            }
        }
    }
    else
    {
        for (int i = 0; i < w * h; i++)
        {
            for(int compno = 0; compno < numcomps; compno++)
            {
                /* compno : 0 = GREY, (0, 1, 2) = (R, G, B) */
                image->comps[compno].data[i] = *p;
                ++p;
            }
        }
    }
}




static
opj_image_t* rawtoimage(
                        char *inputbuffer,
                        opj_cparameters_t *parameters,
                        int fragment_size,
                        int image_width,
                        int image_height,
                        int sample_pixel,
                        int bitsallocated,
                        int bitsstored,
                        int sign,
                        /*int quality,*/
                        int pc
                        )
{
    //(void)quality;
    int w, h;
    int numcomps;
    OPJ_COLOR_SPACE color_space;
    opj_image_cmptparm_t cmptparm[3]; /* maximum of 3 components */
    opj_image_t * image = NULL;
    
    assert( sample_pixel == 1 || sample_pixel == 3 );
    if( sample_pixel == 1 )
    {
        numcomps = 1;
        color_space = OPJ_CLRSPC_GRAY;
    }
    else // sample_pixel == 3
    {
        numcomps = 3;
        color_space = OPJ_CLRSPC_SRGB;
        /* Does OpenJPEG support: OPJ_CLRSPC_SYCC ?? */
    }
    if( bitsallocated % 8 != 0 )
    {
        return 0;
    }
    assert( bitsallocated % 8 == 0 );
    // eg. fragment_size == 63532 and 181 * 117 * 3 * 8 == 63531 ...
    assert( ((fragment_size + 1)/2 ) * 2 == ((image_height * image_width * numcomps * (bitsallocated/8) + 1)/ 2 )* 2 );
    int subsampling_dx = parameters->subsampling_dx;
    int subsampling_dy = parameters->subsampling_dy;
    
    // FIXME
    w = image_width;
    h = image_height;
    
    /* initialize image components */
    memset(&cmptparm[0], 0, 3 * sizeof(opj_image_cmptparm_t));
    //assert( bitsallocated == 8 );
    
    for(int i = 0; i < numcomps; i++)
    {
        cmptparm[i].prec = bitsstored;
        cmptparm[i].bpp = bitsallocated;
        cmptparm[i].sgnd = sign;
        cmptparm[i].dx = subsampling_dx;
        cmptparm[i].dy = subsampling_dy;
        cmptparm[i].w = w;
        cmptparm[i].h = h;
    }
    
    /* create the image */
    image = opj_image_create(numcomps, &cmptparm[0], color_space);
    if (!image)
    {
        return NULL;
    }
    
    /* set image offset and reference grid */
    image->x0 = parameters->image_offset_x0;
    image->y0 = parameters->image_offset_y0;
    image->x1 = parameters->image_offset_x0 + (w - 1) * subsampling_dx + 1;
    image->y1 = parameters->image_offset_y0 + (h - 1) * subsampling_dy + 1;
    
    /* set image data */
    
    //assert( fragment_size == numcomps*w*h*(bitsallocated/8) );
    if (bitsallocated <= 8)
    {
        if( sign )
        {
            rawtoimage_fill<int8_t>((int8_t*)inputbuffer,w,h,numcomps,image,pc);
        }
        else
        {
            rawtoimage_fill<uint8_t>((uint8_t*)inputbuffer,w,h,numcomps,image,pc);
        }
    }
    else if (bitsallocated <= 16)
    {
        if( sign )
        {
            rawtoimage_fill<int16_t>((int16_t*)inputbuffer,w,h,numcomps,image,pc);
        }
        else
        {
            rawtoimage_fill<uint16_t>((uint16_t*)inputbuffer,w,h,numcomps,image,pc);
        }
    }
    else if (bitsallocated <= 32)
    {
        if( sign )
        {
            rawtoimage_fill<int32_t>((int32_t*)inputbuffer,w,h,numcomps,image,pc);
        }
        else
        {
            rawtoimage_fill<uint32_t>((uint32_t*)inputbuffer,w,h,numcomps,image,pc);
        }
    }
    else
    {
        return NULL;
    }
    
    return image;
}


OFCondition opjrInstance::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 bitsAllocated)
{
    //*JF
#pragma mark parameters
    
    OPJ_BOOL bUseTiles = OPJ_FALSE; //OPJ_TRUE
    OPJ_UINT32 l_nb_tiles = 4;
    
    opj_cparameters_t parameters;
    memset(&parameters, 0, sizeof(parameters));
    opj_set_default_encoder_parameters(&parameters);
    parameters.outfile[0] = '\0';
    parameters.tcp_numlayers = 1;
    parameters.cp_disto_alloc = 1;
    parameters.tcp_rates[0] = 1;//layers of quality (one lossless in this case)
    parameters.cod_format = J2K_CFMT; //JP2_CFMT; //J2K_CFMT;//JF changed to J2K
    parameters.tcp_mct = (char) 255; //This will be set later according to the input image or the provided option

#pragma mark raw
    // raw_cp initialization
    raw_cparameters_t raw_cp;
    raw_cp.rawBitDepth = 0;
    raw_cp.rawComp = 0;
    raw_cp.rawComps = 0;
    raw_cp.rawHeight = 0;
    raw_cp.rawSigned = 0;
    raw_cp.rawWidth = 0;

#pragma mark image with parameters
    //rawtoimage(parameters.infile, &parameters, &raw_cp)
    opj_image_t *image = NULL;
    image = rawtoimage( (char*) image_buffer,
                       &parameters,
                       static_cast<int>( columns*rows*samplesPerPixel*bitsAllocated/8), // [data length], fragment_size
                       columns,
                       rows,
                       samplesPerPixel,
                       bitsAllocated,
                       16,//bitstored
                       0,//sign
                       //quality
                       0);
    if (image == NULL)
    {
        fprintf(stdout,"E: OPJSupport could not create opj_image_t\r\n");
        if (image) opj_image_destroy(image);
        length = 0;
        return EC_IllegalParameter;
    }
    fprintf(stdout,"D: opj_image_t created\r\n");
    
#pragma mark l_codec
    //see test_tile_encoder.c:232 and opj_compress.c:1746
    opj_codec_t* l_codec = 00;
    l_codec = opj_create_compress(OPJ_CODEC_J2K);//OPJ_CODEC_FORMAT
    if (!l_codec)
    {
        fprintf(stderr,"%s:%d:\n\tNO codec\n",__FILE__,__LINE__);
        if (l_codec) opj_destroy_codec(l_codec);
        if (image) opj_image_destroy(image);
        length = 0;
        return EC_IllegalParameter;
    }
    
    
    opj_set_info_handler(l_codec, info_callback, 00);
    opj_set_warning_handler(l_codec, warning_callback, 00);
    opj_set_error_handler(l_codec, error_callback, 00);
    
#pragma mark opj_setup_encoder
    if ( !opj_setup_encoder(l_codec, &parameters, image))
    {
        fprintf(stderr,"%s:%d:\n\topj_setup_encoder failed\n",__FILE__,__LINE__);
        if (l_codec) opj_destroy_codec(l_codec);
        if (image) opj_image_destroy(image);
        length = 0;
        return EC_IllegalParameter;
    }
    
    
    
#pragma mark l_stream creation
    //line 1848: l_stream = opj_stream_create_default_file_stream

    opj_buffer_info_t bufferInfo;
    bufferInfo.cur = bufferInfo.buf = (OPJ_BYTE *)image_buffer;
    bufferInfo.len = (OPJ_SIZE_T) rows * columns;
    
    opj_stream_t *l_stream = 00;
    l_stream = opj_stream_create_buffer_stream(&bufferInfo, OPJ_STREAM_WRITE);
    //printf("%p\n",bufferInfo.buf);
    //printf("%lu\n",bufferInfo.len);
    if (!l_stream)
    {
        fprintf(stderr,"%s:%d:\n\tstream creation failed\n",__FILE__,__LINE__);
        if (l_stream) opj_stream_destroy(l_stream);
        if (l_codec) opj_destroy_codec(l_codec);
        if (image) opj_image_destroy(image);
        length = 0;
        return EC_IllegalParameter;
    }

#pragma mark encode image
    if (! opj_start_compress(l_codec, image, l_stream))
        fprintf(stdout, "failed to encode image: opj_start_compress\n");
    else if (! opj_encode(l_codec, l_stream))
        fprintf(stdout, "failed to encode image: opj_encode\n");
    else if (! opj_end_compress(l_codec, l_stream))
        fprintf(stdout, "failed to encode image: opj_end_compress\n");
    else
    {
        printf("%p\n",bufferInfo.buf);
        printf("%lu\n",bufferInfo.len);
        //to=(unsigned char *) malloc(bufferInfo.len);
        //memcpy(to,l_stream,bufferInfo.len);
    }
    

    if (l_stream) opj_stream_destroy(l_stream);
    if (l_codec) opj_destroy_codec(l_codec);
    if (image) opj_image_destroy(image);
    remove(parameters.outfile);
    //*/
    return EC_Normal;


}

