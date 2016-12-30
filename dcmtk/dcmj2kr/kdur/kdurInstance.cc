#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmj2kr/kdur/kdurInstance.h"
#include "dcmtk/dcmj2kr/j2krParams.h"
#include "dcmtk/ofstd/ofconsol.h"
#include "dcmtk/ofstd/ofstdinc.h"

extern "C" void* kdu_compressJPEG2K( void *data, int samplesPerPixel, int rows, int columns, int precision, bool sign, int rate, long *compressedDataSize, int num_threads);

kdurInstance::kdurInstance(const j2krParams& cp, EJ_Mode mode,
    Uint8 bitsPerSample)
                                : DJEncoder()
                                , cparam(&cp)
                                , bitsPerSampleValue(bitsPerSample)
                                , modeofOperation(mode)
{}

kdurInstance::~kdurInstance()
{}


//imageBuffer 16bit - abstract class DJEncoder
OFCondition kdurInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: kdurInstance (16)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 16);
}


//imageBuffer 8bit - abstract class DJEncoder
OFCondition kdurInstance::encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length)
{
    fprintf(stdout,"D: kdurInstance (8)\r\n");
    return encode( columns, rows, interpr, samplesPerPixel, (Uint8*) image_buffer, to, length, 8);
}

//abstract class DJEncoder
Uint16 kdurInstance::bytesPerSample() const
{
	if( bitsPerSampleValue <= 8) return 1;
	else return 2;
}

//abstract class DJEncoder
Uint16 kdurInstance::bitsPerSample() const
{
	return bitsPerSampleValue;
}

OFCondition kdurInstance::encode( 
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation colorSpace,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 bitsAllocated)
{
    //to = (Uint8 *) results in segmantation fault
    long compressedLength = 0;
    void *outBuffer = (Uint8 *)kdu_compressJPEG2K(
       (void*)image_buffer,
       samplesPerPixel,
       rows,
       columns,
       bitsAllocated,
       false,
       0,
       &compressedLength,
       0);
    //[7] rate (0=lossless), [9] processors

    if( outBuffer)
    {
        to = new Uint8[ compressedLength];
        memcpy( to, outBuffer, compressedLength);
        length = compressedLength;
    
        free( outBuffer);
    }
    return EC_Normal;
}


E_TransferSyntax kdurInstance::supportedTransferSyntax()
{
    return EXS_JPEG2000LosslessOnly;
}
