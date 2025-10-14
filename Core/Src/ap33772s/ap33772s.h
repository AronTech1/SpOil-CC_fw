/*
 * ap33772s.h
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#ifndef AP33772S_H
#define AP33772S_H

#include "i2c.h"
#include "log.h"

#define MAX_PDO_ENTRIES 13  // Define the maximum number of PDO entries you expect

#define AP_ADRS 0x52
#define READ_BUFF_LENGTH 128
#define WRITE_BUFF_LENGTH 6
#define SRCPDO_LENGTH 28

#define CMD_STATUS    0x01 //Reset to 0 after very Read
#define CMD_MASK      0x02
#define CMD_OPMODE    0x03
#define CMD_CONFIG    0x04
#define CMD_PDCONFIG  0x05
#define CMD_SYSTEM    0x06

/*Temperature setting register*/
#define CMD_TR25     0x0C
#define CMD_TR50     0x0D
#define CMD_TR75     0x0E
#define CMD_TR100    0x0F

/*Power reading*/
#define CMD_VOLTAGE   0x11
#define CMD_CURRENT   0x12
#define CMD_TEMP      0x13
#define CMD_VREQ      0x14
#define CMD_IREQ      0x15


#define CMD_VSELMIN   0x16 // Minimum Selection Voltage
#define CMD_UVPTHR    0x17
#define CMD_OVPTHR    0x18
#define CMD_OCPTHR    0x19
#define CMD_OTPTHR    0x1A
#define CMD_DRTHR     0x1B

#define CMD_SRCPDO    0x20

#define CMD_PD_REQMSG 0x31
#define CMD_PD_CMDMSG 0x32
#define CMD_PD_MSGRLT 0x33

/*Timer for AVS reminder signal*/
#define ALARM_NUM1 1 // Timer 1
#define ALARM_IRQ1 TIMER_IRQ_1
#define DELAY1 500000 // In seconds , 0.5s

typedef enum
{
  STARTED_MSK   = 1 << 0,     // 0000 0001
  READY_MSK     = 1 << 1,     // 0000 0010
  NEWPDO_MSK    = 1 << 2,     // 0000 0100
  UVP_MSK       = 1 << 3,     // 0001 0000
  OVP_MSK       = 1 << 4,     // 0010 0000
  OCP_MSK       = 1 << 5,     // 0100 0000
  OTP_MSK       = 1 << 6      // 1000 0000
} ApMsk;

typedef struct
{
  union
  {
    struct
    {
      byte newNegoSuccess : 1;
      byte newNegoFail : 1;
      byte negoSuccess : 1;
      byte negoFail : 1;
      byte reserved_1 : 4;
    };
    byte negoEvent;
  };
  union
  {
    struct
    {
      byte ovp : 1;
      byte ocp : 1;
      byte otp : 1;
      byte dr : 1;
      byte reserved_2 : 4;
    };
    byte protectEvent;
  };
} eventFlag_t;

typedef struct {
  union {
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int peak_current: 2;  // Bits 9:8, PEAK_CURRENT field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } fixed;
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } pps;
  struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } avs;
  struct {
      byte byte0;
      byte byte1;
  };
  };
  unsigned long data;
} sprEprPdo_t;

typedef struct {
  union {
    struct{
      unsigned int voltageSel: 8;  // Bits 7:0, Output Voltage Select
      unsigned int currentSel: 4;  // Bits 11:8, Operating Current Select
      unsigned int pdoIndex: 4;  // Bits 15:12, Source PDO index select
    }reqMsg;

    struct{
      byte byte0;
      byte byte1;
    };

    unsigned long data;
  };
} rdoData_t;

/*Readiness Inquiry & NMOS switching*/

/**
 * @brief: Checks for the readiness of the AP after boot up
 *  After boot up expect:
 *  NEWPDO      = 1
 *  READY       = 1
 *  STARTED     = 1
 *  After read, STATUS register will reset back to 0
 */
byte apReady(void);

/**
 * @brief Turn on/off the NMOS switch
 * @param flag 0 or 1 for OFF/ON
 * @return 1 if flag make sense
 * @bug can add code to check Vout voltage to ensure on or off, worry about settle time required for VOUT
 */
byte setOutput(byte flag);

/*Source PDO Information acquisition*/
/**
 * @brief: Check if power supply is good and fetch PDO profile 26 byte data in to the read buffer
 */
void fetchPdoInfo(void);

/**
 * @brief: Search through the PDO profile list and look for PPS and AVS. Update the index
 */
void mapPpsAvsInfo(void);

