//
// Created by imanuel on 05.11.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
#define TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
#define DEBUG 1

#if DEBUG == 1
#define dprint(message) (Serial.print(message))
#define dprintln(message) (Serial.println(message))
#else
#define dprint(message)
#define dprintln(message)
#endif

#endif //TERRARIUM_MEASURE_AND_CONTROL_SYMBOLS_H
