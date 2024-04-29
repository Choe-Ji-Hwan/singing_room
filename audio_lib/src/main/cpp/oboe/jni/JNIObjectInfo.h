#ifndef SINGINGROOM_JNIOBJECTINFO_H
#define SINGINGROOM_JNIOBJECTINFO_H

#include "oboejni.h"

struct JNIObjectInfo {
public:
    /**
     * 생성자. jni 맞춤.
     */
    JNIObjectInfo(JNIEnv* jniEnv, jobject jObject, const char* methodName, const char* sig) {
        jniEnv->NewGlobalRef(jObject);
        this->jniEnv = jniEnv;
        this->jObject = jObject;
        this->clazz = jniEnv->GetObjectClass(jObject);
        this->methodId = jniEnv->GetMethodID(clazz, methodName, sig);
    }

    /**
     * 소멸자.
     */
    ~JNIObjectInfo() {
        jniEnv->DeleteGlobalRef(jObject);
    }

    /**
     * 메서드에 필욯나 매개 변수 넣음.
     * @tparam Args: 매개 변수 리스트.
     * @param args: 매개 변수 스트림.
     */
    template<typename ...Args>
    void callMethod(Args&&... args) {
        if (methodId == nullptr) {
            // 메서드를 찾지 못한 경우 에러 처리
            return;
        }
        jniEnv->CallVoidMethod(jObject, methodId, std::forward<Args>(args)...);
    }

private:
    // jni를 통해 C++ 파일에서 java 파일의 callback 실행을 위한 필요 정보.
    JNIEnv *jniEnv;
    jobject jObject;
    jclass clazz;
    jmethodID methodId;
};

#endif //SINGINGROOM_JNIOBJECTINFO_H
