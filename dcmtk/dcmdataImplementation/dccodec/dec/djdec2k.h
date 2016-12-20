/*  Copyright (C) 1997-2005, OFFIS
 *  This software and supporting documentation were developed by
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *  Module:  dcmjpeg
 *  Author:  Marco Eichelberg, Norbert Olges
 *  Purpose: Codec class for decoding JP2K
 *  Last Update:      $Author: lpysher $
 *  Update Date:      $Date: 2006/03/01 20:15:44 $
 */

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdataImplementation/dccodec/djcodecd.h" /* for class DJCodecDecoder */
//#include "djencabs.h"


#pragma mark Decoder class for JPEG 2K Lossy

class DJDecoderJP2k : public DJCodecDecoder
{
public: 
  DJDecoderJP2k();
  virtual ~DJDecoderJP2k();

    
  virtual OFBool canChangeCoding(
    const E_TransferSyntax oldRepType,
    const E_TransferSyntax newRepType) const;

    /*djcodecd
    virtual OFCondition decode(
                               const DcmRepresentationParameter * fromRepParam,
                               DcmPixelSequence * pixSeq,
                               DcmPolymorphOBOW& uncompressedPixelData,
                               const DcmCodecParameter * cp,
                               const DcmStack& objStack) const;
     */

    
    /*djcodecd
     virtual OFCondition decodeFrame(
     const DcmRepresentationParameter * fromParam,
     DcmPixelSequence * fromPixSeq,
     const DcmCodecParameter * cp,
     DcmItem *dataset,
     Uint32 frameNo,
     Uint32& startFragment,
     void *buffer,
     Uint32 bufSize,
     OFString& decompressedColorModel) const;
     */

    
    /*djcodecd
     virtual OFCondition encode(
     const Uint16 * pixelData,
     const Uint32 length,
     const DcmRepresentationParameter * toRepParam,
     DcmPixelSequence * & pixSeq,
     const DcmCodecParameter *cp,
     DcmStack & objStack) const;
     */
    
    
  /** returns the transfer syntax that this particular codec
   *  is able to encode and decode.
   *  @return supported transfer syntax
   */
  virtual OFCondition encode(
    const E_TransferSyntax fromRepType,
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence *fromPixSeq,
    const DcmRepresentationParameter *toRepParam,
    DcmPixelSequence * & toPixSeq,
    const DcmCodecParameter * cp,
    DcmStack & objStack) const;

    
    /*djcodecd
     virtual OFCondition determineDecompressedColorModel(
     const DcmRepresentationParameter *fromParam,
     DcmPixelSequence *fromPixSeq,
     const DcmCodecParameter *cp,
     DcmItem *dataset,
     OFString &decompressedColorModel) const;
     */
    
    
#pragma mark abstract implemented
  virtual E_TransferSyntax supportedTransferSyntax() const;

#pragma mark additional public
  virtual OFBool isJPEG2000() const;

#pragma mark -
#pragma mark private
private:

#pragma mark abstract implemented
  /** creates an instance of the compression library to be used for decoding.
   *  @param toRepParam representation parameter passed to decode()
   *  @param cp codec parameter passed to decode()
   *  @param bitsPerSample bits per sample for the image data
   *  @param isYBR flag indicating whether DICOM photometric interpretation is YCbCr
   *  @return pointer to newly allocated decoder object
   */
  virtual DJDecoder *createDecoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const;
};


#pragma mark -
#pragma mark -
#pragma mark Decoder class for JPEG 2K Lossless

class DJDecoderJP2kLossLess : public DJCodecDecoder
{
public: 
  DJDecoderJP2kLossLess();
  virtual ~DJDecoderJP2kLossLess();

  virtual OFBool canChangeCoding(
    const E_TransferSyntax oldRepType,
    const E_TransferSyntax newRepType) const;

  /** returns the transfer syntax that this particular codec
   *  is able to encode and decode.
   *  @return supported transfer syntax
   */
  virtual E_TransferSyntax supportedTransferSyntax() const;
  
  virtual OFBool isJPEG2000() const;
  
  virtual OFCondition encode(
    const E_TransferSyntax fromRepType,
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence *fromPixSeq,
    const DcmRepresentationParameter *toRepParam,
    DcmPixelSequence * & toPixSeq,
    const DcmCodecParameter * cp,
    DcmStack & objStack) const;
	
private:

  /** creates an instance of the compression library to be used for decoding.
   *  @param toRepParam representation parameter passed to decode()
   *  @param cp codec parameter passed to decode()
   *  @param bitsPerSample bits per sample for the image data
   *  @param isYBR flag indicating whether DICOM photometric interpretation is YCbCr
   *  @return pointer to newly allocated decoder object
   */
  virtual DJDecoder *createDecoderInstance(
    const DcmRepresentationParameter * toRepParam,
    const DJCodecParameter *cp,
    Uint8 bitsPerSample,
    OFBool isYBR) const;
};
