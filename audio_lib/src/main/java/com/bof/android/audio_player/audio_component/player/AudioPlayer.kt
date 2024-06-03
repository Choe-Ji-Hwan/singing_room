package com.bof.android.audio_player.audio_component.player

/**
 * 오디오 플레이어 기본 제공 인터페이스.
 * - 이 오디오 플레이어는, 항상 PCM 데이터를 이용하여 소모.
 */
interface AudioPlayer {

    /**
     * 오디오 플레이어를 초기화.
     */
    fun prepare(sampleRate: Int, channelCnt: Int)

    /**
     * 시작.
     */
    fun start()

    /**
     * PCM 데이터를 받아서, 재생.
     */
    fun consumeData(pcmData: ShortArray, chunkSize: Int)

    /**
     * 종료.
     */
    fun finish()
}