package com.bof.android.audio_player.audio_processor

import android.util.Log
import com.bof.android.audio_player.audio_component.capture.AudioCapture
import com.bof.android.audio_player.audio_component.capture.OboeAudioCapture
import com.bof.android.audio_player.audio_component.player.AudioPlayer
import com.bof.android.audio_player.audio_component.player.OboeAudioPlayer

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
    }

    // 오디오 캡처.
    private var audioCapture: AudioCapture? = null
    // 오디오 재생기.
    private var audioPlayer: AudioPlayer? = null

    /**
     * 플레이 백 준비.
     */
    fun prepare() {
        val onCapture: (ShortArray, Int) -> Unit = { pcmData, chunkSize ->
            Log.d("test2", "${pcmData[0]}")
            audioPlayer?.consumeData(pcmData, chunkSize)
        }

        // prepare audio start.
        audioCapture = OboeAudioCapture().also {
            it.prepare(SAMPLE_RATE, CHANNEL_CNT, onCapture)
        }
        // prepare audio player.
        audioPlayer = OboeAudioPlayer().also {
            it.prepare(SAMPLE_RATE, CHANNEL_CNT)
        }
    }

    /**
     * 플레이 백 재생.
     */
    fun play() {
        if (audioCapture == null || audioPlayer == null) return

        // 플레이어를 먼저 실행해 놓음.
        audioPlayer?.start()
        Log.d("test2", "start")
        // 레코딩 하면서, 동시에 녹음한 데이터 재생.
        audioCapture?.capture()
    }

    /**
     * 플레이 백 종료.
     */
    fun finish() {
        // recorder 종료.
        audioCapture?.finish()
        audioCapture = null

        // player 종료.
        audioPlayer?.finish()
        audioPlayer = null
    }
}