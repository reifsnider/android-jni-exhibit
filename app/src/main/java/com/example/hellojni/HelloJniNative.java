package com.example.hellojni;

/**
 * Packages the native interface and JNI calls, along with the loading of the JNI library *.so,
 * into one place ... better not to let an Activity own this sort of stuff
 */
public class HelloJniNative {

    /** reference to the native method we define in our Android-specific interface layer,
     which will call non-Android-specific C code in turn */
    public static native String  stringFromJNI();

    /** store a small string into the C library for later retrieval - must be 80 chars or less */
    public static native void storeTinyString(String storeMe);

    /** fetch previously stored small string from the C library ... will get what you put in earlier
     * via the storeTinyString() call ... if you do not call storeTinyString() first, you will
     * get an empty string */
    public static native String retrieveTinyString();

    /** demonstrate C's pthreads working fine on Android */
    public static native void pthreadDemo();

    static {

        // you must load your JNI library before you use it, generally this is done
        // very early ... so we can for example put it in the static block of our
        // primary activity, so that it is run when the activity's class is
        // initialized, which will happen just _before_ the first use of that class
        // ... the build system forms up our C code into a *.so, and that is what is
        // getting loaded
        System.loadLibrary("hello-jni");

    }

}
