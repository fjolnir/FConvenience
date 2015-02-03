#import <Foundation/Foundation.h>

typedef id(^FCMapBlock)(id obj);
typedef BOOL(^FCFilterBlock)(id obj);

@interface NSArray (FConvenience)
- (NSArray *)fc_map:(FCMapBlock)blk;
- (NSArray *)fc_filter:(FCFilterBlock)blk;
@end

@interface NSSet (FConvenience)
- (NSSet *)fc_map:(FCMapBlock)blk;
- (NSSet *)fc_filter:(FCFilterBlock)blk;
@end

@interface NSDictionary (FConvenience)
- (NSDictionary *)fc_mapKeys:(FCMapBlock)blk;
- (NSDictionary *)fc_filterKeys:(FCFilterBlock)blk;
@end
