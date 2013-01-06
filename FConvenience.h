#import <Foundation/Foundation.h>

// Frequently used macros for uncluttering things (Import in your PCH)
#define Log(...) fprintf(stderr, "%s[%u] %s: %s\n", \
    [[[NSProcessInfo processInfo] processName] UTF8String], \
    getpid(),\
    [[NSString stringWithFormat:@"%10.15s:%u", \
                                [[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String], \
                                __LINE__] UTF8String],\
    [[NSString stringWithFormat:__VA_ARGS__] UTF8String])

#ifdef DEBUG
    #define CrashHere() { *(int *)0 = 0xDEADBEEF; }
    // We make it a warning because when unit testing it's nice to see the logs in the list view instead of having to scroll through all the compiler output
    #define DebugLog(...) Log(@"Warning: " __VA_ARGS__)
#else
    #define CrashHere()
    #define DebugLog(...) 
#endif

#define Once(...) do { \
    static dispatch_once_t __token; \
    dispatch_once(&__token, ##__VA_ARGS__); \
} while(0)
#define Async(...) dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ##__VA_ARGS__)
#define AsyncOnMain(...) dispatch_async(dispatch_get_main_queue(), ##__VA_ARGS__)

#if TARGET_OS_IPHONE
#define Animate(dur, ...) [UIView animateWithDuration:(dur) animations:__VA_ARGS__]
#endif
