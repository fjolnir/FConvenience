#import <AppKit/AppKit.h>

#define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:(r) green:(g) blue:(b) alpha:(a)]
#define HSBA(h,s,b,a) [NSColor colorWithCalibratedHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#define RGB(r,g,b) RGBA((r), (g), (b), 1)
#define HSB(h,s,b) HSBA((h), (s), (b), 1)
#define GRAY(b) ({ __typeof(b) b_ = (b); RGB(b_,b_,b_); })

#define LetPath(__path, body...) ({ \
    NSBezierPath * const path = (__path); \
    do { body; } while(0); \
    path; \
})