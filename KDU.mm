#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>

#undef verify
#include "osconfig.h" /* make sure OS specific configuration is included first */
#include "djdecode.h"  /* for dcmjpeg decoders */
#include "djencode.h"  /* for dcmjpeg encoders */
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

enum DCM_CompressionQuality {
    DCMLosslessQuality = 0,
    DCMHighQuality,
    DCMMediumQuality,
    DCMLowQuality
};

enum basePath {
    fileRelativePath=1,
    inBasePath,
    doneBasePath,
    errBasePath,
};

//JF extern void dcmtkSetJPEGColorSpace( int);

/*
 //unlink removes a reference to a file. 
 //When reference counter comes to zero, 
 //the space occupied in memory by the file can be freed.
 
void myunlink(const char * path) {
    NSLog(@"Unlinking %s", path);
    unlink(path);
    NSLog(@"... Unlinked %s", path);
}
*/
#define myunlink unlink

static NSFileManager *fileManager=nil;
static NSError *err=nil;
short Use_kdu_IfAvailable = 1;

BOOL folderExists(NSString *f, BOOL shouldBeEmptyAndWritable)
{
    NSString *folderPath=[f stringByExpandingTildeInPath];
    BOOL isFolder=false;
    BOOL exists=[fileManager fileExistsAtPath:folderPath isDirectory:&isFolder];

    
    if (!exists)
    {
        [fileManager createDirectoryAtPath:folderPath withIntermediateDirectories:YES attributes:nil error:&err];
        if (![fileManager fileExistsAtPath:folderPath])
        {
            NSLog(@"could not create: %@",folderPath);
            return false;
        }
        return true;
    }
    else if (!isFolder)
    {
        NSLog(@"is not folder: %@",folderPath);
        return false;
    }
    else if (shouldBeEmptyAndWritable)
    {
        NSArray *array=[fileManager contentsOfDirectoryAtPath:folderPath error:nil];
        for (NSString *name in array)
        {
            if (![name hasPrefix:@"."])
            {
                NSLog(@"%@ INITIALLY NOT empty\r\nContains: %@",folderPath,name);
                return false;
            }
        }

#pragma mark --- missing writability check ---
    }
    return true;
}
/*
NSString* attrStringValue(DcmDataset dataset, short g, short e, NSStringEncoding c)
{
    const char *s = NULL;
    if (dataset->findAndGetString(DcmTagKey(g,e), s, OFFalse).good() && s != NULL)
        return [NSString stringWithCString:s encoding:c];
    return nil;
}
*/
int main(int argc, const char *argv[])
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSString *DT=[[NSDate date]descriptionWithCalendarFormat:@"%Y%m%d%H%M%S%F" timeZone:nil locale:nil];
    fileManager=[NSFileManager defaultManager];
    NSNull *null = [NSNull null];
    NSMutableArray *args=[NSMutableArray arrayWithArray:[[NSProcessInfo processInfo] arguments]];
    NSLog(@"%@\r\n%@",DT,[args description]);

    
#pragma mark args[1] relativePaths (could be adapted to recursive)
    NSArray *r;
    if (
        [args[1] isEqualToString:@"*"]
        ||!(r=[fileManager contentsOfDirectoryAtPath:[args[inBasePath]stringByExpandingTildeInPath] error:&err])
        )
    {
        NSLog(@"could not get contents of %@\r%@",args[inBasePath],[err description]);
        return 1;
    }
    else
    {
        BOOL isDirectory=true;
        if (([fileManager fileExistsAtPath:[[args[inBasePath]stringByExpandingTildeInPath]stringByAppendingPathComponent:args[fileRelativePath]] isDirectory:&isDirectory]) && !isDirectory)
            r=@[args[fileRelativePath]];
        else
        {
            NSLog(@"no %@ in %@",args[fileRelativePath],args[inBasePath]);
            return 1;
        }
        
    }
    
    NSString *syntax=@"syntax:\r\nKDU\r\n\t[1]inRelativePathOr*\r\n\t[2]inBasePath/\r\n\t[3]doneBasePath/\r\n\t[4]errBasePath/";
    //\r\n[5..]opt\r\n
    //\t!\"attrPath\" (delete attribute)\r\n
    //\t!\"attrPath\"!{\"Value\":[]} (delete attribute contents)\r\n
    //\t!\"attrPath\"!{\"Value\":[a,b]} (delete values a and b from the values of the attribute)\r\n
    //\t+\"attrPath\"+{\"vr\":\"xx\"} (add empty attribute, if it does not exist)\r\n
    //\t+\"attrPath\"+{\"vr\":\"xx\",\"Value\":[a,b]} (add values a, b to attribute)
    //\r\n
    //\r\n
    //attrPath=ggggeeee[.0001-ggggeeee][..]\r\n
    //deletes performed before additions\r\n

    NSUInteger argsCount=[args count];
    if (argsCount<5) {NSLog(@"%@",syntax);return 1;}
    
    //check folders existence
    NSString *iFolder=[args[inBasePath]stringByExpandingTildeInPath];
    NSString *oFolder=[args[doneBasePath]stringByExpandingTildeInPath];
    NSString *eFolder=[args[errBasePath]stringByExpandingTildeInPath];

    if(
         !folderExists(oFolder,true)
       ||!folderExists(eFolder,true)
       )
        return 1;//exit error

    
