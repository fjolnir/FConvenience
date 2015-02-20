#import "NSObject+FConvenience.h"

@implementation NSObject (FConvenience)
- (instancetype)fc_withSelf:(void (^ const)(id))aBlock
{
    aBlock(self);
    return self;
}
@end
