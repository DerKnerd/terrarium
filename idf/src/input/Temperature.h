//
// Created by imanuel on 03.04.21.
//

#ifndef IDF_TEMPERATURE_H
#define IDF_TEMPERATURE_H

namespace input {
    enum Side {
        HOT,
        COLD
    };

    class Temperature {
    public:
        Temperature();

        ~Temperature();

        auto getTemperature(Side side) -> float;

    private:
    };
}


#endif //IDF_TEMPERATURE_H
