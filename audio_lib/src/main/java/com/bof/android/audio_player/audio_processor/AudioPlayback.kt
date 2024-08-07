package com.bof.android.audio_player.audio_processor

import android.media.AudioRecord

/**
 * 오디오를 입력받고, 그대로 출력한다.
 * - 노래방 시스템을 구현하기 위한 테스트 시스템.
 * - 안드로이드 29 이상부터 사용 가능.
 */
abstract class AudioPlayback {

    enum class State { TERMINATED, INIT, PLAY, STOP }

    companion object {
        // 녹음하고 재생할 고정 Sample Rate, 48000Hz
        const val SAMPLE_RATE = 48000
        // 녹음하고 재생할 고정 Channel Count는 1. 마이크는 하나의 마이크만 사용 가정.
        const val CHANNEL_CNT = 1
        // 마이크 사용
        const val AUDIO_TYPE = AudioRecord.MIC_DIRECTION_TOWARDS_USER
    }
    // 플레이 백 상태
    var state: State = State.TERMINATED

    /**
     * 플레이 백 실행.
     */
    abstract fun start()

    /**
     * 플레이 백 종료.
     */
    abstract fun finish()
}