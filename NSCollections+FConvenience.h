#import <Foundation/Foundation.h>

typedef id(^FCMapBlock)(id obj);
typedef id(^FCMapPairBlock)(id *key, id obj);
typedef BOOL(^FCFilterBlock)(id obj);

// Return from a map method to remove the value from the result.
extern id const FCSkipSentinel;

@interface NSArray (FConvenience)
- (NSArray *)fc_map:(FCMapBlock)blk;
- (NSArray *)fc_filter:(FCFilterBlock)blk;
@end

@interface NSSet (FConvenience)
- (NSSet *)fc_map:(FCMapBlock)blk;
- (NSSet *)fc_filter:(FCFilterBlock)blk;
@end

@interface NSDictionary (FConvenience)
- (NSDictionary *)fc_map:(FCMapPairBlock)blk;
- (NSDictionary *)fc_mapKeys:(FCMapBlock)blk;
- (NSDictionary *)fc_filter:(FCFilterBlock)blk;
- (NSDictionary *)fc_filterKeys:(FCFilterBlock)blk;
@end
