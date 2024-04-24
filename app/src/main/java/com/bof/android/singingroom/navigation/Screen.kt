package com.bof.android.singingroom.navigation

/**
 * Screen root 정의.
 * navigation 이용 시, 여기서 스크린 정의 및 route 설정.
 */
sealed class Screen(val route: String) {

    /**
     * 홈 스크린.
     */
    object Home : Screen("home_screen")
}