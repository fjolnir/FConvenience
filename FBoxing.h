#import <Foundation/Foundation.h>

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
#   import <UIKit/UIKit.h>
    static inline NSValue * OVERLOADABLE FBox(CGRect x)            { return [NSValue valueWithCGRect:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGPoint x)           { return [NSValue valueWithCGPoint:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGSize x)            { return [NSValue valueWithCGSize:x]; }
    static inline NSValue * OVERLOADABLE FBox(CGAffineTransform x) { return [NSValue valueWithCGAffineTransform:x]; }
    static inline NSValue * OVERLOADABLE FBox(CATransform3D x)     { return [NSValue valueWithCATransform3D:x]; }

    static inline NSValue * OVERLOADABLE FBox(UIOffset x)          { return [NSValue valueWithUIOffset:x]; }
    static inline NSValue * OVERLOADABLE FBox(UIEdgeInsets x)      { return [NSValue valueWithUIEdgeInsets:x]; }
#   ifdef CMTIMERANGE_H
        static inline NSValue * OVERLOADABLE FBox(CMTimeRange x)   { return [NSValue valueWithCMTimeRange:x]; }
        static inline NSValue * OVERLOADABLE FBox(CMTimeMapping x) { return [NSValue valueWithCMTimeMapping:x]; }
#   endif
#   ifdef MK_EXTERN
        static inline NSValue * OVERLOADABLE FBox(CLLocationCoordinate2D x) { return [NSValue valueWithMKCoordinate:x]; }
        static inline NSValue * OVERLOADABLE FBox(MKCoordinateSpan x)       { return [NSValue valueWithMKCoordinateSpan:x]; }
#   endif
#else
    static inline NSValue * OVERLOADABLE FBox(NSRect x)  { return [NSValue valueWithRect:x]; }
    static inline NSValue * OVERLOADABLE FBox(NSPoint x) { return [NSValue valueWithPoint:x]; }
    static inline NSValue * OVERLOADABLE FBox(NSSize x)  { return [NSValue valueWithSize:x]; }
#endif

#undef OVERLOADABLE
