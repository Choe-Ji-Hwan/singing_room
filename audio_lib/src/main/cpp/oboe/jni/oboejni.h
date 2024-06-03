#include <jni.h>

extern "C" JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_prepareNative(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_startNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_consumeDataNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId,
        jshortArray shortArray,
        jint chunkSize
);

extern "C" JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_player_OboeAudioPlayer_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong playerId
);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareNativeCapture(
        JNIEnv *env,
        jobject thiz,
        jint sample_rate,
        jint channel_cnt
);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_prepareActionOnCapture(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jobject action_object
);

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_captureNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jlong action_id
);

extern "C"
JNIEXPORT void JNICALL
Java_com_bof_android_audio_1player_audio_1component_capture_OboeAudioCapture_finishNative(
        JNIEnv *env,
        jobject thiz,
        jlong obj_id,
        jlong action_id
);