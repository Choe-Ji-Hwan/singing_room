#include "oboejni.h"
#include "../audio_component/AudioCapture.h"
#include "../audio_component/OboeEngine.h"
#include "../../oboe/jni/JNIObjectInfo.h"
#include <iostream>

// ---- Audio Player to use Oboe ----

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
) {
/*    auto* player = new OboeAudioPlayer(sampleRate, channelCnt);
    player->prepare();
    return reinterpret_cast<jlong>(player);*/
    auto* player = new OboeEngine();
    player->startPlaybackAndRecording();
    return reinterpret_cast<jlong>(player);
}


extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_OboeAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray shortArray,
        jint chunkSize
) {
/*    auto* player = reinterpret_cast<OboeAudioPlayer*>(playerId);

    auto* dataArray = new short[chunkSize];
    env->GetShortArrayRegion(shortArray, 0, chunkSize, dataArray);
    // 전달.
    player->consumeData(dataArray, chunkSize);
    // 전달 역할을 다 했으니 해제.
    delete[] dataArray;*/
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_OboeAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
) {
/*    auto* player = reinterpret_cast<OboeAudioPlayer*>(playerId);
    player->finish();
    delete player;*/
}

// ---- Audio Capture to use Oboe ----

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareNative(
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
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_capturingNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jobject on_capture
) {
    auto* audioCapture = reinterpret_cast<AudioCapture*>(obj_id);
    auto* jniObjectInfo = new JNIObjectInfo(env, on_capture, "onCapture", "([SI)V");

    // capture 시, 실행할 callback.
    auto onCapture = [jniObjectInfo](short* audioData, int chunkSize) {
        jniObjectInfo->callMethod(audioData, chunkSize);
        // 메서드를 콜 했으니, 제거.
        delete jniObjectInfo;
    };
    // capture 진행.
    audioCapture->capture(onCapture);
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