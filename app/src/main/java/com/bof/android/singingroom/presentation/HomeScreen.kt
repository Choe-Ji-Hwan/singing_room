package com.bof.android.singingroom.presentation

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext

/**
 * 가장 먼저 나오는, 홈 스크린 화면.
 */
@Composable
fun HomeScreen(modifier: Modifier = Modifier, viewModel: HomeViewModel = HomeViewModel()) {
    // local context.
    val context = LocalContext.current

    // 제목
    Text(
        text = "노래방 어플 테스트!",
        modifier = modifier
    )

    // 버튼을 세로 나열.
    Column(
        modifier = Modifier.fillMaxSize(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        // 시작 버튼
        Button(onClick = {
            viewModel.checkAudioPermission(context)
            viewModel.preparePlayBack()
            viewModel.playPlayBack()
        }) {
            Text("Play")
        }

        // 종료 버튼
        Button(onClick = {
            viewModel.finishPlayBack()
        }) {
            Text("Stop")
        }
    }
}