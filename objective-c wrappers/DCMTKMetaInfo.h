#import <Foundation/Foundation.h>
#import "NSString+opendicom.h"

#include "osconfig.h"
#include "dcitem.h"

@interface DCMTKItem : NSObject {
    DcmItem* item;
}

+(id)itemWithItem:(DcmItem*)dcmItem;
-(id)initWithItem:(DcmItem*)dcmItem;
-(DcmItem*)item;

@end
