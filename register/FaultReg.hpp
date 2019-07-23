#pragma once

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {
        struct FaultReg {
            FaultReg() {
                std::memset(this, 0, 2);
            }

            FaultReg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            bool lockHardwareCurrentLimit :1;
            bool lockSpeedAbnormal :1;
            bool lockKtAbnormal :1;
            bool lockNoMotor :1;
            bool lockOpenLoop :1;
            bool lockClosedLoop :1;
            bool :1;
            bool v3p3UnderVoltage :1;
            bool vccUnderVoltage :1;
            bool vregUnderVoltage :1;
            bool cpUnderVoltage :1;
            bool overCurr :1;
            bool vregOverCurrent :1;
            bool vccOverVoltage :1;
            bool tempWarning :1;
            bool overTemp :1;

            bool anyFault() {
                return *reinterpret_cast<uint16_t*>(this) != 0x00;
            }

            std::string toString() {
                std::ostringstream stream;
                if(anyFault()){
                    stream << "Faults:";
                    if(overTemp)
                        stream << " OverTemp";
                    if(tempWarning)
                        stream << " TempWarning";
                    if(vccOverVoltage)
                        stream << " VCCOverVoltage";
                    if(vregOverCurrent)
                        stream << " VREGOverCurrent";
                    if(overCurr)
                        stream << " OverCurrent";
                    if(cpUnderVoltage)
                        stream << " CPUnderVoltage";
                    if(vregUnderVoltage)
                        stream << " VREGUnderVoltage";
                    if(vccUnderVoltage)
                        stream << " VCCUnderVoltage";
                    if(v3p3UnderVoltage)
                        stream << " V3P3underVoltage";
                    if(lockClosedLoop)
                        stream << " LockClosedLoop";
                    if(lockOpenLoop)
                        stream << " LockOpenLoop";
                    if(lockNoMotor)
                        stream << " LockNoMotor";
                    if(lockKtAbnormal)
                        stream << " LockKtAbnormal";
                    if(lockSpeedAbnormal)
                        stream << " LockSpeedAbnormal";
                    if(lockHardwareCurrentLimit)
                        stream << " LockHardwareCurrentLimit";
                }
                else
                    stream << "No fault";
                return stream.str();
            }
        };
    }
}
