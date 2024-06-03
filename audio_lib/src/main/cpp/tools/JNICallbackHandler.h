#include <jni.h>
#include <functional>
#include <memory>
#include <android/log.h>

class JNICallbackHandler {
public:
    JNICallbackHandler(JNIEnv* env, jobject callback) {
        env->GetJavaVM(&javaVM);
        globalCallback = env->NewGlobalRef(callback);
    }

    ~JNICallbackHandler() {
        JNIEnv* env = getJNIEnv();
        if (env && globalCallback) {
            env->DeleteGlobalRef(globalCallback);
        }
    }

    void call(jshortArray data, jint size) {
        JNIEnv* env = getJNIEnv();
        if (env && globalCallback) {
            jclass callbackClass = env->GetObjectClass(globalCallback);
            jmethodID invokeMethod = env->GetMethodID(callbackClass, "invoke", "([SI)V");

            env->CallVoidMethod(globalCallback, invokeMethod, data, size);
        }
    }

    JNIEnv* getJNIEnv() {
        JNIEnv* env;
        if (javaVM->AttachCurrentThread(reinterpret_cast<JNIEnv**>(&env), nullptr) != JNI_OK) {
            javaVM->DetachCurrentThread();
            return nullptr;
        }
        return env;
    }


    JavaVM* javaVM = nullptr;
    jobject globalCallback = nullptr;
};
