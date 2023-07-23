#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Stream.h>

#if defined(ESP32)
#include "esp32_eeprom_storage.h"
#elif defined(ESP8266)
#include "esp8266_eeprom_storage.h"
#endif


template <typename T, int SIZE>
class JsonConfig
{
private:
  int N;
  String **_keys;
  String _values[SIZE];
  EepromStorage eeprom;

public:
  JsonConfig(T (&keys)[SIZE]) : N(SIZE)
  {
    this->_keys = (String **)keys;
    // this->_values = new String[N];
  }

  virtual ~JsonConfig()
  {
  }

  String get(const String &key)
  {
    for (int i = 0; i < N; i++)
    {
      if (*(this->_keys[i]) == key)
      {
        return this->_values[i];
      }
    }
    return "";
  }

  void set(const String &key, const String &value)
  {
    for (int i = 0; i < N; i++)
    {
      if (*(this->_keys[i]) == key)
      {
        this->_values[i] = value;
      }
    }
  }

  String operator[](const String &key)
  {
    return get(key);
  }

  bool readFromStream(Stream *stream, const int size = 0)
  {
    int _size = size > 0
                    ? size
                    : 2 * stream->available();
    if (!_size)
    {
      PRINTLN("Stream is not available or size is 0");
      return false;
    }
    PRINT("Parsing stream to JSON... ");
    DynamicJsonDocument doc(_size);
    DeserializationError error = deserializeJson(doc, *stream);
    JsonObject object = doc.as<JsonObject>();
    if (error)
    {
      PRINTLN("Failed to deserialize JSON");
      PRINTLN(error.c_str());
      return false;
    }
    for (int i = 0; i < N; i++)
    {
      String *key = this->_keys[i];
      this->_values[i] = object[*key].as<String>();
    }
    PRINTLN("DONE");
    return true;
  }

  void init(Stream *stream, const int size = -1)
  {
    PRINTLN("Init config");
    if (readFromStream(stream, size))
    {
      storeToEeprom();
    }
    else
    {
      loadFromEeprom();
    }

    dump_confg();
  }

  void dump_confg()
  {
    PRINTLN("Dumping configuration values:");
    for (int i = 0; i < N; i++)
    {
      PRINT(*(this->_keys[i]));
      PRINT('=');
      PRINTLN(this->_values[i]);
    }
  }

  void loadFromEeprom()
  {
    PRINTLN("Loading from EEPROM...");
    if (!eeprom.init())
    {
      PRINTLN("Can't init storage");
      return;
    }
    for (int i = 0; i < N; i++)
    {
      this->_values[i] = eeprom.getString(i);
      PRINT("Read ");
      PRINTLN(this->_values[i]);
    }
    eeprom.end();
    PRINTLN("DONE");
  }

  bool storeToEeprom()
  {
    PRINTLN("Storing to EEPROM...");
    if (!eeprom.init())
    {
      PRINTLN("Can't init NVS storage");
      return false;
    }

    for (int i = 0; i < N; i++)
    {
      eeprom.setString(i, this->_values[i]);
    }
    eeprom.end();
    PRINTLN("DONE");
    return true;
  }
};

#endif
