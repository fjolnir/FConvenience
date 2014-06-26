// Frequently used macros for uncluttering things (Import in your PCH)

#ifndef _FCONV_H_
#define _FCONV_H_

#pragma mark - C -

#include <pthread.h>
#include <stdio.h>

#pragma mark Debug Utils
#ifdef DEBUG
#   define DEBUG_ON YES
#   define CrashHere()   { *(int *)0 = 0xDEADBEEF; }
#   define IfDebug(body...) body
#else
#   define DEBUG_ON NO
#   define CrashHere()
#   define IfDebug(body...)
#endif

#pragma mark Looping

#define unless(body...) if(!(body))
#define until(body...)  while(!(body))

#pragma mark Numerical Ranges

#define PASTE(a,b) a##b
#ifndef MIN
#   define __MIN(a,b,c) ({ \
        __typeof(a) const PASTE(__a,c) = (a); \
        __typeof(b) const PASTE(__b,c) = (b); \
        (__typeof(a))(PASTE(__a,c) > PASTE(__b,c) ? PASTE(__b,c) : PASTE(__a,c)); \
    })
#   define MIN(a,b) __MIN((a), (b), __COUNTER__)
#endif
#ifndef MAX
#   define __MAX(a,b,c) ({ \
        __typeof(a) const PASTE(__a,c) = (a); \
        __typeof(b) const PASTE(__b,c) = (b); \
        PASTE(__a,c) > PASTE(__b,c) ? PASTE(__a,c) : PASTE(__b,c); \
    })
#   define MAX(a,b) __MAX((a), (b), __COUNTER__)
#endif

#ifndef BETWEEN
#   define __BETWEEN(val, low, high, c) ({ \
        __typeof(val) const PASTE(__val,c) = (val); \
        PASTE(__val,c) > (low) && PASTE(__val,c) < (high); \
    })
#   define BETWEEN(val, low, high) __BETWEEN((val), (low), (high), __COUNTER__)
#endif
#ifndef INRANGE
#   define __INRANGE(val, low, high, c) ({ \
        __typeof(val) const PASTE(__val,c) = (val); \
        PASTE(__val,c) >= (low) && PASTE(__val,c) <= (high); \
    })
#   define INRANGE(val, low, high) __INRANGE((val), (low), (high), __COUNTER__)
#endif
#ifndef CLAMP
#   define CLAMP(val, min, max) MAX((min), MIN((val), (max)))
#endif

#define POWOF2(n) ({ __typeof(n) __n = (n); (__n != 0) && !(__n & (__n - 1)); })

#if defined(TARGET_OS_IPHONE) && !FCONVENIENCE_USE_DOUBLE
    typedef float FFloat;
#else
    typedef double FFloat;
#endif

typedef struct _FFloatRange {
    FFloat location, length;
} FFloatRange;

static inline FFloat FFloatRangeMax(FFloatRange const aRange) {
    return aRange.location + aRange.length;
}

#if CGFLOAT_IS_DOUBLE
#    define CGFLOAT_EPSILON DBL_EPSILON
#    define cgfabs fabs
#else
#    define CGFLOAT_EPSILON FLT_EPSILON
#    define cgfabs fabsf
#endif

#define _NSCompare(x, y, c) ({ \
    __typeof(x) PASTE(__x,c) = (x); \
    __typeof(y) PASTE(__y,c) = (y); \
      PASTE(__x,c) > PASTE(__y,c) ? NSOrderedDescending \
    : PASTE(__x,c) < PASTE(__y,c) ? NSOrderedAscending \
                                  : NSOrderedSame; \
})
#define NSCompare(x,y) _NSCompare((x), (y), __COUNTER__)

#pragma mark GCD Utilities
#if __has_extension(blocks)
#   include <dispatch/dispatch.h>
#   define GlobalQueue dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
#   define MainQueue dispatch_get_main_queue()

