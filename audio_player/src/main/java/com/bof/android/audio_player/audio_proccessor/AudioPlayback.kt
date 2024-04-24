package com.bof.android.audio_player.audio_proccessor

import android.media.AudioRecord
import com.bof.android.audio_player.audio_component.AudioPlayer
import com.bof.android.audio_player.audio_component.AudioRecorder

/**
 * 오디오를 입력받고, 그대로 출력한다.
 * - 노래방 시스템을 구현하기 위한 테스트 시스템.
 * - 안드로이드 29 이상부터 사용 가능.
 */
class AudioPlayback {

    companion object {
        // 녹음하고 재생할 고정 Sample Rate, 44100Hz
        private const val SAMPLE_RATE = 44100
        // 녹음하고 재생할 고정 Channel Count는 1. 마이크는 하나의 마이크만 사용 가정.
        private const val CHANNEL_CNT = 1
        // 마이크 사용
        private const val AUDIO_TYPE = AudioRecord.MIC_DIRECTION_TOWARDS_USER
    }

    // 오디오 녹음기
    private var audioRecorder: AudioRecorder? = null
    // 오디오 재생기
    private var audioPlayer: AudioPlayer? = null

    /**
     * 플레이 백 준비.
     */
    fun prepare() {
        // recorder 준비.
        audioRecorder = AudioRecorder().also {
            it.prepare(AUDIO_TYPE, SAMPLE_RATE, CHANNEL_CNT)
        }
        // player 준비.
        audioPlayer = AudioPlayer().also {
            it.prepare(SAMPLE_RATE, CHANNEL_CNT)
        }
    }

    /**
     * 플레이 백 재생.
     */
    fun play() {
        if (audioRecorder == null || audioPlayer == null) return

        // 레코딩 하면서, 동시에 녹음한 데이터 재생.
        audioRecorder?.recording(onRecording = { byteArray, size ->
            audioPlayer?.consumeData(byteArray, size)
        })
    }

    /**
     * 플레이 백 종료.
     */
    fun finish() {
        // recorder 종료.
        audioRecorder?.finish()
        audioRecorder = null

        // player 종료.
        audioPlayer?.finish()
        audioPlayer = null
    }
}