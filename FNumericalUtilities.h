#ifndef FConvenience_FNumericalUtilities_h
#define FConvenience_FNumericalUtilities_h

#ifdef __OBJC__
#   import <Foundation/Foundation.h>
#endif

#define PASTE(a,b) a##b
#if !defined(MIN)
#   define __MIN(a,b,c) ({ \
        __typeof(a) const PASTE(__a,c) = (a); \
        __typeof(b) const PASTE(__b,c) = (b); \
        (__typeof(a))(PASTE(__a,c) > PASTE(__b,c) ? PASTE(__b,c) : PASTE(__a,c)); \
    })
#   define MIN(a,b) __MIN((a), (b), __COUNTER__)
#endif
#if !defined(MAX)
#   define __MAX(a,b,c) ({ \
        __typeof(a) const PASTE(__a,c) = (a); \
        __typeof(b) const PASTE(__b,c) = (b); \
        PASTE(__a,c) > PASTE(__b,c) ? PASTE(__a,c) : PASTE(__b,c); \
    })
#   define MAX(a,b) __MAX((a), (b), __COUNTER__)
#endif

#if !defined(BETWEEN)
#   define __BETWEEN(val, low, high, c) ({ \
        __typeof(val) const PASTE(__val,c) = (val); \
        PASTE(__val,c) > (low) && PASTE(__val,c) < (high); \
    })
#   define BETWEEN(val, low, high) __BETWEEN((val), (low), (high), __COUNTER__)
#endif
#if !defined(INRANGE)
#   define __INRANGE(val, low, high, c) ({ \
        __typeof(val) const PASTE(__val,c) = (val); \
        PASTE(__val,c) >= (low) && PASTE(__val,c) <= (high); \
    })
#   define INRANGE(val, low, high) __INRANGE((val), (low), (high), __COUNTER__)
#endif
#if !defined(CLAMP)
#   define CLAMP(val, min, max) MAX((min), MIN((val), (max)))
#endif

#define POWOF2(n) ({ __typeof(n) __n = (n); (__n != 0) && !(__n & (__n - 1)); })

#if CGFLOAT_IS_DOUBLE
#    define CGFLOAT_EPSILON DBL_EPSILON
#    define cgfabs fabs
#else
#    define CGFLOAT_EPSILON FLT_EPSILON
#    define cgfabs fabsf
#endif

#endif
