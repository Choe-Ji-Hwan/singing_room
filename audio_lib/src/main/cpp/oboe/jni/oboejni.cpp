#include "oboejni.h"
#include "../audio_processor/OboeAudioPlayer.h"
#include "../audio_processor/OboePlayback.h"
#include <iostream>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayBack_startNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
) {
    // playback 생성.
    auto* playback = new OboePlayback(sampleRate, channelCnt);

    // playback 준비 및 시작.
    playback->preparePlayback();
    playback->startPlayback();

    // 아직 메모리 안에 점유하는 playback 객체 Long으로 리턴.
    return reinterpret_cast<jlong>(playback);
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayBack_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playbackId
) {
    auto* playback = reinterpret_cast<OboePlayback*>(playbackId);
    // 삭제.
    delete playback;
}