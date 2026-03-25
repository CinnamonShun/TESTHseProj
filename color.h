#pragma once

#include <cstdint>

class Color {
public:
    Color();
    Color(uint8_t red, uint8_t green, uint8_t blue);

    uint8_t GetRed() const;
    uint8_t GetGreen() const;
    uint8_t GetBlue() const;

    void SetRed(uint8_t red);
    void SetGreen(uint8_t green);
    void SetBlue(uint8_t blue);

    static uint8_t ClampToByte(double value);

private:
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
};
