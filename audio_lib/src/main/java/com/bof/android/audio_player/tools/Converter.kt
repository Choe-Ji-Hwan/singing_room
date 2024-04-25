package com.bof.android.audio_player.tools

import java.nio.ByteBuffer
import java.nio.ByteOrder


/**
 * ByteArray to ShortArray
 */
fun ByteArray.toShortArray(): ShortArray {
    val shortArray = ShortArray(this.size / 2)
    ByteBuffer.wrap(this).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shortArray)
    return shortArray
}
