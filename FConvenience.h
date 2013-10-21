// Frequently used macros for uncluttering things (Import in your PCH)

#ifndef _FCONV_H_
#define _FCONV_H_

#include <pthread.h>
#include <stdio.h>

#ifdef __COREFOUNDATION_CFBASE__
#define CF_AUTORELEASED __attribute__ ((cleanup(CFReleaseCleanup)))
static inline void CFReleaseCleanup(CF_CONSUMED void *objPtr) {
    if(*(CFTypeRef *)objPtr)
        CFRelease(*(CFTypeRef *)objPtr);
}
#endif

#ifdef __OBJC__
#   define _Log(prefix, format, ...) \
        fprintf(stderr, prefix "%s[%u] %10.15s:%u: %s\n", \
        [[[NSProcessInfo processInfo] processName] UTF8String], \
        getpid(),\
        __FILE__, __LINE__, \
        [[NSString stringWithFormat:format, ##__VA_ARGS__] UTF8String])

#   define UIIdiomString() ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) ? @"ipad" : @"iphone")
#   define DeviceIsIPad() (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#else
#   define _Log(prefix, format, ...) \
        fprintf(stderr, prefix "%10.15s:%u: " format "\n", \
        __FILE__, __LINE__, \
        ##__VA_ARGS__)
#endif

#define Log(...) _Log("V ", ##__VA_ARGS__) // V: Verbose

#define _CheckOSErr(shouldAssert, error, fmt, ...) do { \
    OSStatus __err = (error); \
    if(__err) { \
        Log(@"OSErr %d: " fmt, (int)__err, ##__VA_ARGS__); \
        assert(!shouldAssert); \
    } \
} while(0)

#ifdef DEBUG
#   define DEBUG_ON YES
#   define CrashHere()   { *(int *)0 = 0xDEADBEEF; }
#   define DebugLog(...) _Log("D ", ##__VA_ARGS__) // D: Debug
#   define CheckOSErr(err, fmt, ...) _CheckOSErr(true, err, fmt, ##__VA_ARGS__)
#   define IfDebug(...) __VA_ARGS__
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
            DebugLog(@"glError(0x%04x): %s caught at %s:%u\n", \
                     err, errStr, __FILE__, __LINE__); \
        } \
        NSCAssert(errStr == NULL, @"OpenGL Error"); \
    } while(0)
#else
#   define DEBUG_ON NO
#   define CrashHere()
#   define DebugLog(...)
#   define CheckOSErr(err, fmt, ...) _CheckOSErr(false, err, fmt, ##__VA_ARGS__)
#   define IfDebug(...)
#   define glError()
#endif

#define GlobalQueue dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
#define MainQueue dispatch_get_main_queue()

#define Once(...) do { \
    static dispatch_once_t __token; \
    dispatch_once(&__token, ##__VA_ARGS__); \
} while(0)
#define Async(...) dispatch_async(GlobalQueue, ##__VA_ARGS__)
#define AsyncOnMain(...) dispatch_async(MainQueue, ##__VA_ARGS__)
#define SyncOnMain(...) do { \
    dispatch_block_t const __blk = __VA_ARGS__; \
    if(pthread_main_np()) \
        __blk(); \
    else \
        dispatch_sync(MainQueue, __blk); \
} while(0)
#define AfterDelay(seconds, ...) \
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (seconds) * NSEC_PER_SEC), \
                   MainQueue, ##__VA_ARGS__)

#define unless(...) if(!(__VA_ARGS__))
#define until(...)  while(!(__VA_ARGS__))

#ifndef MIN
#   define MIN(a,b) ({ \
        __typeof(a) const __a = (a); \
        __typeof(b) const __b = (b); \
        __a > __b ? __b : __a; \
    })
#endif
#ifndef MAX
#   define MAX(a,b) ({ \
        __typeof(a) const __a = (a); \
        __typeof(b) const __b = (b); \
        __a > __b ? __a : __b; \
    })
#endif

#define CLAMP(val, min, max) MAX((min), MIN((val), (max)))
#define BETWEEN(val, low, high) ({ \
    __typeof(val) const __val = (val); \
    __val > (low) && __val < (high); \
})
#define INRANGE(val, low, high) ({ \
    __typeof(val) const __val = (val); \
    __val >= (low) && __val <= (high); \
})
#define POWOF2(n) ({ __typeof(n) __n = (n); (__n != 0) && !(__n & (__n - 1)); })

#ifdef __OBJC__
#   define NotificationCenter [NSNotificationCenter defaultCenter]
#   define Bundle        [NSBundle mainBundle]
#   define Workspace     [NSWorkspace sharedWorkspace]
#   define FileManager   [NSFileManager defaultManager]
#   define Defaults      [NSUserDefaults standardUserDefaults]
#   define NSFormat(...) [NSString stringWithFormat:__VA_ARGS__]
#   define FetchReq(name) [NSFetchRequest fetchRequestWithEntityName:(name)]
#   define NSNullToNil(x) ({ \
        __typeof(x) __x = (x); \
        [[NSNull null] isEqual:__x] ? nil : __x; \
    })
#endif

#ifdef __OBJC__

// iOS specific
#if TARGET_OS_IPHONE

#    ifndef __IPHONE_7_0
#        define __IPHONE_7_0 70000
#    endif

#   define LetPath(__path, code...) ({ \
        UIBezierPath *path = (__path); \
        do { code; } while(0); \
        path; \
    })
#   define WithDur UIView animateWithDuration // Use like: [WithDur:0.3 animations:^{...}]
#   define RGBA(r,g,b,a) [UIColor colorWithRed:(r) green:(g) blue:(b) alpha:(a)]
#   define HSBA(h,s,b,a) [UIColor colorWithHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#   define Device [UIDevice currentDevice]
#   define UIApp  [UIApplication sharedApplication]
#   define SetVolume(vol) \
        [[MPMusicPlayerController applicationMusicPlayer] setVolume:(vol)];
#   define Running
#   define SevenOrNewer() \
        ([[[[[UIDevice currentDevice] systemVersion] componentsSeparatedByString:@"."] objectAtIndex:0] intValue] >= 7)
// Runs a block of Code only if building using the iOS 7 SDK & running on iOS 7
// (iOS 6 SDK build running on iOS 7 => does not get executed
#   if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_7_0
#       define IfIOS7(code...)              if(SevenOrNewer()) { code }
#       define IfIOS7Or(extraCond, code...) if(SevenOrNewer() || (extraCond)) { code }
#       define UnlessIOS7(code...)          if(!SevenOrNewer()) { code }
#   else
#       define IfIOS7(code...)               if(0) {}
#       define IfIOS7Or(extraConds, code...) if(0) {}
#       define UnlessIOS7(code...)           if(1) { code }
#   endif

#else
#   define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:(r) green:(g) blue:(b) alpha:(a)]
#   define HSBA(h,s,b,a) [NSColor colorWithCalibratedHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#   define LetPath(__path, code...) ({ \
        NSBezierPath *path = (__path); \
        do { #code; } while(0); \
        path; \
    })
#endif

#define RGB(r,g,b) RGBA((r), (g), (b), 1)
#define HSB(h,s,b) HSBA((h), (s), (b), 1)
#define GRAY(b) ({ __typeof(b) b_ = (b); RGB(b_,b_,b_); })

#define StrokePath(__path, code...) [LetPath(__path, #code) stroke]
#define FillPath(__path, code...) [LetPath(__path, #code) fill]

#pragma mark - Subscripts

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

#pragma mark -
@class UIImage;
UIImage *FScreenshot(float aScale);
#endif
#endif