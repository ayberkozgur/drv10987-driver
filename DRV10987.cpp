#include "DRV10987.hpp"
#include "DRV10987.h"

#include <iostream>
#include <sstream>
#include <bitset>
#include <cstring>

extern "C" {
    using namespace drv10987;

    DRV10987_c_ptr DRV10987_instance(I2C_HandleTypeDef i2cHandle) {
        return static_cast<DRV10987_c_ptr>(new DRV10987(i2cHandle));
    }

    void DRV10987_delete(DRV10987_c_ptr dev) {
        delete static_cast<DRV10987*>(dev);
    }

    void DRV10987_printEEPROMConfig(DRV10987_c_ptr dev) {
        static_cast<DRV10987*>(dev)->printEEPROMConfig();
    }

    void DRV10987_writeDefaultEEPROMConfig(DRV10987_c_ptr dev) {
        static_cast<DRV10987*>(dev)->writeDefaultEEPROMConfig();
    }

    void DRV10987_testOpenLoop(DRV10987_c_ptr dev) {
        static_cast<DRV10987*>(dev)->testOpenLoop();
    }
}

namespace drv10987 {

    namespace {
        static constexpr uint16_t I2C_ADDR = 0b101'0010;
        static constexpr uint16_t I2C_ADDR_W = I2C_ADDR << 1;
        static constexpr uint16_t I2C_ADDR_R = ((I2C_ADDR << 1) | 0b0000'0001);

        static constexpr uint8_t REG_ADDR_FAULT = 0x00;
        static constexpr uint8_t REG_ADDR_MOTOR_SPEED = 0x01;
        static constexpr uint8_t REG_ADDR_MOTOR_PERIOD = 0x02;
        static constexpr uint8_t REG_ADDR_MOTOR_KT = 0x03;
        static constexpr uint8_t REG_ADDR_MOTOR_CURRENT = 0x04;
        static constexpr uint8_t REG_ADDR_IPD_POSITION_SUPPLY_VOLTAGE = 0x05;
        static constexpr uint8_t REG_ADDR_SPEED_CMD_SPD_CMD_BUFFER = 0x06;
        static constexpr uint8_t REG_ADDR_ANALOG_IN_LVL = 0x07;
        static constexpr uint8_t REG_ADDR_DEVICE_ID_REVISION_ID = 0x08;
        static constexpr uint8_t REG_ADDR_SPEED_CTRL = 0x30;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_1 = 0x31;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_2 = 0x32;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_3 = 0x33;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_4 = 0x34;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_5 = 0x35;
        static constexpr uint8_t REG_ADDR_EEPROM_PROGRAMMING_6 = 0x36;
        static constexpr uint8_t REG_ADDR_EECTRL = 0x60;

        static constexpr uint8_t REG_ADDR_CONFIG1 = 0x90;
        static constexpr uint8_t REG_ADDR_CONFIG2 = 0x91;
        static constexpr uint8_t REG_ADDR_CONFIG3 = 0x92;
        static constexpr uint8_t REG_ADDR_CONFIG4 = 0x93;
        static constexpr uint8_t REG_ADDR_CONFIG5 = 0x94;
        static constexpr uint8_t REG_ADDR_CONFIG6 = 0x95;
        static constexpr uint8_t REG_ADDR_CONFIG7 = 0x96;

        struct EEPROMProgramming2Reg {
            bool eepromReady :1;
            uint16_t :15;

            EEPROMProgramming2Reg() {
                memset(this, 0, 2);
            }
        };

        struct EEPROMProgramming5Reg {
            enum EEPROMWriteNReadEanble {
                ReadEnable = 0b0,
                WriteEnable = 0b1
            };
            enum EEPROMAccessMode {
                AccessDisabled = 0b00,
                IndividualAccessEnabled = 0b01,
                MassAccessEnabled = 0b10
            };

            EEPROMAccessMode eepromAccessMode :2;
            EEPROMWriteNReadEanble eepromWriteNRreadEnable :1;
            uint8_t :5;
            bool eepromRefresh :1;
            uint8_t :3;
            bool shadowRegEnable :1;
            uint8_t :3;

            EEPROMProgramming5Reg() {
                memset(this, 0, 2);
            }
        };

        struct EECTRLReg {
            uint16_t :15;
            bool motorDisable :1;

            EECTRLReg() {
                memset(this, 0, 2);
            }
        };
    }

    HAL_StatusTypeDef DRV10987::dummyStatus = HAL_OK;

    DRV10987::DRV10987(const I2C_HandleTypeDef& i2cHandle_) {
        i2cHandle = i2cHandle_;
    }