#pragma mark args opt
    NSMutableDictionary *dDict=[NSMutableDictionary dictionary];
    NSMutableDictionary *aDict=[NSMutableDictionary dictionary];
    for (int i=5;i<argsCount;i++)
    {
        if ([args[i] hasPrefix:@"!"])
        {
            NSMutableArray *remove=[NSMutableArray arrayWithArray:[args[i] componentsSeparatedByString:@"!"]];
            switch ([remove count]) {
                case 2:
                    [dDict setObject:[NSNull null] forKey:remove[1]];
                    break;
                case 3:
                    [dDict setObject:remove[2] forKey:remove[1]];
                    break;
                default:
                    NSString *attrPath=[NSString stringWithString:remove[1]];
                    [remove removeObjectAtIndex:1];
                    [remove removeObjectAtIndex:0];
                    [dDict setObject:[remove componentsJoinedByString:@"!"] forKey:attrPath];
                    break;
            }
            continue;
        }
        if ([args[i] hasPrefix:@"+"])
        {
            NSMutableArray *add=[NSMutableArray arrayWithArray:[args[i] componentsSeparatedByString:@"+"]];
            switch ([add count]) {
                case 2:
                    [aDict setObject:null forKey:add[1]];
                    break;
                case 3:
                    [aDict setObject:add[2] forKey:add[1]];
                    break;
                default:
                    NSString *attrPath=[NSString stringWithString:add[1]];
                    [add removeObjectAtIndex:1];
                    [add removeObjectAtIndex:0];
                    [aDict setObject:[add componentsJoinedByString:@"+"] forKey:attrPath];
                    break;
            }
            continue;
        }
    }

#pragma mark codecs

    DJEncoderRegistration::registerCodecs(
        ECC_lossyRGB,
        EUC_never,
        OFFalse,
        OFFalse,
        0,
        0,
        0,
        OFTrue,
        ESS_444,
        OFFalse,
        OFFalse,
        0,
        0,
        0.0,
        0.0,
        0,
        0,
        0,
        0,
        OFTrue,
        OFTrue,
        OFFalse,
        OFFalse,
        OFTrue);
    

