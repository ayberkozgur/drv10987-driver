#pragma once

#include "register/AnalogInLvlReg.hpp"
#include "register/Config1Reg.hpp"
#include "register/Config2Reg.hpp"
#include "register/Config3Reg.hpp"
#include "register/Config4Reg.hpp"
#include "register/Config5Reg.hpp"
#include "register/Config6Reg.hpp"
#include "register/Config7Reg.hpp"
#include "register/DeviceIDRevisionIDReg.hpp"
#include "register/FaultReg.hpp"
#include "register/IPDPositionSupplyVoltageReg.hpp"
#include "register/MotorCurrentReg.hpp"
#include "register/SpeedCmdReg.hpp"

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_i2c.h>

#include <cstdint>
#include <string>
#include <utility>
#include <cmath>

using namespace drv10987::registers;

namespace drv10987 {

    class DRV10987 {
    public:
        DRV10987(I2C_HandleTypeDef const& i2cHandle_);
        ~DRV10987();

        void printEEPROMConfig();
        void writeDefaultEEPROMConfig();
        void testOpenLoop();

        FaultReg getFaultReg(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        uint16_t getMotorSpeed(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        uint16_t getMotorPeriod(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        MotorCurrentReg getMotorCurrent(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        IPDPositionSupplyVoltageReg getIPDPositionSupplyVoltage(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        SpeedCmdReg getSpeedCmd(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        AnalogInLvlReg getAnalogInLvl(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        DeviceIDRevisionIDReg getDeviceIDRevisionID(HAL_StatusTypeDef& i2cStatus = dummyStatus);

        void setSpeed(uint16_t speed, HAL_StatusTypeDef& i2cStatus = dummyStatus);

        void disableMotor(bool disable, HAL_StatusTypeDef& i2cStatus = dummyStatus);

        void setConfig1(Config1Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig2(Config2Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig3(Config3Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig4(Config4Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig5(Config5Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig6(Config6Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void setConfig7(Config7Reg config, HAL_StatusTypeDef& i2cStatus = dummyStatus);

        Config1Reg getConfig1(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config2Reg getConfig2(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config3Reg getConfig3(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config4Reg getConfig4(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config5Reg getConfig5(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config6Reg getConfig6(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        Config7Reg getConfig7(HAL_StatusTypeDef& i2cStatus = dummyStatus);

        void waitUntilEEPROMReady(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void enableEEPROMProgramming(bool enable, HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void loadCONFIGToEEPROM(HAL_StatusTypeDef& i2cStatus = dummyStatus);
        void loadEEPROMToCONFIG(HAL_StatusTypeDef& i2cStatus = dummyStatus);

    private:
        I2C_HandleTypeDef i2cHandle;

        union I2CBuffer {
            uint8_t bytes[2];
            uint16_t shortVal;
        } i2cBuffer;

        inline HAL_StatusTypeDef readRegister(uint8_t addr, uint8_t* data);
        inline HAL_StatusTypeDef writeRegister(uint8_t addr, uint8_t* data);

        static HAL_StatusTypeDef dummyStatus;
    };

}
