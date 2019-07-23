#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config7Reg {
            enum IPDAdvanceAngle {
                IPDAdvanceAngle_30deg = 0b00,
                IPDAdvanceAngle_60deg = 0b01,
                IPDAdvanceAngle_90deg = 0b10,
                IPDAdvanceAngle_120deg = 0b11
            };
            enum IPDClock_Resolution {
                IPDClock_12Hz_Resolution_2_56us = 0b00,
                IPDClock_24Hz_Resolution_1_28us = 0b01,
                IPDClock_47Hz_Resolution_0_64us = 0b10,
                IPDClock_95Hz_Resolution_0_32us = 0b11,
            };
            enum SCOREControlConstant {
                SCOREControlConstant_0_25 = 0b00,
                SCOREControlConstant_0_50 = 0b01,
                SCOREControlConstant_0_75 = 0b10,
                SCOREControlConstant_1_00 = 0b11
            };

            Config7Reg() {
                std::memset(this, 0, 2);

                driverDeadTime = 0b1'1010;
                scoreControlConstant = SCOREControlConstant_1_00;
                ipdClock_resolution = IPDClock_12Hz_Resolution_2_56us;
                ipdCurrentThreshold_alignAndGo = 0;
                ipdAdvanceAngle = IPDAdvanceAngle_30deg;
            }

            Config7Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t getIPDCurrentThreshold_alignAndGo(bool ipdEnabled, float currentA) {
                if(ipdEnabled){
                    if(currentA < 0.4)
                        return 0b0001;
                    else if(currentA <= 3.2)
                        return (uint8_t) std::round(currentA / 0.2) - 1;
                    else
                        return 0b1111;
                }
                else
                    return 0b0000;
            }

            uint8_t getDriverDeadTime(float deadTime_ns) {
                if(deadTime_ns < 40)
                    return 0b0'0000;
                else if(deadTime_ns < 1280)
                    return (uint8_t) std::round(deadTime_ns / 40) - 1;
                else
                    return 0b1'1111;
            }

            uint8_t driverDeadTime :5;
            SCOREControlConstant scoreControlConstant :2;
            bool :1;
            IPDClock_Resolution ipdClock_resolution :2;
            uint8_t ipdCurrentThreshold_alignAndGo :4;
            IPDAdvanceAngle ipdAdvanceAngle :2;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];

                switch(ipdAdvanceAngle){
                case IPDAdvanceAngle_30deg:
                    stream << "IPDAdvanceAngle: 30deg";
                    break;
                case IPDAdvanceAngle_60deg:
                    stream << "IPDAdvanceAngle: 60deg";
                    break;
                case IPDAdvanceAngle_90deg:
                    stream << "IPDAdvanceAngle: 90deg";
                    break;
                case IPDAdvanceAngle_120deg:
                    stream << "IPDAdvanceAngle: 120deg";
                    break;
                default:
                    stream << "IPDAdvanceAngle: UNKNOWN";
                    break;
                }
                stream << ", ";

                if(ipdCurrentThreshold_alignAndGo == 0)
                    stream << "IPDCurrentThreshold: AlignAndGo";
                else if(ipdCurrentThreshold_alignAndGo <= 0b1111){
                    dtoa(buf, (static_cast<uint32_t>(ipdCurrentThreshold_alignAndGo) + 1) * 0.2);
                    stream << "IPDCurrentThreshold: " << buf << "A";
                }
                else
                    stream << "IPDCurrentThreshold: UNKNOWN";
                stream << ", ";

                switch(ipdClock_resolution){
                case IPDClock_12Hz_Resolution_2_56us:
                    stream << "IPDClock: 12Hz - Resolution: 2.56us";
                    break;
                case IPDClock_24Hz_Resolution_1_28us:
                    stream << "IPDClock: 24Hz - Resolution: 1.28us";
                    break;
                case IPDClock_47Hz_Resolution_0_64us:
                    stream << "IPDClock: 47Hz - Resolution: 0.64us";
                    break;
                case IPDClock_95Hz_Resolution_0_32us:
                    stream << "IPDClock: 95Hz - Resolution: 0.32us";
                    break;
                default:
                    stream << "IPDClock: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(scoreControlConstant){
                case SCOREControlConstant_0_25:
                    stream << "SCOREControlConstant: 0.25";
                    break;
                case SCOREControlConstant_0_50:
                    stream << "SCOREControlConstant: 0.50";
                    break;
                case SCOREControlConstant_0_75:
                    stream << "SCOREControlConstant: 0.75";
                    break;
                case SCOREControlConstant_1_00:
                    stream << "SCOREControlConstant: 1.00";
                    break;
                default:
                    stream << "SCOREControlConstant: UNKNOWN";
                    break;
                }
                stream << ", ";

                dtoa(buf, (static_cast<uint32_t>(driverDeadTime) + 1) * 40.0);
                stream << "Driver dead time: " << buf << "ns";

                return stream.str();
            }
        };
    }
}
