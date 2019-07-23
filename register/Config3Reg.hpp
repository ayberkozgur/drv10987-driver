#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config3Reg {
            enum ISDStationaryThreshold {
                ISDStationaryThreshold_80ms = 0b00,
                ISDStationaryThreshold_160ms = 0b01,
                ISDStationaryThreshold_320ms = 0b10,
                ISDStationaryThreshold_640ms = 0b11
            };
            enum BrakeCurrentThreshold {
                BrakeCurrentThreshold_24mA = 0b0,
                BrakeCurrentThreshold_48mA = 0b1
            };
            enum BEMFHysteresis {
                BEMFHysteresis_20mV = 0b0,
                BEMFHysteresis_40mV = 0b1
            };
            enum ReverseDriveThreshold {
                ReverseDriveThreshold_6_3Hz = 0b00,
                ReverseDriveThreshold_13Hz = 0b01,
                ReverseDriveThreshold_26Hz = 0b10,
                ReverseDriveThreshold_51Hz = 0b11
            };
            enum OpenLoopCurrentAlignCurrent {
                OpenLoopCurrent_200mA_AlignCurrent_150mA = 0b00,
                OpenLoopCurrent_400mA_AlignCurrent_300mA = 0b01,
                OpenLoopCurrent_800mA_AlignCurrent_600mA = 0b10,
                OpenLoopCurrent_1600mA_AlignCurrent_1200mA = 0b11
            };
            enum OpenLoopCurrentRampUp {
                OpenLoopCurrentRampUp_6VCC_s = 0b000,
                OpenLoopCurrentRampUp_3VCC_s = 0b001,
                OpenLoopCurrentRampUp_1_5VCC_s = 0b010,
                OpenLoopCurrentRampUp_0_7VCC_s = 0b011,
                OpenLoopCurrentRampUp_0_34VCC_s = 0b100,
                OpenLoopCurrentRampUp_0_16VCC_s = 0b101,
                OpenLoopCurrentRampUp_0_07VCC_s = 0b110,
                OpenLoopCurrentRampUp_0_023VCC_s = 0b111
            };
            enum BrakeMode {
                BrakeModeNone = 0b000,
                BrakeMode_2_7s = 0b001,
                BrakeMode_1_3s = 0b010,
                BrakeMode_0_67s = 0b011,
                BrakeMode_0_33s = 0b100,
                BrakeMode_0_16s = 0b101,
                BrakeMode_0_08s = 0b110,
                BrakeMode_0_04s = 0b111
            };

            Config3Reg() {
                std::memset(this, 0, 2);

                brakeMode = BrakeMode_2_7s;
                openLoopCurrentRampUp = OpenLoopCurrentRampUp_6VCC_s;
                openLoopCurrentAlignCurrent = OpenLoopCurrent_1600mA_AlignCurrent_1200mA;
                reverseDriveThreshold = ReverseDriveThreshold_6_3Hz;
                reverseDriveEnabled = false;
                isdEnable = false;
                bemfHysteresis = BEMFHysteresis_20mV;
                brakeCurrentThreshold = BrakeCurrentThreshold_24mA;
                isdStationaryThreshold = ISDStationaryThreshold_80ms;
            }

            Config3Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            BrakeMode brakeMode :3;
            OpenLoopCurrentRampUp openLoopCurrentRampUp :3;
            OpenLoopCurrentAlignCurrent openLoopCurrentAlignCurrent :2;
            ReverseDriveThreshold reverseDriveThreshold :2;
            bool reverseDriveEnabled :1;
            bool isdEnable :1;
            BEMFHysteresis bemfHysteresis :1;
            BrakeCurrentThreshold brakeCurrentThreshold :1;
            ISDStationaryThreshold isdStationaryThreshold :2;

            std::string toString() {
                std::ostringstream stream;

                switch(isdStationaryThreshold){
                case ISDStationaryThreshold_80ms:
                    stream << "ISDStationaryThreshold: 80ms";
                    break;
                case ISDStationaryThreshold_160ms:
                    stream << "ISDStationaryThreshold: 160ms";
                    break;
                case ISDStationaryThreshold_320ms:
                    stream << "ISDStationaryThreshold: 320ms";
                    break;
                case ISDStationaryThreshold_640ms:
                    stream << "ISDStationaryThreshold: 640ms";
                    break;
                default:
                    stream << "ISDStationaryThreshold: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(brakeCurrentThreshold){
                case BrakeCurrentThreshold_24mA:
                    stream << "BrakeCurrentThreshold: 24mA";
                    break;
                case BrakeCurrentThreshold_48mA:
                    stream << "BrakeCurrentThreshold: 48mA";
                    break;
                default:
                    stream << "BrakeCurrentThreshold: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(bemfHysteresis){
                case BEMFHysteresis_20mV:
                    stream << "BEMFHysteresis: 20mV";
                    break;
                case BEMFHysteresis_40mV:
                    stream << "BEMFHysteresis: 40mV";
                    break;
                default:
                    stream << "BEMFHysteresis: UNKNOWN";
                    break;
                }
                stream << ", ";

                stream << "ISD enabled: " << isdEnable << ", ";

                stream << "Reverse drive enabled: " << reverseDriveEnabled << ", ";

                switch(reverseDriveThreshold){
                case ReverseDriveThreshold_6_3Hz:
                    stream << "ReverseDriveThreshold: 6.3Hz";
                    break;
                case ReverseDriveThreshold_13Hz:
                    stream << "ReverseDriveThreshold: 12Hz";
                    break;
                case ReverseDriveThreshold_26Hz:
                    stream << "ReverseDriveThreshold: 26Hz";
                    break;
                case ReverseDriveThreshold_51Hz:
                    stream << "ReverseDriveThreshold: 51Hz";
                    break;
                default:
                    stream << "ReverseDriveThreshold: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(openLoopCurrentAlignCurrent){
                case OpenLoopCurrent_200mA_AlignCurrent_150mA:
                    stream << "OpenLoopCurrent: 200mA - AlignCurrent: 150mA";
                    break;
                case OpenLoopCurrent_400mA_AlignCurrent_300mA:
                    stream << "OpenLoopCurrent: 400mA - AlignCurrent: 300mA";
                    break;
                case OpenLoopCurrent_800mA_AlignCurrent_600mA:
                    stream << "OpenLoopCurrent: 800mA - AlignCurrent: 600mA";
                    break;
                case OpenLoopCurrent_1600mA_AlignCurrent_1200mA:
                    stream << "OpenLoopCurrent: 1600mA - AlignCurrent: 1200mA";
                    break;
                default:
                    stream << "OpenLoopCurrent - AlignCurrent: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(openLoopCurrentRampUp){
                case OpenLoopCurrentRampUp_6VCC_s:
                    stream << "OpenLoopCurrentRampUp: 6VCC/s";
                    break;
                case OpenLoopCurrentRampUp_3VCC_s:
                    stream << "OpenLoopCurrentRampUp: 3VCC/s";
                    break;
                case OpenLoopCurrentRampUp_1_5VCC_s:
                    stream << "OpenLoopCurrentRampUp: 1.5VCC/s";
                    break;
                case OpenLoopCurrentRampUp_0_7VCC_s:
                    stream << "OpenLoopCurrentRampUp: 0.7VCC/s";
                    break;
                case OpenLoopCurrentRampUp_0_34VCC_s:
                    stream << "OpenLoopCurrentRampUp: 0.34VCC/s";
                    break;
                case OpenLoopCurrentRampUp_0_16VCC_s:
                    stream << "OpenLoopCurrentRampUp: 0.16VCC/s";
                    break;
                case OpenLoopCurrentRampUp_0_07VCC_s:
                    stream << "OpenLoopCurrentRampUp: 0.07VCC/s";
                    break;
                case OpenLoopCurrentRampUp_0_023VCC_s:
                    stream << "OpenLoopCurrentRampUp: 0.023VCC/s";
                    break;
                default:
                    stream << "OpenLoopCurrentRampUp: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(brakeMode){
                case BrakeModeNone:
                    stream << "BrakeMode: None";
                    break;
                case BrakeMode_2_7s:
                    stream << "BrakeMode: 2.7s";
                    break;
                case BrakeMode_1_3s:
                    stream << "BrakeMode: 1.3s";
                    break;
                case BrakeMode_0_67s:
                    stream << "BrakeMode: 0.67s";
                    break;
                case BrakeMode_0_33s:
                    stream << "BrakeMode: 0.33s";
                    break;
                case BrakeMode_0_16s:
                    stream << "BrakeMode: 0.16s";
                    break;
                case BrakeMode_0_08s:
                    stream << "BrakeMode: 0.08s";
                    break;
                case BrakeMode_0_04s:
                    stream << "BrakeMode: 0.04s";
                    break;
                default:
                    stream << "BrakeMode: UNKNOWN";
                    break;
                }

                return stream.str();
            }
        };
    }
}
