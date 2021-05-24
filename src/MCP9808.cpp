#include "MCP9808.h"
#include "logger.h"

MCP9808Class::MCP9808Class()
{
}

TempReading MCP9808Class::readTemp()
{
    TempReading tempReading;
    tempReading.readError = true;

    if (!selectRegister(5))
    {
        return tempReading;
    }

    Wire.requestFrom(i2c_addr, 2);

    if (!Wire.available())
    {
        Serial.println("Could not read msb");
        return tempReading;
    }

    int msb = Wire.read();

    if (!Wire.available())
    {
        Serial.println("Could not read lsb");
        return tempReading;
    }
    int lsb = Wire.read();

    msb &= 0b11111;

    int temp = 0;
    if (msb & 0b10000)
    {
        msb &= 0b1111; // clear sign
        temp |= msb;
        temp <<= 8;
        temp |= lsb;
        temp |= 0xFFFFF000;
    }
    else
    {
        temp |= msb;
        temp <<= 8;
        temp |= lsb;
    }

    tempReading.readError = false;
    tempReading.temperature = temp;

    return tempReading;
}

bool MCP9808Class::selectRegister(unsigned char reg)
{
    Wire.beginTransmission(i2c_addr);
    Wire.write(reg);
    uint8_t result = Wire.endTransmission(false);
    if (result != 0)
    {
        Log.log("Error setAddrForRead()");
        return false;
    }
    else
    {
        return true;
    }
}

MCP9808Class MCP9808;