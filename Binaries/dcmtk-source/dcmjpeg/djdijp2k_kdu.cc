#include "osconfig.h"
#include "djdijp2k.h"
#include "djcparam.h"
#include "ofconsol.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#define INCLUDE_CSTDIO
#define INCLUDE_CSETJMP
#include "ofstdinc.h"

extern "C" void* kdu_decompressJPEG2K( void* jp2Data, long jp2DataSize, long *decompressedBufferSize, int *colorModel, int num_threads);
extern "C" void* kdu_decompressJPEG2KWithBuffer( void* inputBuffer, void* jp2Data, long jp2DataSize, long *decompressedBufferSize, int *colorModel, int num_threads);

// These two macros are re-defined in the IJG header files.
// We undefine them here and hope that IJG's configure has
// come to the same conclusion that we have...
#ifdef HAVE_STDLIB_H
#undef HAVE_STDLIB_H
#endif
#ifdef HAVE_STDDEF_H
#undef HAVE_STDDEF_H
#endif

BEGIN_EXTERN_C
#define boolean ijg_boolean
#include "jpeglib16.h"
#include "jerror16.h"
#undef boolean

// disable any preprocessor magic the IJG library might be doing with the "const" keyword
#ifdef const
#undef const
#endif


//#include "openjpeg.h"

/**
sample error callback expecting a FILE* client object
JF removed
static void error_callback(const char *msg, void *a)
{
//	printf( "%s\r\r", msg);
}
 */

/**
sample warning callback expecting a FILE* client object
JF removed
static void warning_callback(const char *msg, void *a)
{
//	printf( "%s\r\r", msg);
}
*/

/**
sample debug callback expecting no client object
JF removed
static void info_callback(const char *msg, void *a)
{
//	printf( "%s\r\r", msg);
}
*/

/*JF removed
static inline int int_ceildivpow2(int a, int b) {
	return (a + (1 << b) - 1) >> b;
}
*/


DJDecompressJP2k::DJDecompressJP2k(const DJCodecParameter& cp, OFBool isYBR)
: DJDecoder()
, cparam(&cp)
, cinfo(NULL)
, suspension(0)
, jsampBuffer(NULL)
, dicomPhotometricInterpretationIsYCbCr(isYBR)
, decompressedColorModel(EPI_Unknown)
{
}

DJDecompressJP2k::~DJDecompressJP2k()
{
  cleanup();
}


OFCondition DJDecompressJP2k::init()
{
  // everything OK
  return EC_Normal;
}


void DJDecompressJP2k::cleanup()
{
}


OFCondition DJDecompressJP2k::decode(
  Uint8 *compressedFrameBuffer,
  Uint32 compressedFrameBufferSize,
  Uint8 *uncompressedFrameBuffer,
  Uint32 uncompressedFrameBufferSize,
  OFBool isSigned)
{
    int processors = 0;
    
    if( compressedFrameBufferSize > 512*1024)
    {
        int mib[2] = {CTL_HW, HW_NCPU};
        size_t dataLen = sizeof(int); // 'num' is an 'int'
        int result = sysctl(mib, 2, &processors, &dataLen, NULL, 0);
        if (result == -1)
            processors = 1;
        if( processors > 8)
            processors = 8;
    }
    
    long decompressedBufferSize = 0;
    int colorModel;
    
    kdu_decompressJPEG2KWithBuffer( uncompressedFrameBuffer, compressedFrameBuffer, compressedFrameBufferSize, &decompressedBufferSize, &colorModel, processors);
    
    if( colorModel == 1)
        decompressedColorModel = (EP_Interpretation) EPI_RGB;
	return EC_Normal;
}

void DJDecompressJP2k::outputMessage() const
{

}
}
