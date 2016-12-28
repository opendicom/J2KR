#import "DCMTKFile.h"

@implementation DCMTKFile

- (id) initWithFilePath:(NSString*)filePath
{
	if ((self = [super init])) {
        if ((file->loadFile(
                     filePath.UTF8String,
                     EXS_Unknown,
                     EGL_noChange,
                     DCM_MaxReadLength,
                     ERM_autoDetect
                     )).good())
        {
            dataset=[DCMTKDataset datasetWithDataset:(file->getDataset())];
            //metaInfo=[DCMTKMetaInfo metaInfoWithMetaInfo:(fileformat->getMetaInfo()) ofFile:self];
            return self;
        }
    }
    return nil;
}

- (void) dealloc
{
    delete file;
    file = nil;
    [dataset release];
    //[metainfo release];
    [super dealloc];
}

-(DcmFileFormat*)file
{
    return file;
}

/*
-(MetaInfo*)metaInfo
{
    return metaInfo;
}
*/

-(DCMTKDataset*)dataset
{
    return dataset;
}

@end
