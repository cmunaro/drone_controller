package com.example.dronecontroller.screen

import no.nordicsemi.android.kotlin.ble.core.ServerDevice

data class MainScreenState(
    val connection: ConnectionState = ConnectionState.Disconnected.Idle,
    val dronePositionState: DronePositionState? = null,
    val debugPIDState: DebugPIDState? = null,
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

data class DronePositionState(
    val pitch: Float?,
    val roll: Float?,
    val yaw: Float?,
)

data class DebugPIDState(
    val pidDebugEnabled: Boolean,
    val pWeight: Float?,
    val iWeight: Float?,
    val dWeight: Float?,
)
