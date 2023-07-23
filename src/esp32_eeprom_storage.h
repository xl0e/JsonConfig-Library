#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <ArduinoNvs.h>

class EepromStorage
{
private:
  const String _namespace = "eeprom";
  const String pfx = "k";
public:
  bool init()
  {
    return NVS.begin(_namespace);
  }

  bool end()
  {
    NVS.close();
    return true;
  }

  String getString(const uint16_t id)
  {
    return NVS.getString(pfx + String(id));
  }

  void setString(const uint16_t id, const String &value)
  {
    NVS.setString(pfx + String(id), value);
  }
};

#endif //EEPROM_STORAGE_H