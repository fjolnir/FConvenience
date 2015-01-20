#import "FLogging.h"
#import "FDispatch.h"
#import <asl.h>

static pthread_key_t _ASLClientThreadLocal;
static void _aslClientCleanup(void *client)
{
    asl_close(client);
}

void _FLog(enum FLogLevel const aLevel,
           const char * const aFile,
           int const aLine,
           CFStringRef aFormat, ...)
{
    Once(^{ pthread_key_create(&_ASLClientThreadLocal, &_aslClientCleanup); });
    
    aslclient client = pthread_getspecific(_ASLClientThreadLocal);
    if(!client) {
        CFBundleRef bundle = CFBundleGetMainBundle();
        CFDictionaryRef bundleInfo = CFBundleGetInfoDictionary(bundle);
        client = asl_open(NULL,
                          CFStringGetCStringPtr(CFDictionaryGetValue(bundleInfo, kCFBundleIdentifierKey), kCFStringEncodingUTF8),
                          ASL_OPT_STDERR|ASL_OPT_NO_DELAY);
        pthread_setspecific(_ASLClientThreadLocal, client);
#ifndef DEBUG
        asl_set_filter(client, ASL_FILTER_MASK_UPTO(ASL_LEVEL_WARNING));
#endif
    }
    
#ifndef DEBUG
    // For some reason, asl_set_filter isn't doing its job.. so for now I'm just
    // not logging anything above warning in production builds
    if(aLevel <= ASL_LEVEL_WARNING) {
#endif
    va_list argList;
    va_start(argList, aFormat);
    CFStringRef message = CFStringCreateWithFormatAndArguments(NULL, NULL, aFormat, argList);
    va_end(argList);
    
    CFStringRef path = CFStringCreateWithCStringNoCopy(NULL, aFile, kCFStringEncodingUTF8, kCFAllocatorNull);
    CFURLRef URL = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
    CFStringRef lastPathComponent = CFURLCopyLastPathComponent(URL);
    CFRelease(URL);
    CFRelease(path);
        
    aslmsg const msg = asl_new(ASL_TYPE_MSG);
    asl_set(msg, ASL_KEY_READ_UID, "-1");
    asl_log(client, msg, aLevel,
            "%10.15s:%u: %s",
            CFStringGetCStringPtr(lastPathComponent, kCFStringEncodingUTF8),
            aLine,
            CFStringGetCStringPtr(message, kCFStringEncodingUTF8));
    asl_free(msg);
    CFRelease(message);
    CFRelease(lastPathComponent);
#ifndef DEBUG
    }
#endif
}