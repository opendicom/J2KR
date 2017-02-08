typedef int OPJ_BOOL;
#define OPJ_TRUE 1
#define OPJ_FALSE 0

typedef char          OPJ_CHAR;
typedef float         OPJ_FLOAT32;
typedef double        OPJ_FLOAT64;
typedef unsigned char OPJ_BYTE;

typedef int8_t   OPJ_INT8;
typedef uint8_t  OPJ_UINT8;
typedef int16_t  OPJ_INT16;
typedef uint16_t OPJ_UINT16;
typedef int32_t  OPJ_INT32;
typedef uint32_t OPJ_UINT32;
typedef int64_t  OPJ_INT64;
typedef uint64_t OPJ_UINT64;

typedef int64_t  OPJ_OFF_T; //64-bit file offset type

#include <stdio.h>
typedef size_t   OPJ_SIZE_T;

//Avoid compile-time warning because parameter is not used
#define OPJ_ARG_NOT_USED(x) (void)(x)

//==========================================================
//   Useful constant definitions
//==========================================================

#define OPJ_PATH_LEN 4096 // Maximum allowed size for filenames
#define OPJ_J2K_MAXRLVLS 33	//Number of maximum resolution level authorized
#define OPJ_J2K_MAXBANDS (3*OPJ_J2K_MAXRLVLS-2)	//Number of maximum sub-band linked to number of resolution level

#define OPJ_J2K_DEFAULT_NB_SEGS				10
#define OPJ_J2K_STREAM_CHUNK_SIZE			0x100000 //1 mega by default
#define OPJ_J2K_DEFAULT_HEADER_SIZE			1000
#define OPJ_J2K_MCC_DEFAULT_NB_RECORDS		10
#define OPJ_J2K_MCT_DEFAULT_NB_RECORDS		10

/*UniPG NOT YET USED IN THE V2 VERSION OF OPENJPEG

//increase at your will
#define JPWL_MAX_NO_TILESPECS	16 //Maximum number of tile parts expected by JPWL
#define JPWL_MAX_NO_PACKSPECS	16 //Maximum number of packet parts expected by JPWL
#define JPWL_MAX_NO_MARKERS	512 //Maximum number of JPWL markers
#define JPWL_PRIVATEINDEX_NAME "jpwl_index_privatefilename" //index file name used when JPWL is on
#define JPWL_EXPECTED_COMPONENTS 3 //Expect this number of components, so you'll find better the first EPB
#define JPWL_MAXIMUM_TILES 8192 //Expect this maximum number of tiles, to avoid some crashes
#define JPWL_MAXIMUM_HAMMING 2 //Expect this maximum number of bit errors in marker id's
#define JPWL_MAXIMUM_EPB_ROOM 65450 //Expect this maximum number of bytes for composition of EPBs
*/

/**
 * EXPERIMENTAL FOR THE MOMENT
 * Supported options about file information used only in j2k_dump

#define OPJ_IMG_INFO		1	//Basic image information provided to the user
#define OPJ_J2K_MH_INFO		2	//Codestream information based only on the main header
#define OPJ_J2K_TH_INFO		4	//Tile information based on the current tile header
#define OPJ_J2K_TCH_INFO	8	//Tile/Component information of all tiles
#define OPJ_J2K_MH_IND		16	//Codestream index based only on the main header
#define OPJ_J2K_TH_IND		32	//Tile index based on the current tile
//FIXME #define OPJ_J2K_CSTR_IND	48
#define OPJ_JP2_INFO		128	//JP2 file information
#define OPJ_JP2_IND			256	//JP2 file index
*/

/**
 * JPEG 2000 Profiles, see Table A.10 from 15444-1 (updated in various AMD)
 * These values help chosing the RSIZ value for the J2K codestream.
 * The RSIZ value triggers various encoding options, as detailed in Table A.10.
 * If OPJ_PROFILE_PART2 is chosen, it has to be combined with one or more extensions
 * described hereunder.
 *   Example: rsiz = OPJ_PROFILE_PART2 | OPJ_EXTENSION_MCT;
 * For broadcast profiles, the OPJ_PROFILE value has to be combined with the targeted
 * mainlevel (3-0 LSB, value between 0 and 11):
 *   Example: rsiz = OPJ_PROFILE_BC_MULTI | 0x0005; (here mainlevel 5)
 * For IMF profiles, the OPJ_PROFILE value has to be combined with the targeted mainlevel
 * (3-0 LSB, value between 0 and 11) and sublevel (7-4 LSB, value between 0 and 9):
 *   Example: rsiz = OPJ_PROFILE_IMF_2K | 0x0040 | 0x0005; (here main 5 and sublevel 4)
 * */
