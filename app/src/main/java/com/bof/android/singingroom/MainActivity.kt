package com.bof.android.singingroom

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.navigation.NavHostController
import androidx.navigation.compose.rememberNavController
import com.bof.android.singingroom.navigation.NavGraph
import com.bof.android.singingroom.ui.theme.SingingRoomTheme

/**
 * Main App Controller 역할.
 * - single activity 사용.
 */
class MainActivity : ComponentActivity() {
    private lateinit var navController: NavHostController

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            SingingRoomTheme {
                navController = rememberNavController()
                NavGraph(navController = navController)
            }
        }
    }
}