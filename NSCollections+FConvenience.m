#import "NSCollections+FConvenience.h"

id const FCSkipSentinel = @"__FCSkipSentinel__";

@implementation NSArray (FConvenience)
- (NSArray *)fc_map:(FCMapBlock)blk
{
    NSParameterAssert(blk);

    NSMutableArray *result = [NSMutableArray arrayWithCapacity:self.count];
    for(__strong id obj in self) {
        if((obj = blk(obj)) != FCSkipSentinel)
            [result addObject:obj];
    }
    return result;
}
- (NSArray *)fc_filter:(FCFilterBlock)blk
{
    NSParameterAssert(blk);

    NSMutableArray *result = [NSMutableArray arrayWithCapacity:self.count];
    for(id obj in self) {
        if(blk(obj))
            [result addObject:obj];
    }
    return result;
}
@end

@implementation NSSet (FConvenience)
- (NSSet *)fc_map:(FCMapBlock)blk
{
    NSParameterAssert(blk);

    NSMutableSet *result = [NSMutableSet setWithCapacity:self.count];
    for(__strong id obj in self) {
        if((obj = blk(obj)) != FCSkipSentinel)
            [result addObject:obj];
    }
    return result;
}
- (NSSet *)fc_filter:(FCFilterBlock)blk
{
    NSParameterAssert(blk);

    NSMutableSet *result = [NSMutableSet setWithCapacity:self.count];
    for(id obj in self) {
        if(blk(obj))
            [result addObject:obj];
    }
    return result;
}
@end

@implementation NSDictionary (FConvenience)
- (NSDictionary *)fc_map:(FCMapPairBlock)blk
{
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:self.count];
    for(__strong id key in self.allKeys) {
        id value = blk(&key, self[key]);
        if(value != FCSkipSentinel)
            result[key] = value;
    }
    return result;
}
- (NSDictionary *)fc_mapKeys:(FCMapBlock)blk
{
    NSParameterAssert(blk);
    
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:self.count];
    for(__strong id key in self) {
        id mappedKey = blk(key);
        if(mappedKey != FCSkipSentinel)
            result[blk(key)] = self[key];
    }
    return result;
}
- (NSDictionary *)fc_filterKeys:(FCFilterBlock)blk
{
    NSParameterAssert(blk);
    
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:self.count];
    for(id key in self) {
        if(blk(key))
            result[key] = self[key];
    }
    return result;
}
@end
