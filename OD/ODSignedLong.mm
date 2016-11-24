#import "NSString+opendicom.h"
#import "ODvr.h"
#import "ODElement.h"

@implementation ODElement

+(id)ODObjectWithDcm:(DcmElement*)dcm parentNode:(id)pn tagPath:(NSString*)tp
{
	return [[[[self class] alloc] initWithDcm:dcm parentNode:pn tagPath:tp] autorelease];
}

-(id)initWithDcm:(DcmElement*)dcm parentNode:(id)o tagPath:(NSString*)tp
{
	if ((self = [super init]))
    {
        dcmElement = dcm;
        tagPath=tp;
        parentNode=o;
        NSLog(@"%d%@ \"%@\" ",[self retainCount],[self debugDescription],tagPath);
        return self;
	}	
	return nil;
}

-(void)dealloc {
    NSLog(@"%d%@ \"%@\" -> dealloc",[self retainCount],[self debugDescription],tagPath);
    [super dealloc];
}

-(DcmElement*)dcmElement {
	return dcmElement;
}


-(NSString*)tagPath
{
    return tagPath;
}

/*
-(NSString*)description {
	NSMutableString* str = [NSMutableString stringWithFormat:@"%@ = %@", self.name, self.stringValue];
	unsigned int vm = _element->getVM();
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
	
	return str;
}

-(NSString*)name {
	return @"";// [NSString stringWithCString:DcmTag(_element->getTag()).getTagName() encoding:NSUTF8StringEncoding];
}
 */
-(NSString*)stringValue
{
    NSLog(@"%d%@ \"%@\" -> stringValue",[self retainCount],[self debugDescription],tagPath);
    return [self stringValueWithEncodings:NSISOLatin1StringEncoding];
}

-(NSString*)stringValueWithEncodings:(NSStringEncoding)encoding{

    OFString ofstr;
    if (dcmElement->getOFStringArray(ofstr).good())
        return [NSString stringWithCString:ofstr.c_str() encoding:encoding];
	return nil;
}

-(DcmEVR)dcmEVR
{
    return dcmElement->ident();
}

-(BOOL)overwriteWithJSONValues:(NSString*)s
{
    NSDictionary *d=[s dictionaryForJSONElement];//validity check performed
    if (d) return [self overwriteWithDictionaryValues:d];
    return false;
}
-(BOOL)overwriteWithDictionaryValues:(NSDictionary*)d
{
    uint16 vrShort=[[d objectForKey:@"vr"]dicomVrShortValue];
    NSLog(@"%d",vrShort);
    NSString *n=[[d objectForKey:@"Value"]componentsJoinedByString:@"\\"];
    switch (vrShort) {
//string
        case AE:
        case AS:
        case CS:
        case DA:
        case DT:
        case LO:
        case LT:
        case PN:
        case SH:
        case ST:
        case TM:
        case UC:
        case UI:
        case UR:
        case UT:
            return (dcmElement->putOFStringArray(OFString([n cStringUsingEncoding:NSISOLatin1StringEncoding]))).good();
//number strings
        case DS:
        case IS:
            
//number
        case FL:
            //putFloat32Array (const Float32 *vals, const unsigned long num)
        case FD:
            //putFloat64Array (const Float64 *vals, const unsigned long num)
        case SL:
            //putSint32Array (const Sint32 *vals, const unsigned long num)
        case SS:
            //putSint16Array (const Sint16 *vals, const unsigned long num)
        case UL:
            //putUint32Array (const Uint32 *vals, const unsigned long num)
        case US:
            //putUint16Array (const Uint16 *vals, const unsigned long num)
            NSLog(@"vr not implemented");
            return false;
            
            //putUint8Array (const Uint8 *vals, const unsigned long num)
            //Reimplemented in DcmPixelData, DcmOtherByteOtherWord, and DcmPolymorphOBOW

//base64
        case OB:
        case OD:
        case OF:
        case OW:
        case UN:
            NSLog(@"vr not implemented");
            return false;
 
//Attribute Tag
        case AT:
            //AT putTagVal (const DcmTagKey &attrTag, const unsigned long pos=0)
            NSLog(@"vr not implemented");
            return false;
    }
    return false;
}
-(BOOL)clearValues
{
    NSLog(@"%d%@ %@ -> clearValues",[self retainCount],[self debugDescription],tagPath);
    return (BOOL)(dcmElement->clear()).good();
}

-(void)remove
{
    NSLog(@"%d%@ %@ -> remove",[self retainCount],[self debugDescription],tagPath);
    [parentNode removeElement:[tagPath substringFromIndex:[tagPath length]-8]];
}
@end



