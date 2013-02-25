#import "FConvenience.h"

#if !defined(__IPHONE_6_0) || __IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_6_0
@implementation NSArray (Subscripts)
- (id)objectAtIndexedSubscript:(NSUInteger)idx
{
    return [self objectAtIndex:idx];
}
@end
@implementation NSMutableArray (Subscripts)
- (void)setObject:(id)obj atIndexedSubscript:(NSUInteger)idx
{
    [self replaceObjectAtIndex:idx withObject:obj];
}
@end

// NSMutableDictionary+Subscripts.m
@implementation  NSDictionary (Subscripts)

- (id)objectForKeyedSubscript:(id)key
{
    return [self objectForKey:key];
}
@end
@implementation  NSMutableDictionary (Subscripts)
- (void)setObject:(id)obj forKeyedSubscript:(id)key
{
    [self setObject:obj forKey:key];
}
@end
#endif

@implementation NSUserDefaults (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey
{
    return [self objectForKey:aKey];
}
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey
{
    [self setObject:aObj forKey:aKey];
}
@end

@implementation NSCache (Subscripts)
- (id)objectForKeyedSubscript:(id)aKey
{
    return [self objectForKey:aKey];
}
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey
{
    [self setObject:aObj forKey:aKey];
}
@end