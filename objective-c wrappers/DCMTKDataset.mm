#import "DCMTKDataset.h"

@implementation DCMTKDataset

+(id)datasetWithDataset:(DcmDataset*)dcmDataset// ofFile:(DCMTKFile *)file
{
    return [[[[self class] alloc] initWithDataset:dcmDataset]autorelease];// ofFile:(DCMTKFile *)file]
}

-(id)initWithDataset:(DcmDataset*)dcmDataset {
	if ((self = [super init])) {
		dataset=dcmDataset; // new DcmItem(item)
        tagPath=@"";
        //container=file;
        NSMutableArray *contents=[NSMutableArray array];
        return self;
	}
	return nil;
}

-(void)dealloc {
	delete dataset;
    [tagPath release];
    //[container release];
    [contents release];
	[super dealloc];
}

-(DcmDataset*)dataset {
	return dataset;
}

-(DcmEVR)vr
{
    return dataset->ident();
}
@end



