package com.example.dronecontroller.screen

import androidx.compose.foundation.ExperimentalFoundationApi
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.example.dronecontroller.screen.components.ProgressIndicator
import no.nordicsemi.android.kotlin.ble.core.ServerDevice

@OptIn(ExperimentalFoundationApi::class)
@Composable
fun ConnectionScreen(
    disconnectedState: ConnectionState.Disconnected,
    onSearch: () -> Unit,
    onDeviceSelected: (ServerDevice) -> Unit
) {
    LaunchedEffect(Unit) {
        onSearch()
    }

    Box(
        modifier = Modifier.fillMaxSize(),
        contentAlignment = Alignment.Center
    ) {
        when (disconnectedState) {
            ConnectionState.Disconnected.Idle -> Button(onClick = onSearch) {
                Text(text = "Scan")
            }

            is ConnectionState.Disconnected.Searching -> LazyColumn(
                modifier = Modifier.fillMaxSize(),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                stickyHeader {
                    Text(text = "SEARCHING")
                }
                items(
                    disconnectedState.devices,
                    key = { it.address }) { device ->
                    Column(
                        modifier = Modifier
                            .padding(horizontal = 42.dp)
                            .clickable { onDeviceSelected(device) }
                    ) {
                        Text(text = "${device.name}", style = MaterialTheme.typography.titleMedium)
                        Text(text = "MAC: ${device.address}")
                    }
                }
            }

            ConnectionState.Disconnected.Connecting -> ProgressIndicator()
        }
    }
}
