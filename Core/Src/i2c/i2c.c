/*
 * i2c.c
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#include "i2c.h"

byte readBuf[READ_BUFF_LEN] = {0};
byte writeBuf[WRITE_BUFF_LEN] = {0};

void clearWriteBuf(void){
	for (byte i = 0; i < WRITE_BUFF_LEN; i++)
	{
		writeBuf[i] = 0;
	}
}

void clearReadBuf(void){
	for (byte i = 0; i < READ_BUFF_LEN; i++)
	{
		readBuf[i] = 0;
	}
}

byte* i2cConcatenate(byte cmdOrRegAdrss, byte* data, byte dlen){
   if((dlen+1) > WRITE_BUFF_LEN){
#ifdef LOG_EN
	   logString("CONCATENATE: WRITE BUFFER LENGTH TOO SMALL/r/n");
#endif
	   return NULL;
   }

   writeBuf[0] = cmdOrRegAdrss;

   for(byte i = 0; i < dlen; i++){
	   writeBuf[i+1] = data[i];
   }

   return writeBuf;
}

HAL_StatusTypeDef  i2cApWrite(byte slvAdrss, byte cmd, byte* data, byte dlen){
    /*Prepare the payload using the global writeBuf*/
	byte* payLoad = i2cConcatenate(cmd, data, dlen);
  if(HAL_I2C_Master_Transmit(&hi2c1, slvAdrss, payLoad, sizeof(payLoad), I2C_TO) == HAL_OK){

	  clearWriteBuf();
	  return HAL_OK;
  }
  else{
#ifdef LOG_EN
      logString("AP WRITE: WRITE FAILURE/r/n");
#endif
	  return HAL_ERROR;
  }
}

HAL_StatusTypeDef  i2cApRead(byte slvAdrss, byte cmd, dbyte len){
   clearReadBuf();
   if((HAL_I2C_Master_Transmit(&hi2c1, slvAdrss, &cmd, sizeof(cmd), I2C_TO) == HAL_OK) && (HAL_I2C_Master_Receive(&hi2c1, slvAdrss, readBuf, len, I2C_TO) == HAL_OK)){
	   return HAL_OK;
   }
   else{
 #ifdef LOG_EN
       logString("AP READ: READ FAILURE/r/n");
 #endif
 	   return HAL_ERROR;
   }
}

HAL_StatusTypeDef  i2cBqWrite(byte slvAdrss, byte regAdrss, byte* data, byte dlen){
    /*Prepare the payload using the global writeBuf*/
	byte* payLoad = i2cConcatenate(regAdrss, data, dlen);
  if(HAL_I2C_Master_Transmit(&hi2c1, slvAdrss, payLoad, sizeof(payLoad), I2C_TO) == HAL_OK){

	  clearWriteBuf();
	  return HAL_OK;
  }
  else{
#ifdef LOG_EN
      logString("BQ WRITE: WRITE FAILURE/r/n");
#endif
	  return HAL_ERROR;
  }
}

HAL_StatusTypeDef  i2cBqRead(byte slvAdrss, byte regAdrss, byte *p, dbyte len){
   clearReadBuf();
   if((HAL_I2C_Master_Transmit(&hi2c1, slvAdrss, &regAdrss, sizeof(regAdrss), I2C_TO) == HAL_OK) && (HAL_I2C_Master_Receive(&hi2c1, slvAdrss, p, len, I2C_TO) == HAL_OK)){
	   return HAL_OK;
   }
   else{
 #ifdef LOG_EN
       logString("BQ READ: READ FAILURE/r/n");
 #endif
 	   return HAL_ERROR;
   }
}