    DRV10987::~DRV10987() {
    }

    void DRV10987::printEEPROMConfig() {
        std::cout << "Disabling motor..." << std::endl;
        disableMotor(true);

        std::cout << "Enabling EEPROM programming..." << std::endl;
        enableEEPROMProgramming(true);

        std::cout << "Loading EEPROM registers into shadow registers..." << std::endl;
        loadEEPROMToCONFIG();

        //std::cout << "Disabling eeprom programming..." << std::endl; //Somehow resets shadow registers to 0x00
        //enableEEPROMProgramming(false);

        std::cout << "Reenabling motor..." << std::endl;
        disableMotor(false);

        std::cout << "Reading shadow registers..." << std::endl;
        std::cout << "CONFIG1: " << getConfig1().toString() << std::endl;
        std::cout << "CONFIG2: " << getConfig2().toString() << std::endl;
        std::cout << "CONFIG3: " << getConfig3().toString() << std::endl;
        std::cout << "CONFIG4: " << getConfig4().toString() << std::endl;
        std::cout << "CONFIG5: " << getConfig5().toString() << std::endl;
        std::cout << "CONFIG6: " << getConfig6().toString() << std::endl;
        std::cout << "CONFIG7: " << getConfig7().toString() << std::endl;
    }

    void DRV10987::writeDefaultEEPROMConfig() {
        std::cout << "Disabling motor..." << std::endl;
        disableMotor(true);

        std::cout << "Enabling EEPROM programming..." << std::endl;
        enableEEPROMProgramming(true);

        std::cout << "Writing shadow registers..." << std::endl;

        Config1Reg config1;
        config1.clockCycleAdjust = Config1Reg::ClockCycleAdjust_Half;
        config1.ssmConfig = Config1Reg::SSMDithering_15Percent;
        config1.rmShift = 0b011;
        config1.rmValue = 0b1010;
        setConfig1(config1);

        Config2Reg config2;
        config2.ktShift = 0b000;
        config2.ktValue = 0b0010;
        config2.commutationAdvanceMode = Config2Reg::CommutationAdvanceMode_Fixed;
        config2.commutationAdvanceTimeShift = 0b000;
        config2.commutationAdvanceTimeValue = 0b1011;
        setConfig2(config2);

        Config3Reg config3;
        config3.openLoopCurrentRampUp = Config3Reg::OpenLoopCurrentRampUp_6VCC_s;
        config3.openLoopCurrentAlignCurrent = Config3Reg::OpenLoopCurrent_800mA_AlignCurrent_600mA;
        config3.bemfHysteresis = Config3Reg::BEMFHysteresis_20mV;
        setConfig3(config3);

        Config4Reg config4;
        config4.alignTime = Config4Reg::AlignTime_0_67s;
        config4.openToClosedLoopThreshold = config4.getOpenToClosedLoopThreshold(200.0);
        config4.accelerationRange = Config4Reg::AccelerationRangeSlow;
        config4.openLoopStartUpAccelerationCoeff = Config4Reg::OpenLoopStartUpAccelerationCoeff_Fast_38Hz_s_Slow_2_4Hz_s;
        config4.openLoopStartUpAccelerationCoeff2 = Config4Reg::OpenLoopStartUpAccelerationCoeff2_Fast_57Hz_s2_Slow_0_22Hz_s2;
        setConfig4(config4);

        Config5Reg config5;
        config5.hardwareCurrentLimit = Config5Reg::HardwareCurrentLimit_Range1_0_9A_Range2_3_2A;
        config5.hardwareCurrentLimitRange = Config5Reg::HardwareCurrentLimitRange2;
        config5.softwareCurrentLimit = 0b0000;//0b1111;
        config5.enableCurrentLimitLock = false;//false;
        setConfig5(config5);

        Config6Reg config6;
        config6.openToClosedLoopMode = Config6Reg::OpenToClosedLoopMode_Transfer;
        config6.slewRate = Config6Reg::SlewRate_120V_us;
        config6.inductiveAVSEnable = false;//true;//false;
        config6.closedLoopAccelerate = Config6Reg::ClosedLoopAccelerate_0_045Vcc_s;
        setConfig6(config6);

        Config7Reg config7;
        config7.driverDeadTime = config7.getDriverDeadTime(1080);//440);
        //config7.scoreControlConstant = Config7Reg::SCOREControlConstant_0_25;
        setConfig7(config7);

        std::cout << "Loading shadow registers into EEPROM registers..." << std::endl;
        loadCONFIGToEEPROM();

        std::cout << "Reenabling motor..." << std::endl;
        disableMotor(false);

        std::cout << "Done." << std::endl;
    }

