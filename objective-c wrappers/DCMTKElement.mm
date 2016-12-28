#import "DCMTKElement.h"

@implementation DCMTKElement

+(id)elementWithElement:(DcmElement*)dcmElement {
	return [[[[self class] alloc] initWithElement:dcmElement] autorelease];
}

-(id)initWithElement:(DcmElement*)dcmElement {
	if ((self = [super init])) {
		element = dcmElement; // new DcmElement(element)
        return self;
	}	
	return nil;
}

-(void)dealloc {
	//delete _element;
	[super dealloc];
}

-(DcmElement*)element {
	return element;
}

-(NSString*)description {
	NSMutableString* str = [NSMutableString stringWithFormat:@"%@ = %@", self.name, self.stringValue];
/*	unsigned int vm = _element->getVM();
	if (vm > 1)
		for (unsigned int i = 0; i < vm; ++i) {
			OFString ofstr;
			if (_element->getOFString(ofstr,i).good())
				[str appendFormat:@"[%d][%s] ", i, ofstr.c_str()];
		}
	else {
		OFString ofstr;
		if (_element->getOFString(ofstr,0).good())
			[str appendFormat:@"%s", ofstr.c_str()];
	}
	*/
	return str;
}

-(NSString*)name {
	return @"";// [NSString stringWithCString:DcmTag(_element->getTag()).getTagName() encoding:NSUTF8StringEncoding];
}
-(NSString*)stringValue
{
    return [self stringValueWithEncodings:NSISOLatin1StringEncoding];
}

-(NSString*)stringValueWithEncodings:(NSStringEncoding)encoding{

    OFString ofstr;
    
	//if (_element->getOFStringArray(ofstr).good())
        return [NSString stringWithCString:ofstr.c_str() encoding:encoding];
	return nil;
}

-(NSInteger)integerValue {
	NSString* str = [self stringValue];
	return [str integerValue];
}

-(NSNumber*)integerNumberValue {
	return [NSNumber numberWithInteger:[self integerValue]];
}

@end



