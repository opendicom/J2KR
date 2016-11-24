#import "DCMTKItem.h"

@implementation DCMTKItem

+(id)itemWithItem:(DcmItem*)dcmItem {
	return [[[[self class] alloc] initWithItem:dcmItem] autorelease];
}

-(id)initWithItem:(DcmItem*)dcmItem {
	if ((self = [super init])) {
		item = dcmItem; // new DcmItem(item)
        return self;
	}
	return nil;
}

-(void)dealloc {
	//delete item;
	[super dealloc];
}

-(DcmItem*)item {
	return item;
}


@end



