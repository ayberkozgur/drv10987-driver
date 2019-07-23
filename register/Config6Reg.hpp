#pragma once

#include "../../dtoa.h"

#include <sstream>
#include <cstdint>
#include <cstring>

namespace drv10987 {
    namespace registers {

        struct Config6Reg {
            enum SpeedCtrlMode {
                SpeedCtrlModeAnalog = 0b0,
                SpeedCtrlModePWM = 0b1
            };
            enum PWMFrequency {
                PWMFrequency_25kHz = 0b0,
                PWMFrequency_50kHz = 0b1
            };
            enum AbnormalKtDetectThreshold {
                AbnormalKtDetectThreshold_High_1_5Kt_Low_0_75Kt = 0b00,
                AbnormalKtDetectThreshold_High_2Kt_Low_0_75Kt = 0b01,
                AbnormalKtDetectThreshold_High_1_5Kt_Low_0_5Kt = 0b10,
                AbnormalKtDetectThreshold_High_2Kt_Low_0_5Kt = 0b11
            };
            enum MechanicalAVSMode {
                MechanicalAVSMode_VCC = 0b0,
                MechanicalAVSMode_24V = 0b1
            };
            enum IPDReleaseMode {
                IPDReleaseMode_Brake = 0b0,
                IPDReleaseMode_HiZ = 0b1
            };
            enum OpenToClosedLoopMode {
                OpenToClosedLoopMode_Transfer = 0b0,
                OpenToClosedLoopMode_NoTransfer = 0b1
            };
            enum ClosedLoopAccelerate {
                ClosedLoopAccelerate_Immediate = 0b000,
                ClosedLoopAccelerate_48Vcc_s = 0b001,
                ClosedLoopAccelerate_48Vcc_s_ = 0b010,
                ClosedLoopAccelerate_0_77Vcc_s = 0b011,
                ClosedLoopAccelerate_0_37Vcc_s = 0b100,
                ClosedLoopAccelerate_0_19Vcc_s = 0b101,
                ClosedLoopAccelerate_0_091Vcc_s = 0b110,
                ClosedLoopAccelerate_0_045Vcc_s = 0b111
            };
            enum DutyCycleLowerLimit {
                DutyCycleLowerLimit_linear_until_5_percent_hold_until_1_5_percent_then_0_percent = 0b00,
                DutyCycleLowerLimit_linear_until_10_percent_hold_until_1_5_percent_then_0_percent = 0b01,
                DutyCycleLowerLimit_linear_until_5_percent_hold_until_1_5_percent_then_100_percent = 0b10,
                DutyCycleLowerLimit_linear_until_10_percent_hold_until_1_5_percent_then_100_percent = 0b11
            };
            enum SlewRate {
                SlewRate_35V_us = 0b00,
                SlewRate_50V_us = 0b01,
                SlewRate_80V_us = 0b10,
                SlewRate_120V_us = 0b11
            };

            Config6Reg() {
                std::memset(this, 0, 2);

                slewRate = SlewRate_35V_us;
                dutyCycleLowerLimit = DutyCycleLowerLimit_linear_until_5_percent_hold_until_1_5_percent_then_0_percent;
                closedLoopAccelerate = ClosedLoopAccelerate_0_37Vcc_s;
                openToClosedLoopMode = OpenToClosedLoopMode_Transfer;
                ipdReleaseMode = IPDReleaseMode_Brake;
                mechanicalAVSMode = MechanicalAVSMode_VCC;
                mechanicalAVSEnable = false;
                inductiveAVSEnable = true;
                abnormalKtDetectThreshold = AbnormalKtDetectThreshold_High_2Kt_Low_0_5Kt;
                pwmFrequency = PWMFrequency_50kHz;
                speedCtrlMode = SpeedCtrlModeAnalog;
            }

            Config6Reg(const void* buffer) {
                std::memcpy(this, buffer, 2);
            }

            SlewRate slewRate :2;
            DutyCycleLowerLimit dutyCycleLowerLimit :2;
            ClosedLoopAccelerate closedLoopAccelerate :3;
            OpenToClosedLoopMode openToClosedLoopMode :1;
            IPDReleaseMode ipdReleaseMode :1;
            MechanicalAVSMode mechanicalAVSMode :1;
            bool mechanicalAVSEnable :1;
            bool inductiveAVSEnable :1;
            AbnormalKtDetectThreshold abnormalKtDetectThreshold :2;
            PWMFrequency pwmFrequency :1;
            SpeedCtrlMode speedCtrlMode :1;

