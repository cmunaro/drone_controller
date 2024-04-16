package com.example.dronecontroller.screen

import android.annotation.SuppressLint
import android.content.Context
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.Job
import kotlinx.coroutines.cancel
import kotlinx.coroutines.cancelAndJoin
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asFlow
import kotlinx.coroutines.flow.collect
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.flatMapConcat
import kotlinx.coroutines.flow.flowOn
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.mapNotNull
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import no.nordicsemi.android.common.core.DataByteArray
import no.nordicsemi.android.common.core.FloatFormat
import no.nordicsemi.android.kotlin.ble.client.main.callback.ClientBleGatt
import no.nordicsemi.android.kotlin.ble.core.ServerDevice
import no.nordicsemi.android.kotlin.ble.core.data.BleGattConnectOptions
import no.nordicsemi.android.kotlin.ble.core.data.GattConnectionState
import no.nordicsemi.android.kotlin.ble.scanner.BleScanner
import no.nordicsemi.android.kotlin.ble.scanner.aggregator.BleScanResultAggregator
import java.nio.ByteBuffer
import java.util.UUID
import kotlin.coroutines.coroutineContext

class MainScreenViewModel : ViewModel() {
    private val _state = MutableStateFlow(MainScreenState())
    val state: StateFlow<MainScreenState> = _state
    private var scanJob: Job? = null

    private val aggregator = BleScanResultAggregator()

    @SuppressLint("MissingPermission")
    fun onSearch(context: Context) {
        if (scanJob?.isActive == true) return
        scanJob = viewModelScope.launch {
            if (state.value.connection is ConnectionState.Disconnected.Searching) return@launch
            _state.update { it.copy(connection = ConnectionState.Disconnected.Searching(emptyList())) }
            BleScanner(context).scan()
                .map { aggregator.aggregateDevices(it) }
                .collect { devices ->
                    _state.update {
                        it.copy(connection = ConnectionState.Disconnected.Searching(devices))
                    }
                }
        }
    }

    @SuppressLint("MissingPermission")
    fun connect(serverDevice: ServerDevice, context: Context) = viewModelScope.launch {
        if (state.value.connection is ConnectionState.Connected) return@launch
        scanJob?.cancelAndJoin()

        val gattClient =
            ClientBleGatt.connect(context = context, device = serverDevice, scope = viewModelScope)
        gattClient.connectionStateWithStatus.collect { connectionState ->
            when (val asd = connectionState?.state) {
                GattConnectionState.STATE_DISCONNECTED -> {
                    _state.update {
                        it.copy(connection = ConnectionState.Disconnected.Idle)
                    }
                    this.cancel("Disconnected", Exception("Disconnected"))
                }

                GattConnectionState.STATE_CONNECTING -> _state.update {
                    it.copy(connection = ConnectionState.Disconnected.Connecting)
                }

                GattConnectionState.STATE_CONNECTED -> launch {
                    startObservingDroneState(gattClient = gattClient, serverDevice = serverDevice)
                }

                else -> {
                    println(asd)
                }
            }
        }
    }

    @OptIn(ExperimentalCoroutinesApi::class)
    private suspend fun startObservingDroneState(
        gattClient: ClientBleGatt,
        serverDevice: ServerDevice
    ) {
        _state.update {
            it.copy(
                connection = ConnectionState.Connected.DiscoveringServicesAndCharacteristics(
                    serverDevice.name.orEmpty()
                )
            )
        }

        gattClient.discoverServices()

        val stateService =
            gattClient.services.mapNotNull { it?.services }.flatMapConcat { it.asFlow() }
                .first { it.uuid == stateServiceUUID }
        val pidDebugService =
            gattClient.services.mapNotNull { it?.services }.flatMapConcat { it.asFlow() }
                .first { it.uuid == pidDebugServiceUUID }

        val pitchCharacteristic = stateService.findCharacteristic(pitchChUUID)!!
        val rollCharacteristic = stateService.findCharacteristic(rollChUUID)!!
        val yawCharacteristic = stateService.findCharacteristic(yawChUUID)!!
        val enableDebugPIDCharacteristic = pidDebugService.findCharacteristic(enableDebugPIDUUID)!!
        val pDebugPidCharacteristic = pidDebugService.findCharacteristic(pDebugPidUUID)!!
        val iDebugPidCharacteristic = pidDebugService.findCharacteristic(iDebugPidUUID)!!
        val dDebugPidCharacteristic = pidDebugService.findCharacteristic(dDebugPidUUID)!!

        _state.update {
            it.copy(
                connection = ConnectionState.Connected.Ready(serverDevice.name.orEmpty())
            )
        }

        viewModelScope.launch {
            combine(
                pitchCharacteristic.getNotifications().map { it.getFloat() },
                rollCharacteristic.getNotifications().map { it.getFloat() },
                yawCharacteristic.getNotifications().map { it.getFloat() }
            ) { pitch, roll, yaw ->
                _state.update {
                    it.copy(droneState = DroneState.Online(pitch, roll, yaw))
                }
            }.collect()
        }
    }

    private fun DataByteArray.getFloat(): Float? = runCatching {
        ByteBuffer.wrap(value.reversedArray()).getFloat()
    }.getOrNull()

    companion object {
        private val stateServiceUUID = UUID.fromString("00700700-0001-0001-0001-111111111111")
        private val pidDebugServiceUUID = UUID.fromString("00700700-0001-0001-0001-222222222222")
        private val pitchChUUID = UUID.fromString("00001111-0000-0000-0001-000000000001")
        private val rollChUUID = UUID.fromString("00001111-0000-0000-0001-000000000002")
        private val yawChUUID = UUID.fromString("00001111-0000-0000-0001-000000000003")
        private val enableDebugPIDUUID = UUID.fromString("00001111-0000-0000-0002-000000000000")
        private val pDebugPidUUID = UUID.fromString("00001111-0000-0000-0002-000000000001")
        private val iDebugPidUUID = UUID.fromString("00001111-0000-0000-0002-000000000002")
        private val dDebugPidUUID = UUID.fromString("00001111-0000-0000-0002-000000000003")
    }
}
