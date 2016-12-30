#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/opjr/opjrInstance.h"
#include "dcmtk/dcmj2kr/j2krParams.h"
#include "dcmtk/ofstd/ofconsol.h"
#include "dcmtk/ofstd/ofstdinc.h"

#include "OPJSupport.h"
#include "openjpeg.h"


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
    fprintf(stdout,"D: (16bit) opjrInstance encode imageBuffer (abstract)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint16*) image_buffer, to, length, 0, 0.0L, 0.0L);
    //pixel representation 0000H unsigned integer, 0001H 2's complement
}

//imageBuffer 16bit - used
OFCondition opjrInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to ,
  Uint32 &length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    fprintf(stdout,"D: (16bit) opjrInstance pixelRepresentation:0000H (unsigned) minUsed:0.0(fixed) maxUsed:0.0(fixed)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16, pixelRepresentation, minUsed, maxUsed);
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
    fprintf(stdout,"D: opjrInstance imageBuffer 8bit\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 0, 0.0L, 0.0L);
}

//imageBuffer 8bit
OFCondition opjrInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
    fprintf(stdout,"D: opjrInstance pixelRepresentation:0000H (unsigned) minUsed:0.0(fixed) maxUsed:0.0(fixed)\r\n");
    return encode( columns, rows, colorSpace, samplesPerPixel, (Uint8*) image_buffer, to, length, 8, pixelRepresentation, minUsed, maxUsed);
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

OFCondition opjrInstance::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 bitsAllocated,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed)
{
     long compressedLength = 0;//type mismatch
     OPJSupport opj;
     to = (Uint8 *)opj.compressJPEG2K(
     (void*) image_buffer,
     samplesPerPixel,
     rows,
     columns,
     bitsAllocated,
     bitsAllocated,
     false,
     0,
     &compressedLength);
     //[5] bitsstored, [7] signed, [8] rate=0=reversible
     length = compressedLength;
     if (length) fprintf(stdout,"D: OPJSupport.compressJPEG2K result length:%d\r\n",length);
     else fprintf(stdout,"E: OPJSupport.compressJPEG2K empty output\r\n");
    
     return EC_Normal;
}


E_TransferSyntax opjrInstance::supportedTransferSyntax()
{
    return EXS_JPEG2000LosslessOnly;
}
