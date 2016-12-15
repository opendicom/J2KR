#import <Foundation/Foundation.h>
#import "NSString+opendicom.h"
#import "DCMTKFile.h"

#include "osconfig.h"
#include "dcvr.h"
//#include "dcfilefo.h"
//#include "dcmetinf.h"
#include "dcdatset.h"
//#include "dcitem.h"


@interface DCMTKDataset : NSObject {
    NSString *tagPath;
    NSMutableArray *contents;

    DcmDataset *dataset;
    //DCMTKFile  *container;
}

+(id)datasetWithDataset:(DcmDataset*)dcmDataset;// ofFile:(DCMTKFile*)file;
-(id)initWithDataset:(DcmDataset*)dcmDataset;// ofFile:(DCMTKFile*)file;
-(DcmDataset*)dataset;
-(DcmEVR)vr;

@end
