package com.example.dronecontroller

import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.platform.LocalContext
import com.example.dronecontroller.screen.ConnectionScreen
import com.example.dronecontroller.screen.ConnectionState
import com.example.dronecontroller.screen.MainScreenViewModel
import com.example.dronecontroller.screen.components.ControlsScreen

@Composable
fun MainPage() {
    val viewModel = remember {
        MainScreenViewModel()
    }
    val state by viewModel.state.collectAsState()
    val context = LocalContext.current.applicationContext

    when (val connectionState = state.connection) {
        is ConnectionState.Connected -> ControlsScreen(connectionState, state.droneState)
        is ConnectionState.Disconnected -> ConnectionScreen(
            disconnectedState = connectionState,
            onSearch = { viewModel.onSearch(context = context) },
            onDeviceSelected = { viewModel.connect(serverDevice = it, context = context) }
        )
    }
}

