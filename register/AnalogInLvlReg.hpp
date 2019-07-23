#pragma once

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct AnalogInLvlReg {
            AnalogInLvlReg() {
                std::memset(this, 0, 2);
            }

            AnalogInLvlReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint16_t commandSenseAdc :10;
            uint8_t :6;

            std::string toString() {
                std::ostringstream stream;
                stream << "Analog speed input: " << commandSenseAdc * 3.3 / 1024.0 << "V";
                return stream.str();
            }
        };
    }
}
