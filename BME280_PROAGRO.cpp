#include "BME280_PROAGRO.h"

#define BME280_REGISTER_CONTROLHUMID 0xF2
#define BME280_REGISTER_CONTROL      0xF4
#define BME280_REGISTER_CONFIG       0xF5
#define BME280_REGISTER_PRESSUREDATA 0xF7
#define BME280_REGISTER_TEMPDATA     0xFA
#define BME280_REGISTER_HUMIDDATA    0xFD

// ✔ NUEVA función compatible con tu datalogger
bool BME280::beginI2C() {
    return beginI2C(0x77);  // dirección confirmada
}

// ✔ función original (NO tocar)
bool BME280::beginI2C(uint8_t address) {
    _addr = address;
    Wire.begin();

    if (read8(0xD0) != 0x60) return false;

    readCoefficients();

    write8(BME280_REGISTER_CONTROLHUMID, 0x03);
    write8(BME280_REGISTER_CONTROL, 0x27);
    write8(BME280_REGISTER_CONFIG, 0xA0);

    return true;
}

float BME280::readTempC() {
    int32_t adc_T = read24(BME280_REGISTER_TEMPDATA) >> 4;

    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) *
                    ((int32_t)dig_T2)) >> 11;

    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
                      ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                    ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;

    float T = (t_fine * 5 + 128) >> 8;
    return T / 100.0;
}

float BME280::readFloatPressure() {
    readTempC();

    int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA) >> 4;

    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) +
           ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) *
           ((int64_t)dig_P1) >> 33;

    if (var1 == 0) return 0;

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

    return (float)p / 256.0;
}

float BME280::readFloatHumidity() {
    readTempC();

    int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);

    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) -
                    (((int32_t)dig_H5) * v_x1_u32r)) +
                   ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) +
                       ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) *
                   ((int32_t)dig_H2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r -
                 (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                   ((int32_t)dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    float h = (v_x1_u32r >> 12);
    return h / 1024.0;
}

// -------- LOW LEVEL ------------

void BME280::readCoefficients() {
    dig_T1 = read16_LE(0x88);
    dig_T2 = readS16_LE(0x8A);
    dig_T3 = readS16_LE(0x8C);

    dig_P1 = read16_LE(0x8E);
    dig_P2 = readS16_LE(0x90);
    dig_P3 = readS16_LE(0x92);
    dig_P4 = readS16_LE(0x94);
    dig_P5 = readS16_LE(0x96);
    dig_P6 = readS16_LE(0x98);
    dig_P7 = readS16_LE(0x9A);
    dig_P8 = readS16_LE(0x9C);
    dig_P9 = readS16_LE(0x9E);

    dig_H1 = read8(0xA1);
    dig_H2 = readS16_LE(0xE1);
    dig_H3 = read8(0xE3);
    dig_H4 = (read8(0xE4) << 4) | (read8(0xE5) & 0x0F);
    dig_H5 = (read8(0xE6) << 4) | (read8(0xE5) >> 4);
    dig_H6 = (int8_t)read8(0xE7);
}

uint8_t BME280::read8(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_addr, (uint8_t)1);
    return Wire.read();
}

uint16_t BME280::read16(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_addr, (uint8_t)2);
    return (Wire.read() << 8) | Wire.read();
}

uint16_t BME280::read16_LE(uint8_t reg) {
    uint16_t temp = read16(reg);
    return (temp >> 8) | (temp << 8);
}

int16_t BME280::readS16_LE(uint8_t reg) {
    return (int16_t)read16_LE(reg);
}

uint32_t BME280::read24(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_addr, (uint8_t)3);
    return ((uint32_t)Wire.read() << 16) |
           ((uint32_t)Wire.read() << 8) |
           Wire.read();
}

void BME280::write8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}