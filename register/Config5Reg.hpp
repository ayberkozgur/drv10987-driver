#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config5Reg {
            enum OvertempWarningCurrentLimit {
                OvertempWarningCurrentLimit_None = 0b00,
                OvertempWarningCurrentLimit_1A = 0b01,
                OvertempWarningCurrentLimit_1_6A = 0b10,
                OvertempWarningCurrentLimit_2A = 0b11
            };
            enum HardwareCurrentLimit {
                HardwareCurrentLimit_Range1_2_5A_Range2_0_4A = 0b000,
                HardwareCurrentLimit_Range1_1_9A_Range2_0_8A = 0b001,
                HardwareCurrentLimit_Range1_1_5A_Range2_1_2A = 0b010,
                HardwareCurrentLimit_Range1_0_9A_Range2_1_6A = 0b011,
                HardwareCurrentLimit_Range1_2_5A_Range2_2_0A = 0b100,
                HardwareCurrentLimit_Range1_1_9A_Range2_2_4A = 0b101,
                HardwareCurrentLimit_Range1_1_5A_Range2_2_8A = 0b110,
                HardwareCurrentLimit_Range1_0_9A_Range2_3_2A = 0b111
            };
            enum HardwareCurrentLimitRange {
                HardwareCurrentLimitRange1 = 0b0,
                HardwareCurrentLimitRange2 = 0b1
            };

            Config5Reg() {
                std::memset(this, 0, 2);

                overtempWarningCurrentLimit = OvertempWarningCurrentLimit_None;
                hardwareCurrentLimit = HardwareCurrentLimit_Range1_0_9A_Range2_3_2A;
                softwareCurrentLimit = 0;
                enableClosedLoopLock = true;
                enableOpenLoopLock = true;
                enableNoMotorLock = true;
                enableAbnormalKtLock = false;
                enableAbnormalSpeedLock = true;
                enableCurrentLimitLock = true;
                hardwareCurrentLimitRange = HardwareCurrentLimitRange2;
            }

            Config5Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t getSoftwareCurrentLimit(float currentA) {
                if(currentA < 0.0)
                    return 0b0000;
                else if(currentA < 3.0)
                    return (uint8_t) std::round(currentA / 0.2);
                else
                    return 0b1111;
            }

            HardwareCurrentLimitRange hardwareCurrentLimitRange :1;
            HardwareCurrentLimit hardwareCurrentLimit :3;
            uint8_t softwareCurrentLimit :4;
            bool enableCurrentLimitLock :1;
            bool enableAbnormalSpeedLock :1;
            bool enableAbnormalKtLock :1;
            bool enableNoMotorLock :1;
            bool enableOpenLoopLock :1;
            bool enableClosedLoopLock :1;
            OvertempWarningCurrentLimit overtempWarningCurrentLimit :2;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];

                switch(overtempWarningCurrentLimit){
                case OvertempWarningCurrentLimit_None:
                    stream << "OvertempWarningCurrentLimit: None";
                    break;
                case OvertempWarningCurrentLimit_1A:
                    stream << "OvertempWarningCurrentLimit: 1A";
                    break;
                case OvertempWarningCurrentLimit_1_6A:
                    stream << "OvertempWarningCurrentLimit: 1.6A";
                    break;
                case OvertempWarningCurrentLimit_2A:
                    stream << "OvertempWarningCurrentLimit: 2A";
                    break;
                default:
                    stream << "OvertempWarningCurrentLimit: UNKNOWN";
                    break;
                }
                stream << ", ";

                stream << "ClosedLoopLockEnabled: " << enableClosedLoopLock << ", ";
                stream << "OpenLoopLockEnabled: " << enableOpenLoopLock << ", ";
                stream << "NoMotorLockEnabled: " << enableNoMotorLock << ", ";
                stream << "AbnormalKtLockEnabled: " << enableAbnormalKtLock << ", ";
                stream << "AbnormalSpeedLockEnabled: " << enableAbnormalSpeedLock << ", ";
                stream << "CurrentLimitLockEnabled: " << enableCurrentLimitLock << ", ";

                if(softwareCurrentLimit == 0)
                    stream << "SoftwareCurrentLimit: None";
                else{
                    dtoa(buf, softwareCurrentLimit * 0.2);
                    stream << "SoftwareCurrentLimit: " << buf << "A";
                }
                stream << ", ";

                switch(hardwareCurrentLimit){
                case HardwareCurrentLimit_Range1_2_5A_Range2_0_4A:
                    stream << "HardwareCurrentLimit: Range1 2.5A - Range2 0.4A";
                    break;
                case HardwareCurrentLimit_Range1_1_9A_Range2_0_8A:
                    stream << "HardwareCurrentLimit: Range1 1.9A - Range2 0.8A";
                    break;
                case HardwareCurrentLimit_Range1_1_5A_Range2_1_2A:
                    stream << "HardwareCurrentLimit: Range1 1.5A - Range2 1.2A";
                    break;
                case HardwareCurrentLimit_Range1_0_9A_Range2_1_6A:
                    stream << "HardwareCurrentLimit: Range1 0.9A - Range2 1.6A";
                    break;
                case HardwareCurrentLimit_Range1_2_5A_Range2_2_0A:
                    stream << "HardwareCurrentLimit: Range1 2.5A - Range2 2.0A";
                    break;
                case HardwareCurrentLimit_Range1_1_9A_Range2_2_4A:
                    stream << "HardwareCurrentLimit: Range1 1.9A - Range2 2.4A";
                    break;
                case HardwareCurrentLimit_Range1_1_5A_Range2_2_8A:
                    stream << "HardwareCurrentLimit: Range1 1.5A - Range2 2.8A";
                    break;
                case HardwareCurrentLimit_Range1_0_9A_Range2_3_2A:
                    stream << "HardwareCurrentLimit: Range1 0.9A - Range2 3.2A";
                    break;
                default:
                    stream << "HardwareCurrentLimit: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(hardwareCurrentLimitRange){
                case HardwareCurrentLimitRange1:
                    stream << "HardwareCurrentLimitRange: Range1";
                    break;
                case HardwareCurrentLimitRange2:
                    stream << "HardwareCurrentLimitRange: Range2";
                    break;
                default:
                    stream << "HardwareCurrentLimitRange: UNKNOWN";
                    break;
                }

                return stream.str();
            }
        };
    }
}
