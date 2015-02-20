#import <Foundation/Foundation.h>

@interface NSObject (FConvenience)
// A workaround the fact that Objective-C does not have cascading
- (instancetype)fc_withSelf:(void (^)(id))aBlock;
@end
