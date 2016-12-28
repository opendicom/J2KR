/*
 you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation,  version 3 of the License.
 It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY EXPRESS OR IMPLIED, INCLUDING ANY WARRANTY OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE OR USE.  See the GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License. If not, see http://www.gnu.org/licenses/lgpl.html

 Created by Aaron Boxer on 1/21/14.
*/

#include <iostream>

class OPJSupport {
    
public:
    OPJSupport();
    ~OPJSupport();
    
    void* decompressJPEG2K( void* jp2Data, long jp2DataSize, long *decompressedBufferSize, int *colorModel);
    void* decompressJPEG2KWithBuffer( void* inputBuffer, void* jp2Data, long jp2DataSize, long *decompressedBufferSize, int *colorModel);
    
    unsigned char *compressJPEG2K(   void *data,
                                  int samplesPerPixel,
                                  int rows,
                                  int columns,
                                  int bitsstored, //precision,
                                  unsigned char bitsAllocated,
                                  bool sign,
                                  int rate,
                                  long *compressedDataSize);
};
