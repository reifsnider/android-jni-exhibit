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
 */
package com.example.hellojni;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class HelloJni extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // pretty simplistic app: its entire main window is a single text view
        // that we create on the fly and then adopt using setContentView(),
        // instead of having a layout resource

        // make the TextView instance we are gonna populate
        TextView  tv = new TextView(this);

        // get a String that we are actually
        // instantiating in the JNI code ... when you use the JNI
        // environment to instantiate Java objects in C, it's the same as
        // if we'd instantiated them in Java
        String stringAllocatedByJNI = HelloJniNative.stringFromJNI();

        // pthreads work fine on both iOS and Android
        HelloJniNative.pthreadDemo();

        // demonstrate storing and retrieving a string from JNI in separate calls -
        // persistence of information in the C code
        String storeThis = "Test string to store in the C library.";
        String uninitializedString = HelloJniNative.retrieveTinyString();

        if (0 != uninitializedString.length()) {

            throw new RuntimeException("string should have had zero length");

        }

        HelloJniNative.storeTinyString(storeThis);
        String previouslyStoredString = HelloJniNative.retrieveTinyString();

        String stringToDisplay = "\n" + stringAllocatedByJNI + "\n\n" + previouslyStoredString;

        // set the TextView's text
        tv.setText(stringToDisplay);

        // make this TextView our entire window
        setContentView(tv);

    }

}
