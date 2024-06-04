package com.bof.android.audio_player.audio_component

import com.bof.android.audio_player.audio_processor.AudioPlayback

/**
 * 낮은 지연율을 사용하는 오디오 플레이어.
 * - Oboe 라이브러리를 이용하여 구현할 수 있음.
 * - 실제는 C++ 파일에 있고, 이 객체는 gateway 역할.
 * - 녹음기, 재생기를 나눌 수도 있었으나, 최소 latency를 보장하기 위해서는, 이와 같은 방법이 더 적합하다.
 *      (jni를 통한 콜백에서 지연이 발생함) <- oboe 라이브러리는, 콜백을 이용할 수 밖에 없는 구조라 이와 같이 한 덩어리로 구성.
 *      따라서, 오디오 데이터 변환이 존재한다면, AudioPlayBack에 오디오 데이터 프로세서를 작동시키는 것이 맞다고 생각.
 */
class LawLatencyAudioPlayBack: AudioPlayback {
    companion object {
        // 연결 라이브러리 이름.
        init {
            System.loadLibrary("main")
        }
    }

    // native audioPlayBack object id.
    private var playbackId: Long? = null

    /**
     * playback 시작.
     */
    override fun start() {
        playbackId = startNative(AudioPlayback.SAMPLE_RATE, AudioPlayback.CHANNEL_CNT)
    }

    /**
     * playback 종료.
     */
    override fun finish() {
        if (playbackId == null) return

        finishNative(playbackId!!)
        playbackId = null
    }

    // ---------------------------------------------------------------------------
    /**
     * native methods.
     */
    private external fun startNative(sampleRate: Int, channelCnt: Int): Long

    private external fun finishNative(objId:Long)
}