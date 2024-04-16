#include "BLEController.h"
#include <mbed.h>

BLEService StateService("00700700-0001-0001-0001-111111111111");
BLEFloatCharacteristic PitchCharacteristic(
  "00001111-0000-0000-0001-000000000001", BLERead | BLENotify);
BLEFloatCharacteristic RollCharacteristic(
  "00001111-0000-0000-0001-000000000002", BLERead | BLENotify);
BLEFloatCharacteristic YawCharacteristic(
  "00001111-0000-0000-0001-000000000003", BLERead | BLENotify);

BLEService PIDDebugService("00700700-0001-0001-0001-222222222222");
BLEBoolCharacteristic PIDDebugEnabledCharacteristic(
  "00001111-0000-0000-0002-000000000000", BLERead | BLEWrite);
BLEFloatCharacteristic PWeightCharacteristic(
  "00001111-0000-0000-0002-000000000001", BLEWrite);
BLEFloatCharacteristic IWeightCharacteristic(
  "00001111-0000-0000-0002-000000000002", BLEWrite);
BLEFloatCharacteristic DWeightCharacteristic(
  "00001111-0000-0000-0002-000000000003", BLEWrite);

void BLEController::initialize() {
  while (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
  }

  Serial.print("MAC: ");
  Serial.println(BLE.address());

  BLE.setLocalName("Drone");

  BLE.setAdvertisedService(StateService);
  StateService.addCharacteristic(PitchCharacteristic);
  StateService.addCharacteristic(RollCharacteristic);
  StateService.addCharacteristic(YawCharacteristic);
  BLE.addService(StateService);

  BLE.setAdvertisedService(PIDDebugService);
  PIDDebugService.addCharacteristic(PIDDebugEnabledCharacteristic);
  PIDDebugService.addCharacteristic(PWeightCharacteristic);
  PIDDebugService.addCharacteristic(IWeightCharacteristic);
  PIDDebugService.addCharacteristic(DWeightCharacteristic);
  BLE.addService(PIDDebugService);

  BLE.advertise();
  Serial.println("BLE Service started");
}

void BLEController::updateBLEState() {
  central = BLE.central();

  if (PIDDebugEnabledCharacteristic.written()) {
    pidDebugEnabled = PIDDebugEnabledCharacteristic.value();
  }
  if (PWeightCharacteristic.written()) {
    pWeight = PWeightCharacteristic.value();
  }
  if (IWeightCharacteristic.written()) {
    iWeight = IWeightCharacteristic.value();
  }
  if (DWeightCharacteristic.written()) {
    dWeight = DWeightCharacteristic.value();
  }
}

void BLEController::publishPitchValue(float value) {
  PitchCharacteristic.setValue(value);
}

void BLEController::publishRollValue(float value) {
  RollCharacteristic.setValue(value);
}

void BLEController::publishYawValue(float value) {
  YawCharacteristic.setValue(value);
}
