#import <Foundation/Foundation.h>

#define NotificationCenter      [NSNotificationCenter defaultCenter]
#define Bundle                  [NSBundle mainBundle]
#define Workspace               [NSWorkspace sharedWorkspace]
#define FileManager             [NSFileManager defaultManager]
#define Defaults                [NSUserDefaults standardUserDefaults]
#define NSFormat(fmt...)        [NSString stringWithFormat:fmt]
#define NSMutableFormat(fmt...) [NSMutableString stringWithFormat:fmt]
#define Predicate(fmt...)       [NSPredicate predicateWithFormat:fmt]
#define FetchReq(name)          [NSFetchRequest fetchRequestWithEntityName:(name)]
#define ThreadDict              [[NSThread currentThread] threadDictionary]

#define NSNullToNil(x) ({ \
    __typeof(x) __x = (x); \
    [[NSNull null] isEqual:__x] ? nil : __x; \
})

#define _NSCompare(x, y, c) ({ \
    __typeof(x) PASTE(__x,c) = (x); \
    __typeof(y) PASTE(__y,c) = (y); \
      PASTE(__x,c) > PASTE(__y,c) ? NSOrderedDescending \
    : PASTE(__x,c) < PASTE(__y,c) ? NSOrderedAscending \
                                  : NSOrderedSame; \
})
#define NSCompare(x,y) _NSCompare((x), (y), __COUNTER__)