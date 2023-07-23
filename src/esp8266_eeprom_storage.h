#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <EEPROM.h>

class EepromStorage
{
private:
  uint16_t _address = 0;

public:
  bool init()
  {
    _address = 0;
    EEPROM.begin(SPI_FLASH_SEC_SIZE);
    return true;
  }

  bool end()
  {
    _address = 0;
    EEPROM.end();
    return true;
  }

  String getString(const uint16_t id)
  {
    uint8_t len = EEPROM.read(_address++);
    char chars[len + 1];
    for (uint8_t i = 0; i < len; i++)
    {
      chars[i] = EEPROM.read(_address++);
    }
    chars[len] = '\0';
    return String(chars);
  }

  void setString(const uint16_t id, const String &value)
  {
    uint8_t len = value.length();
    EEPROM.write(_address++, len);
    for (uint8_t i = 0; i < len; i++)
    {
      EEPROM.write(_address++, value[i]);
    }
  }
};

#endif //EEPROM_STORAGE_H