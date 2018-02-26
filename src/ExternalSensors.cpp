/**
 *  Copyright (c) 2018 La Cool Co SAS
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 */

#include <Arduino.h>
#include <FS.h>

#include <ArduinoJson.h>
#include <OneWire.h>

#include "CoolConfig.h"
#include "CoolLog.h"
#include "ExternalSensor.h"
#include "ExternalSensors.h"

OneWire oneWire(0);

/**
 *  ExternalSensors::begin():
 *  This method is provided to initialise
 *  the external sensors.
 */
void ExternalSensors::begin() {
  DEBUG_LOG("Enter ExternalSensors.begin()");

  for (int i = 0; i < this->sensorsNumber; i++) {
    if ((sensors[i].reference) == "NDIR_I2C") {
      std::unique_ptr<ExternalSensor<NDIR_I2C>> sensorCO2(
          new ExternalSensor<NDIR_I2C>(this->sensors[i].address));

      sensors[i].exSensor = sensorCO2.release();
      sensors[i].exSensor->begin();
      sensors[i].exSensor->read();
    } else if ((sensors[i].reference) == "DallasTemperature") {
      std::unique_ptr<ExternalSensor<DallasTemperature>> dallasTemp(
          new ExternalSensor<DallasTemperature>(&oneWire));

      sensors[i].exSensor = dallasTemp.release();
      sensors[i].exSensor->begin();
      sensors[i].exSensor->read();
    } else if ((sensors[i].reference) == "Adafruit_TCS34725") {
      int16_t r, g, b, c, colorTemp, lux;
      std::unique_ptr<ExternalSensor<Adafruit_TCS34725>> rgbSensor(
          new ExternalSensor<Adafruit_TCS34725>());

      sensors[i].exSensor = rgbSensor.release();
      sensors[i].exSensor->begin();
      sensors[i].exSensor->read(&r, &g, &b, &c, &colorTemp, &lux);
    } else if ((sensors[i].reference) == "Adafruit_CCS811") {
      int16_t C02, VOT;
      float Temp;
      std::unique_ptr<ExternalSensor<Adafruit_CCS811>> aqSensor(
          new ExternalSensor<Adafruit_CCS811>(this->sensors[i].address));

      sensors[i].exSensor = aqSensor.release();
      sensors[i].exSensor->begin();
      sensors[i].exSensor->read(&C02, &VOT, &Temp);
    } else if ((sensors[i].reference) == "Adafruit_ADS1015") {
      int16_t channel0, channel1, channel2, channel3, diff01, diff23;
      int16_t gain0, gain1, gain2, gain3;
      std::unique_ptr<ExternalSensor<Adafruit_ADS1015>> analogI2C(
          new ExternalSensor<Adafruit_ADS1015>(this->sensors[i].address));

      sensors[i].exSensor = analogI2C.release();
      sensors[i].exSensor->begin();
      sensors[i].exSensor->read(&channel0, &gain0, &channel1, &gain1, &channel2,
                                &gain2, &channel3, &gain3);

    } else if ((sensors[i].reference) == "Adafruit_ADS1115") {
      // Serial.print("ADS1115 !!!  address: ");
      // DEBUG_LOG(this->sensors[i].address);
      int16_t channel0, channel1, channel2, channel3, diff01, diff23;
      int16_t gain0, gain1, gain2, gain3;

      std::unique_ptr<ExternalSensor<Adafruit_ADS1115>> analogI2C(
          new ExternalSensor<Adafruit_ADS1115>(this->sensors[i].address));
      sensors[i].exSensor = analogI2C.release();
      // sensors[i].exSensor->begin();
      sensors[i].exSensor->read(&channel0, &gain0, &channel1, &gain1, &channel2,
                                &gain2, &channel3, &gain3);
    } else if ((sensors[i].reference) == "CoolGauge") {
      uint32_t A, B, C;
      std::unique_ptr<ExternalSensor<Gauges>> gauge(
          new ExternalSensor<Gauges>());

      sensors[i].exSensor = gauge.release();
      // sensors[i].exSensor->begin();
      sensors[i].exSensor->read(&A, &B, &C);
    }
  }
}

/**
 *  ExternalSensors::read():
 *  This method is provided to
 *  read the data from the external sensors
 *
 *  \return json string that contains the
 *  sensors data
 */
