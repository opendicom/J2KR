/*  Copyright (C) 1997-2011, OFFIS e.V. Developed by
 *    OFFIS e.V.,R&D Division Health,Escherweg 2,D-26121 Oldenburg, Germany
 *  Module:  dcmjpeg
 *  Author:  Norbert Olges, Marco Eichelberg
 *  Purpose: abstract base class for compression classes
 */
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctypes.h"
#include "dcmtk/dcmimgle/diutils.h" /* for EP_Interpretation */
#include "djutils.h" /* for enums */

// abstract base class for compression classes.
class DCMTK_DCMJPEG_EXPORT DJEncoder
{
public: 
           DJEncoder(){}
  virtual ~DJEncoder(){}

  /** single frame compression routine for 16-bit raw pixel data.
   *  May only be called if bytesPerSample() == 2.
   *  @param *&to compressed frame returned in this parameter upon success
   *  @param length length of compressed frame (in bytes) returned in this parameter
   *    upon success; length guaranteed to be always even.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition encode(
    Uint16 columns,
    Uint16 rows,
    EP_Interpretation interpr,
    Uint16 samplesPerPixel,
    Uint16 *image_buffer,
    Uint8 *&to,
    Uint32 &length) = 0;

  /** single frame compression routine for 8-bit raw pixel data.
   *  May only be called if bytesPerSample() == 1.
   *  @param *&to compressed frame returned in this parameter upon success
   *  @param length length of compressed frame (in bytes) returned in this parameter
   *    upon success; length guaranteed to be always even.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition encode(
    Uint16 columns,
    Uint16 rows,
    EP_Interpretation interpr,
    Uint16 samplesPerPixel,
    Uint8 *image_buffer,
    Uint8 *&to,
    Uint32 &length) = 0;

  virtual Uint16 bytesPerSample() const = 0;
  virtual Uint16 bitsPerSample() const = 0;
};
