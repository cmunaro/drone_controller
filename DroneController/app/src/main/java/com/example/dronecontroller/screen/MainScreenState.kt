package com.example.dronecontroller.screen

import no.nordicsemi.android.kotlin.ble.core.ServerDevice

data class MainScreenState(
    val connection: ConnectionState = ConnectionState.Disconnected.Idle,
    val droneState: DroneState = DroneState.Uninitialized
)

sealed interface ConnectionState {
    sealed interface Connected: ConnectionState {
        val deviceName: String
        data class DiscoveringServicesAndCharacteristics(override val deviceName: String): Connected
        data class Ready(override val deviceName: String): Connected
    }
    sealed interface Disconnected: ConnectionState {
        data object Idle: Disconnected
        data object Connecting: Disconnected
        data class Searching(val devices: List<ServerDevice>): Disconnected
    }
}

sealed interface DroneState {
    data object Uninitialized: DroneState
    data class Online(
        val pitch: Float?,
        val roll: Float?,
        val yaw: Float?,
    ): DroneState
}
