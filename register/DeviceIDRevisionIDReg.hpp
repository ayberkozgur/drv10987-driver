#pragma once

#include <sstream>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct DeviceIDRevisionIDReg {
            DeviceIDRevisionIDReg() {
                std::memset(this, 0, 2);
            }

            DeviceIDRevisionIDReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t revisionID :8;
            uint8_t dieID :8;

            std::string toString() {
                std::ostringstream stream;
                stream << "DieID: " << static_cast<int>(dieID) << ", RevisionID: " << static_cast<int>(revisionID);
                return stream.str();
            }
        };
    }
}
