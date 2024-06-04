#include <jni.h>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayBack_startNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_LawLatencyAudioPlayBack_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playbackId
);