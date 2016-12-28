//#import <DCMPix.h>
#import <Foundation/Foundation.h>
#import "NSString+opendicom.h"

//#import "DCMTKMetaInfo.h"
#import "DCMTKDataset.h"

#include "osconfig.h"
#include "dcvr.h"
#include "dcfilefo.h"

@class DCMTKDataset;

@interface DCMTKFile : NSObject {
    DcmFileFormat *file;
    //DCMTKMetaInfo *metaInfo;
    DCMTKDataset  *dataset;
}

-(id)initWithFilePath:(NSString*)filePath;
-(DcmFileFormat*)file;
//-(DCMTKMetaInfo*)metaInfo;
-(DCMTKDataset*)dataset;
@end
