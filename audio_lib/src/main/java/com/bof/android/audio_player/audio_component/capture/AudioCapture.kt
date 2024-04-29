package com.bof.android.audio_player.audio_component.capture

/**
 * 오디오 인식 객체.
 */
abstract class AudioCapture {
    /**
     * 레코드 상태.
     */
    enum class RecordState {
        INIT, RUNNING, STOP
    }

    /**
     * delegate
     */
    interface Delegate {
        fun onCapture(pcmData: ShortArray, chunkSize: Int)
    }

    // 현재 셋팅된 Sample Rate, Channel Count
    var sampleRate: Int = 48000 // default sampleRate: 48000Hz
    var channelCount: Int = 1 // default channelCount: Mono(1)

    // 현재 오디오 레코더의 상태.
    var recorderState: RecordState = RecordState.STOP
    // delegate.
    var delegate: Delegate? = null

    /**
     * 오디오 캡처 사용 전, 초기화 과정.
     */
    open fun prepare(sampleRate: Int = 48000, channelCnt: Int = 1, delegate: Delegate) {
        // 초기화 과정.
        this.sampleRate = sampleRate
        this.channelCount = channelCnt
        this.delegate = delegate
    }

    /**
     * 오디오 캡처 실행.
     */
    abstract fun capture()

    /**
     * 오디오 캡처 종료.
     */
    abstract fun finish()
}