#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <android/log.h>        // for logging stuff

#include "hellodefs.h"

// generic logging interface class that helps abstract logging functions so that
// code can be portable between Android and iOS or another system ... could also have
// put the code in different C source files and then linked in only the one relevant to
// our platform

#undef LOG_TAG
#define LOG_TAG "logutils"

// ANDROID is defined by the Android compiler on our behalf ... could also have used gradle cflags
#ifdef ANDROID

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#elif defined(IOS)

#define LOGD(...) printf("D/some other logging paradigm" __VA_ARGS__)
#define LOGI(...) printf("I/some other logging paradigm" __VA_ARGS__)
#define LOGE(...) printf("E/some other logging paradigm" __VA_ARGS__)

#endif

#endif  // ifndef LOG_UTILS_H