#   define Once(body...) do { \
        static dispatch_once_t __token; \
        dispatch_once(&__token, ##body); \
    } while(0)
    static inline void Async(dispatch_block_t const blk)       { dispatch_async(GlobalQueue, blk); }
    static inline void AsyncOnMain(dispatch_block_t const blk) { dispatch_async(MainQueue, blk);   }
    static inline void SyncOnMain(dispatch_block_t  const blk) {
        if(pthread_main_np())
            blk();
        else
            dispatch_sync(MainQueue, blk);
    }
    static inline void AfterDelay(float const seconds, dispatch_block_t const blk) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, seconds * NSEC_PER_SEC),
                       MainQueue, blk);
    }

#   define Memoize(x...) ({ \
        static __typeof(({ x; })) __memoized_x; \
        Once(^{ __memoized_x = ({ x; }); }); \
        __memoized_x; \
    })
#endif


#pragma mark - Objective-C -
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#if TARGET_OS_IPHONE
#   include <UIKit/UIKit.h>
#endif


#pragma mark Logging
enum FLogLevel {
    FLogLevel_Emergency,
    FLogLevel_Alert,
    FLogLevel_Critical,
    FLogLevel_Err,
    FLogLevel_Warning,
    FLogLevel_Notice
};

void _FLog(enum FLogLevel aLevel,
           const char * aFile,
           int aLine,
           NSString *aFormat, ...) NS_FORMAT_FUNCTION(4,5);
#define Log(lvl, msg...)   _FLog((lvl), __FILE__, __LINE__, ##msg)
#define LogError(msg...)   Log(FLogLevel_Err, ##msg)
#define LogWarning(msg...) Log(FLogLevel_Warning, ##msg)
#define LogNotice(msg...)  Log(FLogLevel_Notice, ##msg)

#define _CheckOSErr(shouldAssert, error, fmt, params...) do { \
    OSStatus __err = (error); \
    if(__err) { \
        LogError(@"OSErr %d: " fmt, (int)__err, ##params); \
        assert(__err == noErr); \
    } \
} while(0)

#ifdef DEBUG
#   define CheckOSErr(err, fmt, params...) _CheckOSErr(true, err, fmt, ##params)
#   define glError() do { \
        const char *errStr = NULL; \
        GLenum err; \
        while((err = glGetError()) != GL_NO_ERROR) { \
            switch(err) { \
                case GL_INVALID_ENUM:      errStr = "GL_INVALID_ENUM";      break; \
                case GL_INVALID_VALUE:     errStr = "GL_INVALID_VALUE";     break; \
                case GL_OUT_OF_MEMORY:     errStr = "GL_OUT_OF_MEMORY";     break; \
                case GL_INVALID_OPERATION: errStr = "GL_INVALID_OPERATION"; break; \
                case GL_INVALID_FRAMEBUFFER_OPERATION: \
                    errStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
                default: errStr = "UNKNOWN"; \
            } \
            LogError(@"glError(0x%04x): %s caught at %s:%u\n", \
                     err, errStr, __FILE__, __LINE__); \
        } \
        NSCAssert(errStr == NULL, @"OpenGL Error"); \
    } while(0)
#else
#   define CheckOSErr(err, fmt, params...) _CheckOSErr(false, err, fmt, ##params)
#   define glError()
#endif

