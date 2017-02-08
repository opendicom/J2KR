#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/dcmj2kr/j2krCoder.h" //opjrInstance constructor +

#include "dcmtk/dcmjpeg/djencabs.h"

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

class j2krParams;

class opjrInstance: public DJEncoder
{
public:

  /** constructor for J2PK
   *  @param cp codec parameters
   *  @param mode mode of operation
   *  @param quality compression quality
   */
opjrInstance(
  const j2krParams& cp,
  EJ_Mode mode,
  Uint8 bitsPerSample);
    
virtual ~opjrInstance();

    
//imageBuffer 16bit - abstract class DJEncoder
virtual OFCondition encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint16 *image_buffer,
  Uint8 *&to,
  Uint32 &length);

    
//imageBuffer 8bit - abstract class DJEncoder
virtual OFCondition encode(
  Uint16 columns,
  Uint16 rows,
  EP_Interpretation interpr,
  Uint16 samplesPerPixel,
  Uint8 *image_buffer,
  Uint8 *&to,
  Uint32 &length);


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
    Uint8 bitsAllocated);
    
#pragma mark methods to be added to fullfill abstract definition

  virtual E_TransferSyntax supportedTransferSyntax();

private:

  /// private undefined copy constructor
  opjrInstance(const opjrInstance&);

  /// private undefined copy assignment operator
  opjrInstance& operator=(const opjrInstance&);

  /// codec parameters
  const j2krParams *cparam;
  
  void findMinMax( int &_min, int &_max, char *bytes, long length, OFBool isSigned, int rows, int columns, int bitsAllocated);
  
  /// for lossy compression, defines compression quality factor
  Uint8 bitsPerSampleValue;

  /// enum for mode of operation (baseline, sequential, progressive etc.)
  EJ_Mode modeofOperation;
 
};
