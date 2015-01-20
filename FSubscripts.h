#import <Foundation/Foundation.h>

@interface NSUserDefaults (FSubscripts)
- (id)objectForKeyedSubscript:(id)aKey;
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey;
@end

@interface NSCache (FSubscripts)
- (id)objectForKeyedSubscript:(id)aKey;
- (void)setObject:(id)aObj forKeyedSubscript:(id)aKey;
@end
