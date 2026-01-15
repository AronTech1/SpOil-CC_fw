/*
 * ap33772s.c
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */
// For practice purpose


#include "ap33772s.h"

static int indexPpsUser = -1; // for getPpsIndex();
static int indexAvsUser = -1; // for getAvsIndex();

//For timer;
static int voltageAvsByte;
static int currentAvsByte;
static int indexAvs;


sprEprPdo_t sprEprArray[MAX_PDO_ENTRIES] = {0};
eventFlag_t event_flag = {0};
rdoData_t rdoData = {0};


byte apReady(void){
  i2cApRead(AP_ADRS, CMD_STATUS, 1); // CMD: Read Status into read buff
	 uint8_t sts = readBuf[0] & 0b00000111; //Isolate bit 0 to 2
	 if(sts == 0b111) // NEWPDO = 1, I2C READY = 1, STARTED = 1;
	 {
		 HAL_Delay(100); //delay 100ms
#ifdef LOG_EN
		 logString("AP READY!/r/n");
#endif
//	         i2cApRead(AP_ADRS, CMD_SRCPDO, 26);
		 return 1;
	 }
	 else{
		 HAL_Delay(100);
#ifdef LOG_EN
		 logString("AP NOT READY YET!/r/n");
#endif
		 return 0;
	 }
}

void fetchPdoInfo(void){
	byte readyFlag = apReady();
  if(readyFlag){
  HAL_Delay(100);
  i2cApRead(AP_ADRS, CMD_SRCPDO, 26);
  for (byte i = 0; i < 26; i+=2){
      byte pdoIndex = (i/2);

      sprEprArray[pdoIndex].byte0 = readBuf[i];
      sprEprArray[pdoIndex].byte1 = readBuf[i+1];
  }
  mapPpsAvsInfo();
  }
}

void mapPpsAvsInfo(void){
  for(byte i = 1; i <= 13; i++){
	 if(i < 8 && sprEprArray[i-1].pps.type == 1){
#ifdef LOG_EN
		 logString("PPS PROFILE FOUND");
#endif
		 indexPpsUser = i;
	 }
	 else if(i >= 8 && sprEprArray[i-1].avs.type == 1){
#ifdef LOG_EN
		 logString("AVS PROFILE FOUND");
#endif
		 indexAvsUser = i;
	 }
  }
}

void displayPdoInfo(byte pdoIndex){
	byte vMax = (sprEprArray[pdoIndex].fixed.voltage_max * (200/1000));
	/*Identify PDO (EPR or SPR)*/
	byte isEPR;
	  if(pdoIndex >= 7 && pdoIndex <= 12){
		isEPR = 1;
	  }
	  else{
		isEPR = 0;
	  }

	/*Check if the array is empty. Return if empty*/
	  if (sprEprArray[pdoIndex].byte0 == 0 && sprEprArray[pdoIndex].byte1 == 0) {
		return;  // If both bytes are zero, exit the function
	  }

	  if(pdoIndex <= 7){
#ifdef LOG_EN
		 logString("SRC_SPR_PDO");
		 logInteger(pdoIndex + 1);
		 logString(":/r/n");
#endif
	  }
	  else {
#ifdef LOG_EN
		 logString("SRC_EPR_PDO");
		 logInteger(pdoIndex + 1);
		 logString(":/r/n");
#endif
	  }

	if(sprEprArray[pdoIndex].fixed.type == 0){
	  logString("Fixed PDO:");

	  if(isEPR == 1){
#ifdef LOG_EN
		  logInteger(vMax);
		  logString("V/r/n");
#endif
		  displayCurrentRange(sprEprArray[pdoIndex].fixed.current_max);
	  }
	else{

	  if(isEPR == 1){
#ifdef LOG_EN
		logString("AVS PDO:");
#endif
	  }
	  else if (isEPR == 0){
#ifdef LOG_EN
		logString("PPS PDO:");
#endif
	  }

	  if(isEPR == 1){
		displayEprVoltageMin(sprEprArray[pdoIndex].avs.voltage_min);
	  }
	  else if (isEPR == 0){
		displaySprVoltageMin(sprEprArray[pdoIndex].pps.voltage_min);
	  }
#ifdef LOG_EN
	  logInteger(vMax);
	  logString("V/r/n");
#endif
	  displayCurrentRange(sprEprArray[pdoIndex].fixed.current_max);
	}
  }
}

