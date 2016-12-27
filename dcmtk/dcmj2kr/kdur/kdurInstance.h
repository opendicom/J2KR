#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/djencabs.h"  //DJEncoder
#include "dcmtk/dcmj2kr/j2krCoder.h" //kdurInstance constructor +

class j2krParams;

class kdurInstance: public DJEncoder
{
public:

  /** constructor for J2PK
   *  @param cp codec parameters
   *  @param mode mode of operation
   *  @param quality compression quality
   */
kdurInstance(
  const j2krParams& cp,
  EJ_Mode mode,
  Uint8 bitsPerSample);
    
virtual ~kdurInstance();

    
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
  kdurInstance(const kdurInstance&);

  /// private undefined copy assignment operator
  kdurInstance& operator=(const kdurInstance&);

  /// codec parameters
  const j2krParams *cparam;
  
  void findMinMax( int &_min, int &_max, char *bytes, long length, OFBool isSigned, int rows, int columns, int bitsAllocated);
  
  /// for lossy compression, defines compression quality factor
  Uint8 bitsPerSampleValue;

  /// enum for mode of operation (baseline, sequential, progressive etc.)
  EJ_Mode modeofOperation;
 
};
