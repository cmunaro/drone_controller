package com.example.dronecontroller.screen.components

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.example.dronecontroller.screen.ConnectionState
import com.example.dronecontroller.screen.DroneState

@Composable
fun ControlsScreen(connectionState: ConnectionState.Connected, droneState: DroneState) {
    Column(modifier = Modifier.fillMaxWidth()) {
        Text(text = "Connected to ${connectionState.deviceName}")
        when (connectionState) {
            is ConnectionState.Connected.DiscoveringServicesAndCharacteristics -> ProgressIndicator()
            is ConnectionState.Connected.Ready -> Text(text = "READY")
        }

        when (val drone = droneState) {
            DroneState.Uninitialized -> Unit
            is DroneState.Online -> {
                Text(text = "Pitch: ${drone.pitch}")
                Text(text = "Roll: ${drone.roll}")
                Text(text = "Yaw: ${drone.yaw}")
                Box(
                    modifier = Modifier
                        .fillMaxWidth()
                        .height(200.dp)
                        .background(Color.White)
                ) {
                    GraphWithMemory(
                        modifier = Modifier.fillMaxSize(),
                        value = drone.pitch ?: 0f,
                        color = Color.Blue
                    )
                    GraphWithMemory(
                        modifier = Modifier.fillMaxSize(),
                        value = drone.roll ?: 0f,
                        color = Color.Green
                    )
                    GraphWithMemory(
                        modifier = Modifier.fillMaxSize(),
                        value = drone.yaw ?: 0f,
                        color = Color.Red
                    )
                    Column(modifier = Modifier.align(Alignment.TopEnd)) {
                        Text(text = "Pitch", color = Color.Blue)
                        Text(text = "Roll", color = Color.Green)
                        Text(text = "Yaw", color = Color.Red)
                    }
                }
            }
        }
    }
}
