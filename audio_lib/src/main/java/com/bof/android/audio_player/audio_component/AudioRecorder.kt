package com.bof.android.audio_player.audio_component

import android.annotation.SuppressLint
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.AudioTrack
import kotlin.concurrent.thread

/**
 * 오디오 녹음기
 * - 녹음기 기본 기능 제공.
 */
open class AudioRecorder {
    /**
     * 레코드 상태.
     */
    enum class RecordState {
        INIT, RUNNING, STOP
    }

    // 실제로 사용 코어 레코더.
    private var coreRecorder: AudioRecord? = null

    // 사용되는 버퍼 사이즈. 초기화 값: -1
    private var chunkSize = -1
    // 현재 셋팅된 Sample Rate, Channel Count
    private var curSampleRate = 44100   // default 44100Hz
    private var curChannelCount = 1     // default Mono

    // 현재 오디오 레코더의 상태.
    private var recorderState = RecordState.STOP


    /**
     * 레코더 사용 전, 초기화 과정.
     * * caution: 레코더 사용을 위해 초기화 과정이 항상 필요함.
     * * Android version Q(29) 이상만 사용 가능.
     * * 권한은 외부에서 체크하므로, 여기서 린트를 사용하여 컴파일 에러를 막음. -> AudioRecord 생성 시 필수.
     */
    @SuppressLint("MissingPermission")
    fun prepare(
        audioSourceId: Int,
        sampleRate: Int,
        channelCnt: Int
    ) {
        curSampleRate = sampleRate
        curChannelCount = channelCnt
        // 현재는, 모노/스테레오 만 지원한다.
        val inChannelConfig = if (channelCnt == 1) AudioFormat.CHANNEL_IN_MONO else AudioFormat.CHANNEL_IN_STEREO
        val outChannelConfig = if (channelCnt == 1) AudioFormat.CHANNEL_OUT_MONO else AudioFormat.CHANNEL_OUT_STEREO

        // 값을 받아올 버퍼의 최소 사이즈.
        chunkSize = AudioTrack.getMinBufferSize(
            curSampleRate,
            outChannelConfig,
            AudioFormat.ENCODING_PCM_16BIT
        )

        // 리코더 초기화.
        coreRecorder = AudioRecord(
            audioSourceId,
            curSampleRate,
            inChannelConfig,
            AudioFormat.ENCODING_PCM_16BIT,
            chunkSize
        )
        // 초기화 상태로 변경.
        recorderState = RecordState.INIT
    }

    /**
     * 녹음기 실행.
     */
    fun recording(onRecording: (ByteArray, Int) -> Unit) {
        // 준비가 되지 않거나, state 상태가 준비된 상태가 아니라면, play 하지 않음.
        if (coreRecorder == null || recorderState != RecordState.INIT) return
        coreRecorder?.startRecording()
        recorderState = RecordState.RUNNING

        thread(start = true) {
            while(recorderState == RecordState.RUNNING) {
                val readData = ByteArray(chunkSize)
                val ret = coreRecorder?.read(readData, 0, chunkSize)

                onRecording(readData, chunkSize)
            }
        }

    }

    fun finish() {
        recorderState = RecordState.STOP
        coreRecorder?.stop()
        coreRecorder?.release()
        coreRecorder = null
    }
}