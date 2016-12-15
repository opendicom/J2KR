#import "DCMTKfileFormat.h"

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

@interface DCMTKfileFormat()
-(BOOL)moveToTagPath:(NSString*)tagPath;

@end


@implementation DCMTKfileFormat

-(BOOL)moveToTagPath:(NSString*)tagPath
{
    currentItem=dataset;
    if (!tagPath) return false;
    NSArray *tagPathComponents=[tagPath tagPathComponents];
    NSUInteger lastElementIndex=[tagPathComponents count]-1;
    NSLog(@"%@",[tagPathComponents description]);
    
    for (int c=0;c<lastElementIndex;c++)
    {
        NSString *tagPathComponent=[tagPathComponents objectAtIndex:c];
        currentTag=[[tagPathComponent stringByDeletingPathExtension]tagLittleEndian];
        if (currentTag==0)return false;
        
        DcmSequenceOfItems *nodeSequenceOfItems;
        if ((currentItem->findAndGetSequence(DcmTagKey(currentTag/0x10000,currentTag%0x10000), nodeSequenceOfItems)).bad())
        {
            NSLog(@"sequence (%04x,%04x) not found",(unsigned int)currentTag/0x10000,(unsigned int)currentTag%0x10000);
            return false;
        }
        NSLog(@"nodeSequenceOfItems length %lu",(unsigned long)(nodeSequenceOfItems->getLength(EXS_LittleEndianImplicit,EET_UndefinedLength)));

        //*nodeSequenceOfItems = dynamic_cast<DcmSequenceOfItems*> (nodeElement);
        if (nodeSequenceOfItems==NULL)
        {
            NSLog(@"nodeSequenceOfItems NULL");
            return false;
        }
        NSString *itemCardString=[tagPathComponent pathExtension];
        if ([itemCardString length]==0)
        {
            NSLog(@"no item card");
            return false;
        }
        NSUInteger card=(NSUInteger)[itemCardString integerValue];
        NSLog(@"itemCard: %d",card);
        DcmItem *Item=nodeSequenceOfItems->getItem(card);
        //unsigned long cc=Item->card();
        NSLog(@"item length: %lu",(unsigned long)(Item->getLength(EXS_LittleEndianImplicit,EET_UndefinedLength)));
        if (currentItem==NULL)NSLog(@"currentItem NULL");
    }
    NSString *tagString=[[tagPathComponents lastObject] stringByDeletingPathExtension];
    currentTag=[tagString tagLittleEndian];
    NSLog(@"\"%@\"=%d",tagString,(unsigned int)currentTag);
    return currentTag;
}

-(id)initWithPath:(NSString*)path
{
    self=[super init];
    if (self)
    {
        fileformat = new DcmFileFormat();
        if ((fileformat->loadFile([path UTF8String])).good())
        {
            dataset = new DcmDataset();
            dataset =fileformat->getDataset();
            metaInfo = new DcmMetaInfo();
            metaInfo = fileformat->getMetaInfo();
            baseItem = new DcmItem();
            currentItem = new DcmItem();
            baseItem = metaInfo;
            currentItem = metaInfo;
            currentTag=0;
            currentVR = new DcmVR();
            currentValue=[NSMutableArray array];
            return self;
        }
        NSLog(@"could not load file for path: %@",path);
        [self release];
        return nil;
    }
    NSLog(@"could not instantiate DcmFileFormatWrapper");
    return nil;
 }

-(BOOL)datasetAsBaseItem
{
    if (dataset==nil)return false;
    baseItem = dataset;
    return true;
}
-(BOOL)metaInfoAsBaseItem;
{
    if (metaInfo==nil)return false;
    baseItem = metaInfo;
    return true;
}
-(BOOL)curentItemAsBaseItem
{
    if (currentItem==nil)return false;
    baseItem = currentItem;
    return true;
}

-(void)dealloc
{
    delete fileformat;
    delete dataset;
    delete metaInfo;
    delete baseItem;
    delete currentItem;
    delete currentVR;
    [super dealloc];
}

-(BOOL)isEncapsulated
{
    DcmXfer original_xfer(dataset->getOriginalXfer());
    return original_xfer.isEncapsulated();
}

-(NSString*)calculatePixelsMd5AndOverwrite:(BOOL)overwrite
{
    const Uint8 *pixelBuffer = nil;
    unsigned int pixelLength;
    if (!(dataset->findAndGetUint8Array(DCM_PixelData, pixelBuffer, &pixelLength, OFFalse).good() && pixelLength > 0))
    {
        NSLog(@"can not get pixel data");
        return nil;
    }
    
    unsigned char md5[CC_MD5_DIGEST_LENGTH];
    CC_MD5(pixelBuffer, CC_MD5_DIGEST_LENGTH, md5);
    //NSLog(@"pixelLength:%d md5:%s",pixelLength,md5);
    
    if (overwrite)
    {
        delete metaInfo->remove(DCM_PrivateInformationCreatorUID);
        char privateInformationCreatorUID[]="2.16.858.0.2.6.46.5.1.0.213918600019";
        metaInfo->putAndInsertString( DCM_PrivateInformationCreatorUID, privateInformationCreatorUID);
        
        delete metaInfo->remove(DCM_PrivateInformation);
        metaInfo->putAndInsertUint8Array(DCM_PrivateInformation, md5, CC_MD5_DIGEST_LENGTH);
    }
    
    return [NSString stringWithFormat:@"%s",md5];
}


-(BOOL)deleteTagPath:(NSString*)tagPath
{
    if(![self moveToTagPath:tagPath]) return false;
    
    NSString *tagPathExtension=[tagPath pathExtension];
    if ([tagPathExtension length])
    {
        NSLog(@"dcmtk findAndDeleteSequenceItem not available");
        return false;
    }
    else if ((currentItem->findAndDeleteElement (DcmTagKey(currentTag/0x10000,currentTag%0x10000), OFFalse, OFFalse)).bad()) return false;
    return true;
}


-(BOOL)selectTagPath:(NSString*)tagPath
{
    if(![self moveToTagPath:tagPath])
    {
        NSLog(@"could not get last path component");
        return false;
    }
    NSString *tagPathExtension=[tagPath pathExtension];
    if ([tagPathExtension length])
    {
        NSLog(@"dcmtk findAndDeleteSequenceItem not available");
        return false;
    }
    //tagPath may point to an element
    if (currentItem==nil)
    {
        NSLog(@"currentItem nil");
        return false;
    }
    
    //get element VR
    
    unsigned int count=0;
    const Uint8 *uint8Buffer=nil;
    if ((currentItem->findAndGetUint8Array (DcmTagKey(currentTag/0x10000,currentTag%0x10000), uint8Buffer, &count ,OFFalse)).good())
    {
        NSLog(@"%s",uint8Buffer);
        NSLog(@"hola");
    }

    
    
    const char *string = nil;
    if ((dataset->findAndGetString (DcmTagKey(currentTag/0x10000,currentTag%0x10000), string)).good())
    {
        NSLog(@"patient name: %s",string);
    }
    return false;
}

/*-(BOOL)overwriteTagPath:(NSString*)tagPath withJSON:(NSString*)JSONString
{
    
}
*/

-(NSString*)elementStringValue
{
    
}

-(NSArray*)elementStringValues
{
    
}

@end
