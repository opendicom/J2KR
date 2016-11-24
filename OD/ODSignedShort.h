#import <Foundation/Foundation.h>

#import "ODItem.h"
#include "osconfig.h"
#include "dcvr.h"
#include "dcelem.h"

@interface ODElement : NSObject
{
    NSString        *tagPath;
    DcmElement      *dcmElement;
    id              parentNode;
}

+(id)ODObjectWithDcm:(DcmElement*)dcm parentNode:(id)pn tagPath:(NSString*)tp;
-(id)initWithDcm:(DcmElement*)dcm parentNode:(id)pn tagPath:(NSString*)tp;
-(DcmElement*)dcmElement;
-(NSString*)tagPath;
-(NSString*)stringValue;
-(NSString*)stringValueWithEncodings:(NSStringEncoding)encoding;
-(DcmEVR)dcmEVR;
-(BOOL)clearValues;
-(BOOL)overwriteWithJSONValues:(NSString*)s;
-(BOOL)overwriteWithDictionaryValues:(NSDictionary*)d;
-(void)remove;

@end