            std::string toString() {
                std::ostringstream stream;

                switch(speedCtrlMode){
                case SpeedCtrlModeAnalog:
                    stream << "SpeedCtrlMode: Analog";
                    break;
                case SpeedCtrlModePWM:
                    stream << "SpeedCtrlMode: PWM";
                    break;
                default:
                    stream << "SpeedCtrlMode: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(pwmFrequency){
                case PWMFrequency_25kHz:
                    stream << "PWMFrequency: 25kHz";
                    break;
                case PWMFrequency_50kHz:
                    stream << "PWMFrequency: 50kHz";
                    break;
                default:
                    stream << "PWMFrequency: UNKNOWN";
                    break;
                }
                stream << ", ";

                switch(abnormalKtDetectThreshold){
                case AbnormalKtDetectThreshold_High_1_5Kt_Low_0_75Kt:
                    stream << "AbnormalKtDetectThreshold: High 1.5Kt - Low 0.75Kt";
                    break;
                case AbnormalKtDetectThreshold_High_2Kt_Low_0_75Kt:
                    stream << "AbnormalKtDetectThreshold: High 2Kt - Low 0.75Kt";
                    break;
                case AbnormalKtDetectThreshold_High_1_5Kt_Low_0_5Kt:
                    stream << "AbnormalKtDetectThreshold: High 1.5Kt - Low 0.5Kt";
                    break;
                case AbnormalKtDetectThreshold_High_2Kt_Low_0_5Kt:
                    stream << "AbnormalKtDetectThreshold: High 2Kt - Low 0.5Kt";
                    break;
                default:
                    stream << "AbnormalKtDetectThreshold: UNKNOWN";
                    break;
                }
                stream << ", ";

                stream << "Inductive AVS enabled: " << inductiveAVSEnable << ", ";
                stream << "Mechanical AVS enabled: " << mechanicalAVSEnable << ", ";

                switch(mechanicalAVSMode){
                case MechanicalAVSMode_VCC:
                    stream << "MechanicalAVSMode: VCC";
                    break;
                case MechanicalAVSMode_24V:
                    stream << "MechanicalAVSMode: 24V";
                    break;
                default:
                    stream << "MechanicalAVSMode: UNKNOWN";
                    break;

                };
                stream << ", ";

                switch(ipdReleaseMode){
                case IPDReleaseMode_Brake:
                    stream << "IPDReleaseMode: Brake";
                    break;
                case IPDReleaseMode_HiZ:
                    stream << "IPDReleaseMode: HiZ";
                    break;
                default:
                    stream << "IPDReleaseMode: UNKNOWN";
                    break;
                };
                stream << ", ";

                switch(openToClosedLoopMode){
                case OpenToClosedLoopMode_Transfer:
                    stream << "OpenToClosedLoopMode: Transfer";
                    break;
                case OpenToClosedLoopMode_NoTransfer:
                    stream << "OpenToClosedLoopMode: NoTransfer";
                    break;
                default:
                    stream << "OpenToClosedLoopMode: UNKNOWN";
                    break;
                };
                stream << ", ";

                switch(closedLoopAccelerate){
                case ClosedLoopAccelerate_Immediate:
                    stream << "ClosedLoopAccelerate: Immediate";
                    break;
                case ClosedLoopAccelerate_48Vcc_s:
                    stream << "ClosedLoopAccelerate: 48Vcc/s (1?)";
                    break;
                case ClosedLoopAccelerate_48Vcc_s_:
                    stream << "ClosedLoopAccelerate: 48Vcc/s (2?)";
                    break;
                case ClosedLoopAccelerate_0_77Vcc_s:
                    stream << "ClosedLoopAccelerate: 0.77Vcc/s";
                    break;
                case ClosedLoopAccelerate_0_37Vcc_s:
                    stream << "ClosedLoopAccelerate: 0.37Vcc/s";
                    break;
                case ClosedLoopAccelerate_0_19Vcc_s:
                    stream << "ClosedLoopAccelerate: 0.19Vcc/s";
                    break;
                case ClosedLoopAccelerate_0_091Vcc_s:
                    stream << "ClosedLoopAccelerate: 0.091Vcc/s";
                    break;
                case ClosedLoopAccelerate_0_045Vcc_s:
                    stream << "ClosedLoopAccelerate: 0.045Vcc/s";
                    break;
                default:
                    stream << "ClosedLoopAccelerate: UNKNOWN";
                    break;
                };
                stream << ", ";

                switch(dutyCycleLowerLimit){
                case DutyCycleLowerLimit_linear_until_5_percent_hold_until_1_5_percent_then_0_percent:
                    stream << "DutyCycleLowerLimit: linear until 5% hold until 1.5% then 0%";
                    break;
                case DutyCycleLowerLimit_linear_until_10_percent_hold_until_1_5_percent_then_0_percent:
                    stream << "DutyCycleLowerLimit: linear until 10% hold until 1.5% then 0%";
                    break;
                case DutyCycleLowerLimit_linear_until_5_percent_hold_until_1_5_percent_then_100_percent:
                    stream << "DutyCycleLowerLimit: linear until 5% hold until 1.5% then 100%";
                    break;
                case DutyCycleLowerLimit_linear_until_10_percent_hold_until_1_5_percent_then_100_percent:
                    stream << "DutyCycleLowerLimit: linear until 10% hold until 1.5% then 100%";
                    break;
                default:
                    stream << "DutyCycleLowerLimit: UNKNOWN";
                    break;
                };
                stream << ", ";

                switch(slewRate){
                case SlewRate_35V_us:
                    stream << "SlewRate: 35V/us";
                    break;
                case SlewRate_50V_us:
                    stream << "SlewRate: 50V/us";
                    break;
                case SlewRate_80V_us:
                    stream << "SlewRate: 80V/us";
                    break;
                case SlewRate_120V_us:
                    stream << "SlewRate: 120V/us";
                    break;
                default:
                    stream << "SlewRate: UNKNOWN";
                    break;
                };

                return stream.str();
            }
        };
    }
}
