#include "MPU6050Driver.h"

#include <Arduino.h>
#include <Wire.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

static constexpr uint8_t MPU_ADDRESS = 0x68;

static bool writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

static bool readRegisters(uint8_t reg, uint8_t *buffer, size_t length) {
    if (buffer == nullptr || length == 0 || length > 32) {
        return false;
    }

    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    if (Wire.requestFrom(MPU_ADDRESS, (uint8_t)length) != length) {
        while (Wire.available()) {
            Wire.read();
        }
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = Wire.read();
    }

    return true;
}

static int16_t combineBytes(uint8_t high, uint8_t low) {
    return (int16_t)(((uint16_t)high << 8) | low);
}

bool MPU6050Init() {
    Wire.setClock(I2C_FAST_MODE_HZ);

    Wire.beginTransmission(MPU_ADDRESS);
    if (Wire.endTransmission() != 0) {
        Serial.println("[MPU] Dispositivo non rilevato a 0x68");
        return false;
    }

    uint8_t identity = 0;
    if (!readRegisters(0x75, &identity, 1)) {
        Serial.println("[MPU] Errore lettura WHO_AM_I");
        return false;
    }

    if (identity != 0x68 && identity != 0x70) {
        Serial.println("[MPU] Identita' WHO_AM_I non riconosciuta");
        return false;
    }

    // Wake up e selezione PLL giroscopio asse X
    if (!writeRegister(0x6B, 0x01)) return false;
    delay(50);

    // Disabilita I2C master ausiliario
    if (!writeRegister(0x6A, 0x00)) return false;

    // Filtro passa-basso digitale (DLPF = 4 -> 20 Hz)
    if (!writeRegister(0x1A, 0x04)) return false;

    // Range accelerometro: +/- 8g (4096 LSB/g)
    if (!writeRegister(0x1C, 0x10)) return false;

    // Range giroscopio: +/- 500 deg/s (65.5 LSB/(deg/s))
    if (!writeRegister(0x1B, 0x08)) return false;

    delay(20);
    Serial.println("[MPU] Inizializzazione I2C completata (400 kHz)");
    return true;
}

bool MPU6050Read(MotionData *data) {
    if (data == nullptr) {
        return false;
    }

    // Lettura burst singola di 14 byte (0x3B..0x48: Accel[6] + Temp[2] + Gyro[6])
    // Durata su bus I2C a 400 kHz: ~0.45 ms << WCET C_1 = 2.0 ms
    uint8_t raw[14];
    if (!readRegisters(0x3B, raw, sizeof(raw))) {
        return false;
    }

    constexpr float ACCEL_SCALE = 9.80665f / 4096.0f;
    data->accelerationX = combineBytes(raw[0], raw[1]) * ACCEL_SCALE;
    data->accelerationY = combineBytes(raw[2], raw[3]) * ACCEL_SCALE;
    data->accelerationZ = combineBytes(raw[4], raw[5]) * ACCEL_SCALE;

    constexpr float GYRO_SCALE = 0.01745329252f / 65.5f;
    data->gyroX = combineBytes(raw[8],  raw[9])  * GYRO_SCALE;
    data->gyroY = combineBytes(raw[10], raw[11]) * GYRO_SCALE;
    data->gyroZ = combineBytes(raw[12], raw[13]) * GYRO_SCALE;

    return true;
}