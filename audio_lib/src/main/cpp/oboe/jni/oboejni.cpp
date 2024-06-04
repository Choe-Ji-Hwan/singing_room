#include "oboejni.h"
#include "../audio_component/AudioCapture.h"
#include "../audio_component/AudioPlayer.h"
#include "../../tools/JNICallbackWrapper.h"
#include <iostream>

// ---- Audio Player to use Oboe ----

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
) {
    auto* player = new AudioPlayer();
    player->prepare(sample_rate, channel_cnt);

    return reinterpret_cast<jlong>(player);
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_startNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
) {
    auto* player = reinterpret_cast<AudioPlayer*>(playerId);
    player->start();
}


extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray shortArray,
        jint chunkSize
) {
    auto* player = reinterpret_cast<AudioPlayer*>(playerId);

    auto* dataArray = new short[chunkSize];
    env->GetShortArrayRegion(shortArray, 0, chunkSize, dataArray);
    // 전달.
    player->consumeData(dataArray, chunkSize);
    // 전달 역할을 다 했으니 해제.
    delete[] dataArray;
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
) {
    auto* player = reinterpret_cast<AudioPlayer*>(playerId);
    player->finish();
    delete player;
}



// todo 나중에, jni 자체도 용도에 따라서 분리해야 됨.
// ---- Audio Capture to use Oboe ----

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareNativeCapture(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt,
        jobject on_capture
) {
    // 오디오 캡처 생성.
    auto* audioCapture = new AudioCapture();

    // 오디오 캡처 중, 실행할 콜백 생성.
    auto* actionOnCapture = new JNICallbackWrapper(env, on_capture, "invoke", "([SI)V");
    function<void(short*, int)> onCapture = [actionOnCapture](short* data, int size) {
        auto targetEnv = actionOnCapture->getJNIEnv();
        jshortArray shortArray = targetEnv->NewShortArray(size);
        targetEnv->SetShortArrayRegion(shortArray, 0, size, data);

        auto callbackObj = actionOnCapture->getCallbackObject();
        auto methodId = actionOnCapture->getMethodId();

        targetEnv->CallVoidMethod(callbackObj, methodId, shortArray, size);
        targetEnv->DeleteLocalRef(shortArray);
    };

    audioCapture->prepare(sample_rate, channel_cnt, onCapture);

    return reinterpret_cast<jlong>(audioCapture);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_captureNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id
) {
    // get Audio Capture Object
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);

    // start 진행.
    audioCapture->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id
) {
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);
    // 내부 할당 해제.
    audioCapture->finish();
    // 최종 할당 해제.
    delete audioCapture;
}
