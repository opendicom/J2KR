#import <Foundation/Foundation.h>
#import "NSString+opendicom.h"

#include "osconfig.h"
#include "dcelem.h"

@interface DCMTKElement : NSObject {
    DcmElement* element;
}

+(id)elementWithElement:(DcmElement*)dcmElement;
-(id)initWithElement:(DcmElement*)dcmElement;
-(DcmElement*)element;
-(NSString*)stringValue;
-(NSString*)stringValueWithEncodings:(NSStringEncoding)encoding;
-(NSInteger)integerValue;
-(NSNumber*)integerNumberValue;
-(NSString*)name;

@end
