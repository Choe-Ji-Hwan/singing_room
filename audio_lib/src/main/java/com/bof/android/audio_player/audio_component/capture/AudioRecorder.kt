package com.bof.android.audio_player.audio_component.capture

import android.annotation.SuppressLint
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.AudioTrack
import kotlin.concurrent.thread

/**
 * 오디오 녹음기(AudioRecord 사용) version 1.
 * - 녹음기 기본 기능 제공.
 */
class AudioRecorder: AudioCapture() {

    // 실제로 사용 코어 레코더.
    private var coreRecorder: AudioRecord? = null

    // 사용되는 버퍼 사이즈. 초기화 값: -1
    private var chunkSize = -1

    private val audioSourceId = AudioRecord.MIC_DIRECTION_TOWARDS_USER

    /**
     * 오디오 캡처 사용 전, 초기화 과정.
     * * caution: 캡처 사용을 위해 초기화 과정이 항상 필요함.x
     * * Android version Q(29) 이상만 사용 가능.
     * * 권한은 외부에서 체크하므로, 여기서 린트를 사용하여 컴파일 에러를 막음. -> AudioRecord 생성 시 필수.
     */
    @SuppressLint("MissingPermission")
    override fun prepare(sampleRate: Int, channelCnt: Int, onCapture: ((ShortArray, Int) -> Unit)?) {
        // super.
        super.prepare(sampleRate, channelCnt, onCapture)

        // 현재는, 모노/스테레오 만 지원한다.
        // AudioRecord를 위한 값.
        val inChannelConfig = if (this.channelCount == 1) AudioFormat.CHANNEL_IN_MONO else AudioFormat.CHANNEL_IN_STEREO
        val outChannelConfig = if (this.channelCount == 1) AudioFormat.CHANNEL_OUT_MONO else AudioFormat.CHANNEL_OUT_STEREO

        // 값을 받아올 버퍼의 최소 사이즈(byte array 길이).
        val byteArrayLength = AudioTrack.getMinBufferSize(
            this.sampleRate,
            outChannelConfig,
            AudioFormat.ENCODING_PCM_16BIT
        )
        // byteArray length to shortArray length.
        chunkSize = byteArrayLength / 2

        // 리코더 초기화.
        coreRecorder = AudioRecord(
            audioSourceId,
            this.sampleRate,
            inChannelConfig,
            AudioFormat.ENCODING_PCM_16BIT,
            byteArrayLength
        )

        // 초기화 상태로 변경.
        recorderState = CaptureState.INIT
    }

    /**
     * 오디오 캡처 실행.
     */
    override fun capture() {
        // 준비가 되지 않거나, state 상태가 준비된 상태가 아니라면, play 하지 않음.
        if (coreRecorder == null || recorderState != CaptureState.INIT) return
        coreRecorder?.startRecording()
        recorderState = CaptureState.RUNNING

        thread(start = true) {
            while(recorderState == CaptureState.RUNNING) {
                val readData = ShortArray(chunkSize)
                val ret = coreRecorder?.read(readData, 0, chunkSize)

                actionOnCapture?.invoke(readData, chunkSize)
            }
        }
    }

    /**
     * 오디오 캡처 종료.
     */
    override fun finish() {
        recorderState = CaptureState.STOP
        coreRecorder?.stop()
        coreRecorder?.release()
        coreRecorder = null
    }
}