/*PDO info logging*/
/**
 * @brief: Display current range based of max current
 * @param: Maximum current
 */
void displayCurrentRange(byte currentMax);
void displayEprVoltageMin(byte currentMax);
void displaySprVoltageMin(byte currentMax);

/**
 * @brief take in current in mA unit
 * @return value from 0 to 15
 * @return -1 if there is an error
 */
int currentMap(int current);

/**
 * @brief: Decode PDO info from sprEprPdo
 * @param: pdoIndex - index of the PDO profile to be decoded
 */
void displayPdoInfo(byte pdoIndex);

/**
 * @brief: Log all available PDO profile data
 */
void displayProfiles(void);

/*Latest PPS & AVS index tracking*/
/**
 * @brief: Get internal PPS profile index (index start at 1)
 * @return: indexPps
 */
int getPpsIndex(void);

/**
 * @brief: Get internal AVS profile index (index start at 1)
 * @return: indexAvs
 */
int getAvsIndex(void);

/*Source PDO requesting*/
/**
 * @brief: Request fixed PDO voltage. Works for both SPR and EPR mode
 * @param: pdoIndex - profile index (1-13)
 * @param: currentMax - target current in mA
 */
void setFixedPdo(int pdoIndex, int currentMax);

/**
 *@brief: Request PPS voltage
 *@param: pdoIndex - profile index (1-7)
 *@param: targetVolatge - mV
 *@param: currentMax - mA
 *@bug: only work if min PPS voltage is 3.3V
 */
void setPpsPdo(int pdoIndex, int targetVolatge, int currentMax);

/**
 *@brief: Request AVS voltage
 *@param: pdoIndex - profile index (8-13)
 *@param: targetVolatge - mV
 *@param: currentMax - mA
 *@bug: only work if min AVS voltage is 15V, AVS max voltage is not capped at 30V
 */
void setAvsPdo(int pdoIndex, int targetVoltage, int currentMax);

/*USBC-PD Parameter reading and setting*/
/**
 * @brief Set resistance value of 10K NTC at 25C, 50C, 75C and 100C.
 *          Default is 10000, 4161, 1928, 974Ohm
 * @param TR25, TR50, TR75, TR100 unit in Ohm
 * @attention Blocking function due to long I2C write, min blocking time 15ms
 */
void setNTC(qbyte TR25, qbyte TR50, qbyte TR75, qbyte TR100);

/**
 * @brief: Read NTC temperature in 0C
 */
float readTemperature(void);

/**
 * @brief: Read VBUS voltage in Volts
 */
float readVoltage(void);

/*
 * @brief: Read VBUS current in Amperes
 */
float readCurrent(void);

/**
 * @brief: Read latest voltage negotiated with the source in volts
 */
float readVREQ(void);

/**
 * @brief: Read latest current negotiated with the source in amperes
 */
float readIREQ(void);

/**
 * @brief: Read VSELMIN register. The Minimum Selection Voltage in Volts
 */
float readVSELMIN(void);

/**
 * @brief: Set VSELMIN register. The Minimum Selection Voltage
 * @param: voltage - in mV
 */
void setVSELMIN(byte voltage);

/*Threshold reading and setting*/
/**
 * @brief Read UVP Threshold, percentage(%) of VREQ
 * @return percentage, should only return 80%, 75%, or 70%. -1 for error
 */
int readUVPTHR(void);

/**
 * @brief Set UVP Threshold, percentage(%) of VREQ
 * @param value percentage. If 80% then value = 80
 */
void setUVPTHR(int value);

/**
 * @brief: Read OVP Threshold Voltage is the VREQ voltage plus OVPTHR offset voltage(V)
 */
float readOVPTHR(void);

/**
 * @brief: Set OVP Threshold Voltage is the VREQ voltage plus OVPTHR offset voltage (mV)
 * @param: value - in mV
 */
void setOVPTHR(byte value);

/**
 * @brief: Read OCP Threshold Voltage(V)
 */
float readOCPTHR(void);

/**
 * @brief: Set OCP Threshold Voltage(mV)
 * @param: value - in mV
 */
void setOCPTHR(byte value);

/**
 * @brief: Read OTP Threshold Voltage(V)
 */
float readOTPTHR(void);

/**
 * @brief: Set OTP Threshold Voltage(mV)
 * @param: value - in mV
 */
void setOTPTHR(byte value);

/**
 * @brief: Read De-rating Threshold Voltage(V)
 */
float readDRTHR(void);

/**
 * @brief: Set De-rating Threshold Voltage(mV)
 * @param: value - in mV
 */
void setDRTHR(byte value);

#endif /*AP33772S_H*/