void displayProfiles(void){
#ifdef LOG_EN
	logString("Profile list detected:");
#endif
	for(byte i = 0; i < 26; i += 2 ){
		int pdoIndex = (i/2);
		displayPdoInfo(pdoIndex);
	}
}


void displayCurrentRange(byte currentMax){
   switch(currentMax){
   case 0:
#ifdef LOG_EN
      logString("0.00A ~ 1.24A (Less than)");
#endif
      break;

   case 1:
#ifdef LOG_EN
	  logString("1.25A ~ 1.49A");
#endif
      break;

   case 2:
#ifdef LOG_EN
	   logString("1.50A ~ 1.74A");
#endif
      break;

   case 3:
#ifdef LOG_EN
	  logString("1.75A ~ 1.99A");
#endif
      break;

   case 4:
#ifdef LOG_EN
	  logString("2.00A ~ 2.24A");
#endif
      break;

   case 5:
#ifdef LOG_EN
	  logString("2.25A ~ 2.49A");
#endif
      break;

   case 6:
#ifdef LOG_EN
	  logString("2.50A ~ 2.74A");
#endif
      break;

   case 7:
#ifdef LOG_EN
	  logString("2.75A ~ 2.99A");
#endif
      break;

   case 8:
#ifdef LOG_EN
	  logString("3.00A ~ 3.24A");
#endif
      break;

   case 9:
#ifdef LOG_EN
	  logString("3.25A ~ 3.49A");
#endif
      break;

   case 10:
#ifdef LOG_EN
	  logString("3.50A ~ 3.74A");
#endif
      break;

   case 11:
#ifdef LOG_EN
      logString("3.75A ~ 3.99A");
#endif
      break;

   case 12:
#ifdef LOG_EN
	  logString("4.00A ~ 4.24A");
#endif
      break;

   case 13:
#ifdef LOG_EN
     logString("4.25A ~ 4.49A");
#endif
      break;

   case 14:
#ifdef LOG_EN
	  logString("4.50A ~ 4.99A");
#endif
      break;

   case 15:
#ifdef LOG_EN
	  logString("5.00A ~ (More than)");
#endif
      break;

   default:
#ifdef LOG_EN
     logString("Invalid value");
#endif
	 break;
   }
}


void displayEprVoltageMin(byte currentMax){
	switch(currentMax){
	case 0:
#ifdef LOG_EN
	   logString("Reserved");
#endif
	   break;

	case 1:
#ifdef LOG_EN
	   logString("15V~");
#endif
	   break;

	case 2:
#ifdef LOG_EN
	   logString("15V < VOLTAGE_MIN ≤ 20V ");
#endif
	   break;

	case 3:
#ifdef LOG_EN
	   logString("Others");
#endif
	   break;

	default:
#ifdef LOG_EN
	   logString("Invalid value");
#endif
	   break;
	}
}

void displaySprVoltageMin(byte currentMax){
	switch(currentMax){
	case 0:
#ifdef LOG_EN
	   logString("Reserved");
#endif
	   break;

	case 1:
#ifdef LOG_EN
	   logString("3V3");
#endif
	   break;

	case 2:
#ifdef LOG_EN
	   logString("3V3 < VOLTAGE_MIN ≤ 5V");
#endif
	   break;

	case 3:
#ifdef LOG_EN
	   logString("Others");
#endif
	   break;

	default:
#ifdef LOG_EN
	   logString("Invalid value");
#endif
	   break;
	}
}

int getPpsIndex(void)
{
  return indexPpsUser;
}

int getAvsIndex(void)
{
  return indexAvsUser;
}

int currentMap(int current)
{
  // Check if the value is out of bounds
  if (current < 0 || current > 5000) {
      return -1; // Return -1 for invalid inputs
  }

  // If value is below 1250, return 0
  if (current < 1250) {
      return 0;
  }

  // Calculate the result for ranges above 1250
  return ((current - 1250) / 250) + 1;
}

void setFixedPdo(int pdoIndex, int currentMax){
//	rdoData_t rdoData = {0};

	if(currentMax <= 0){
#ifdef LOG_EN
		logString("Invalid arguments for fixed");
#endif
		return;
	}

	if(sprEprArray[pdoIndex-1].fixed.type == 0){
#ifdef LOG_EN
	  logString("FIXED TYPE/r/n");
#endif
	  rdoData.reqMsg.pdoIndex = pdoIndex;

	  if(currentMax > sprEprArray[pdoIndex-1].fixed.current_max){
		currentMax = sprEprArray[pdoIndex-1].fixed.current_max;
#ifdef LOG_EN
		logString("Preferred fixed current out of range but set to max available current/r/n");
#endif
//		return;
	  }

	  rdoData.reqMsg.currentSel = currentMap(currentMax);

	  writeBuf[0] = rdoData.byte0;
	  writeBuf[1] = rdoData.byte1;

	  i2cApWrite(AP_ADRS, CMD_PD_REQMSG, writeBuf, sizeof(writeBuf));
	}
	return;
}


