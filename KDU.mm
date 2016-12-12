#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>

#undef verify
#include "osconfig.h" /* make sure OS specific configuration is included first */
//JF#include "dcdebug.h" deprecated in 2009.11.04
#include "dcmtk/oflog/oflog.h"
#include "djdecode.h" /* for dcmjpeg decoders */
#include "djencode.h" /* for dcmjpeg encoders */
#include "djrploss.h"
#include "djrplol.h"
#include "dcpixel.h"
#include "dcrlerp.h"
#include "dcdicdir.h"
#include "dcdatset.h"
#include "dcmetinf.h"
#include "dcfilefo.h"
#include "dcuid.h"
#include "dcdict.h"
#include "dcdeftag.h"

enum DCM_CompressionQuality {
    DCMLosslessQuality = 0,
    DCMHighQuality,
    DCMMediumQuality,
    DCMLowQuality
};

enum baseArgs {
    fileRelativePath=1,
    inBasePath,
    doneBasePath,
    errBasePath,
    logVerbosity,
};

//JF extern void dcmtkSetJPEGColorSpace( int);

static NSFileManager *fileManager=nil;
static NSError *err=nil;
//short Use_kdu_IfAvailable = 1;

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

NSString* attrStringValue(DcmDataset *dataset, short g, short e, NSStringEncoding c)
{
    const char *s = NULL;
    if (dataset->findAndGetString(DcmTagKey(g,e), s, OFFalse).good() && s != NULL)
        return [NSString stringWithCString:s encoding:c];
    return nil;
}

