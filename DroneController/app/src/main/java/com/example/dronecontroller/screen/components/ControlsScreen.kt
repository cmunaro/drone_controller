package com.example.dronecontroller.screen.components

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material3.Button
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp
import com.example.dronecontroller.screen.ConnectionState
import com.example.dronecontroller.screen.DebugPIDState
import com.example.dronecontroller.screen.DronePositionState

@Composable
fun ControlsScreen(
    connectionState: ConnectionState.Connected,
    dronePositionState: DronePositionState?,
    debugPIDState: DebugPIDState?,
    onTogglePIDDebug: () -> Unit,
    onAlterP: (String) -> Unit,
    onAlterI: (String) -> Unit,
    onAlterD: (String) -> Unit
) {
    Column(modifier = Modifier.fillMaxWidth()) {
        Text(text = "Connected to ${connectionState.deviceName}")
        when (connectionState) {
            is ConnectionState.Connected.DiscoveringServicesAndCharacteristics -> ProgressIndicator()
            is ConnectionState.Connected.Ready -> Text(text = "READY")
        }

        if (dronePositionState != null) {
            DroneStateInfo(dronePositionState = dronePositionState)
        }

        if (debugPIDState != null) {
            DroneDebugPID(
                debugPIDState = debugPIDState,
                onTogglePIDDebug = onTogglePIDDebug,
                onAlterP = onAlterP,
                onAlterI = onAlterI,
                onAlterD = onAlterD
            )
        }
    }
}

@Composable
fun DroneStateInfo(dronePositionState: DronePositionState) {
    Text(text = "Pitch: ${dronePositionState.pitch}")
    Text(text = "Roll: ${dronePositionState.roll}")
    Text(text = "Yaw: ${dronePositionState.yaw}")
    Box(
        modifier = Modifier
            .fillMaxWidth()
            .height(200.dp)
            .background(Color.White)
    ) {
        GraphWithMemory(
            modifier = Modifier.fillMaxSize(),
            value = dronePositionState.pitch ?: 0f,
            color = Color.Blue
        )
        GraphWithMemory(
            modifier = Modifier.fillMaxSize(),
            value = dronePositionState.roll ?: 0f,
            color = Color.Green
        )
        GraphWithMemory(
            modifier = Modifier.fillMaxSize(),
            value = dronePositionState.yaw ?: 0f,
            color = Color.Red
        )
        Column(modifier = Modifier.align(Alignment.TopEnd)) {
            Text(text = "Pitch", color = Color.Blue)
            Text(text = "Roll", color = Color.Green)
            Text(text = "Yaw", color = Color.Red)
        }
    }
}

@Composable
fun DroneDebugPID(
    debugPIDState: DebugPIDState,
    onTogglePIDDebug: () -> Unit,
    onAlterP: (String) -> Unit,
    onAlterI: (String) -> Unit,
    onAlterD: (String) -> Unit
) {
    Column(modifier = Modifier.fillMaxWidth()) {
        Row(
            horizontalArrangement = Arrangement.spacedBy(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Text(text = "Pid debug state")
            Switch(
                checked = debugPIDState.pidDebugEnabled,
                onCheckedChange = { onTogglePIDDebug() }
            )
        }

        DebugPIDEntry(text = "Current P weight: ${debugPIDState.pWeight}", onSet = onAlterP)
        DebugPIDEntry(text = "Current I weight: ${debugPIDState.iWeight}", onSet = onAlterI)
        DebugPIDEntry(text = "Current D weight: ${debugPIDState.dWeight}", onSet = onAlterD)
    }
}

@Composable
fun DebugPIDEntry(
    text: String,
    onSet: (String) -> Unit
) {
    Column {
        Text(text = text)
        Row(verticalAlignment = Alignment.CenterVertically) {
            var textFieldValue by remember { mutableStateOf("") }
            TextField(
                modifier = Modifier.width(100.dp),
                value = textFieldValue,
                onValueChange = { textFieldValue = it },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number)
            )
            Button(onClick = { onSet(textFieldValue) }) {
                Text(text = "Set")
            }
        }
    }
}
