package com.bof.android.audio_player.audio_component

import android.media.AudioAttributes
import android.media.AudioFormat
import android.media.AudioTrack

/**
 * AudioTrack을 이용한 오디오 재생기.
 * - 현재, 말하는 것이 바로 들리는 것이 아니라, 딜레이가 생김 -> 딜레이가 없도록 구현 필요.
 */
class AudioPlayer {

    companion object {
        const val DATA_TYPE = AudioFormat.ENCODING_PCM_16BIT
    }

    // 실행 할 주체.
    // * caution: 항상 16비트 PCM 데이터만 관리함.
    // - todo: PCM 데이터 말고, 다른 데이터로 재생할 수 있도록 추가 구현 예정.
    private var player: AudioTrack? = null

    /**
     * 오디오 플레이어를 초기화.
     */
    fun prepare(sampleRate: Int, channelCnt: Int) {
        // 현재는, 모노/스테레오 만 지원한다.
        val outChannelConfig = if (channelCnt == 1) AudioFormat.CHANNEL_OUT_MONO else AudioFormat.CHANNEL_OUT_STEREO

        // 버퍼 사이즈.
        val bufSize = AudioTrack.getMinBufferSize(sampleRate, outChannelConfig, DATA_TYPE)

        player = AudioTrack.Builder()
            .setAudioAttributes(
                AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                    .build()
            )
            .setAudioFormat(
                AudioFormat.Builder()
                    .setEncoding(AudioFormat.ENCODING_PCM_16BIT)
                    .setSampleRate(sampleRate)
                    .setChannelMask(outChannelConfig)
                    .build()
            )
            .setBufferSizeInBytes(bufSize)
            .build()

        player?.play()
    }


    /**
     * PCM 데이터를 받아서, 재생.
     * - todo 나중에 Oboe로 교체 예정.
     */
    fun consumeData(shortArray: ByteArray, chunkSize: Int) {
        player?.write(shortArray, 0, chunkSize)
    }

    fun finish() {
        player?.release()
        player = null
    }

}