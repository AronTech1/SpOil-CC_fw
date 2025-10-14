/*
 * bq25790.h
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#ifndef BQ25790_H
#define BQ25790_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "i2c.h"
#include "log.h"

#define BQ_ADRS		0x6B /* 7-bit addressing only */


//static qbyte targIntrMsk;

/*A standard technique in programming for creating bit flags (also called bitmasks or enum flags). Each value represents a single, unique bit position in an 8-bit (or larger) integer*/
enum bq25790IntrMask{
	/*REG28_Charger_Mask_0*/
	IINDPM_MASK = 0x01,
	VINDPM_MASK = 0x02,
	WD_MASK = 0x04,
	POORSRC_MASK = 0x08,
	PG_MASK = 0x10,
	AC2_PRESENT_MASK = 0x20,
	AC1_PRESENT_MASK = 0x40,
	VBUS_PRESENT_MASK = 0x80,
	/*REG29_Charger_Mask_1*/
	CHG_MASK = 0x100,
	ICO_MASK = 0x200,
	VBUS_MASK = 0x400,
	TREG_MASK = 0x800,
	VBAT_PRESENT_MASK = 0x1000,
	BC1_2_DONE_MASK = 0x2000,
	/*REG2A_Charger_Mask_2 */
	DPDM_DONE_MASK = 0x4000,
	ADC_DONE_MASK = 0x8000,
	VSYS_MASK = 0x10000,
	CHG_TMR_MASK = 0x20000,
	TRICHG_TMR_MASK = 0x40000,
	PRECHG_TMR_MASK = 0x80000,
	TOPOFF_TMR_MASK = 0x100000,
	/*REG2B_Charger_Mask_3*/
	VBATOTG_LOW_MASK = 0x200000,
	TS_COLD_MASK = 0x400000,
	TS_COOL_MASK = 0x800000,
	TS_WARM_MASK = 0x1000000,
	TS_HOT_MASK = 0x2000000,
	/*REG2C_FAULT_Mask_0*/
	IBAT_REG_MASK = 0x4000000,
	VBUS_OVP_MASK = 0x8000000,
	VBAT_OVP_MASK = 0x10000000,
	IBUS_OCP_MASK = 0x20000000,
	IBAT_OCP_MASK = 0x40000000,
	CONV_OCP_MASK = 0x80000000,
	VAC2_OVP_MASK = 0x100000000,
	VAC1_OVP_MASK = 0x200000000,
    /*REG2D_FAULT_Mask_1*/
	VSYS_SHORT_MASK = 0x400000000,
	VSYS_OVP_MASK = 0x800000000,
	OTG_OVP_MASK = 0x1000000000,
	OTG_UVP_MASK = 0x2000000000,
	TSHUT_MASK = 0x4000000000,
};

enum bq25790Watchdog{
	WDT_DISABLE = 0,
	WDT_0_5_SEC,
	WDT_1_SEC,
	WDT_2_SEC,
	WDT_20_SEC,
	WDT_DEFAULT,
	WDT_80_SEC,
	WDT_160_SEC
};

enum bq25790FastChargeThreshold{
	FC_THRESHOLD_15PRCNT = 0,
	FC_THRESHOLD_62_2PCRNT,
	FC_THRESHOLD_66_7_PRCNT,
	FC_THRESHOLD_71_4_PRNCT
};

struct bq25790PartInfo{
	/*REG48_Part_Information*/
	byte dvc_rvsn : 3;
	byte dvc_pn : 3;
};

struct bq25790Status1_3{
   /*REG1B_Charger_Status_0*/
   qbyte vbus_present_stat : 1;
   qbyte ac1_present_stat : 1;
   qbyte ac2_present_stat : 1;
   qbyte pg_stat : 1;
   qbyte poorsrc_stat : 1;
   qbyte wd_stat : 1;
   qbyte vindpm_stat : 1;
   qbyte iindpm_stat : 1;
   /*REG1C_Charger_Status_1*/
   qbyte bc1_2_done_stat : 1;
   qbyte vbus_stat : 4;
   qbyte chg_stat_2 : 3;
   /*REG1D_Charger_Status_2 */
   qbyte vbat_present_stat : 1;
   qbyte dpdm_stat : 1;
   qbyte treg_stat : 1;
   qbyte ico_stat : 2;
   /*REG1E_Charger_Status_3*/
   qbyte prechg_tmr_stat : 1;
   qbyte trichg_tmr_stat : 1;
   qbyte chg_tmr_stat : 1;
   qbyte vsys_stat : 1;
   qbyte adc_done_stat : 1;
   qbyte acrb1_stat : 1;
   qbyte acrb2_stat : 1;
};

struct bq25790Status4{
   /*REG1F_Charger_Status_4*/
	byte ts_hot_stat : 1;
	byte ts_warm_stat : 1;
	byte ts_cool_stat : 1;
	byte ts_cold_stat: 1;
	byte vbatotg_low_stat : 1;
};

struct bq25790StatusFault{
	/*REG20_FAULT_Status_0*/
	dbyte vac1_ovp_stat : 1;
	dbyte vac2_ovp_stat : 1;
	dbyte conv_ocp_stat : 1;
	dbyte ibat_ocp_stat : 1;
	dbyte ibus_ocp_stat : 1;
	dbyte vbat_ovp_stat : 1;
	dbyte vbus_ovp_stat : 1;
	dbyte ibat_reg_stat : 1;
	/*REG21_FAULT_Status_1 */
	dbyte thut_stat : 1;
	dbyte otg_uvp_stat : 1;
	dbyte otg_ovp_stat : 1;
	dbyte vsys_ovp_stat : 1;
	dbyte vsys_short_stat : 1;
};

static HAL_StatusTypeDef bqWriteReg(byte regAdrss, byte data, byte len);
static HAL_StatusTypeDef bqReadReg(byte regAdrss, byte *p, byte len);

static void bqSetReg(uint8_t regAdrss, uint8_t bit, uint8_t mask, uint8_t val);

void bqRprtPartInfo(struct bq25790PartInfo *p);
void bqReset(void);

void bqEnOrMaskInterrupt(qbyte targIntMsk, qbyte bqIntMsk, byte intrMskReg, byte startBit, byte enOrDis);
void bqSetWatchdogTimer(enum bq25790Watchdog opt);

void bqReadStatus1_3(struct bq25790Status1_3 *p);
void bqReadTempStatus(struct bq25790Status4 *q);
void bqReadStatusFault(struct bq25790StatusFault *r);

void bqRprtStsSftyTmrExp(struct bq25790Status1_3 *p);
void bqRprtStsTempRng(struct bq25790Status4 *p);

void bqSetMinimalSystemVoltageLimit(qbyte volts);
void bqSetChargeVoltageLimit(qbyte volts);
void bqSetChargeCurrentLimit(qbyte amperes);
void bqSetInputVoltageLimit(qbyte volts);
void bqSetInputCurrentLimit(qbyte amperes);

void bqSetFastChargeVoltageThreshold(enum bq25790FastChargeThreshold opt);
void bqSetPrechargeCurrentLimit(qbyte amperes);
void bqSetTerminationCurrent(qbyte mAmps);
void bqSetRechargeThreshold(byte volts);

int16_t bqReadIBus(void);
int16_t bqReadVBus(void);
int16_t bqReadIBat(void);
int16_t bqReadVBat(void);
int16_t bqReadVSys(void);

#endif /* BQ25790_H */
