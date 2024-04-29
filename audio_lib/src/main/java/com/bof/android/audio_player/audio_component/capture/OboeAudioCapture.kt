package com.bof.android.audio_player.audio_component.capture

/**
 * 오디오 녹음기(Oboe 사용) version 2.
 * - 녹음기 기본 기능 제공.
 */
class OboeAudioCapture : AudioCapture() {
    companion object {
        init {
            System.loadLibrary("main")
        }
    }

    // native capture object id.
    private var captureId: Long? = null

    /**
     * 오디오 캡처 사용 전, 초기화 과정.
     * * caution: 캡처 사용을 위해 초기화 과정이 항상 필요함.x
     */
    override fun prepare(sampleRate: Int, channelCnt: Int, delegate: Delegate) {
        super.prepare(sampleRate, channelCnt, delegate)
        captureId = prepareNative(this.sampleRate, this.channelCount)
    }

    /**
     * 오디오 캡처 실행.
     */
    override fun capture() {
        if (captureId == null || delegate == null) return
        capturingNative(captureId!!, delegate!!)
    }

    /**
     * 오디오 캡처 종료.
     */
    override fun finish() {
        finishNative(captureId!!)
    }

    private external fun prepareNative(sampleRate: Int, channelCnt: Int,): Long

    private external fun capturingNative(objId: Long, onCapture: Delegate)

    private external fun finishNative(objId:Long)
}