#define OPJ_PROFILE_NONE        0x0000 //no profile, conform to 15444-1
#define OPJ_PROFILE_0           0x0001 //Profile 0 as described in 15444-1,Table A.45
#define OPJ_PROFILE_1           0x0002 //Profile 1 as described in 15444-1,Table A.45
#define OPJ_PROFILE_PART2       0x8000 //At least 1 extension defined in 15444-2 (Part-2)
#define OPJ_PROFILE_CINEMA_2K   0x0003 //2K cinema profile defined in 15444-1 AMD1
#define OPJ_PROFILE_CINEMA_4K   0x0004 //4K cinema profile defined in 15444-1 AMD1
#define OPJ_PROFILE_CINEMA_S2K  0x0005 //Scalable 2K cinema profile defined in 15444-1 AMD2
#define OPJ_PROFILE_CINEMA_S4K  0x0006 //Scalable 4K cinema profile defined in 15444-1 AMD2
#define OPJ_PROFILE_CINEMA_LTS  0x0007 //Long term storage cinema profile defined in 15444-1 AMD2
#define OPJ_PROFILE_BC_SINGLE   0x0100 //Single Tile Broadcast profile defined in 15444-1 AMD3
#define OPJ_PROFILE_BC_MULTI    0x0200 //Multi Tile Broadcast profile defined in 15444-1 AMD3
#define OPJ_PROFILE_BC_MULTI_R  0x0300 //Multi Tile Reversible Broadcast profile defined in 15444-1 AMD3
#define OPJ_PROFILE_IMF_2K      0x0400 //2K Single Tile Lossy IMF profile defined in 15444-1 AMD 8
#define OPJ_PROFILE_IMF_4K      0x0401 //4K Single Tile Lossy IMF profile defined in 15444-1 AMD 8
#define OPJ_PROFILE_IMF_8K      0x0402 //8K Single Tile Lossy IMF profile defined in 15444-1 AMD 8
#define OPJ_PROFILE_IMF_2K_R    0x0403 //2K Single/Multi Tile Reversible IMF profile defined in 15444-1 AMD 8
#define OPJ_PROFILE_IMF_4K_R    0x0800 //4K Single/Multi Tile Reversible IMF profile defined in 15444-1 AMD 8
#define OPJ_PROFILE_IMF_8K_R    0x0801  //8K Single/Multi Tile Reversible IMF profile defined in 15444-1 AMD 8

//JPEG 2000 Part-2 extensions
#define OPJ_EXTENSION_NONE      0x0000 //No Part-2 extension
#define OPJ_EXTENSION_MCT       0x0100  //Custom MCT support

//JPEG 2000 profile macros
#define OPJ_IS_CINEMA(v)     (((v) >= OPJ_PROFILE_CINEMA_2K)&&((v) <= OPJ_PROFILE_CINEMA_S4K))
#define OPJ_IS_STORAGE(v)    ((v) == OPJ_PROFILE_CINEMA_LTS)
#define OPJ_IS_BROADCAST(v)  (((v) >= OPJ_PROFILE_BC_SINGLE)&&((v) <= ((OPJ_PROFILE_BC_MULTI_R) | (0x000b))))
#define OPJ_IS_IMF(v)        (((v) >= OPJ_PROFILE_IMF_2K)&&((v) <= ((OPJ_PROFILE_IMF_8K_R) | (0x009b))))
#define OPJ_IS_PART2(v)      ((v) & OPJ_PROFILE_PART2)

//JPEG 2000 codestream and component size limits in cinema profiles
#define OPJ_CINEMA_24_CS     1302083 //Maximum codestream length for 24fps
#define OPJ_CINEMA_48_CS     651041  //Maximum codestream length for 48fps
#define OPJ_CINEMA_24_COMP   1041666 //Maximum size per color component for 2K & 4K @ 24fps
#define OPJ_CINEMA_48_COMP   520833	 //Maximum size per color component for 2K @ 48fps