#pragma mark Shorthands
#define NotificationCenter      [NSNotificationCenter defaultCenter]
#define Bundle                  [NSBundle mainBundle]
#define Workspace               [NSWorkspace sharedWorkspace]
#define FileManager             [NSFileManager defaultManager]
#define Defaults                [NSUserDefaults standardUserDefaults]
#define NSFormat(fmt...)        [NSString stringWithFormat:fmt]
#define NSMutableFormat(fmt...) [NSMutableString stringWithFormat:fmt]
#define Predicate(fmt...)       [NSPredicate predicateWithFormat:fmt]
#define FetchReq(name)          [NSFetchRequest fetchRequestWithEntityName:(name)]
#define ThreadDict              [[NSThread currentThread] threadDictionary]
#define NSNullToNil(x) ({ \
    __typeof(x) __x = (x); \
    [[NSNull null] isEqual:__x] ? nil : __x; \
})
#if TARGET_OS_IPHONE
#   define Device [UIDevice currentDevice]
#   define UIApp  [UIApplication sharedApplication]
#   define CacheDir ({ \
    NSArray * const cacheURLs = [FileManager URLsForDirectory:NSCachesDirectory \
                                                    inDomains:NSUserDomainMask]; \
    NSCAssert([cacheURLs count] > 0, @"Couldn't get cache directory path"); \
    (NSString *)[cacheURLs[0] path]; \
})
#endif

#define DefineThreadLocal(name) \
    static void __ThreadLocalCleanup_##name##__(void *ptr) { id const __unused obj = (__bridge_transfer id)ptr; } \
    static pthread_key_t __ThreadLocal_##name##__
