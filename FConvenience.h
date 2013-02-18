// Frequently used macros for uncluttering things (Import in your PCH)
#import <pthread.h>

#ifdef __OBJC__
    #define _Log(prefix, format, ...) \
        fprintf(stderr, prefix "%s[%u] %10.15s:%u: %s\n", \
        [[[NSProcessInfo processInfo] processName] UTF8String], \
        getpid(),\
        __FILE__, __LINE__, \
        [[NSString stringWithFormat:format, ##__VA_ARGS__] UTF8String])
#else
    #define _Log(prefix, format, ...) \
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
    #define CrashHere()   { *(int *)0 = 0xDEADBEEF; }
    #define DebugLog(...) _Log("D ", ##__VA_ARGS__) // D: Debug
    #define CheckOSErr(err, fmt, ...) _CheckOSErr(true, err, fmt, ##__VA_ARGS__)
    #define IfDebug(...) __VA_ARGS__
    #define glError() do { \
        const char *errStr; \
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
            printf("glError(0x%04x): %s caught at %s:%u\n", \
                   err, errStr, __FILE__, __LINE__); \
        } \
    } while(0)
#else
    #define CrashHere()
    #define DebugLog(...) 
    #define CheckOSErr(err, fmt, ...) _CheckOSErr(false, err, fmt, ##__VA_ARGS__)
    #define IfDebug(...)
    #define glError()
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
    const dispatch_block_t __blk = __VA_ARGS__; \
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

#define CLAMP(val, min, max) MAX((min), MIN((val), (max)))
#define BETWEEN(val, low, high) ({ \
        const __typeof(val) __val = (val); \
        __val > (low) && __val < (high); \
})
#define INRANGE(val, low, high) ({ \
        const __typeof(val) __val = (val); \
        __val >= (low) && __val <= (high); \
})
#define POWOF2(n) ({ __typeof(n) __n = (n); (__n != 0) && !(__n & (__n - 1)); })

#ifdef __OBJC__
    #define NotificationCenter [NSNotificationCenter defaultCenter]
    #define Workspace     [NSWorkspace sharedWorkspace]
    #define FileManager   [NSFileManager defaultManager]
    #define Defaults      [NSUserDefaults standardUserDefaults]
    #define NSFormat(...) [NSString stringWithFormat:__VA_ARGS__]
#endif

// iOS specific
#if TARGET_OS_IPHONE && defined(__OBJC__)
    #define WithDur UIView animateWithDuration // Use like: [WithDur:0.3
                                               //         animations:^{...}]
    #define RGBA(r,g,b,a) [UIColor colorWithRed:(r) green:(g) blue:(b) alpha:(a)]
#define Device [UIDevice currentDevice]
    #define UIApp  [UIApplication sharedApplication]
    #define SetVolume(vol) \
        [[MPMusicPlayerController applicationMusicPlayer] setVolume:(vol)];
#else
    #define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:(r) green:(g) blue:(b) alpha:(a)]
#endif
#define RGB(r,g,b)  RGBA((r), (g), (b), 1)

@interface NSUserDefaults (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey;
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey;
@end
