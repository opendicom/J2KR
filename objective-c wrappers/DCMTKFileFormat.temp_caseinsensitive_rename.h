#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>
#import "NSString+opendicom.h"

#undef verify
#include "osconfig.h" /* make sure OS specific configuration is included first */
#include "djdecode.h"  /* for dcmjpeg decoders */
#include "djencode.h"  /* for dcmjpeg encoders */
#include "dcrledrg.h"  /* for DcmRLEDecoderRegistration */
#include "dcrleerg.h"  /* for DcmRLEEncoderRegistration */
#include "djrploss.h"
#include "djrplol.h"
#include "dcpixel.h"
#include "dcrlerp.h"
#include "dcdicdir.h"
#include "dcdatset.h"
#include "dcmetinf.h"
#include "dcfilefo.h"
#include "dcdebug.h"
#include "dcuid.h"
#include "dcdict.h"
#include "dcdeftag.h"
#include "Binaries/dcmtk-source/dcmjpls/djdecode.h" //JPEG-LS
#include "Binaries/dcmtk-source/dcmjpls/djencode.h" //JPEG-LS

/*
 tagPath
 =======
 
 ggggeeee						(mod14=8)
 ggggeeee.0001					(mod14=13)
 ggggeeee.0001-ggggeeee			(mod14=8)
 ggggeeee.0001_					(mod14=0
 ggggeeee~ 						(mod14=9)
 
 For encapsulated fragments of pixels, the paths are:
 
 7FE00010						(pixel attribute)
 7FE00010.0000					(basic table)
 7FE00010.0001					(first fragment)
 ...
 7FE00010 ~

 */


@interface DCMTKfileFormat : NSObject
{
    DcmFileFormat *fileformat;
    DcmDataset *dataset;
    DcmMetaInfo *metaInfo;
    DcmItem *baseItem;
    DcmItem *currentItem;
    UInt32 currentTag;
    DcmVR *currentVR;
    NSMutableArray *currentValue;
}

-(id)initWithPath:(NSString*)path;

//initially baseItem = dataset
-(BOOL)datasetAsBaseItem;
-(BOOL)metaInfoAsBaseItem;
-(BOOL)curentItemAsBaseItem;

-(void)dealloc;

-(BOOL)isEncapsulated;
-(NSString*)calculatePixelsMd5AndOverwrite:(BOOL)insert;

-(BOOL)deleteTagPath:(NSString*)tagPath;
-(BOOL)selectTagPath:(NSString*)tagPath;
//-(BOOL)overwriteTagPath:(NSString*)tagPath withJSON:(NSString*)JSONString;

-(NSString*)elementStringValue;
-(NSArray*)elementStringValues;
@end
