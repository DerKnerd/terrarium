//
// Created by imanuel on 05.11.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
#define TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
#define DEBUG 1

#if DEBUG == 1
#define dprint(message) (Serial.print(message))
#define dprintln(message) (Serial.println(message))
#define INFLUXDB_CLIENT_DEBUG_ENABLE
#else
#define dprint(message)
#define dprintln(message)
#endif

#define EPD_RESET_PIN 33
#define EPD_DC_PIN 25
#define EPD_CS_PIN 26
#define EPD_BUSY_PIN 27
#define THERMOMETER_PIN 15
#define HEAT_RELAY_PIN 32
#define UV_RELAY_PIN 19

#endif //TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
