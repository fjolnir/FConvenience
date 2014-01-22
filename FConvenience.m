#import "FConvenience.h"
#import <QuartzCore/QuartzCore.h>
#import <asl.h>

static pthread_key_t _ASLClientThreadLocal;
static void _aslClientCleanup(void *client)
{
    asl_close(client);
}

void _FLog(enum FLogLevel const aLevel,
           const char * const aFile,
           int const aLine,
           NSString * const aFormat, ...)
{
    Once(^{ pthread_key_create(&_ASLClientThreadLocal, &_aslClientCleanup); });
    
    aslclient client = pthread_getspecific(_ASLClientThreadLocal);
    if(!client) {
        client = asl_open(NULL,
                          [[Bundle bundleIdentifier] UTF8String],
                          ASL_OPT_STDERR|ASL_OPT_NO_DELAY);
#ifndef DEBUG
        asl_set_filter(client, ASL_FILTER_MASK_UPTO(ASL_LEVEL_WARNING));
#endif
    }
    
    va_list argList;
    va_start(argList, aFormat);
    NSString * const message = [[NSString alloc] initWithFormat:aFormat
                                                      arguments:argList];
    va_end(argList);
    
    aslmsg const msg = asl_new(ASL_TYPE_MSG);
    asl_set(msg, ASL_KEY_READ_UID, "-1");
    asl_log(client, msg, aLevel,
            "%10.15s:%u: %s",
            [[@(aFile) lastPathComponent] UTF8String], aLine, [message UTF8String]);
    asl_free(msg);
}

#if !defined(__IPHONE_6_0) || __IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_6_0
@implementation NSArray (Subscripts)
- (id)objectAtIndexedSubscript:(NSUInteger)idx
{
    return [self objectAtIndex:idx];
}
@end
@implementation NSMutableArray (Subscripts)
- (void)setObject:(id)obj atIndexedSubscript:(NSUInteger)idx
{
    [self replaceObjectAtIndex:idx withObject:obj];
}
@end

// NSMutableDictionary+Subscripts.m
@implementation  NSDictionary (Subscripts)

- (id)objectForKeyedSubscript:(id)key
{
    return [self objectForKey:key];
}
@end
@implementation  NSMutableDictionary (Subscripts)
- (void)setObject:(id)obj forKeyedSubscript:(id)key
{
    [self setObject:obj forKey:key];
}
@end
#endif

@implementation NSUserDefaults (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey
{
    return [self objectForKey:aKey];
}
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey
{
    [self setObject:aObj forKey:aKey];
}
@end

@implementation NSCache (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey
{
    return [self objectForKey:aKey];
}
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey
{
    [self setObject:aObj forKey:aKey];
}
@end

#if TARGET_OS_IPHONE && defined(__OBJC__)

UIImage *FScreenshot(float const aScale)
{
    CGSize const imageSize = [[UIScreen mainScreen] bounds].size;
    UIGraphicsBeginImageContextWithOptions(imageSize, YES, aScale);

    CGContextRef const context = UIGraphicsGetCurrentContext();

    UIGraphicsPushContext(context);
    for(UIWindow * const win in UIApp.windows)
    {
        if([win screen] == [UIScreen mainScreen]) {
            CGContextSaveGState(context);
            CGContextTranslateCTM(context, [win center].x, [win center].y);
            CGContextConcatCTM(context, [win transform]);
            CGContextTranslateCTM(context,
                                  -[win bounds].size.width * [[win layer] anchorPoint].x,
                                  -[win bounds].size.height * [[win layer] anchorPoint].y);

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 70000
            if([win respondsToSelector:@selector(drawViewHierarchyInRect:afterScreenUpdates:)])
                [win drawViewHierarchyInRect:win.frame afterScreenUpdates:NO];
            else
#endif
                [[win layer] renderInContext:context];

            CGContextRestoreGState(context);
        }
    }
    UIGraphicsPopContext();
    const uint8_t * const pixels = CGBitmapContextGetData(context);
    
    size_t const bpr  = CGBitmapContextGetBytesPerRow(context);
    
    UIInterfaceOrientation const orien = UIApp.statusBarOrientation;
    BOOL const upsideDown = (orien == UIInterfaceOrientationPortraitUpsideDown)
                         || (orien == UIInterfaceOrientationLandscapeRight);

    size_t startIdx;
    if(UIInterfaceOrientationIsLandscape(orien) && upsideDown)
        startIdx = 4 * (CGBitmapContextGetWidth(context) - 21);
    else if(UIInterfaceOrientationIsLandscape(orien))
        startIdx = 21 * 4;
    else if(upsideDown)
        startIdx = bpr * (CGBitmapContextGetHeight(context) - 21);
    else
        startIdx = bpr * 21;
    
    CGFloat topColor[4] = {
        pixels[startIdx    ] / 255.0f,
        pixels[startIdx + 1] / 255.0f,
        pixels[startIdx + 2] / 255.0f,
        pixels[startIdx + 3] / 255.0f
    };

    CGContextSetFillColorWithColor(context,
                                   RGBA(topColor[2], topColor[1], topColor[0], topColor[3]).CGColor);
    
    if(UIInterfaceOrientationIsLandscape(orien)) CGContextFillRect(context, (CGRect) {
        upsideDown ? imageSize.width - 20.0f : 0.0f,
        0,
        20, imageSize.height
    });
    else CGContextFillRect(context, (CGRect) {
        0,
        upsideDown ? imageSize.height - 20.0f : 0,
        imageSize.width, 20
    });

    UIImage * const image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

#endif
