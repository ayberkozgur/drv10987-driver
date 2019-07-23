#pragma once

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct IPDPositionSupplyVoltageReg {
            IPDPositionSupplyVoltageReg() {
                std::memset(this, 0, 2);
            }

            IPDPositionSupplyVoltageReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t supplyVoltage :8;
            uint8_t ipdPosition :8;

            std::string toString() {
                std::ostringstream stream;
                stream << "IPD Pos: " << static_cast<int>(ipdPosition) << ", VSupply: " << supplyVoltage * 30.0 / 255.0 << "V";
                return stream.str();
            }
        };
    }
}
