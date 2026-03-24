#ifndef BME280_PROAGRO_H
#define BME280_PROAGRO_H

#include <Arduino.h>
#include <Wire.h>

class BME280 {
public:
    bool beginI2C();                 // ✔ compatible con tu datalogger
    bool beginI2C(uint8_t address);  // ✔ modo manual

    float readTempC();
    float readFloatPressure();
    float readFloatHumidity();

private:
    uint8_t _addr;

    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4, dig_H5;
    int8_t dig_H6;

    int32_t t_fine;

    void readCoefficients();

    uint8_t read8(uint8_t reg);
    uint16_t read16(uint8_t reg);
    uint16_t read16_LE(uint8_t reg);
    int16_t readS16_LE(uint8_t reg);
    uint32_t read24(uint8_t reg);
    void write8(uint8_t reg, uint8_t value);
};

#endif