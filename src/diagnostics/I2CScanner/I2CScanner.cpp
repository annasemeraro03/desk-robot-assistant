#include "I2CScanner.h"

/**
 * @brief Scan the I2C bus for connected devices.
 *
 * The function probes every valid I2C address and prints the
 * addresses of all detected devices to the serial monitor.
 */
void scanI2CBus(){
    Serial.println();
    Serial.println("================================");
    Serial.println("        I2C BUS SCANNER");
    Serial.println("================================");

    uint8_t devicesFound = 0;

    for(uint8_t address = 1; address < 127; address++){

        Wire.beginTransmission(address);

        if(Wire.endTransmission() == 0){

            Serial.print("Device found at 0x");

            if(address < 16)
                Serial.print("0");

            Serial.println(address, HEX);

            devicesFound++;
        }
    }

    Serial.println("--------------------------------");

    if(devicesFound == 0){

        Serial.println("No I2C devices found.");
    }
    else{

        Serial.print("Devices detected: ");
        Serial.println(devicesFound);
    }

    Serial.println("================================");
}