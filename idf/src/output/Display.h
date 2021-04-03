//
// Created by imanuel on 03.04.21.
//

#ifndef IDF_DISPLAY_H
#define IDF_DISPLAY_H

#include <string>

#define COLORED     0
#define UNCOLORED   1

namespace output {
    class Display {
    public:
        Display();

        auto displayString(int line, const std::string &text) -> void;

        auto clear() ->void;

    private:
        epaper::Epd epd;
        epaper::Paint paint;
    };
}

#endif //IDF_DISPLAY_H
