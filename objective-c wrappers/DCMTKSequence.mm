#import "DCMTKSequence.h"

@implementation DCMTKSequence

+(id)sequenceWithSequence:(DcmSequenceOfItems*)dcmSequenceOfItems {
    return [[[[self class] alloc] initWithSequence:dcmSequenceOfItems] autorelease];
}

-(id)initWithSequence:(DcmSequenceOfItems*)dcmSequenceOfItems {
	if ((self = [super init])) {
		sequence = dcmSequenceOfItems; // new DcmSequenceOfItems(sequenceOfItems)
        return self;
	}	
	return nil;
}

-(void)dealloc {
	//delete _sequenceOfItems;
	[super dealloc];
}

-(DcmSequenceOfItems*)sequence {
	return sequence;
}

@end



