#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct MotorCurrentReg {
            MotorCurrentReg() {
                std::memset(this, 0, 2);
            }

            MotorCurrentReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint16_t motorCurrent :11;
            uint8_t :5;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];
                dtoa(buf, 3 * (static_cast<int>(motorCurrent) - 1023) / 2048.0);
                stream << "Motor cur: " << buf << "A";
                return stream.str();
            }
        };
    }
}
