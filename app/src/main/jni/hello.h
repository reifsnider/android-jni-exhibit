#ifndef HELLO_H
#define HELLO_H

// non-Android-specific calls for showing logs and returning strings

#include "hellodefs.h"

#undef LOG_TAG
#define LOG_TAG "hello"

// length of our demo string buf
#define STRING_BUFFER_LENGTH (80)

// get a message about the platform we compiled for
void getPlatformMessage(char *pBuffer);

// store a string (must be < 80 chars) in our string buf - it's global so will persist
void storeTinyString(char *pStringUtf8);

// retrieve stored string
void retrieveTinyString(char *pStringUtf8);

// clear/initialize the stored string
void clearTinyString();

#endif  // ifndef HELLO_H