// functions for demonstrating logging and string return values


#include "hello.h"

#include <string.h>             // for strlen
#include <stdio.h>              // for sprintf, which is not in string.h ...
#include <pthread.h>            // for pthreads
#include <unistd.h>             // int16_t and other standardized types
#include <stdlib.h>             // for exit()


#include "logutils.h"

#undef LOG_TAG
#define LOG_TAG ("hello")

// integer value we use to demo some C Android code
#define TEST_INTEGER (5)

// a static string buffer that we'll use to demo saving stuff persistently in C lib
static char gpStringBuf[STRING_BUFFER_LENGTH];

// get a message about the platform we compiled for
void getPlatformMessage(char *pBuffer) {

    LOGD("Test integer value: %d", TEST_INTEGER);

    char platformBuf[80];

#if defined(__MACH__)

    // NeXTSTEP or Apple

    #if defined(__APPLE__)

        // Apple, either Mac OSX or iOS

        // ... so we have this now
        #include <TargetConditionals.h>

        // check the stuff from TargetConditionals.h
        #if TARGET_IPHONE_SIMULATOR == 1

	        /* iOS in Xcode simulator */
	        sprintf(platformBuf, "iOS simulator");

        #elif TARGET_OS_IPHONE == 1

	        /* iOS on iPhone, iPad, etc. */
	        sprintf(platformBuf, "iOS device");


        #elif TARGET_OS_MAC == 1

	        /* OSX */
	        sprintf(platformBuf, "Mac");

	    #else

	        /* huh? */
	        sprintf(platformBuf, "unknown Mach Apple");

        #endif // TARGET_OS_MAC

    #endif // __APPLE__

#elif defined(__ANDROID__)

    // Android

    #include <android/api-level.h>

    sprintf(platformBuf, "Android API level %d", __ANDROID_API__);

#else

    sprintf(platformBuf, "unknown architecture");

#endif

    // 80-char string we will use to generate our ABI version message
    sprintf(pBuffer, "Hi from C code ! %s", platformBuf);

    return ;
}


// store a string (must be < 80 chars) in our string buf - it's global so will persist
void storeTinyString(char *pStringUtf8) {

    if (NULL == pStringUtf8) {

        // bad arg
        LOGE("null arg to storeTinyString");
        return;

    }

    if (STRING_BUFFER_LENGTH <= (strlen(pStringUtf8) + 1)) {

        // no room!
        LOGE("did not store tiny string - it was too long");
        return;
    }

    sprintf(gpStringBuf, "%s", pStringUtf8);

}

// retrieve stored string
void retrieveTinyString(char *pStringUtf8) {

    if (NULL == pStringUtf8) {

        // bad arg
        LOGE("null arg to storeTinyString");
        return;

    }

    sprintf(pStringUtf8, "%s", gpStringBuf);

}

// clear/initialize the stored string
void clearTinyString() {

    gpStringBuf[0] = '\0';

}

// task to be run on an individual thread in our pthread demo function
void *pthreadDemoTask(void *pThreadIndex) {

    int16_t threadIndex = (intptr_t)pThreadIndex;

    intptr_t result = threadIndex*2;

    pthread_exit((void*) result);

}

// demonstrate pthreads, which work fine on Android and on iOS
#define THREAD_COUNT (4)
void pthreadDemo() {

    pthread_t thread[THREAD_COUNT];
    intptr_t threadArg[THREAD_COUNT];
    // configuration of the pthreads, most importantly that they are joinable
    pthread_attr_t attr;

    // pthread calls return code
    int rc;
    // pthread join status
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int16_t whichThread;

    for (whichThread = 0; whichThread < THREAD_COUNT; whichThread++) {

        threadArg[whichThread] = whichThread;

        rc = pthread_create(&thread[whichThread], &attr, pthreadDemoTask, (void *)threadArg[whichThread]);
        if (rc) {
            LOGE("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

    }

    // done with this, need to clean up
    pthread_attr_destroy(&attr);

    // could wait on them all at the same time, but we won't proceed until all finish
    for (whichThread = 0; whichThread < THREAD_COUNT; whichThread++) {

        rc = pthread_join(thread[whichThread], &status);

        if (rc) {

            LOGE("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }

        LOGD("Thread %d completed with value %p", whichThread, status);

    }

}

__attribute__((constructor))
static void initialize_stringBuffer() {

    // the __attribute__((constructor)) function is supported by Android's compiler
    // as well as the clang iOS compiler - this code is run when the shared library
    // is loaded, so we can use it to do initialization

    LOGD("initializing C library");

    // demonstration of getting a C compile definition from the "command line" as a
    // gradle build script argument ... use the cFlags option in your gradle android ndk section
#ifdef _FROM_GRADLE
    LOGD("From Gradle!");
#else
    LOGD("Did not get flag _FROM_GRADLE");
#endif

    clearTinyString();

}

__attribute__((destructor))
static void destroy_nothingAtAll() {

    // the __attribute__((destructor)) function is supported by Android's compiler
    // as well as the clang iOS compiler - this code is run when the shared library
    // is unloaded

  // we have no such work to do so it's a no-op that we include to show it's possible
}