#pragma mark -for each file
    for( NSString *relFilePath in r)
    {
        if ([[relFilePath lastPathComponent] hasPrefix:@"."])continue;

        NSString *i=[iFolder stringByAppendingPathComponent:relFilePath];
        NSString *o=[oFolder stringByAppendingPathComponent:relFilePath];
        NSString *e=[eFolder stringByAppendingPathComponent:relFilePath];

#pragma mark read
        
        DcmFileFormat fileformat;
        OFCondition cond = fileformat.loadFile( [i UTF8String]);
        if(!cond.good())
        {
            NSLog(@"symlink in err (can not read: %@)",i);
            //myunlink([i fileSystemRepresentation]);
            if (![fileManager createSymbolicLinkAtPath:e
                                   withDestinationPath:i
                                                 error:&err]
                );
#pragma mark ??? fileformat. delete ;
                NSLog(@"can not make symlink: %@\r\n%@",e,[err description]);
            continue;
        }

        //Àneeds to be unlinked, at least before moving it around?
#pragma mark parse
        DcmDataset *dataset = fileformat.getDataset();
        DcmXfer original_xfer(dataset->getOriginalXfer());
        
        if (original_xfer.isEncapsulated())
        {
            NSLog(@"[symlink in done (already compressed:%@)",i);
            //myunlink([i fileSystemRepresentation]);
            if (![fileManager createSymbolicLinkAtPath:o
                                   withDestinationPath:i
                                                 error:&err]
                )
                NSLog(@"can not make symlink: %@\r\n%@",e,[err description]);
            continue;
        }
        
#pragma mark apply opt args
        /*
         NSString *sopInstanceUID=attrStringValue(dataset, 0x0008, 0x0018, NSASCIIStringEncoding);
         NSString *studyInstanceUID=attrStringValue(dataset, 0x0020, 0x000D, NSASCIIStringEncoding);
         //remove NameofPhysiciansReadingStudy (custom3 is already filled with chosen radiologist)
         delete dataset->remove( DcmTagKey( 0x0008, 0x1060));
         //remove SQ reqService (custom2 is already filled with chosen requesting institution)
         delete dataset->remove( DcmTagKey( 0x0032, 0x1034));
         
         // "GEIIS" The problematic private group, containing a *always* JPEG compressed PixelData
         delete dataset->remove( DcmTagKey( 0x0009, 0x1110));
         */
        
        
#pragma mark delete operations
        for (NSString *tagPath in [dDict allKeys])
        {
            id object=[dDict objectForKey:tagPath];
            if (object==[NSNull null])
            {
                NSLog(@"delete the attribute: %@",tagPath);
            }
            else
            {
                NSData *jsonData=[object dataUsingEncoding:NSUTF8StringEncoding];
                id jsonObject=[NSJSONSerialization JSONObjectWithData:jsonData options:0 error:&err];
                if (err)
                {
                    NSLog(@"%@\r\n%@\r\n%@",tagPath,[dDict objectForKey:tagPath],[err description]);
                    err=nil;
                    continue;
                }
                
                if (![jsonObject isKindOfClass:[NSDictionary class]])
                {
                    NSLog(@"%@\r\n%@\r\n%@",tagPath,[dDict objectForKey:tagPath],[err description]);
                    err=nil;
                    continue;
                }
                //there is a dictionary
                id value=[jsonObject valueForKey:@"Value"];
                if ([value isKindOfClass:[NSArray class]])
                {
                    if (![value count])
                    {
                        NSLog(@"delete all values of attribute: %@",tagPath);
                    }
                    else
                    {
                        NSLog(@"delete %@ of attribute: %@", [value description],tagPath);
                    }
                }
            }
        }
        
        
        
#pragma mark additions
        for (NSString *tagPath in [aDict allKeys])
        {
            id object=[dDict objectForKey:tagPath];
            NSData *jsonData=[object dataUsingEncoding:NSUTF8StringEncoding];
            id jsonObject=[NSJSONSerialization JSONObjectWithData:jsonData options:0 error:&err];
            if (err)
            {
                NSLog(@"%@\r\n%@\r\n%@",tagPath,[dDict objectForKey:tagPath],[err description]);
                err=nil;
                continue;
            }
            
            if (![jsonObject isKindOfClass:[NSDictionary class]])
            {
                NSLog(@"%@\r\n%@\r\n%@",tagPath,[dDict objectForKey:tagPath],[err description]);
                err=nil;
                continue;
            }
            //there is a dictionary
            id vr=[jsonObject valueForKey:@"vr"];
            id value=[jsonObject valueForKey:@"Value"];
            if ([value isKindOfClass:[NSString class]])
            {
                if (![value count])
                {
                    NSLog(@"delete all values of attribute: %@",tagPath);
                }
                else
                {
                    NSLog(@"delete %@ of attribute: %@", [value description],tagPath);
                }
            }
        }
        
        
        
#pragma mark compress (revisar bien a que corresponde toda esta sintaxis!!!)
        DJ_RPLossy JP2KParamsLossLess( DCMLosslessQuality);
        DcmRepresentationParameter *params = &JP2KParamsLossLess;        
        DcmXfer oxferSyn( EXS_JPEG2000LosslessOnly);
        dataset->chooseRepresentation(EXS_JPEG2000LosslessOnly, params);
        if (!dataset->canWriteXfer(EXS_JPEG2000LosslessOnly))
        {
            NSLog(@"symlink in err (cannot J2KR for:%@)",i);
            //myunlink([i fileSystemRepresentation]);
            if (![fileManager createSymbolicLinkAtPath:e
                                   withDestinationPath:i
                                                 error:&err]
                )
                NSLog(@"can not make symlink: %@\r\n%@",e,[err description]);
            continue;
        }
        fileformat.loadAllDataIntoMemory();
        
        //write compressed file
        cond = fileformat.saveFile( [o UTF8String], EXS_JPEG2000LosslessOnly);
        if (!cond.good())
        {
            NSLog(@"symlink in err (cannot save J2KR for:%@)",i);
            //myunlink([i fileSystemRepresentation]);
            if (![fileManager createSymbolicLinkAtPath:e
                                   withDestinationPath:i
                                                 error:&err]
                )
            continue;
        }
    }
    [pool release];
    return 0;
}
