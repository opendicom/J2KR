#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dccodec.h" /* for DcmCodecParameter */
#include "djutils.h" /* for enums */
#include "openjpegDef.h"

//v2 c=codification
typedef struct opj_cparameters {
    
    OPJ_BOOL tile_size_on;//size of tile in argument
    int cp_tx0;//XTOsiz
    int cp_ty0;//YTOsiz
    int cp_tdx;//XTsiz
    int cp_tdy;//YTsiz
    
    
    int cp_disto_alloc;//allocation by rate/distortion
    int cp_fixed_alloc;//allocation by fixed layer
    int cp_fixed_quality;//add fixed_quality
    
    int *cp_matrice;//fixed layer
    
    char *cp_comment;//comment for coding
    int csty;//coding style
    OPJ_PROG_ORDER prog_order;//progression order (default OPJ_LRCP)
    opj_poc_t POC[32];//progression order changes
    OPJ_UINT32 numpocs;//number of progression order changes (POC), default to 0
    int tcp_numlayers;//number of layers
    float tcp_rates[100];//rates of layers - might be subsequently limited by the max_cs_size field
    float tcp_distoratio[100];//different psnr for successive layers
    int numresolution;//number of resolutions
    int cblockw_init;//initial code block width, default to 64
    int cblockh_init;//initial code block height, default to 64
    int mode;//mode switch (cblk_style)
    int irreversible;//1:use the irreversible DWT 9-7, 0:use lossless compression(default)
    int roi_compno;//region of interest: affected component in [0..3], -1 means no ROI
    int roi_shift;//region of interest: upshift value
    int res_spec;//number of precinct size specifications
    int prcw_init[OPJ_J2K_MAXRLVLS];//initial precinct width
    int prch_init[OPJ_J2K_MAXRLVLS];//initial precinct height
    
    /* @name command line encoder parameters (not used inside the library)
    @{
    char infile[OPJ_PATH_LEN];//input file name
    char outfile[OPJ_PATH_LEN];//output file name
    int index_on;//DEPRECATED. Index generation is now handeld with the opj_encode_with_info() function. Set to NULL
    char index[OPJ_PATH_LEN];//DEPRECATED. Index generation is now handeld with the opj_encode_with_info() function. Set to NULL
    int image_offset_x0;//subimage encoding: origin image offset in x direction
    int image_offset_y0;//subimage encoding: origin image offset in y direction
    int subsampling_dx;
    int subsampling_dy;
    int decod_format;//input file format 0: PGX, 1: PxM, 2: BMP 3:TIF
    int cod_format;//output file format 0: J2K, 1: JP2, 2: JPT
    @}
     */
    
    /* UniPG NOT YET USED IN THE V2 VERSION OF OPENJPEG
    @name JPWL encoding parameters
    @{
    OPJ_BOOL jpwl_epc_on;//enables writing of EPC in MH, thus activating JPWL
    int jpwl_hprot_MH;//error protection method for MH (0,1,16,32,37-128)
    int jpwl_hprot_TPH_tileno[JPWL_MAX_NO_TILESPECS];//tile number of header protection specification (>=0)
    int jpwl_hprot_TPH[JPWL_MAX_NO_TILESPECS];//error protection methods for TPHs (0,1,16,32,37-128)
    int jpwl_pprot_tileno[JPWL_MAX_NO_PACKSPECS];//tile number of packet protection specification (>=0)
    int jpwl_pprot_packno[JPWL_MAX_NO_PACKSPECS];//packet number of packet protection specification (>=0)
    int jpwl_pprot[JPWL_MAX_NO_PACKSPECS];//error protection methods for packets (0,1,16,32,37-128)
    int jpwl_sens_size;//enables writing of ESD, (0=no/1/2 bytes)
    int jpwl_sens_addr;//sensitivity addressing size (0=auto/2/4 bytes)
    int jpwl_sens_range;//sensitivity range (0-3)
    int jpwl_sens_MH;//sensitivity method for MH (-1=no,0-7)
    int jpwl_sens_TPH_tileno[JPWL_MAX_NO_TILESPECS];//tile number of sensitivity specification (>=0)
    int jpwl_sens_TPH[JPWL_MAX_NO_TILESPECS];//sensitivity methods for TPHs (-1=no,0-7)
    @}
    */
    
    /**
     * DEPRECATED: use RSIZ, OPJ_PROFILE_* and MAX_COMP_SIZE instead
     * Digital Cinema compliance 0-not compliant, 1-compliant
     *
    OPJ_CINEMA_MODE cp_cinema;
     */
    
    int max_comp_size;//Maximum size (in bytes) for each component. If == 0, component size limitation is not considered
    
    
    /**
     * DEPRECATED: use RSIZ, OPJ_PROFILE_* and OPJ_EXTENSION_* instead
     * Profile name
     *
    OPJ_RSIZ_CAPABILITIES cp_rsiz;
    */
    char tp_on;//Tile part generation
    char tp_flag;//Flag for Tile part generation
    char tcp_mct;//MCT (multiple component transform)
    OPJ_BOOL jpip_on;//Enable JPIP indexing
    void * mct_data;//Naive implementation of MCT restricted to a single reversible array based encoding without offset concerning all the components.
    int max_cs_size;//Maximum size (in bytes) for the whole codestream. If == 0, codestream size limitation is not considered. If it does not comply with tcp_rates, max_cs_size prevails and a warning is issued.
    OPJ_UINT16 rsiz;//RSIZ value To be used to combine OPJ_PROFILE_*, OPJ_EXTENSION_* and (sub)levels values.
} opj_cparameters_t;  




