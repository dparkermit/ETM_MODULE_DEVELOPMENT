#include <p30F6014a.h>
#include "Main.h"
#include "ETM_SCALE.h"

_FOSC(ECIO & CSW_FSCM_OFF); 
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_2); 
_FBORPOR(PWRT_OFF & BORV_45 & PBOR_OFF & MCLR_EN);
_FBS(WR_PROTECT_BOOT_OFF & NO_BOOT_CODE & NO_BOOT_EEPROM & NO_BOOT_RAM);
_FSS(WR_PROT_SEC_OFF & NO_SEC_CODE & NO_SEC_EEPROM & NO_SEC_RAM);
_FGS(CODE_PROT_OFF);
_FICD(PGD);

int main(void) {
  unsigned int cal_0;
  unsigned int cal_1;
  
  unsigned int scale_0;
  unsigned int scale_1;

  unsigned int result_0;
  unsigned int result_1;

  unsigned int error_count;

  scale_0 = DEC_TO_SCALE_FACTOR(SCALE_FACTOR_0);
  scale_1 = SCALE_INT_1;
  cal_0 = DEC_TO_CAL_FACTOR(CALIBRATION_FACTOR_0);
  cal_1 = DEC_TO_CAL_FACTOR(1.11231212);


  result_0 = 0x1100;
  result_0 = EtmScaleFactor16(result_0, scale_0);
  result_1 = 0x1100;
  result_1 = EtmScaleFactor16(result_1, scale_1);
  
  error_count = saturation_etmscalefactor2_count + saturation_etmscalefactor16_count;

  while (1) {
    
  }
}


