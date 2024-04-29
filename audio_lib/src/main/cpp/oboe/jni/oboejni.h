#include <jni.h>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_OboeAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        int sampleRate,
        int channelCnt
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_OboeAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray shortArray,
        jint chunkSize
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_OboeAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareNative(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
);

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_capturingNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jobject on_capture
);

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id
);