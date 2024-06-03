#include "oboejni.h"
#include "../audio_component/AudioCapture.h"
#include "../audio_component/AudioPlayer.h"
#include "../../tools/JNICallbackHandler.h"
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

// ---- Audio Capture to use Oboe ----

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareNativeCapture(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
) {
    // 디오 캡처 생성.
    auto* audioCapture = new AudioCapture();
    audioCapture->prepare(sample_rate, channel_cnt);

    return reinterpret_cast<jlong>(audioCapture);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareActionOnCapture(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jobject on_capture
) {
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);
    auto* jniCallback = new JNICallbackHandler(env, on_capture);

    auto onCapture = [jniCallback](short* data, int size) {
        auto targetEnv = jniCallback->getJNIEnv();
        jshortArray javaArray = targetEnv->NewShortArray(size);

        targetEnv->SetShortArrayRegion(javaArray, 0, size, data);
        jniCallback->call(javaArray, size);
    };

    audioCapture->prepareActionOnCapture(onCapture);

    return reinterpret_cast<jlong>(jniCallback);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_captureNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jlong action_id
) {
    // get Audio Capture Object.
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);

    // start 진행.
    audioCapture->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jlong action_id
) {
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);
    // 내부 할당 해제.
    audioCapture->finish();
    // 최종 할당 해제.
    delete audioCapture;

    // Release the global reference for on_capture
    auto actionOnCapture = reinterpret_cast<JNICallbackHandler*>(action_id);
    // 내부 할당 해제.
    delete actionOnCapture;
}
