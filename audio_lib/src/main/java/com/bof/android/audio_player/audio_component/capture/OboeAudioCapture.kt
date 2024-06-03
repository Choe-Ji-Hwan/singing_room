package com.bof.android.audio_player.audio_component.capture

import android.util.Log

/**
 * 오디오 녹음기(Oboe 사용) version 2.
 * - 녹음기 기본 기능 제공.
 */
class OboeAudioCapture : AudioCapture() {
    companion object {
        init {
            System.loadLibrary("main")
        }

        // 제대로 된 ID를 받지 않으면, -1을 가짐.
        private const val INVALID_ID: Long = -1L
    }

    // native start object id.
    // 제대로 된 ID를 받지 않으면, -1을 가짐.
    private var captureId: Long = INVALID_ID

    // native onCapture Callback object id.
    // 제대로 된 ID를 받지 않으면, -1을 가짐.
    private var actionOnCaptureId: Long = INVALID_ID

    /**
     * 오디오 캡처 사용 전, 초기화 과정.
     * * caution: 캡처 사용을 위해 초기화 과정이 항상 필요함.x
     */
    override fun prepare(sampleRate: Int, channelCnt: Int, onCapture: ((ShortArray, Int) -> Unit)?) {
        super.prepare(sampleRate, channelCnt, onCapture)
        captureId = prepareNativeCapture(this.sampleRate, this.channelCount)
        actionOnCaptureId = prepareActionOnCapture(captureId, actionOnCapture)
    }

    /**
     * 오디오 캡처 실행.
     */
    override fun capture() {
        if (captureId == INVALID_ID) return

        captureNative(captureId, actionOnCaptureId)
    }

    /**
     * 오디오 캡처 종료.
     */
    override fun finish() {
        if (captureId == INVALID_ID) return

        // todo action id가 필요한가..?
        finishNative(captureId, actionOnCaptureId)

        // id 해제.
        captureId = INVALID_ID
        actionOnCaptureId = INVALID_ID
    }

    private external fun prepareNativeCapture(sampleRate: Int, channelCnt: Int): Long

    private external fun prepareActionOnCapture(objId: Long, onCapture: ((ShortArray, Int) -> Unit)?): Long

    private external fun captureNative(objId: Long, actionOnCaptureId: Long)

    private external fun finishNative(objId: Long, onCaptureCallbackObjId: Long)
}