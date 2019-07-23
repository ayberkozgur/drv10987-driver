#pragma once

#include "../../dtoa.h"

#include <cmath>
#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config4Reg {
            enum AccelerationRange {
                AccelerationRangeFast = 0b0,
                AccelerationRangeSlow = 0b1
            };
            enum OpenLoopStartUpAccelerationCoeff2 {
                OpenLoopStartUpAccelerationCoeff2_Fast_57Hz_s2_Slow_0_22Hz_s2 = 0b000,
                OpenLoopStartUpAccelerationCoeff2_Fast_29Hz_s2_Slow_0_11Hz_s2 = 0b001,
                OpenLoopStartUpAccelerationCoeff2_Fast_14Hz_s2_Slow_0_055Hz_s2 = 0b010,
                OpenLoopStartUpAccelerationCoeff2_Fast_6_9Hz_s2_Slow_0_027Hz_s2 = 0b011,
                OpenLoopStartUpAccelerationCoeff2_Fast_3_3Hz_s2_Slow_0_013Hz_s2 = 0b100,
                OpenLoopStartUpAccelerationCoeff2_Fast_1_6Hz_s2_Slow_0_0063Hz_s2 = 0b101,
                OpenLoopStartUpAccelerationCoeff2_Fast_0_66Hz_s2_Slow_0_0026Hz_s2 = 0b110,
                OpenLoopStartUpAccelerationCoeff2_Fast_0Hz_s2_Slow_0Hz_s2 = 0b111
            };
            enum OpenLoopStartUpAccelerationCoeff {
                OpenLoopStartUpAccelerationCoeff_Fast_76Hz_s_Slow_4_8Hz_s = 0b000,
                OpenLoopStartUpAccelerationCoeff_Fast_38Hz_s_Slow_2_4Hz_s = 0b001,
                OpenLoopStartUpAccelerationCoeff_Fast_19Hz_s_Slow_1_2Hz_s = 0b010,
                OpenLoopStartUpAccelerationCoeff_Fast_9_2Hz_s_Slow_0_58Hz_s = 0b011,
                OpenLoopStartUpAccelerationCoeff_Fast_4_5Hz_s_Slow_0_28Hz_s = 0b100,
                OpenLoopStartUpAccelerationCoeff_Fast_2_1Hz_s_Slow_0_13Hz_s = 0b101,
                OpenLoopStartUpAccelerationCoeff_Fast_0_9Hz_s_Slow_0_056Hz_s = 0b110,
                OpenLoopStartUpAccelerationCoeff_Fast_0_3Hz_s_Slow_0_019Hz_s = 0b111
            };
            enum AlignTime {
                AlignTime_5_3s = 0b000,
                AlignTime_2_7s = 0b001,
                AlignTime_1_3s = 0b010,
                AlignTime_0_67s = 0b011,
                AlignTime_0_33s = 0b100,
                AlignTime_0_16s = 0b101,
                AlignTime_0_08s = 0b110,
                AlignTime_0_04s = 0b111
            };

            Config4Reg() {
                std::memset(this, 0, 2);

                alignTime = AlignTime_5_3s;
                openToClosedLoopThreshold = 0b1'0001;
                openLoopStartUpAccelerationCoeff = OpenLoopStartUpAccelerationCoeff_Fast_0_3Hz_s_Slow_0_019Hz_s;
                openLoopStartUpAccelerationCoeff2 = OpenLoopStartUpAccelerationCoeff2_Fast_0_66Hz_s2_Slow_0_0026Hz_s2;
                accelerationRange = AccelerationRangeFast;
            }

            Config4Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t getOpenToClosedLoopThreshold(float thresholdHz) {
                if(thresholdHz < 0.8)
                    return 0b0'0001;
                else if(thresholdHz < 12.4)
                    return (uint8_t) std::round(thresholdHz / 0.8);
                else if(thresholdHz <= 204.8)
                    return 0b1'0000 | ((uint8_t) std::round(thresholdHz / 12.8) - 1);
                else
                    return 0b1'1111;
            }

            AlignTime alignTime :3;
            uint8_t openToClosedLoopThreshold :5;
            OpenLoopStartUpAccelerationCoeff openLoopStartUpAccelerationCoeff :3;
            OpenLoopStartUpAccelerationCoeff2 openLoopStartUpAccelerationCoeff2 :3;
            AccelerationRange accelerationRange :1;
            bool :1;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];

                switch(accelerationRange){
                case AccelerationRangeFast:
                    stream << "AccelerationRange: Fast";
                    break;
                case AccelerationRangeSlow:
                    stream << "AccelerationRange: Slow";
                    break;
                default:
                    stream << "AccelerationRange: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(openLoopStartUpAccelerationCoeff2){
                case OpenLoopStartUpAccelerationCoeff2_Fast_57Hz_s2_Slow_0_22Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 57Hz/s2 - Slow 0.22Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_29Hz_s2_Slow_0_11Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 29Hz/s2 - Slow 0.11Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_14Hz_s2_Slow_0_055Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 14Hz/s2 - Slow 0.055Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_6_9Hz_s2_Slow_0_027Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 6.9Hz/s2 - Slow 0.027Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_3_3Hz_s2_Slow_0_013Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 3.3Hz/s2 - Slow 0.013Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_1_6Hz_s2_Slow_0_0063Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 1.6Hz/s2 - Slow 0.0063Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_0_66Hz_s2_Slow_0_0026Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 0.66Hz/s2 - Slow 0.0026Hz/s2";
                    break;
                case OpenLoopStartUpAccelerationCoeff2_Fast_0Hz_s2_Slow_0Hz_s2:
                    stream << "OpenLoopStartUpAccelerationCoeff2: Fast 0Hz/s2 - Slow 0Hz/s2";
                    break;
                default:
                    stream << "OpenLoopStartUpAccelerationCoeff2: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(openLoopStartUpAccelerationCoeff){
                case OpenLoopStartUpAccelerationCoeff_Fast_76Hz_s_Slow_4_8Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 76Hz/s - Slow 4.8Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_38Hz_s_Slow_2_4Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 38Hz/s - Slow 2.4Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_19Hz_s_Slow_1_2Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 19Hz/s - Slow 1.2Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_9_2Hz_s_Slow_0_58Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 9.2Hz/s - Slow 0.58Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_4_5Hz_s_Slow_0_28Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 4.5Hz/s - Slow 0.28Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_2_1Hz_s_Slow_0_13Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 2.1Hz/s - Slow 0.13Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_0_9Hz_s_Slow_0_056Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 0.9Hz/s - Slow 0.056Hz/s";
                    break;
                case OpenLoopStartUpAccelerationCoeff_Fast_0_3Hz_s_Slow_0_019Hz_s:
                    stream << "OpenLoopStartUpAccelerationCoeff: Fast 0.3Hz/s - Slow 0.019Hz/s";
                    break;
                default:
                    stream << "OpenLoopStartUpAccelerationCoeff: UNKNOWN";
                    break;
                }
                stream << ", ";

                if(openToClosedLoopThreshold <= 0b0'1111){
                    dtoa(buf, openToClosedLoopThreshold * 0.8);
                    stream << "OpenToClosedLoopThreshold: " << buf << "Hz";
                }
                else if(openToClosedLoopThreshold <= 0b1'1111){
                    dtoa(buf, (openToClosedLoopThreshold - 0b1'0000 + 1) * 12.8);
                    stream << std::fixed << "OpenToClosedLoopThreshold: " << buf << "Hz";
                }
                else
                    stream << "OpenToClosedLoopThreshold: UNKNOWN";
                stream << ", ";

                switch(alignTime){
                case AlignTime_5_3s:
                    stream << "AlignTime: 5.3s";
                    break;
                case AlignTime_2_7s:
                    stream << "AlignTime: 2.7s";
                    break;
                case AlignTime_1_3s:
                    stream << "AlignTime: 1.3s";
                    break;
                case AlignTime_0_67s:
                    stream << "AlignTime: 0.67s";
                    break;
                case AlignTime_0_33s:
                    stream << "AlignTime: 0.33s";
                    break;
                case AlignTime_0_16s:
                    stream << "AlignTime: 0.16s";
                    break;
                case AlignTime_0_08s:
                    stream << "AlignTime: 0.08s";
                    break;
                case AlignTime_0_04s:
                    stream << "AlignTime: 0.04s";
                    break;
                default:
                    stream << "AlignTime: UNKNOWN";
                    break;
                }
                stream << ", ";

                return stream.str();
            }
        };
    }
}
