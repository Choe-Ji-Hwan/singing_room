package com.bof.android.audio_player.audio_component.player

/**
 * 낮은 지연율을 사용하는 오디오 플레이어.
 * - Oboe 라이브러리를 이용하여 구현할 수 있음.
 * - 실제는 C++ 파일에 있고, 이 객체는 gateway 역할.
 */
class OboeAudioPlayer: AudioPlayer {
    companion object {
        init {
            System.loadLibrary("main")
        }
    }

    // native player object id.
    private var playerId: Long? = null

    override fun prepare(sampleRate: Int, channelCnt: Int) {
        playerId = prepareNative(sampleRate, channelCnt)
    }

    override fun start() {
        if (playerId == null) return

        startNative(playerId!!)
    }

    override fun consumeData(pcmData: ShortArray, chunkSize: Int) {
        if (playerId == null) return

        consumeDataNative(playerId!!, pcmData, chunkSize)
    }

    override fun finish() {
        if (playerId == null) return

        finishNative(playerId!!)
        playerId = null
    }

    private external fun prepareNative(sampleRate: Int, channelCnt: Int): Long

    private external fun startNative(objId: Long)

    private external fun consumeDataNative(objId: Long, shortArray: ShortArray, chunkSize: Int)

    private external fun finishNative(objId:Long)
}