#include "oboejni.h"
#include "../audio_processor/OboeAudioPlayer.h"
#include "../audio_processor/OboeEngine.h"
#include <iostream>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
) {
/*    auto* player = new OboeAudioPlayer(sampleRate, channelCnt);
    player->prepare();
    return reinterpret_cast<jlong>(player);*/
    auto* player = new OboeEngine();
    player->startPlaybackAndRecording();
    return reinterpret_cast<jlong>(player);
}


extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray shortArray,
        jint chunkSize
) {
    auto* player = reinterpret_cast<OboeAudioPlayer*>(playerId);

    auto* dataArray = new short[chunkSize];
    env->GetShortArrayRegion(shortArray, 0, chunkSize, dataArray);
    // 전달.
    player->consumeData(dataArray, chunkSize);
    // 전달 역할을 다 했으니 해제.
    delete[] dataArray;
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
) {
    auto* player = reinterpret_cast<OboeAudioPlayer*>(playerId);
    player->finish();
    delete player;
}