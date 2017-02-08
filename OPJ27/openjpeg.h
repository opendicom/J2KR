 /*
 * The copyright in this software is being made available under the 2-clauses 
 * BSD License, included below. This software may be subject to other third 
 * party and contributor rights, including patent rights, and no such rights
 * are granted under this license.
 *
 * Copyright (c) 2002-2014, Universite catholique de Louvain (UCL), Belgium
 * Copyright (c) 2002-2014, Professor Benoit Macq
 * Copyright (c) 2001-2003, David Janssens
 * Copyright (c) 2002-2003, Yannick Verschueren
 * Copyright (c) 2003-2007, Francois-Olivier Devaux 
 * Copyright (c) 2003-2014, Antonin Descampe
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
 * Copyright (c) 2006-2007, Parvatha Elangovan
 * Copyright (c) 2008, Jerome Fimes, Communications & Systemes <jerome.fimes@c-s.fr>
 * Copyright (c) 2010-2011, Kaori Hagihara
 * Copyright (c) 2011-2012, Centre National d'Etudes Spatiales (CNES), France 
 * Copyright (c) 2012, CS Systemes d'Information, France
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef OPENJPEG_H
#define OPENJPEG_H


/* 
==========================================================
   Compiler directives
==========================================================
*/

/*
The inline keyword is supported by C99 but not by C90. 
Most compilers implement their own version of this keyword ... 
*/
#ifndef INLINE
	#if defined(_MSC_VER)
		#define INLINE __forceinline
	#elif defined(__GNUC__)
		#define INLINE __inline__
	#elif defined(__MWERKS__)
		#define INLINE inline
	#else 
		/* add other compilers here ... */
		#define INLINE 
	#endif /* defined(<Compiler>) */
#endif /* INLINE */

/* deprecated attribute */
#ifdef __GNUC__
	#define OPJ_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
	#define OPJ_DEPRECATED(func) __declspec(deprecated) func
#else
	#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
	#define OPJ_DEPRECATED(func) func
#endif

#if defined(OPJ_STATIC) || !defined(_WIN32)
/* http://gcc.gnu.org/wiki/Visibility */
#	if __GNUC__ >= 4
#		if defined(OPJ_STATIC) /* static library uses "hidden" */
#			define OPJ_API    __attribute__ ((visibility ("hidden")))
#		else
#			define OPJ_API    __attribute__ ((visibility ("default")))
#		endif
#		define OPJ_LOCAL  __attribute__ ((visibility ("hidden")))
#	else
#		define OPJ_API
#		define OPJ_LOCAL
#	endif
#	define OPJ_CALLCONV
#else
#	define OPJ_CALLCONV __stdcall
/*
The following ifdef block is the standard way of creating macros which make exporting 
from a DLL simpler. All files within this DLL are compiled with the OPJ_EXPORTS
symbol defined on the command line. this symbol should not be defined on any project
that uses this DLL. This way any other project whose source files include this file see 
OPJ_API functions as being imported from a DLL, whereas this DLL sees symbols
defined with this macro as being exported.
*/
#	if defined(OPJ_EXPORTS) || defined(DLL_EXPORT)
#		define OPJ_API __declspec(dllexport)
#	else
#		define OPJ_API __declspec(dllimport)
#	endif /* OPJ_EXPORTS */
#endif /* !OPJ_STATIC || !_WIN32 */




/* 
==========================================================
   event manager typedef definitions
==========================================================
*/

/**
 * Callback function prototype for events
 * @param msg               Event message
 * @param client_data       Client object where will be return the event message 
 * */
typedef void (*opj_msg_callback) (const char *msg, void *client_data);

/* 
==========================================================
   codec typedef definitions
==========================================================
*/


/**
 * Creates a J2K/JP2 compression structure
 * @param 	format 		Coder to select
 * @return
 */
OPJ_API opj_codec_t* OPJ_CALLCONV opj_create_compress(OPJ_CODEC_FORMAT format);//format=coder to select
OPJ_API opj_codec_t* OPJ_CALLCONV opj_create_decompress(OPJ_CODEC_FORMAT format);//format=decoder to select
//Returns a handle to a compressor/decompressor if successful, returns NULL otherwise
OPJ_API void OPJ_CALLCONV opj_destroy_codec(opj_codec_t * p_codec);


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