//==========================================================
//   enum definitions
//==========================================================
typedef enum CINEMA_MODE {
    OPJ_OFF = 0,			//Not Digital Cinema
    OPJ_CINEMA2K_24 = 1,	//2K Digital Cinema at 24 fps
    OPJ_CINEMA2K_48 = 2,	//2K Digital Cinema at 48 fps
    OPJ_CINEMA4K_24 = 3		//4K Digital Cinema at 24 fps
}OPJ_CINEMA_MODE;

//Supported image color spaces
typedef enum COLOR_SPACE {
    OPJ_CLRSPC_UNKNOWN = -1,	//not supported by the library
    OPJ_CLRSPC_UNSPECIFIED = 0,	//not specified in the codestream
    OPJ_CLRSPC_SRGB = 1,		//sRGB
    OPJ_CLRSPC_GRAY = 2,		//grayscale
    OPJ_CLRSPC_SYCC = 3,		//YUV
    OPJ_CLRSPC_EYCC = 4,        //e-YCC
    OPJ_CLRSPC_CMYK = 5         //CMYK
} OPJ_COLOR_SPACE;

//Supported codec
typedef void * opj_codec_t;
typedef enum CODEC_FORMAT {
	OPJ_CODEC_UNKNOWN = -1,	//place-holder
	OPJ_CODEC_J2K  = 0,		//JPEG-2000 codestream : read/write
	OPJ_CODEC_JPT  = 1,		//JPT-stream (JPEG 2000, JPIP) : read only
    OPJ_CODEC_JP2  = 2,		//JP2 file format : read/write
    OPJ_CODEC_JPP  = 3,		//JPP-stream (JPEG 2000, JPIP) : to be coded
    OPJ_CODEC_JPX  = 4		//JPX file format (JPEG 2000 Part-2) : to be coded
} OPJ_CODEC_FORMAT;
opj_codec_t* opj_create_compress(OPJ_CODEC_FORMAT format);//format=coder to select
opj_codec_t* opj_create_decompress(OPJ_CODEC_FORMAT format);//format=decoder to select
//Returns a handle to a compressor/decompressor if successful, returns NULL otherwise
void opj_destroy_codec(opj_codec_t * p_codec);

//Progression order
typedef enum PROG_ORDER {
    OPJ_PROG_UNKNOWN = -1,	//place-holder
    OPJ_LRCP = 0,			//layer-resolution-component-precinct order
    OPJ_RLCP = 1,			//resolution-layer-component-precinct order
    OPJ_RPCL = 2,			//resolution-precinct-component-layer order
    OPJ_PCRL = 3,			//precinct-component-resolution-layer order
    OPJ_CPRL = 4			//component-precinct-resolution-layer order
} OPJ_PROG_ORDER;
//Progression order changes
typedef struct opj_poc {
    
    OPJ_UINT32 resno0, compno0;//Resolution num start, Component num start, given by POC
    OPJ_UINT32 layno1, resno1, compno1;//Layer num end,Resolution num end, Component num end, given by POC
    OPJ_UINT32 layno0, precno0, precno1;//Layer num start,Precinct num start, Precinct num end
    OPJ_PROG_ORDER prg1,prg;//Progression order enum
    OPJ_CHAR progorder[5];//Progression order string
    OPJ_UINT32 tile;//Tile number
    OPJ_INT32 tx0,tx1,ty0,ty1;//Start and end values for Tile width and height
    OPJ_UINT32 layS, resS, compS, prcS;//Start value, initialised in pi_initialise_encode
    OPJ_UINT32 layE, resE, compE, prcE;//End value, initialised in pi_initialise_encode
    OPJ_UINT32 txS,txE,tyS,tyE,dx,dy;//Start and end values of Tile width and height, initialised in pi_initialise_encode
    OPJ_UINT32 lay_t, res_t, comp_t, prc_t,tx0_t,ty0_t;//Temporary values for Tile parts, initialised in pi_create_encode
} opj_poc_t;


typedef void (*opj_msg_callback) (const char *msg, void *client_data);//Client object where will be return the event message

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


typedef void * opj_codec_t;

/* 
==========================================================
   I/O stream typedef definitions
==========================================================
*/

/**
 * Stream open flags.
 * */
/** The stream was opened for reading. */
#define OPJ_STREAM_READ	OPJ_TRUE
/** The stream was opened for writing. */
#define OPJ_STREAM_WRITE OPJ_FALSE

/*
 * Callback function prototype for read function
 */
typedef OPJ_SIZE_T (* opj_stream_read_fn) (void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data) ;