String ExternalSensors::read() {
  DEBUG_LOG("Entering ExternalSensors.read()");

  String data;
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  if (!root.success()) {
    DEBUG_LOG("failed to create JSON object for external sensors value");
    // FIXME: return NULL or error code, not some half-assed string
    return ("00");
  } else {
    if (sensorsNumber > 0) {
      for (int i = 0; i < sensorsNumber; i++) {
        if (sensors[i].exSensor != NULL) {
          if (sensors[i].reference == "Adafruit_TCS34725") {
            int16_t r, g, b, c, colorTemp, lux;

            sensors[i].exSensor->read(&r, &g, &b, &c, &colorTemp, &lux);
            root[sensors[i].kind0] = r;
            root[sensors[i].kind1] = g;
            root[sensors[i].kind2] = b;
            root[sensors[i].kind3] = c;
          } else if (sensors[i].reference == "Adafruit_CCS811") {
            int16_t C, V;
            float T;

            sensors[i].exSensor->read(&C, &V, &T);
            root[sensors[i].kind0] = C;
            root[sensors[i].kind1] = V;
            root[sensors[i].kind2] = T;
          } else if ((sensors[i].reference == "Adafruit_ADS1015") ||
                     (sensors[i].reference == "Adafruit_ADS1115")) {
            int16_t channel0, channel1, channel2, channel3, diff01, diff23;
            int16_t gain0, gain1, gain2, gain3;

            sensors[i].exSensor->read(&channel0, &gain0, &channel1, &gain1,
                                      &channel2, &gain2, &channel3, &gain3);
            gain0 = gain0 / 512;
            gain1 = gain1 / 512;
            gain2 = gain2 / 512;
            gain3 = gain3 / 512;
            root["0_" + sensors[i].kind0] = channel0;
            root["G0_" + sensors[i].kind0] = gain0;
            root["1_" + sensors[i].kind1] = channel1;
            root["G1_" + sensors[i].kind1] = gain1;
            root["2_" + sensors[i].kind2] = channel2;
            root["G2_" + sensors[i].kind2] = gain2;
            root["3_" + sensors[i].kind3] = channel3;
            root["G3_" + sensors[i].kind3] = gain3;
          } else if (sensors[i].reference == "CoolGauge") {
            uint32_t A, B, C;

            sensors[i].exSensor->read(&A, &B, &C);
            root[sensors[i].kind0] = A;
            root[sensors[i].kind1] = B;
            root[sensors[i].kind2] = C;
          } else {
            root[sensors[i].type] = sensors[i].exSensor->read();
          }
        } else {
          ERROR_VAR("Undefined (NULL) external sensor at index #", i);
        }
      }
    }
    root.printTo(data);
    DEBUG_JSON("External sensors data:", root);
    DEBUG_VAR("JSON buffer size:", jsonBuffer.size());
    return (data);
  }
}

/**
 *  ExternalSensors::config():
 *  This method is provided to configure
 *  the externalSensors through a configuration
 *  file
 *
 *  \return true if successful,false otherwise
 */
bool ExternalSensors::config() {
  DEBUG_LOG("Entering ExternalSensors.config()");

  CoolConfig config("/externalSensorsConfig.json");

  if (!config.readFileAsJson()) {
    ERROR_LOG("Failed to read /externalSensorsConfig.json");
    return (false);
  }
  JsonObject &json = config.get();

  if (json["sensorsNumber"] != NULL) {
    this->sensorsNumber = json["sensorsNumber"];

    for (int i = 0; i < sensorsNumber; i++) {
      String name = "sensor" + String(i);

      if (json[name].success()) {
        JsonObject &sensorJson = json[name];

        if (sensorJson["reference"].success()) {
          this->sensors[i].reference = sensorJson["reference"].as<String>();
        }
        sensorJson["reference"] = this->sensors[i].reference;

        if (sensorJson["type"].success()) {
          this->sensors[i].type = sensorJson["type"].as<String>();
        }
        sensorJson["type"] = this->sensors[i].type;

        if (sensorJson["address"].success()) {
          this->sensors[i].address = sensorJson["address"];
        }
        sensorJson["address"] = this->sensors[i].address;

        if (sensorJson["kind0"].success()) {
          this->sensors[i].kind0 = sensorJson["kind0"].as<String>();
        }
        sensorJson["kind0"] = this->sensors[i].kind0;

        if (sensorJson["kind1"].success()) {
          this->sensors[i].kind1 = sensorJson["kind1"].as<String>();
        }
        sensorJson["kind1"] = this->sensors[i].kind1;

        if (sensorJson["kind2"].success()) {
          this->sensors[i].kind2 = sensorJson["kind2"].as<String>();
        }
        sensorJson["kind2"] = this->sensors[i].kind2;

        if (sensorJson["kind3"].success()) {
          this->sensors[i].kind3 = sensorJson["kind3"].as<String>();
        }
        sensorJson["sensor3"] = this->sensors[i].kind3;
      } else {
        this->sensors[i] = this->sensors[i];
      }
      json[name]["reference"] = this->sensors[i].reference;
      json[name]["type"] = this->sensors[i].type;
      json[name]["address"] = this->sensors[i].address;
      json[name]["kind0"] = this->sensors[i].kind0;
      json[name]["kind1"] = this->sensors[i].kind1;
      json[name]["kind2"] = this->sensors[i].kind2;
      json[name]["kind3"] = this->sensors[i].kind3;
    }
  } else {
    this->sensorsNumber = this->sensorsNumber;
  }
  json["sensorsNumber"] = this->sensorsNumber;
  if (!config.writeJsonToFile()) {
    ERROR_LOG("Failed to save external sensors configuration");
    return (false);
  }
  return (true);
}

/**
 *  ExternalSensors::printConf():
 *  This method is provided to print the
 *  configuration to the Serial Monitor
 */
void ExternalSensors::printConf() {
  DEBUG_LOG("Entering ExternalSensors.printConf()");
  INFO_LOG("External sensors configuration");
  INFO_VAR("Number of external sensors:", sensorsNumber);

  for (int i = 0; i < sensorsNumber; i++) {
    DEBUG_VAR("Sensor #", i);
    DEBUG_VAR("  Reference:", this->sensors[i].reference);
    DEBUG_VAR("  Type     :", this->sensors[i].type);
    DEBUG_VAR("  Address  :", this->sensors[i].address);
  }
}