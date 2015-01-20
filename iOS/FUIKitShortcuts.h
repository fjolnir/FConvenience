#import <UIKit/UIKit.h>

#define UIApp  [UIApplication sharedApplication]
#define Device [UIDevice currentDevice]

#define UIIdiomString() ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) \
                         ? @"ipad" : @"iphone")
#define DeviceIsIPad() (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)


#define WithDur UIView animateWithDuration // Use like: [WithDur:0.3 animations:^{...}]
#define UIAppGoSlowmo() [[[[[UIApplication sharedApplication] windows] objectAtIndex:0] layer] setSpeed:.1f]

#define RGBA(r,g,b,a) [UIColor colorWithRed:(r) green:(g) blue:(b) alpha:(a)]
#define HSBA(h,s,b,a) [UIColor colorWithHue:(h) saturation:(s) brightness:(b) alpha:(a)]
#define RGB(r,g,b) RGBA((r), (g), (b), 1)
#define HSB(h,s,b) HSBA((h), (s), (b), 1)
#define GRAY(b) ({ __typeof(b) b_ = (b); RGB(b_,b_,b_); })

#define LetPath(__path, body...) ({ \
     UIBezierPath * const path = (__path); \
     do { body; } while(0); \
     path; \
})

#define CGRectGetCenter(r) ({ \
    CGRect __r = (r); \
    (CGPoint) { CGRectGetMidX(__r), CGRectGetMidY(__r) }; \
})