/*
 * Callback function prototype for write function
 */
typedef OPJ_SIZE_T (* opj_stream_write_fn) (void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data) ;

/*
 * Callback function prototype for skip function
 */
typedef OPJ_OFF_T (* opj_stream_skip_fn) (OPJ_OFF_T p_nb_bytes, void * p_user_data) ;

/*
 * Callback function prototype for seek function
 */
typedef OPJ_BOOL (* opj_stream_seek_fn) (OPJ_OFF_T p_nb_bytes, void * p_user_data) ;

/*
 * Callback function prototype for free user data function
 */
typedef void (* opj_stream_free_user_data_fn) (void * p_user_data) ;

/*
 * JPEG2000 Stream.
 */
typedef void * opj_stream_t;

/* 
==========================================================
   image typedef definitions
==========================================================
*/

/**
 * Defines a single image component
 * */
typedef struct opj_image_comp {
	/** XRsiz: horizontal separation of a sample of ith component with respect to the reference grid */
	OPJ_UINT32 dx;
	/** YRsiz: vertical separation of a sample of ith component with respect to the reference grid */
	OPJ_UINT32 dy;
	/** data width */
	OPJ_UINT32 w;
	/** data height */
	OPJ_UINT32 h;
	/** x component offset compared to the whole image */
	OPJ_UINT32 x0;
	/** y component offset compared to the whole image */
	OPJ_UINT32 y0;
	/** precision */
	OPJ_UINT32 prec;
	/** image depth in bits */
	OPJ_UINT32 bpp;
	/** signed (1) / unsigned (0) */
	OPJ_UINT32 sgnd;
	/** number of decoded resolution */
	OPJ_UINT32 resno_decoded;
	/** number of division by 2 of the out image compared to the original size of image */
	OPJ_UINT32 factor;
	/** image component data */
	OPJ_INT32 *data;
  /** alpha channel */
  OPJ_UINT16 alpha;
} opj_image_comp_t;

/** 
 * Defines image data and characteristics
 * */
typedef struct opj_image {
	/** XOsiz: horizontal offset from the origin of the reference grid to the left side of the image area */
	OPJ_UINT32 x0;
	/** YOsiz: vertical offset from the origin of the reference grid to the top side of the image area */
	OPJ_UINT32 y0;
	/** Xsiz: width of the reference grid */
	OPJ_UINT32 x1;
	/** Ysiz: height of the reference grid */
	OPJ_UINT32 y1;
	/** number of components in the image */
	OPJ_UINT32 numcomps;
	/** color space: sRGB, Greyscale or YUV */
	OPJ_COLOR_SPACE color_space;
	/** image components */
	opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	OPJ_BYTE *icc_profile_buf;
	/** size of ICC profile */
	OPJ_UINT32 icc_profile_len;
} opj_image_t;


/**
 * Component parameters structure used by the opj_image_create function
 * */
typedef struct opj_image_comptparm {
	/** XRsiz: horizontal separation of a sample of ith component with respect to the reference grid */
	OPJ_UINT32 dx;
	/** YRsiz: vertical separation of a sample of ith component with respect to the reference grid */
	OPJ_UINT32 dy;
	/** data width */
	OPJ_UINT32 w;
	/** data height */
	OPJ_UINT32 h;
	/** x component offset compared to the whole image */
	OPJ_UINT32 x0;
	/** y component offset compared to the whole image */
	OPJ_UINT32 y0;
	/** precision */
	OPJ_UINT32 prec;
	/** image depth in bits */
	OPJ_UINT32 bpp;
	/** signed (1) / unsigned (0) */
	OPJ_UINT32 sgnd;
} opj_image_cmptparm_t;


/* 
==========================================================
   Information on the JPEG 2000 codestream
==========================================================
*/
/* QUITE EXPERIMENTAL FOR THE MOMENT */

/**
 * Index structure : Information concerning a packet inside tile
 * */
typedef struct opj_packet_info {
	/** packet start position (including SOP marker if it exists) */
	OPJ_OFF_T start_pos;
	/** end of packet header position (including EPH marker if it exists)*/
	OPJ_OFF_T end_ph_pos;
	/** packet end position */
	OPJ_OFF_T end_pos;
	/** packet distorsion */
	double disto;
} opj_packet_info_t;


/* UniPG>> */
/**
 * Marker structure
 * */