void setPpsPdo(int pdoIndex, int targetVoltage, int currentMax){
//    rdoData_t rdoData = {0};
    int voltMinDecoded = 0;

    if(currentMax <= 0 || pdoIndex >= 8){
#ifdef LOG_EN
    	logString("Invalid PPS arguments");
#endif
    	return;
    }

    if(sprEprArray[pdoIndex-1].pps.type == 1){
#ifdef LOG_EN
        logString("PPS TYPE/r/n");
#endif
        rdoData.reqMsg.pdoIndex = pdoIndex;

        if(currentMap(currentMax) > sprEprArray[pdoIndex-1].pps.current_max ){
        	currentMax = sprEprArray[pdoIndex-1].pps.current_max;
#ifdef LOG_EN
        	logString("Preferred PPS current out of range but set to max available current/r/n");
#endif
//        	return:
        }

        if(sprEprArray[pdoIndex-1].pps.voltage_min > 0) voltMinDecoded = 3300;
        if(targetVoltage < voltMinDecoded || targetVoltage > sprEprArray[pdoIndex].pps.voltage_max*100){
        	targetVoltage = sprEprArray[pdoIndex].pps.voltage_max*100;
#ifdef LOG_EN
        	logString("PPS voltage not in range but set to max available voltage/r/n");
#endif
//        	return;
        }

        rdoData.reqMsg.voltageSel = targetVoltage/100;
        rdoData.reqMsg.currentSel = currentMap(currentMax);

		writeBuf[0] = rdoData.byte0;
		writeBuf[1] = rdoData.byte1;

		i2cApWrite(AP_ADRS, CMD_PD_REQMSG, writeBuf, 2);
    }
    return;
}

void setAvsPdo(int pdoIndex, int targetVoltage, int currentMax){
//    rdoData_t rdoData = {0};
    int voltMinDecoded = 0;

    if(currentMax <= 0 || pdoIndex < 8){
#ifdef LOG_EN
    	logString("Invalid AVS arguments");
#endif
    	return;
    }

    if(sprEprArray[pdoIndex-1].avs.type == 1){
#ifdef LOG_EN
        logString("AVS TYPE/r/n");
#endif
        rdoData.reqMsg.pdoIndex = pdoIndex;

        if(currentMap(currentMax) > sprEprArray[pdoIndex-1].avs.current_max ){
        	currentMax = sprEprArray[pdoIndex-1].avs.current_max;
#ifdef LOG_EN
        	logString("Preferred PPS current out of range but set to max available current/r/n");
#endif
//        	return:
        }

        if(sprEprArray[pdoIndex-1].avs.voltage_min > 0) voltMinDecoded = 15000;
        if(targetVoltage < voltMinDecoded || targetVoltage > sprEprArray[pdoIndex].avs.voltage_max*200){
        	targetVoltage = sprEprArray[pdoIndex].avs.voltage_max*200;
#ifdef LOG_EN
        	logString("PPS voltage not in range but set to max available voltage/r/n");
#endif
//        	return;
        }

        rdoData.reqMsg.voltageSel = targetVoltage/200;
        rdoData.reqMsg.currentSel = currentMap(currentMax);

		writeBuf[0] = rdoData.byte0;
		writeBuf[1] = rdoData.byte1;

		i2cApWrite(AP_ADRS, CMD_PD_REQMSG, writeBuf, 2);

		indexAvs = rdoData.reqMsg.pdoIndex;
		voltageAvsByte = rdoData.reqMsg.voltageSel;
		currentAvsByte = rdoData.reqMsg.currentSel;
    }
    return;
}