#define InitThreadLocal(name) \
    Once(^{ pthread_key_create(&__ThreadLocal_##name##__, &__ThreadLocalCleanup_##name##__); })
#define ReadThreadLocal(name) \
    (__bridge id)pthread_getspecific(__ThreadLocal_##name##__)
#define WriteThreadLocal(name, value) ({ \
    id const __unused __oldValue__ = (__bridge_transfer id)pthread_getspecific(__ThreadLocal_##name##__); \
    id const __value__ = (value); \
    pthread_setspecific(__ThreadLocal_##name##__, (__bridge_retained void *)__value__); \
    __value__; \
})

#pragma mark Boxing
#define OVERLOADABLE __attribute((overloadable))
static inline NSNumber * OVERLOADABLE FBox(char x)    { return @(x); }
static inline NSNumber * OVERLOADABLE FBox(short x)   { return @(x); }
static inline NSNumber * OVERLOADABLE FBox(int x)     { return @(x); }
static inline NSNumber * OVERLOADABLE FBox(long x)    { return @(x); }
static inline NSNumber * OVERLOADABLE FBox(float x)   { return @(x); }
static inline NSNumber * OVERLOADABLE FBox(double x)  { return @(x); }
static inline NSString * OVERLOADABLE FBox(char *x)   { return @(x); }
static inline NSValue  * OVERLOADABLE FBox(NSRange x) { return [NSValue valueWithRange:x]; }
#if TARGET_OS_IPHONE
    static inline NSValue * OVERLOADABLE FBox(CGRect x)            { return [NSValue valueWithCGRect:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGPoint x)           { return [NSValue valueWithCGPoint:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGSize x)            { return [NSValue valueWithCGSize:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGAffineTransform x) { return [NSValue valueWithCGAffineTransform:x]; }

    static inline NSValue * OVERLOADABLE FBox(UIOffset x)          { return [NSValue valueWithUIOffset:x]; }
    static inline NSValue * OVERLOADABLE FBox(UIEdgeInsets x)      { return [NSValue valueWithUIEdgeInsets:x]; }
#   ifdef CMTIMERANGE_H
        static inline NSValue * OVERLOADABLE FBox(CMTimeRange x)   { return [NSValue valueWithCMTimeRange:x]; }
        static inline NSValue * OVERLOADABLE FBox(CMTimeMapping x) { return [NSValue valueWithCMTimeMapping:x]; }
#   endif
#   ifdef __CORELOCATION__
        static inline NSValue * OVERLOADABLE FBox(CLLocationCoordinate2D x) { return [NSValue valueWithMKCoordinate:x]; }
        static inline NSValue * OVERLOADABLE FBox(MKCoordinateSpan x)       { return [NSValue valueWithMKCoordinateSpan:x]; }
#   endif
#else
    static inline NSValue * OVERLOADABLE FBox(NSRect x)  { return [NSValue valueWithRect:x]; }
    static inline NSValue * OVERLOADABLE FBox(NSPoint x) { return [NSValue valueWithPoint:x]; }
    static inline NSValue * OVERLOADABLE FBox(NSSize x)  { return [NSValue valueWithSize:x]; }
#endif
#undef OVERLOADABLE


#pragma mark Device Detection (iOS)
#if TARGET_OS_IPHONE
#   define UIIdiomString() ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) ? @"ipad" : @"iphone")
#   define DeviceIsIPad() (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)

#    define iOS Version Detection
#    ifndef __IPHONE_7_0
#        define __IPHONE_7_0 (70000)
#    endif
#    ifndef NSFoundationVersionNumber_iOS_6_1
#        define NSFoundationVersionNumber_iOS_6_1 (DBL_MAX)
#    endif

#   define SevenOrNewer() (NSFoundationVersionNumber > NSFoundationVersionNumber_iOS_6_1)

// Runs a block of Code only if building using the iOS 7 SDK & running on iOS 7
// (iOS 6 SDK build running on iOS 7 => does not get executed
#   if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_7_0
#       define IfIOS7(body...)              if(SevenOrNewer()) { body }
#       define IfIOS7Or(extraCond, body...) if(SevenOrNewer() || (extraCond)) { body }
#       define UnlessIOS7(body...)          if(!SevenOrNewer()) { body }
#   else
#       define IfIOS7(body...)               if(0) {}
#       define IfIOS7Or(extraConds, body...) if(0) {}
#       define UnlessIOS7(body...)           if(1) { body }
#   endif
#endif


#pragma mark Drawing Utils
#if TARGET_OS_IPHONE
#   define LetPath(__path, body...) ({ \
        UIBezierPath * const path = (__path); \
        do { body; } while(0); \
        path; \
    })
#   define RGBA(r,g,b,a) [UIColor colorWithRed:(r) green:(g) blue:(b) alpha:(a)]
#   define HSBA(h,s,b,a) [UIColor colorWithHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#else
#   define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:(r) green:(g) blue:(b) alpha:(a)]
#   define HSBA(h,s,b,a) [NSColor colorWithCalibratedHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#   define LetPath(__path, body...) ({ \
        NSBezierPath * const path = (__path); \
        do { body; } while(0); \
        path; \
    })
#endif
#define RGB(r,g,b) RGBA((r), (g), (b), 1)
#define HSB(h,s,b) HSBA((h), (s), (b), 1)
#define GRAY(b) ({ __typeof(b) b_ = (b); RGB(b_,b_,b_); })

#define StrokePath(__path, body...) [LetPath(__path, body) stroke]
#define FillPath(__path, body...) [LetPath(__path, body) fill]


#pragma mark Animation Utils
#if TARGET_OS_IPHONE
#   define WithDur UIView animateWithDuration // Use like: [WithDur:0.3 animations:^{...}]
#   define UIAppGoSlowmo() [[[[[UIApplication sharedApplication] windows] objectAtIndex:0] layer] setSpeed:.1f]
#   define SetVolume(vol) \
        [[MPMusicPlayerController applicationMusicPlayer] setVolume:(vol)];

    UIImage *FScreenshot(float aScale);
#endif


#pragma mark Subscripts

#if !defined(__IPHONE_6_0) || __IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_6_0
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>

@interface NSArray (Subscripts)
- (id)objectAtIndexedSubscript:(NSUInteger)idx;
@end
@interface NSMutableArray (Subscripts)
- (void)setObject:(id)obj atIndexedSubscript:(NSUInteger)idx;
@end

@interface NSDictionary (Subscripts)
- (id)objectForKeyedSubscript:(id)key;
@end
@interface NSMutableDictionary (Subscripts)
- (void)setObject:(id)obj forKeyedSubscript:(id)key;
@end
#endif

@interface NSUserDefaults (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey;
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey;
@end

@interface NSCache (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey;
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey;
@end

#endif // __OBJC__
#endif // _FCONV_H_