class j2krParams: public DcmCodecParameter
{
public:
  j2krParams();

  /// copy constructor
  j2krParams(const j2krParams& arg);

  /// destructor
  virtual ~j2krParams();

  //create a copy of type DcmCodecParameter
  //must be overweritten in every subclass.
  //return copy of this object
  virtual DcmCodecParameter *clone() const;

  // returns the class name as string.
  //Can be used as poor man's RTTI replacement.
  virtual const char *className() const;

  // returns forced bit depth for image compression, 0 (auto) or 8/12/16
    int getForcedBitDepth() const
    {
        return 0;
    }
    
    //returns maximum fragment size (in kbytes) for compression, 0 for unlimited.
    Uint32 getFragmentSize() const
    {
        return 0;
    }
    
    // returns offset table creation flag
    OFBool getCreateOffsetTable() const
    {
        return OFTrue;
    }
    
    // returns flag indicating whether a compressed YBR color stream should be marked as YBR_FULL or YBR_FULL_422 on DICOM level
    OFBool getWriteYBR422() const
    {
        return OFFalse;
    }

    
    // returns mode for VOI transformation of monochrome images. Mode [0..6]
    //removed 7
    /*
     0  no VOI transformation: disable correction of polarity or any other presentation LUT transformation
     1  use the n-th VOI window from the image file
     2  use the n-th VOI look up table from the image file
     3  compute VOI window using min-max algorithm
     4  compute VOI window using Histogram algorithm, ignoring n percent
     5  compute VOI window using center r and width s
     6  compute VOI window using min-max algorithm ignoring extremes
     */
    size_t getWindowType() const
    {
        return 0;
    }

    
    // returns parameter for VOI transform of monochrome images, used in VOI modes 1, 2, 4, 6
    size_t getWindowParameter() const
    {
        return 0;
    }

    
    // returns VOI window center and width for VOI mode 5
    void getVOIWindow(double& center, double& width) const
    {
        center = 0.0;
        width = 0.0;
    }


    
    // returns color conversion mode for compression
    E_CompressionColorSpaceConversion getCompressionColorSpaceConversion() const
    {
        return ECC_monochrome;//[ECC_lossyYCbCr,ECC_lossyRGB,ECC_monochrome]
    }
    
    
    // returns flag indicating if compression to be optimized for real pixel values
    //OFBool opt_usePixelValues Check smallest, largest pixel value and optimize, mode 0 only
    OFBool getUsePixelValues() const
    {
        return OFTrue;
    }

    
    // returns flag indicating if compression should compute Rescale Slope/Intercept
    //OFBool opt_useModalityRescale to scale back, to original pixel range, mode 0 only
    OFBool getUseModalityRescale() const
    {
        return OFFalse;
    }

    
    // Returns flag indicating if incorrect palette attribute tags should be accepted
    OFBool getAcceptWrongPaletteTags() const
    {
        return OFFalse;
    }
    
    

private:

  /// private undefined copy assignment operator
  j2krParams& operator=(const j2krParams&);

};