#ifdef __cplusplus
extern "C" {
#endif

OPJ_API opj_stream_t* OPJ_CALLCONV opj_stream_create_buffer_stream(opj_buffer_info_t* p_source_buffer,OPJ_BOOL p_is_read_stream);

/* 
==========================================================
   openjpeg version
==========================================================
*/

/* Get the version of the openjpeg library*/
OPJ_API const char * OPJ_CALLCONV opj_version(void);

/* 
==========================================================
   image functions definitions
==========================================================
*/

/**
 * Create an image
 *
 * @param numcmpts      number of components
 * @param cmptparms     components parameters
 * @param clrspc        image color space
 * @return returns      a new image structure if successful, returns NULL otherwise
 * */
OPJ_API opj_image_t* OPJ_CALLCONV opj_image_create(OPJ_UINT32 numcmpts, opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc);

/**
 * Deallocate any resources associated with an image
 *
 * @param image         image to be destroyed
 */
OPJ_API void OPJ_CALLCONV opj_image_destroy(opj_image_t *image);

/**
 * Creates an image without allocating memory for the image (used in the new version of the library).
 *
 * @param	numcmpts    the number of components
 * @param	cmptparms   the components parameters
 * @param	clrspc      the image color space
 *
 * @return	a new image structure if successful, NULL otherwise.
*/
OPJ_API opj_image_t* OPJ_CALLCONV opj_image_tile_create(OPJ_UINT32 numcmpts, opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc);

/* 
==========================================================
   stream functions definitions
==========================================================
*/

/**
 * Creates an abstract stream. This function does nothing except allocating memory and initializing the abstract stream.
 *
 * @param	p_is_input		if set to true then the stream will be an input stream, an output stream else.
 *
 * @return	a stream object.
*/
OPJ_API opj_stream_t* OPJ_CALLCONV opj_stream_default_create(OPJ_BOOL p_is_input);

/**
 * Creates an abstract stream. This function does nothing except allocating memory and initializing the abstract stream.
 *
 * @param	p_buffer_size  FIXME DOC
 * @param	p_is_input		if set to true then the stream will be an input stream, an output stream else.
 *
 * @return	a stream object.
*/
OPJ_API opj_stream_t* OPJ_CALLCONV opj_stream_create(OPJ_SIZE_T p_buffer_size, OPJ_BOOL p_is_input);

/**
 * Destroys a stream created by opj_create_stream. This function does NOT close the abstract stream. If needed the user must
 * close its own implementation of the stream.
 *
 * @param	p_stream	the stream to destroy.
 */
OPJ_API void OPJ_CALLCONV opj_stream_destroy(opj_stream_t* p_stream);
 
/**
 * Sets the given function to be used as a read function.
 * @param		p_stream	the stream to modify
 * @param		p_function	the function to use a read function.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_read_function(opj_stream_t* p_stream, opj_stream_read_fn p_function);

/**
 * Sets the given function to be used as a write function.
 * @param		p_stream	the stream to modify
 * @param		p_function	the function to use a write function.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_write_function(opj_stream_t* p_stream, opj_stream_write_fn p_function);

/**
 * Sets the given function to be used as a skip function.
 * @param		p_stream	the stream to modify
 * @param		p_function	the function to use a skip function.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_skip_function(opj_stream_t* p_stream, opj_stream_skip_fn p_function);

/**
 * Sets the given function to be used as a seek function, the stream is then seekable.
 * @param		p_stream	the stream to modify
 * @param		p_function	the function to use a skip function.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_seek_function(opj_stream_t* p_stream, opj_stream_seek_fn p_function);

/**
 * Sets the given data to be used as a user data for the stream.
 * @param		p_stream	the stream to modify
 * @param		p_data		the data to set.
 * @param		p_function	the function to free p_data when opj_stream_destroy() is called.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_user_data (opj_stream_t* p_stream, void * p_data, opj_stream_free_user_data_fn p_function);

/**
 * Sets the length of the user data for the stream.
 *
 * @param p_stream    the stream to modify
 * @param data_length length of the user_data.
*/
OPJ_API void OPJ_CALLCONV opj_stream_set_user_data_length(opj_stream_t* p_stream, OPJ_UINT64 data_length);

/**
 * Create a stream from a file identified with its filename with default parameters (helper function)
 * @param fname             the filename of the file to stream
 * @param p_is_read_stream  whether the stream is a read stream (true) or not (false)
*/
OPJ_API opj_stream_t* OPJ_CALLCONV opj_stream_create_default_file_stream (const char *fname, OPJ_BOOL p_is_read_stream);
 
/** Create a stream from a file identified with its filename with a specific buffer size
 * @param fname             the filename of the file to stream
 * @param p_buffer_size     size of the chunk used to stream
 * @param p_is_read_stream  whether the stream is a read stream (true) or not (false)
*/
OPJ_API opj_stream_t* OPJ_CALLCONV opj_stream_create_file_stream (const char *fname,
                                                                     OPJ_SIZE_T p_buffer_size,
                                                                     OPJ_BOOL p_is_read_stream);
 
/* 
==========================================================
   event manager functions definitions
==========================================================
*/
/**
 * Set the info handler use by openjpeg.
 * @param p_codec       the codec previously initialise
 * @param p_callback    the callback function which will be used
 * @param p_user_data   client object where will be returned the message
*/
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_info_handler(opj_codec_t * p_codec, 
                                                   opj_msg_callback p_callback,
                                                   void * p_user_data);
/**
 * Set the warning handler use by openjpeg.
 * @param p_codec       the codec previously initialise
 * @param p_callback    the callback function which will be used
 * @param p_user_data   client object where will be returned the message
*/
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_warning_handler(opj_codec_t * p_codec,
                                                      opj_msg_callback p_callback,
                                                      void * p_user_data);
/**
 * Set the error handler use by openjpeg.
 * @param p_codec       the codec previously initialise
 * @param p_callback    the callback function which will be used
 * @param p_user_data   client object where will be returned the message
*/
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_error_handler(opj_codec_t * p_codec, 
                                                    opj_msg_callback p_callback,
                                                    void * p_user_data);

/* 
==========================================================
   codec functions definitions
==========================================================
*/


/**
 * Read after the codestream if necessary
 * @param	p_codec			the JPEG2000 codec to read.
 * @param	p_stream		the JPEG2000 stream.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_end_decompress (	opj_codec_t *p_codec,
													opj_stream_t *p_stream);


/**
 * Set decoding parameters to default values
 * @param parameters Decompression parameters
 */
OPJ_API void OPJ_CALLCONV opj_set_default_decoder_parameters(opj_dparameters_t *parameters);

/**
 * Setup the decoder with decompression parameters provided by the user and with the message handler
 * provided by the user.
 *
 * @param p_codec 		decompressor handler
 * @param parameters 	decompression parameters
 *
 * @return true			if the decoder is correctly set
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_setup_decoder(opj_codec_t *p_codec,
												opj_dparameters_t *parameters );

/**
 * Decodes an image header.
 *
 * @param	p_stream		the jpeg2000 stream.
 * @param	p_codec			the jpeg2000 codec to read.
 * @param	p_image			the image structure initialized with the characteristics of encoded image.
 *
 * @return true				if the main header of the codestream and the JP2 header is correctly read.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_read_header (	opj_stream_t *p_stream,
												opj_codec_t *p_codec,
												opj_image_t **p_image);

/**
 * Sets the given area to be decoded. This function should be called right after opj_read_header and before any tile header reading.
 *
 * @param	p_codec			the jpeg2000 codec.
 * @param	p_image         the decoded image previously setted by opj_read_header
 * @param	p_start_x		the left position of the rectangle to decode (in image coordinates).
 * @param	p_end_x			the right position of the rectangle to decode (in image coordinates).
 * @param	p_start_y		the up position of the rectangle to decode (in image coordinates).
 * @param	p_end_y			the bottom position of the rectangle to decode (in image coordinates).
 *
 * @return	true			if the area could be set.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_decode_area(	opj_codec_t *p_codec,
													opj_image_t* p_image,
													OPJ_INT32 p_start_x, OPJ_INT32 p_start_y,
													OPJ_INT32 p_end_x, OPJ_INT32 p_end_y );

/**
 * Decode an image from a JPEG-2000 codestream
 *
 * @param p_decompressor 	decompressor handle
 * @param p_stream			Input buffer stream
 * @param p_image 			the decoded image
 * @return 					true if success, otherwise false
 * */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_decode(   opj_codec_t *p_decompressor,
                                            opj_stream_t *p_stream,
                                            opj_image_t *p_image);

/**
 * Get the decoded tile from the codec
 *
 * @param	p_codec			the jpeg2000 codec.
 * @param	p_stream		input streamm
 * @param	p_image			output image
 * @param	tile_index		index of the tile which will be decode
 *
 * @return					true if success, otherwise false
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_get_decoded_tile(	opj_codec_t *p_codec,
													opj_stream_t *p_stream,
													opj_image_t *p_image,
													OPJ_UINT32 tile_index);

/**
 * Set the resolution factor of the decoded image
 * @param	p_codec			the jpeg2000 codec.
 * @param	res_factor		resolution factor to set
 *
 * @return					true if success, otherwise false
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_decoded_resolution_factor(opj_codec_t *p_codec, OPJ_UINT32 res_factor);

/**
 * Writes a tile with the given data.
 *
 * @param	p_codec		        the jpeg2000 codec.
 * @param	p_tile_index		the index of the tile to write. At the moment, the tiles must be written from 0 to n-1 in sequence.
 * @param	p_data				pointer to the data to write. Data is arranged in sequence, data_comp0, then data_comp1, then ... NO INTERLEAVING should be set.
 * @param	p_data_size			this value os used to make sure the data being written is correct. The size must be equal to the sum for each component of 
 *                              tile_width * tile_height * component_size. component_size can be 1,2 or 4 bytes, depending on the precision of the given component.
 * @param	p_stream			the stream to write data to.
 *
 * @return	true if the data could be written.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_write_tile (	opj_codec_t *p_codec,
												OPJ_UINT32 p_tile_index,
												OPJ_BYTE * p_data,
												OPJ_UINT32 p_data_size,
												opj_stream_t *p_stream );

/**
 * Reads a tile header. This function is compulsory and allows one to know the size of the tile that will be decoded.
 * The user may need to refer to the image got by opj_read_header to understand the size being taken by the tile.
 *
 * @param	p_codec			the jpeg2000 codec.
 * @param	p_tile_index	pointer to a value that will hold the index of the tile being decoded, in case of success.
 * @param	p_data_size		pointer to a value that will hold the maximum size of the decoded data, in case of success. In case
 *							of truncated codestreams, the actual number of bytes decoded may be lower. The computation of the size is the same
 *							as depicted in opj_write_tile.
 * @param	p_tile_x0		pointer to a value that will hold the x0 pos of the tile (in the image).
 * @param	p_tile_y0		pointer to a value that will hold the y0 pos of the tile (in the image).
 * @param	p_tile_x1		pointer to a value that will hold the x1 pos of the tile (in the image).
 * @param	p_tile_y1		pointer to a value that will hold the y1 pos of the tile (in the image).
 * @param	p_nb_comps		pointer to a value that will hold the number of components in the tile.
 * @param	p_should_go_on	pointer to a boolean that will hold the fact that the decoding should go on. In case the
 *							codestream is over at the time of the call, the value will be set to false. The user should then stop
 *							the decoding.
 * @param	p_stream		the stream to decode.
 * @return	true			if the tile header could be decoded. In case the decoding should end, the returned value is still true.
 *							returning false may be the result of a shortage of memory or an internal error.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_read_tile_header(	opj_codec_t *p_codec,
												opj_stream_t * p_stream,
												OPJ_UINT32 * p_tile_index,
												OPJ_UINT32 * p_data_size,
												OPJ_INT32 * p_tile_x0, OPJ_INT32 * p_tile_y0,
												OPJ_INT32 * p_tile_x1, OPJ_INT32 * p_tile_y1,
												OPJ_UINT32 * p_nb_comps,
												OPJ_BOOL * p_should_go_on );

/**
 * Reads a tile data. This function is compulsory and allows one to decode tile data. opj_read_tile_header should be called before.
 * The user may need to refer to the image got by opj_read_header to understand the size being taken by the tile.
 *
 * @param	p_codec			the jpeg2000 codec.
 * @param	p_tile_index	the index of the tile being decoded, this should be the value set by opj_read_tile_header.
 * @param	p_data			pointer to a memory block that will hold the decoded data.
 * @param	p_data_size		size of p_data. p_data_size should be bigger or equal to the value set by opj_read_tile_header.
 * @param	p_stream		the stream to decode.
 *
 * @return	true			if the data could be decoded.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_decode_tile_data(	opj_codec_t *p_codec,
													OPJ_UINT32 p_tile_index,
													OPJ_BYTE * p_data,
													OPJ_UINT32 p_data_size,
													opj_stream_t *p_stream );

/* COMPRESSION FUNCTIONS*/


/**
Set encoding parameters to default values, that means : 
<ul>
<li>Lossless
<li>1 tile
<li>Size of precinct : 2^15 x 2^15 (means 1 precinct)
<li>Size of code-block : 64 x 64
<li>Number of resolutions: 6
<li>No SOP marker in the codestream
<li>No EPH marker in the codestream
<li>No sub-sampling in x or y direction
<li>No mode switch activated
<li>Progression order: LRCP
<li>No index file
<li>No ROI upshifted
<li>No offset of the origin of the image
<li>No offset of the origin of the tiles
<li>Reversible DWT 5-3
</ul>
@param parameters Compression parameters
*/
OPJ_API void OPJ_CALLCONV opj_set_default_encoder_parameters(opj_cparameters_t *parameters);

/**
 * Setup the encoder parameters using the current image and using user parameters.
 * @param p_codec 		Compressor handle
 * @param parameters 	Compression parameters
 * @param image 		Input filled image
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_setup_encoder(opj_codec_t *p_codec, 
												opj_cparameters_t *parameters, 
												opj_image_t *image);

/**
 * Start to compress the current image.
 * @param p_codec 		Compressor handle
 * @param p_image 	    Input filled image
 * @param p_stream 		Input stgream
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_start_compress (	opj_codec_t  *p_codec,
													opj_image_t  *p_image,
													opj_stream_t *p_stream);

/**
 * End to compress the current image.
 * @param p_codec 		Compressor handle
 * @param p_stream 		Input stgream
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_end_compress (opj_codec_t  *p_codec,
												opj_stream_t *p_stream);

/**
 * Encode an image into a JPEG-2000 codestream
 * @param p_codec 		compressor handle
 * @param p_stream 		Output buffer stream
 *
 * @return 				Returns true if successful, returns false otherwise
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_encode(opj_codec_t  *p_codec,
                                         opj_stream_t *p_stream);
/*
==========================================================
   codec output functions definitions
==========================================================
*/
/* EXPERIMENTAL FUNCTIONS FOR NOW, USED ONLY IN J2K_DUMP*/

/**
Destroy Codestream information after compression or decompression
@param cstr_info Codestream information structure
*/
OPJ_API void OPJ_CALLCONV opj_destroy_cstr_info(opj_codestream_info_v2_t **cstr_info);


/**
 * Dump the codec information into the output stream
 *
 * @param	p_codec			the jpeg2000 codec.
 * @param	info_flag		type of information dump.
 * @param	output_stream	output stream where dump the informations get from the codec.
 *
 */
OPJ_API void OPJ_CALLCONV opj_dump_codec(	opj_codec_t *p_codec,
											OPJ_INT32 info_flag,
											FILE* output_stream);

/**
 * Get the codestream information from the codec
 *
 * @param	p_codec			the jpeg2000 codec.
 *
 * @return					a pointer to a codestream information structure.
 *
 */
OPJ_API opj_codestream_info_v2_t* OPJ_CALLCONV opj_get_cstr_info(opj_codec_t *p_codec);

/**
 * Get the codestream index from the codec
 *
 * @param	p_codec			the jpeg2000 codec.
 *
 * @return					a pointer to a codestream index structure.
 *
 */
OPJ_API opj_codestream_index_t * OPJ_CALLCONV opj_get_cstr_index(opj_codec_t *p_codec);

OPJ_API void OPJ_CALLCONV opj_destroy_cstr_index(opj_codestream_index_t **p_cstr_index);


/**
 * Get the JP2 file information from the codec FIXME
 *
 * @param	p_codec			the jpeg2000 codec.
 *
 * @return					a pointer to a JP2 metadata structure.
 *
 */
OPJ_API opj_jp2_metadata_t* OPJ_CALLCONV opj_get_jp2_metadata(opj_codec_t *p_codec);

/**
 * Get the JP2 file index from the codec FIXME
 *
 * @param	p_codec			the jpeg2000 codec.
 *
 * @return					a pointer to a JP2 index structure.
 *
 */
OPJ_API opj_jp2_index_t* OPJ_CALLCONV opj_get_jp2_index(opj_codec_t *p_codec);


/*
==========================================================
   MCT functions
==========================================================
*/

/**
 * Sets the MCT matrix to use.
 *
 * @param	parameters		the parameters to change.
 * @param	pEncodingMatrix	the encoding matrix.
 * @param	p_dc_shift		the dc shift coefficients to use.
 * @param	pNbComp			the number of components of the image.
 *
 * @return	true if the parameters could be set.
 */
OPJ_API OPJ_BOOL OPJ_CALLCONV opj_set_MCT( opj_cparameters_t *parameters,
		                               	   OPJ_FLOAT32 * pEncodingMatrix,
		                               	   OPJ_INT32 * p_dc_shift,
		                               	   OPJ_UINT32 pNbComp);



#ifdef __cplusplus
}
#endif

#endif /* OPENJPEG_H */