void setNTC(qbyte TR25, qbyte TR50, qbyte TR75, qbyte TR100)
{
  writeBuf[0] = TR25 & 0xff;
  writeBuf[1] = (TR25 >> 8) & 0xff;
  i2cApWrite(AP_ADRS, CMD_TR25, writeBuf, 2);

  HAL_Delay(5);

  writeBuf[0] = TR50 & 0xff;
  writeBuf[1] = (TR50 >> 8) & 0xff;
  i2cApWrite(AP_ADRS, CMD_TR50, writeBuf, 2);

  HAL_Delay(5);

  writeBuf[0] = TR75 & 0xff;
  writeBuf[1] = (TR75 >> 8) & 0xff;
  i2cApWrite(AP_ADRS, CMD_TR75, writeBuf, 2);

  HAL_Delay(5);

  writeBuf[0] = TR100 & 0xff;
  writeBuf[1] = (TR100 >> 8) & 0xff;
  i2cApWrite(AP_ADRS, CMD_TR100, writeBuf, 2);
}

float readTemperature(void){
    i2cApRead(AP_ADRS, CMD_TEMP, 1);
    return readBuf[0];
}

float readVoltage(void){
	i2cApRead(AP_ADRS, CMD_VOLTAGE, 1);
	return (readBuf[0]/1000);
}

float readCurrent(void){
	i2cApRead(AP_ADRS, CMD_CURRENT, 1);
    return (readBuf[0]*(24/1000));
}

float readVREQ(void){
	i2cApRead(AP_ADRS, CMD_VREQ, 1);
	return (readBuf[0]*(50/1000));
}

float readIREQ(void){
	i2cApRead(AP_ADRS, CMD_IREQ, 1);
	return (readBuf[0]*(10/1000));
}


float readVSELMIN(void){
	i2cApRead(AP_ADRS, CMD_VSELMIN, 1);
	return (readBuf[0]*(200/1000));
}

void setVSELMIN(byte voltage){
	writeBuf[0] = (voltage/200);
	i2cApWrite(AP_ADRS, CMD_VSELMIN, writeBuf, 1);
}

int readUVPTHR(void)
{
  i2cApRead(AP_ADRS, CMD_UVPTHR, 1);
  switch(readBuf[0])
  {
    case 1:
      return 80;
    case 2:
      return 75;
    case 3:
      return 70;
  }
  return -1;
}

void setUVPTHR(int value)
{
  if(value >= 70 && value <= 80)
  {
    switch(value)
    {
      case 80:
        writeBuf[0] = 1;
        i2cApWrite(AP_ADRS, CMD_UVPTHR, writeBuf, 1);
        break;
      case 75:
        writeBuf[0] = 2;
        i2cApWrite(AP_ADRS, CMD_UVPTHR, writeBuf, 1);
        break;
      case 70:
        writeBuf[0] = 3;
        i2cApWrite(AP_ADRS, CMD_UVPTHR, writeBuf, 1);
        break;
      default:
        return; // Error
    }
  }
  else return;
}

float readOVPTHR(void){
	i2cApRead(AP_ADRS, CMD_OVPTHR, 1);
	return (readBuf[0]*(80/1000));
}

void setOVPTHR(byte value){
	writeBuf[0] = (value/80);
	i2cApWrite(AP_ADRS, CMD_OVPTHR, writeBuf, 1);
}

float readOCPTHR(void){
	i2cApRead(AP_ADRS, CMD_OCPTHR, 1);
	return (readBuf[0]*(50/1000));
}

void setOCPTHR(byte value){
	writeBuf[0] = (value/50);
	i2cApWrite(AP_ADRS, CMD_OCPTHR, writeBuf, 1);
}

float readOTPTHR(void){
	i2cApRead(AP_ADRS, CMD_OTPTHR, 1);
	return (readBuf[0]/1000);
}

void setOTPTHR(byte value){
	writeBuf[0] = value;
	i2cApWrite(AP_ADRS, CMD_OTPTHR, writeBuf, 1);
}

float readDRTHR(void){
	i2cApRead(AP_ADRS, CMD_DRTHR, 1);
	return readBuf[0];
}


void setDRTHR(byte value){
	writeBuf[0] = value;
	i2cApWrite(AP_ADRS, CMD_DRTHR, writeBuf, 1);
}

byte setOutput(byte flag){
    switch(flag){
        case 0:
            writeBuf[0] = 0b00010001; //turn off
            i2cApWrite(AP_ADRS, CMD_SYSTEM, writeBuf, 1);
            return 1;
            break; //Sanity
        case 1:
            writeBuf[0] = 0b00010010; //turn on
            i2cApWrite(AP_ADRS, CMD_SYSTEM, writeBuf, 1);
            return 1;
            break; //Sanity
        default:
            return 0; //Error, dont know the input
    }
}
