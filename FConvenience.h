#ifndef _FCONVENIENCE_H_
#define _FCONVENIENCE_H_

#include <FConvenience/FLogging.h>
#include <FConvenience/FDispatch.h>
#include <FConvenience/FNumericalUtilities.h>
#include <FConvenience/FShortcuts.h>

#ifdef __OBJC__
#   import <FConvenience/FFoundationShortcuts.h>
#   import <FConvenience/FBoxing.h>
#   import <FConvenience/FSubscripts.h>
#   import <FConvenience/NSObject+FConvenience.h>
#   import <FConvenience/NSCollections+FConvenience.h>

#   if TARGET_OS_IPHONE
#      import <FConvenience/FUIKitShortcuts.h>
#      import <FConvenience/FScreenshot.h>
#   else
#      import <FConvenience/FAppKitShortcuts.h>
#   endif
#endif

#pragma mark Debug Utils
#ifdef DEBUG
#   define DEBUG_ON (1)
#else
#   define DEBUG_ON (0)
#endif
#define IfDebug if(DEBUG_ON)

#endif // _FCONV_H_
