#pragma once

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct SpeedCmdReg {
            SpeedCmdReg() {
                std::memset(this, 0, 2);
            }

            SpeedCmdReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t speedCmdBuffer :8;
            uint8_t speedCmd :8;

            std::string toString() {
                std::ostringstream stream;
                stream << "Speed cmd: " << static_cast<int>(speedCmd) << ", Speed cmd after buffer: " << static_cast<int>(speedCmdBuffer);
                return stream.str();
            }
        };
    }
}
