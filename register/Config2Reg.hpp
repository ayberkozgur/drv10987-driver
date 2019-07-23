#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {
        struct Config2Reg {
            enum CommutationAdvanceMode {
                CommutationAdvanceMode_Fixed = 0b0,
                CommutationAdvanceMode_Variable = 0b1
            };

            Config2Reg() {
                std::memset(this, 0, 2);

                ktValue = 0;
                ktShift = 0;
                commutationAdvanceMode = CommutationAdvanceMode_Fixed;
                commutationAdvanceTimeShift = 0b100;
                commutationAdvanceTimeValue = 0b1001;
            }

            Config2Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t commutationAdvanceTimeValue :4;
            uint8_t commutationAdvanceTimeShift :3;
            CommutationAdvanceMode commutationAdvanceMode :1;
            uint8_t ktValue :4;
            uint8_t ktShift :3;
            bool :1;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];

                dtoa(buf, (static_cast<uint32_t>(ktValue) << static_cast<uint32_t>(ktShift)) / 1090.0);
                stream << "Kt: " << buf << ", ";

                switch(commutationAdvanceMode){
                case CommutationAdvanceMode_Fixed:
                    stream << "CommutationAdvanceMode: Fixed";
                    break;
                case CommutationAdvanceMode_Variable:
                    stream << "CommutationAdvanceMode: Variable";
                    break;
                default:
                    stream << "CommutationAdvanceMode: UNKNOWN";
                    break;
                }
                stream << ", ";

                dtoa(buf, (static_cast<uint32_t>(commutationAdvanceTimeValue) << static_cast<uint32_t>(commutationAdvanceTimeShift)) * 2.5f);
                stream << "Commutation advance time: " << buf << "us";

                return stream.str();
            }
        };
    }
}
