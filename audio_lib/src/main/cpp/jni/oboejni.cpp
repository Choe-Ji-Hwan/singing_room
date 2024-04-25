#include "oboejni.h"
#include <iostream>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
) {
    printf("test.");
}


extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray byteArray,
        jint chunkSize
) {
    printf("test.");
}

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
) {
    printf("test.");
}