int main(int argc, const char *argv[])
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSDateFormatter *DTDateFormatter = [[NSDateFormatter alloc] init];
    [DTDateFormatter setDateFormat:@"yyyyMMddHHmmssSSSSS"];
    NSString *DT = [DTDateFormatter stringFromDate:[NSDate date]];
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
    
    NSString *syntax=@"syntax:\r\nKDU\r\n\t[1]inRelativePathOr*\r\n\t[2]inBasePath/\r\n\t[3]doneBasePath/\r\n\t[4]errBasePath/\r\n\t[5] FATAL_LOG_LEVEL, ERROR_LOG_LEVEL, WARN_LOG_LEVEL, INFO_LOG_LEVEL, DEBUG_LOG_LEVEL, TRACE_LOG_LEVEL";
    //\r\n[6..]opt\r\n
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
    if (argsCount<6) {NSLog(@"%@",syntax);return 1;}
    
    //check folders existence
    NSString *iFolder=[args[inBasePath]stringByExpandingTildeInPath];
    NSString *oFolder=[args[doneBasePath]stringByExpandingTildeInPath];
    NSString *eFolder=[args[errBasePath]stringByExpandingTildeInPath];
    if(
         !folderExists(oFolder,true)
       ||!folderExists(eFolder,true)
       )
        return 1;//exit error

    
    enum loglevel {
        FATAL_LOG_LEVEL,
        ERROR_LOG_LEVEL,
        WARN_LOG_LEVEL,
        INFO_LOG_LEVEL,
        DEBUG_LOG_LEVEL,
        TRACE_LOG_LEVEL,
    };

    BOOL FATAL=false;
    BOOL ERROR=false;
    BOOL WARN=false;
    BOOL INFO=false;
    BOOL DEBUG=false;
    BOOL TRACE=false;
    if      ([args[logVerbosity] isEqualToString:@"FATAL_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::FATAL_LOG_LEVEL);
        FATAL=true;
    }
    else if ([args[logVerbosity] isEqualToString:@"ERROR_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::ERROR_LOG_LEVEL);
        FATAL=true;
        ERROR=true;
    }
    else if ([args[logVerbosity] isEqualToString:@"WARN_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::WARN_LOG_LEVEL);
        FATAL=true;
        ERROR=true;
        WARN=true;
    }
    else if ([args[logVerbosity] isEqualToString:@"INFO_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::INFO_LOG_LEVEL);
        FATAL=true;
        ERROR=true;
        WARN=true;
        INFO=true;
    }
    else if ([args[logVerbosity] isEqualToString:@"DEBUG_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::DEBUG_LOG_LEVEL);
        FATAL=true;
        ERROR=true;
        WARN=true;
        INFO=true;
        DEBUG=true;
    }
    else if ([args[logVerbosity] isEqualToString:@"TRACE_LOG_LEVEL"])
    {
        OFLog::configure(OFLogger::TRACE_LOG_LEVEL);
        FATAL=true;
        ERROR=true;
        WARN=true;
        INFO=true;
        DEBUG=true;
        TRACE=true;
    }
    else
    {
        NSLog(@"log verbosity (arg[5] required to be one of FATAL_LOG_LEVEL, ERROR_LOG_LEVEL, WARN_LOG_LEVEL, INFO_LOG_LEVEL, DEBUG_LOG_LEVEL, TRACE_LOG_LEVEL");
        return 1;//exit error
    }
    
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
    
    // register global JPEG decompression codecs
    DJDecoderRegistration::registerCodecs();
#pragma mark TODO decompression codecs
    // register JPEG-LS decompression codecs
    //JF DJLSDecoderRegistration::registerCodecs();
    // register RLE compression codec
    //JF DcmRLEEncoderRegistration::registerCodecs();


#pragma mark codecs

    DJEncoderRegistration::registerCodecs(
        ECC_lossyRGB, //E_CompressionColorSpaceConversion opt_compCSconversion for decompression only
        EUC_never,  //E_UIDCreation opt_uidcreation
        OFFalse,    //OFBool opt_huffmanOptimize for 8 bits/pixel
        0,          //OFCmdUnsignedInt OFstatic_cast(int, opt_smoothing) [0..100]
        0,          //int opt_compressedBits pForcedBitDepth [0(auto) | 8 | 12 | 16]
        0,          //OFstatic_cast(Uint32, opt_fragmentSize) (in kbytes | 0=unlimited)
        OFTrue,     //OFBool opt_createOffsetTable during image compression
        ESS_444,    //E_SubSampling opt_sampleFactors subsampling mode for color image compression
        OFFalse,    //OFBool opt_useYBR422 [YBR_FULL or YBR_FULL_422]
        OFFalse,    //OFBool opt_secondarycapture
        0,          //int OFstatic_cast(Uint32, opt_windowType) mode [1..7] VOI transformation of monochrome images
        0,          //OFCmdUnsignedInt OFstatic_cast(Uint32, opt_windowParameter) param form modes 1, 2, 4, 6
        0.0,        //OFCmdFloat opt_windowCenter for mode 5
        0.0,        //OFCmdFloat opt_windowWidth for mode 5
        0,          //OFCmdUnsignedInt OFstatic_cast(Uint32, opt_roiLeft) for monochrome images, mode 7
        0,          //OFCmdUnsignedInt OFstatic_cast(Uint32, opt_roiTop) for monochrome images, mode 7
        0,          //OFCmdUnsignedInt OFstatic_cast(Uint32, opt_roiWidth) for monochrome images, mode 7
        0,          //OFCmdUnsignedInt OFstatic_cast(Uint32, opt_roiHeight) for monochrome images, mode 7
        OFTrue,     //OFBool opt_usePixelValues Check smallest, largest pixel value and optimize, mode 0 only
        OFTrue,     //OFBool opt_useModalityRescale to scale back, to original pixel range, mode 0 only
        OFFalse,    //OFBool opt_acceptWrongPaletteTags (only "pseudo lossless" encoder)
        OFFalse,    //OFBool opt_acrNemaCompatibility
        OFTrue);    //OFBool opt_trueLossless

    if (DEBUG)
    {
        //we removed the private caracter of codec registrations
        if (DJEncoderRegistration::encbas != NULL) NSLog(@"codec JPEG encbas registered");
        if (DJEncoderRegistration::encext != NULL) NSLog(@"codec JPEG encext registered");
        if (DJEncoderRegistration::encsps != NULL) NSLog(@"codec JPEG encsps registered");
        if (DJEncoderRegistration::encpro != NULL) NSLog(@"codec JPEG encpro registered");
        if (DJEncoderRegistration::encsv1 != NULL) NSLog(@"codec JPEG encsv1 registered");
        if (DJEncoderRegistration::enclol != NULL) NSLog(@"codec JPEG enclol registered");
    }

    
#pragma mark -for each file
    for( NSString *relFilePath in r)
    {
        if ([[relFilePath lastPathComponent] hasPrefix:@"."])continue;

        NSString *i=[iFolder stringByAppendingPathComponent:relFilePath];
        NSString *o=[oFolder stringByAppendingPathComponent:relFilePath];
        NSString *e=[eFolder stringByAppendingPathComponent:relFilePath];

#pragma mark read
        
        DcmFileFormat fileformat;
        OFCondition cond = fileformat.loadFile( [i UTF8String]);//requires libiconv
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
        
         //NSString *sopInstanceUID=attrStringValue(dataset, 0x0008, 0x0018, NSASCIIStringEncoding);
         //NSString *studyInstanceUID=attrStringValue(dataset, 0x0020, 0x000D, NSASCIIStringEncoding);
         //remove NameofPhysiciansReadingStudy (custom3 is already filled with chosen radiologist)
         delete dataset->remove( DcmTagKey( 0x0008, 0x1060));
         //remove SQ reqService (custom2 is already filled with chosen requesting institution)
         delete dataset->remove( DcmTagKey( 0x0032, 0x1034));
         
         // "GEIIS" The problematic private group, containing a *always* JPEG compressed PixelData
         delete dataset->remove( DcmTagKey( 0x0009, 0x1110));
        

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
        
#pragma mark JF JPEG prueba
/*
 E_TransferSyntax opt_oxfer
 
    EXS_LittleEndianImplicit = 0
    EXS_LittleEndianExplicit = 2
 
    EXS_JPEGProcess14SV1 (lossless-sv1)
    EXS_JPEGProcess1 (baseline lossy) =4
    EXS_JPEGProcess2_4 (extended sequencial lossy 8/12 bits) =5
    EXS_JPEGProcess3_5 (extended sequencial lossy 8/12 bits arithmetic coding) =6
    EXS_JPEGProcess6_8 (spectral Non-Hierarchical lossy, 8/12 bit) =7
    EXS_JPEGProcess7_9 (spectral Non-Hierarchical lossy, 8/12 bit arithmetic coding) =8
    EXS_JPEGProcess10_12 (Full Progression, Non-Hierarchical lossy, 8/12 bit) =9
    EXS_JPEGProcess11_13 (Full Progression, Non-Hierarchical lossy, 8/12 bit arithmetic coding) =10
    EXS_JPEGProcess14 (lossless) =11
    EXS_JPEGProcess15 (lossless arithmetic coding) =12
    EXS_JPEGProcess16_18 (Extended Sequential, Hierarchical lossy, 8/12 bit) = 13
    EXS_JPEGProcess17_19 (Extended Sequential, Hierarchical lossy, 8/12 bit arithmetic coding) = 14

 
 
 /// JPEG Spectral Selection, Hierarchical (lossy, 8/12 bit)
 EXS_JPEGProcess20_22 = 15,
 /// JPEG Spectral Selection, Hierarchical (lossy, 8/12 bit), arithmetic coding
 EXS_JPEGProcess21_23 = 16,
 /// JPEG Full Progression, Hierarchical (lossy, 8/12 bit)
 EXS_JPEGProcess24_26 = 17,
 /// JPEG Full Progression, Hierarchical (lossy, 8/12 bit), arithmetic coding
 EXS_JPEGProcess25_27 = 18,
 /// JPEG Lossless, Hierarchical
 EXS_JPEGProcess28 = 19,
 /// JPEG Lossless, Hierarchical, arithmetic coding
 EXS_JPEGProcess29 = 20,
 /// JPEG Lossless, Selection Value 1
 EXS_JPEGProcess14SV1 = 21,
 /// Run Length Encoding (lossless)
 EXS_RLELossless = 22,
 /// JPEG-LS (lossless)
 EXS_JPEGLSLossless = 23,
 /// JPEG-LS (lossless or near-lossless mode)
 EXS_JPEGLSLossy = 24,
 /// Deflated Explicit VR Little Endian
 EXS_DeflatedLittleEndianExplicit = 25,
 /// JPEG 2000 (lossless)
 EXS_JPEG2000LosslessOnly = 26,
 /// JPEG 2000 (lossless or lossy)
 EXS_JPEG2000 = 27,
 /// MPEG2 Main Profile at Main Level
 EXS_MPEG2MainProfileAtMainLevel = 28,
 /// MPEG2 Main Profile at High Level
 EXS_MPEG2MainProfileAtHighLevel = 29,
 /// MPEG4 High Profile / Level 4.1
 EXS_MPEG4HighProfileLevel4_1 = 30,
 /// MPEG4 BD-compatible High Profile / Level 4.1
 EXS_MPEG4BDcompatibleHighProfileLevel4_1 = 31,
 /// MPEG4 High Profile / Level 4.2 For 2D Video
 EXS_MPEG4HighProfileLevel4_2_For2DVideo = 32,
 /// MPEG4 High Profile / Level 4.2 For 3D Video
 EXS_MPEG4HighProfileLevel4_2_For3DVideo = 33,
 /// MPEG4 Stereo High Profile / Level 4.2
 EXS_MPEG4StereoHighProfileLevel4_2 = 34,
 /// JPEG 2000 part 2 multi-component extensions (lossless)
 EXS_JPEG2000MulticomponentLosslessOnly = 35,
 /// JPEG 2000 part 2 multi-component extensions (lossless or lossy)
 EXS_JPEG2000Multicomponent = 36,
 /// JPIP Referenced
 EXS_JPIPReferenced = 37,
 /// JPIP Referenced Deflate
 EXS_JPIPReferencedDeflate = 38

*/
/*
 DcmXfer opt_oxferSyn()
 ?
 */
        DcmXfer opt_oxferSyn(EXS_JPEGProcess1);
        DJ_RPLossy param;
        dataset->chooseRepresentation(EXS_JPEGProcess1, &param);
        if (dataset->canWriteXfer(EXS_JPEGProcess1))
        {
            NSLog(@"possible JPEGProcess1");
            
        }
        else NSLog(@"not possible JPEGProcess1");
        fileformat.loadAllDataIntoMemory();
        cond = fileformat.saveFile([o UTF8String], EXS_JPEGProcess1);//EXS_LittleEndianExplicit);//
        
#pragma mark TODO compress (revisar bien a que corresponde toda esta sintaxis!!!)
        /*
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
 */

    }
    [pool release];
    return 0;

}
