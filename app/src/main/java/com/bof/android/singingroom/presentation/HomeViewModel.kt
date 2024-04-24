package com.bof.android.singingroom.presentation

import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.lifecycle.ViewModel
import com.bof.android.audio_player.audio_component.AudioRecorder
import com.bof.android.audio_player.audio_proccessor.AudioPlayback

class HomeViewModel : ViewModel() {
    companion object {
        private const val RECORD_AUDIO_PERMISSION_REQUEST_CODE = 1001
    }

    // 오디오 플레이 백.
    private var audioPlayBack: AudioPlayback? = null

    /**
     * 플레이 백 준비.
     * - 안드로이드 29 이상.
     */
    fun preparePlayBack(context: Context) {
        // 권한 체크.
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            // 권한을 요청.
            ActivityCompat.requestPermissions(
                context as Activity,
                arrayOf(Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE),
                RECORD_AUDIO_PERMISSION_REQUEST_CODE
            )
            return
        }
        // 권한이 체크 되면 플레이 백 준비.
        audioPlayBack = AudioPlayback().also { it.prepare() }
    }

    /**
     * 플레이 백 실행.
     */
    fun playPlayBack() {
        audioPlayBack?.play()
    }

    /**
     * 플레이 백 종료.
     */
    fun finishPlayBack() {
        audioPlayBack?.finish()
    }
}