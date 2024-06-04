#ifndef SINGINGROOM_JNICALLBACKWRAPPER_H
#define SINGINGROOM_JNICALLBACKWRAPPER_H

#include <jni.h>
#include <functional>
#include <memory>

class JNICallbackWrapper {
public:
    JNICallbackWrapper(JNIEnv* env, jobject callback, const char* methodName, const char* sig);
    ~JNICallbackWrapper();

    jmethodID getMethodId();
    jobject getCallbackObject();

    JNIEnv* getJNIEnv();

private:
    JavaVM* javaVM = nullptr;
    jobject targetCallback;
    const char* methodName;
    const char* sig;
};


#endif //SINGINGROOM_JNICALLBACKWRAPPER_H