typedef struct opj_marker_info {
	/** marker type */
	unsigned short int type;
	/** position in codestream */
	OPJ_OFF_T pos;
	/** length, marker val included */
	int len;
} opj_marker_info_t;
/* <<UniPG */

/**
 * Index structure : Information concerning tile-parts
*/
typedef struct opj_tp_info {
	/** start position of tile part */
	int tp_start_pos;
	/** end position of tile part header */
	int tp_end_header;
	/** end position of tile part */
	int tp_end_pos;
	/** start packet of tile part */
	int tp_start_pack;
	/** number of packets of tile part */
	int tp_numpacks;
} opj_tp_info_t;

/**
 * Index structure : information regarding tiles
*/
typedef struct opj_tile_info {
	/** value of thresh for each layer by tile cfr. Marcela   */
	double *thresh;
	/** number of tile */
	int tileno;
	/** start position */
	int start_pos;
	/** end position of the header */
	int end_header;
	/** end position */
	int end_pos;
	/** precinct number for each resolution level (width) */
	int pw[33];
	/** precinct number for each resolution level (height) */
	int ph[33];
	/** precinct size (in power of 2), in X for each resolution level */
	int pdx[33];
	/** precinct size (in power of 2), in Y for each resolution level */
	int pdy[33];
	/** information concerning packets inside tile */
	opj_packet_info_t *packet;
	/** add fixed_quality */
	int numpix;
	/** add fixed_quality */
	double distotile;
  	/** number of markers */
	int marknum;
	/** list of markers */
	opj_marker_info_t *marker;
	/** actual size of markers array */
	int maxmarknum;
	/** number of tile parts */
	int num_tps;
	/** information concerning tile parts */
	opj_tp_info_t *tp;
} opj_tile_info_t;

/**
 * Index structure of the codestream
*/
typedef struct opj_codestream_info {
	/** maximum distortion reduction on the whole image (add for Marcela) */
	double D_max;
	/** packet number */
	int packno;
	/** writing the packet in the index with t2_encode_packets */
	int index_write;
	/** image width */
	int image_w;
	/** image height */
	int image_h;
	/** progression order */
	OPJ_PROG_ORDER prog;
	/** tile size in x */
	int tile_x;
	/** tile size in y */
	int tile_y;
	/** */
	int tile_Ox;
	/** */
	int tile_Oy;
	/** number of tiles in X */
	int tw;
	/** number of tiles in Y */
	int th;
	/** component numbers */
	int numcomps;
	/** number of layer */
	int numlayers;
	/** number of decomposition for each component */
	int *numdecompos;
/* UniPG>> */
	/** number of markers */
	int marknum;
	/** list of markers */
	opj_marker_info_t *marker;
	/** actual size of markers array */
	int maxmarknum;
/* <<UniPG */
	/** main header position */
	int main_head_start;
	/** main header position */
	int main_head_end;
	/** codestream's size */
	int codestream_size;
	/** information regarding tiles inside image */
	opj_tile_info_t *tile;
} opj_codestream_info_t;

/* <----------------------------------------------------------- */
/* new output management of the codestream information and index */

/**
 * Tile-component coding parameters information
 */
typedef struct opj_tccp_info
{
	/** component index */
	OPJ_UINT32 compno;
	/** coding style */
	OPJ_UINT32 csty;
	/** number of resolutions */
	OPJ_UINT32 numresolutions;
	/** code-blocks width */
	OPJ_UINT32 cblkw;
	/** code-blocks height */
	OPJ_UINT32 cblkh;
	/** code-block coding style */
	OPJ_UINT32 cblksty;
	/** discrete wavelet transform identifier */
	OPJ_UINT32 qmfbid;
	/** quantisation style */
	OPJ_UINT32 qntsty;
	/** stepsizes used for quantization */
	OPJ_UINT32 stepsizes_mant[OPJ_J2K_MAXBANDS];
	/** stepsizes used for quantization */
	OPJ_UINT32 stepsizes_expn[OPJ_J2K_MAXBANDS];
	/** number of guard bits */
	OPJ_UINT32 numgbits;
	/** Region Of Interest shift */
	OPJ_INT32 roishift;
	/** precinct width */
	OPJ_UINT32 prcw[OPJ_J2K_MAXRLVLS];
	/** precinct height */
	OPJ_UINT32 prch[OPJ_J2K_MAXRLVLS];
}
opj_tccp_info_t;

