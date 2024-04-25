#include <jni.h>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray byteArray,
        jint chunkSize
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
);