#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config1Reg {
            enum SSMConfig {
                SSMNone = 0b00,
                SSMDithering_5Percent = 0b01,
                SSMDithering_10Percent = 0b10,
                SSMDithering_15Percent = 0b11
            };
            enum FGOLOutput {
                FGOLOutput_Both = 0b00,
                FGOLOutput_OnlyClosed = 0b01,
                FGOLOutput_ClosedFirstOpen = 0b10
            };
            enum ClockCycleAdjust {
                ClockCycleAdjust_Full = 0b0,
                ClockCycleAdjust_Half = 0b1
            };

            Config1Reg() {
                std::memset(this, 0, 2);

                ssmConfig = SSMDithering_15Percent;
                fgolOutput = FGOLOutput_Both;
                fgCycle = 0;
                clockCycleAdjust = ClockCycleAdjust_Full;
                rmValue = 0;
                rmShift = 0;
            }

            Config1Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            uint8_t getFGCycle(uint8_t numPoles) {
                if(numPoles < 2)
                    return 0b0000;
                else if(numPoles <= 32)
                    return numPoles / 2 - 1;
                else
                    return 0b1111;
            }

            uint8_t rmValue :4;
            uint8_t rmShift :3;
            ClockCycleAdjust clockCycleAdjust :1;
            uint8_t fgCycle :4;
            FGOLOutput fgolOutput :2;
            SSMConfig ssmConfig :2;

            std::string toString() {
                std::ostringstream stream;
                char buf[32];

                switch(ssmConfig){
                case SSMNone:
                    stream << "SSM: None";
                    break;
                case SSMDithering_5Percent:
                    stream << "SSMDithering: 5Percent";
                    break;
                case SSMDithering_10Percent:
                    stream << "SSMDithering: 10Percent";
                    break;
                case SSMDithering_15Percent:
                    stream << "SSMDithering: 15Percent";
                    break;
                default:
                    stream << "SSM: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(fgolOutput){
                case FGOLOutput_Both:
                    stream << "FGOLOutput: Both";
                    break;
                case FGOLOutput_OnlyClosed:
                    stream << "FGOLOutput: OnlyClosed";
                    break;
                case FGOLOutput_ClosedFirstOpen:
                    stream << "FGOLOutput: ClosedFirstOpen";
                    break;
                default:
                    stream << "FGOLOutput: UNKNOWN";
                    break;
                }
                stream << ", ";

                stream << "Num poles: " << (static_cast<uint32_t>(fgCycle) + 1) * 2 << ", ";

                switch(clockCycleAdjust){
                case ClockCycleAdjust_Full:
                    stream << "ClockCycleAdjust: Full";
                    break;
                case ClockCycleAdjust_Half:
                    stream << "ClockCycleAdjust: Half";
                    break;
                default:
                    stream << "ClockCycleAdjust: UNKNOWN";
                    break;
                }
                stream << ", ";

                dtoa(buf, (static_cast<uint32_t>(rmValue) << static_cast<uint32_t>(rmShift)) * 0.00967);
                stream << "Rmotor: " << buf << "ohms";

                return stream.str();
            }
        };
    }
}
