/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <jni.h>
#include <stdlib.h> // for malloc, free

#include "logutils.h"
#include "hellodefs.h"
#include "hello.h"

// log tag for __android_log ... use undef to allow logutils.h to define a default, then override it here
#undef LOG_TAG
#define LOG_TAG ("hello-jni")

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_example_hellojni_HelloJniNative_stringFromJNI( JNIEnv* env, jobject thiz )
{

    // 80-char string we will use to generate our platform version message
    char *pPlatformMessage = (char *)malloc(STRING_BUFFER_LENGTH*sizeof(char));

    getPlatformMessage(pPlatformMessage);

    // this actually allocates a Java String instance using the Java VM, so that it is
    // safe to pass back to Java callers ... if you try to get fancy and save one of
    // these Java objects, though, beware!  This is a "local reference", which means it
    // is not valid after this native function exits, even though the object itself
    // may live on in the Java caller - our own native reference returnValue is
    // invalid after we exit ... if we want something to be still valid when we
    // do a subsequent native function call we need a global reference e.g. NewGlobalRef()
    jstring returnValue = (*env)->NewStringUTF(env, pPlatformMessage);

    free(pPlatformMessage);

    return returnValue;

}

// store a string (must be < 80 chars) in our string buf - it's global so will persist
void storeTinyString(char *pStringUtf8);

// retrieve stored string
void retrieveTinyString(char *pStringUtf8);

// clear/initialize the stored string
void clearTinyString();

// store a small string into the C library for later retrieval
void Java_com_example_hellojni_HelloJniNative_storeTinyString(JNIEnv* env, jobject thiz, jstring storeMe) {

    // Java is UTF-16, which is not null-terminated (in fact many UTF-16 chars, which are two-byte chars,
    // have one byte that is null) ... so we convert to handy UTF-8 which is null-terminated and contains
    // no other nulls, for our own string handling purposes
    char *charBuf = (*env)->GetStringUTFChars(env, storeMe, NULL);

    // store the string in a global
    storeTinyString(charBuf);

    // must release!
    (*env)->ReleaseStringUTFChars(env, storeMe, charBuf);

}

// retrieve small string previously stored into the C library ... if you have
// not called storeTinyString() to store a string, this function will return
// an empty string ... its string is initialized by the cool __attribute__((constructor))  ...
jstring Java_com_example_hellojni_HelloJniNative_retrieveTinyString(JNIEnv* env, jobject thiz) {

    char charBuf[STRING_BUFFER_LENGTH];

    retrieveTinyString(charBuf);

    jstring returnValue = (*env)->NewStringUTF(env, charBuf);

    return returnValue;

}