/**
 * Tile coding parameters information
 */
typedef struct opj_tile_v2_info {

	/** number (index) of tile */
	int tileno;
	/** coding style */
	OPJ_UINT32 csty;
	/** progression order */
	OPJ_PROG_ORDER prg;
	/** number of layers */
	OPJ_UINT32 numlayers;
	/** multi-component transform identifier */
	OPJ_UINT32 mct;

	/** information concerning tile component parameters*/
	opj_tccp_info_t *tccp_info;

} opj_tile_info_v2_t;

/**
 * Information structure about the codestream (FIXME should be expand and enhance)
 */
typedef struct opj_codestream_info_v2 {
	/* Tile info */
	/** tile origin in x = XTOsiz */
	OPJ_UINT32 tx0;
	/** tile origin in y = YTOsiz */
	OPJ_UINT32 ty0;
	/** tile size in x = XTsiz */
	OPJ_UINT32 tdx;
	/** tile size in y = YTsiz */
	OPJ_UINT32 tdy;
	/** number of tiles in X */
	OPJ_UINT32 tw;
	/** number of tiles in Y */
	OPJ_UINT32 th;

	/** number of components*/
	OPJ_UINT32 nbcomps;

	/** Default information regarding tiles inside image */
	opj_tile_info_v2_t m_default_tile_info;

	/** information regarding tiles inside image */
	opj_tile_info_v2_t *tile_info; /* FIXME not used for the moment */

} opj_codestream_info_v2_t;


/**
 * Index structure about a tile part
 */
typedef struct opj_tp_index {
	/** start position */
	OPJ_OFF_T start_pos;
	/** end position of the header */
	OPJ_OFF_T end_header;
	/** end position */
	OPJ_OFF_T end_pos;

} opj_tp_index_t;

/**
 * Index structure about a tile
 */
typedef struct opj_tile_index {
	/** tile index */
	OPJ_UINT32 tileno;

	/** number of tile parts */
	OPJ_UINT32 nb_tps;
	/** current nb of tile part (allocated)*/
	OPJ_UINT32 current_nb_tps;
	/** current tile-part index */
	OPJ_UINT32 current_tpsno;
	/** information concerning tile parts */
	opj_tp_index_t *tp_index;

	/* UniPG>> */ /* NOT USED FOR THE MOMENT IN THE V2 VERSION */
		/** number of markers */
		OPJ_UINT32 marknum;
		/** list of markers */
		opj_marker_info_t *marker;
		/** actual size of markers array */
		OPJ_UINT32 maxmarknum;
	/* <<UniPG */

	/** packet number */
	OPJ_UINT32 nb_packet;
	/** information concerning packets inside tile */
	opj_packet_info_t *packet_index;

} opj_tile_index_t;

/**
 * Index structure of the codestream (FIXME should be expand and enhance)
 */
typedef struct opj_codestream_index {
	/** main header start position (SOC position) */
	OPJ_OFF_T main_head_start;
	/** main header end position (first SOT position) */
	OPJ_OFF_T main_head_end;

	/** codestream's size */
	OPJ_UINT64 codestream_size;

/* UniPG>> */ /* NOT USED FOR THE MOMENT IN THE V2 VERSION */
	/** number of markers */
	OPJ_UINT32 marknum;
	/** list of markers */
	opj_marker_info_t *marker;
	/** actual size of markers array */
	OPJ_UINT32 maxmarknum;
/* <<UniPG */

	/** */
	OPJ_UINT32 nb_of_tiles;
	/** */
	opj_tile_index_t *tile_index; /* FIXME not used for the moment */

}opj_codestream_index_t;
/* -----------------------------------------------------------> */

/*
==========================================================
   Metadata from the JP2file
==========================================================
*/

/**
 * Info structure of the JP2 file
 * EXPERIMENTAL FOR THE MOMENT
 */
typedef struct opj_jp2_metadata {
	/** */
	OPJ_INT32	not_used;

} opj_jp2_metadata_t;

/**
 * Index structure of the JP2 file
 * EXPERIMENTAL FOR THE MOMENT
 */
typedef struct opj_jp2_index {
	/** */
	OPJ_INT32	not_used;

} opj_jp2_index_t;

typedef struct opj_buffer_info
{
    OPJ_BYTE *buf;
    OPJ_BYTE *cur;
    OPJ_SIZE_T len;
    
} opj_buffer_info_t;

