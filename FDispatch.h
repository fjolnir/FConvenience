#ifndef FConvenience_FDispatch_h
#define FConvenience_FDispatch_h

#include <pthread/pthread.h>

#if __has_extension(blocks)
#   include <dispatch/dispatch.h>
#   define GlobalQueue dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
#   define MainQueue dispatch_get_main_queue()

#   define Once(body...) do { \
        static dispatch_once_t __token; \
        dispatch_once(&__token, ##body); \
    } while(0)
    static inline void Async(dispatch_block_t const blk)       { dispatch_async(GlobalQueue, blk); }
    static inline void AsyncOnMain(dispatch_block_t const blk) { dispatch_async(MainQueue, blk);   }
    static inline void SyncOnMain(dispatch_block_t  const blk) {
        if(pthread_main_np())
            blk();
        else
            dispatch_sync(MainQueue, blk);
    }
    static inline void AfterDelay(float const seconds, dispatch_block_t const blk) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, seconds * NSEC_PER_SEC),
                       MainQueue, blk);
    }

#   define Memoize(x...) ({ \
        static __typeof(({ x; })) __memoized_x; \
        Once(^{ __memoized_x = ({ x; }); }); \
        __memoized_x; \
    })
#endif

#ifdef __OBJC__
#define DefineThreadLocal(name) \
    static void __ThreadLocalCleanup_##name##__(void *ptr) { id const __unused obj = (__bridge_transfer id)ptr; } \
    static pthread_key_t __ThreadLocal_##name##__
#define InitThreadLocal(name) \
    Once(^{ pthread_key_create(&__ThreadLocal_##name##__, &__ThreadLocalCleanup_##name##__); })
#define ReadThreadLocal(name) \
    (__bridge id)pthread_getspecific(__ThreadLocal_##name##__)
#define WriteThreadLocal(name, value) ({ \
    id const __unused __oldValue__ = (__bridge_transfer id)pthread_getspecific(__ThreadLocal_##name##__); \
    id const __value__ = (value); \
    pthread_setspecific(__ThreadLocal_##name##__, (__bridge_retained void *)__value__); \
    __value__; \
})
#endif

#endif
