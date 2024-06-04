#include "JNICallbackWrapper.h"

JNICallbackWrapper::JNICallbackWrapper(
    JNIEnv *env, jobject callback, const char* methodName, const char* sig
):  targetCallback(env->NewGlobalRef(callback)),
    sig(sig),
    methodName(methodName) {

    env->GetJavaVM(&javaVM);
}

JNICallbackWrapper::~JNICallbackWrapper() {
    JNIEnv* targetEnv = getJNIEnv();
    targetEnv->DeleteGlobalRef(targetCallback);

    targetCallback = nullptr;
    sig = nullptr;
    methodName = nullptr;
}

jmethodID JNICallbackWrapper::getMethodId() {
    JNIEnv* targetEnv = getJNIEnv();
    jclass callbackClass = targetEnv->GetObjectClass(targetCallback);
    return targetEnv->GetMethodID(callbackClass, methodName, sig);
}

jobject JNICallbackWrapper::getCallbackObject() {
    return targetCallback;
}

JNIEnv *JNICallbackWrapper::getJNIEnv() {
    JNIEnv* env;
    if (javaVM->AttachCurrentThread(reinterpret_cast<JNIEnv**>(&env), nullptr) != JNI_OK) {
        return nullptr;
    }
    return env;
}
