#import <Foundation/Foundation.h>
#import "NSString+opendicom.h"

#include "osconfig.h"
#include "dcsequen.h"

@interface DCMTKSequence : NSObject {
    DcmSequenceOfItems* sequence;
}

+(id)sequenceWithSequence:(DcmSequenceOfItems*)dcmSequenceOfItems;
-(id)initWithSequence:(DcmSequenceOfItems*)dcmSequenceOfItems;
-(DcmSequenceOfItems*)sequence;

@end