    void DRV10987::testOpenLoop() {
        setSpeed(230); //Max seems to be ~250

        readRegister(REG_ADDR_MOTOR_KT, i2cBuffer.bytes);
        int kt = (int)i2cBuffer.shortVal;

        readRegister(REG_ADDR_SPEED_CMD_SPD_CMD_BUFFER, i2cBuffer.bytes);
        int speed = ((int)i2cBuffer.bytes[0]);
        int speedbuf = ((int)i2cBuffer.bytes[1]);

        int motorspd = getMotorSpeed()/10;

        FaultReg fault = getFaultReg();
        std::cout << "Kt: " << kt << " spd: " << motorspd <<  " spdcmd: " << speed << " " << speedbuf << " " << getMotorCurrent().toString() << " " <<  fault.toString() << std::endl;

        //if(fault.anyFault())
        //    while(1);
        //std::cout << getMotorCurrent().toString() << std::endl;
    }

    FaultReg DRV10987::getFaultReg(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_FAULT, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<FaultReg*>(&i2cBuffer);
        else
            return FaultReg();
    }

    uint16_t DRV10987::getMotorSpeed(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_MOTOR_SPEED, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return i2cBuffer.shortVal;
        else
            return 0xFFFF;
    }

    uint16_t DRV10987::getMotorPeriod(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_MOTOR_PERIOD, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return i2cBuffer.shortVal;
        else
            return 0xFFFF;
    }

    MotorCurrentReg DRV10987::getMotorCurrent(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_MOTOR_CURRENT, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<MotorCurrentReg*>(&i2cBuffer);
        else
            return MotorCurrentReg();
    }

    IPDPositionSupplyVoltageReg DRV10987::getIPDPositionSupplyVoltage(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_IPD_POSITION_SUPPLY_VOLTAGE, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<IPDPositionSupplyVoltageReg*>(&i2cBuffer);
        else
            return IPDPositionSupplyVoltageReg();
    }

    SpeedCmdReg DRV10987::getSpeedCmd(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_SPEED_CMD_SPD_CMD_BUFFER, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<SpeedCmdReg*>(&i2cBuffer);
        else
            return SpeedCmdReg();
    }

    AnalogInLvlReg DRV10987::getAnalogInLvl(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_ANALOG_IN_LVL, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<AnalogInLvlReg*>(&i2cBuffer);
        else
            return AnalogInLvlReg();
    }

    DeviceIDRevisionIDReg DRV10987::getDeviceIDRevisionID(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_DEVICE_ID_REVISION_ID, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return *reinterpret_cast<DeviceIDRevisionIDReg*>(&i2cBuffer);
        else
            return DeviceIDRevisionIDReg();
    }

    void DRV10987::setSpeed(uint16_t speed, HAL_StatusTypeDef& i2cStatus) {
        i2cBuffer.shortVal = speed | 0b1000'0000'0000'0000;
        i2cStatus = writeRegister(REG_ADDR_SPEED_CTRL, i2cBuffer.bytes);
    }

    void DRV10987::disableMotor(bool disable, HAL_StatusTypeDef& i2cStatus) {
        EECTRLReg reg;
        reg.motorDisable = disable;
        std::memcpy(&i2cBuffer.shortVal, &reg, 2);
        i2cStatus = writeRegister(REG_ADDR_EECTRL, i2cBuffer.bytes);
    }

    void DRV10987::setConfig1(Config1Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG1, i2cBuffer.bytes);
    }

