#import <CoreFoundation/CoreFoundation.h>

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
           CFStringRef aFormat, ...) CF_FORMAT_FUNCTION(4,5);
#ifdef __OBJC__
#   define Log(lvl, fmt, args...) \
        _FLog((lvl), __FILE__, __LINE__, (__bridge CFStringRef)(fmt), ##args)
#else
#   define Log(lvl, fmt, args...) \
        _FLog((lvl), __FILE__, __LINE__, CFSTR(fmt), ##args)
#endif
#define LogError(msg...)   Log(FLogLevel_Err, ##msg)
#define LogWarning(msg...) Log(FLogLevel_Warning, ##msg)
#define LogNotice(msg...)  Log(FLogLevel_Notice, ##msg)

#ifdef DEBUG
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
            _FLog(FLogLevel_Err, __FILE__, __LINE__, \
                  CFSTR("glError(0x%04x): %s caught\n"), err, errStr); \
        } \
        assert(errStr == NULL); \
    } while(0)
#else
#   define glError()
#endif
