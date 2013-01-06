// Frequently used macros for uncluttering things (Import in your PCH)

#define _Log(prefix, ...) fprintf(stderr, prefix "%s[%u] %s: %s\n", \
    [[[NSProcessInfo processInfo] processName] UTF8String], \
    getpid(),\
    [[NSString stringWithFormat:@"%10.15s:%u", \
                                [[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String], \
                                __LINE__] UTF8String],\
    [[NSString stringWithFormat:__VA_ARGS__] UTF8String])

#define Log(...) _Log("V ", ##__VA_ARGS__) // V: Verbose

#ifdef DEBUG
    #define CrashHere() { *(int *)0 = 0xDEADBEEF; }
    #define DebugLog(...) _Log("D ", ##__VA_ARGS__) // D: Debug
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

#define Workspace [NSWorkspace sharedWorkspace]
#define FileManager [NSFileManager defaultManager]

// iOS/Mac specific
#if TARGET_OS_IPHONE
    #define WithDur UIView animateWithDuration // Use like: [WithDur:0.3 animations:^{...}]
    #define Device [UIDevice currentDevice]
    #define UIApp [UIApplication sharedApplication]
#else
    #define Defaults [NSUserDefaults standardUserDefaults]
#endif
