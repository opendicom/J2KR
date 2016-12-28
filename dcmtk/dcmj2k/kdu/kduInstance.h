#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/dcmj2k/j2kCoder.h"

#include "dcmtk/djencabs.h"
/*DJEncoder
 
 métodos:
 virtual Uint16 bytesPerSample() const = 0;
 virtual Uint16 bitsPerSample() const = 0;
 
 
 Single frame compression routines for 16-bit raw pixel data (bytesPerSample()==2) and fo 8-bit raw pixel data (bytesPerSample()==1).
 
 @param *&to compressed frame returned in this parameter upon success
 @param length length of compressed frame (in bytes) returned in this parameter
 upon success; length guaranteed to be always even.
 @return EC_Normal if successful, an error code otherwise.
 
 virtual OFCondition encode(
 Uint16 columns,
 Uint16 rows,
 EP_Interpretation interpr,
 Uint16 samplesPerPixel,
 Uint16 *image_buffer,
 Uint8 *&to,
 Uint32 &length) = 0;
 
 virtual OFCondition encode(
 Uint16 columns,
 Uint16 rows,
 EP_Interpretation interpr,
 Uint16 samplesPerPixel,
 Uint8 *image_buffer,
 Uint8 *&to,
 Uint32 &length) = 0;
 */


class DJEncoder;

class kduInstance: public DJEncoder
{
public:
    
  /** constructor for J2PK
   *  @param cp codec parameters
   *  @param mode mode of operation
   *  @param quality compression quality
   */
kduInstance(
  const j2kParams& cp,
  EJ_Mode mode,
  Uint8 quality,
  Uint8 bitsPerSample);
    
virtual ~kduInstance();

    
//imageBuffer 16bit - abstract class DJEncoder
virtual OFCondition encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length);

//imageBuffer 16bit - used
virtual OFCondition encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length,
  Uint8 pixelRepresentation,
  double minUsed,
  double maxUsed);

    
//imageBuffer 8bit - abstract class DJEncoder
virtual OFCondition encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length);
    
//imageBuffer 8bit -used
  virtual OFCondition encode(
    Uint16 columns,
    Uint16 rows,
    EP_Interpretation colorSpace,
    Uint16 samplesPerPixel,
    Uint8 *image_buffer,
    Uint8 *&to,
    Uint32 &length,
	Uint8 pixelRepresentation,
	double minUsed,
    double maxUsed);

//abstract class DJEncoder
  virtual Uint16 bytesPerSample() const;

//abstract class DJEncoder
  virtual Uint16 bitsPerSample() const;

  //imageBuffer 8bit bitsAllocated, execution
  virtual OFCondition encode(
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
    double maxUsed);

#pragma mark methods to be added to fullfill abstract definition
    
    virtual E_TransferSyntax supportedTransferSyntax();


private:

  /// private undefined copy constructor
  kduInstance(const kduInstance&);

  /// private undefined copy assignment operator
  kduInstance& operator=(const kduInstance&);

  /// codec parameters
  const j2kParams *cparam;
  
  void findMinMax( int &_min, int &_max, char *bytes, long length, OFBool isSigned, int rows, int columns, int bitsAllocated);
  
  /// for lossy compression, defines compression quality factor
  Uint8 quality;
  Uint8 bitsPerSampleValue;

  /// enum for mode of operation (baseline, sequential, progressive etc.)
  EJ_Mode modeofOperation;
 
};