    void DRV10987::setConfig2(Config2Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG2, i2cBuffer.bytes);
    }

    void DRV10987::setConfig3(Config3Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG3, i2cBuffer.bytes);
    }

    void DRV10987::setConfig4(Config4Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG4, i2cBuffer.bytes);
    }

    void DRV10987::setConfig5(Config5Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG5, i2cBuffer.bytes);
    }

    void DRV10987::setConfig6(Config6Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG6, i2cBuffer.bytes);
    }

    void DRV10987::setConfig7(Config7Reg config, HAL_StatusTypeDef& i2cStatus) {
        std::memcpy(&i2cBuffer.shortVal, &config, 2);
        i2cStatus = writeRegister(REG_ADDR_CONFIG7, i2cBuffer.bytes);
    }

    Config1Reg DRV10987::getConfig1(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG1, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config1Reg(&i2cBuffer.shortVal);
        else
            return Config1Reg();
    }

    Config2Reg DRV10987::getConfig2(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG2, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config2Reg(&i2cBuffer.shortVal);
        else
            return Config2Reg();
    }

    Config3Reg DRV10987::getConfig3(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG3, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config3Reg(&i2cBuffer.shortVal);
        else
            return Config3Reg();
    }

    Config4Reg DRV10987::getConfig4(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG4, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config4Reg(&i2cBuffer.shortVal);
        else
            return Config4Reg();
    }

    Config5Reg DRV10987::getConfig5(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG5, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config5Reg(&i2cBuffer.shortVal);
        else
            return Config5Reg();
    }

    Config6Reg DRV10987::getConfig6(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG6, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config6Reg(&i2cBuffer.shortVal);
        else
            return Config6Reg();
    }

    Config7Reg DRV10987::getConfig7(HAL_StatusTypeDef& i2cStatus) {
        i2cStatus = readRegister(REG_ADDR_CONFIG7, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK)
            return Config7Reg(&i2cBuffer.shortVal);
        else
            return Config7Reg();
    }

    void DRV10987::waitUntilEEPROMReady(HAL_StatusTypeDef& i2cStatus) {
        do{
            i2cStatus = readRegister(REG_ADDR_EEPROM_PROGRAMMING_2, i2cBuffer.bytes);
            if(i2cStatus != HAL_OK)
                return;
        } while(!reinterpret_cast<EEPROMProgramming2Reg*>(&i2cBuffer)->eepromReady);
    }

    void DRV10987::enableEEPROMProgramming(bool enable, HAL_StatusTypeDef& i2cStatus) {
        i2cBuffer.shortVal = 0x0000;
        i2cStatus = writeRegister(REG_ADDR_EEPROM_PROGRAMMING_1, i2cBuffer.bytes);
        if(i2cStatus == HAL_OK && enable){
            i2cBuffer.shortVal = 0xC0DE;
            i2cStatus = writeRegister(REG_ADDR_EEPROM_PROGRAMMING_1, i2cBuffer.bytes);
            if(i2cStatus == HAL_OK)
                waitUntilEEPROMReady(i2cStatus);
        }
    }

    void DRV10987::loadCONFIGToEEPROM(HAL_StatusTypeDef& i2cStatus) {
        EEPROMProgramming5Reg reg;
        reg.shadowRegEnable = false;
        reg.eepromRefresh = false;
        reg.eepromWriteNRreadEnable = EEPROMProgramming5Reg::WriteEnable;
        reg.eepromAccessMode = EEPROMProgramming5Reg::MassAccessEnabled;
        i2cBuffer.shortVal = *reinterpret_cast<uint16_t*>(&reg);
        i2cStatus = writeRegister(REG_ADDR_EEPROM_PROGRAMMING_5, i2cBuffer.bytes);

        if(i2cStatus == HAL_OK)
            waitUntilEEPROMReady(i2cStatus);
    }

    void DRV10987::loadEEPROMToCONFIG(HAL_StatusTypeDef& i2cStatus) {
        EEPROMProgramming5Reg reg;
        reg.shadowRegEnable = false;
        reg.eepromRefresh = false;
        reg.eepromWriteNRreadEnable = EEPROMProgramming5Reg::ReadEnable;
        reg.eepromAccessMode = EEPROMProgramming5Reg::MassAccessEnabled;
        i2cBuffer.shortVal = *reinterpret_cast<uint16_t*>(&reg);
        i2cStatus = writeRegister(REG_ADDR_EEPROM_PROGRAMMING_5, i2cBuffer.bytes);

        if(i2cStatus == HAL_OK)
            waitUntilEEPROMReady(i2cStatus);
    }

    inline HAL_StatusTypeDef DRV10987::readRegister(uint8_t regAddr, uint8_t* data) {
        HAL_StatusTypeDef i2cStatus = HAL_I2C_Master_Transmit(&i2cHandle, I2C_ADDR_W, &regAddr, 1, 10000);
        if(i2cStatus == HAL_OK){
            i2cStatus = HAL_I2C_Master_Receive(&i2cHandle, I2C_ADDR_R, data, 2, 10000);
            auto temp = data[0];
            data[0] = data[1];
            data[1] = temp;
        }
        return i2cStatus;
    }

    inline HAL_StatusTypeDef DRV10987::writeRegister(uint8_t regAddr, uint8_t* data) {
        uint8_t buffer[3] = { regAddr, data[1], data[0] };
        return HAL_I2C_Master_Transmit(&i2cHandle, I2C_ADDR_W, buffer, 3, 10000);
    }

}
