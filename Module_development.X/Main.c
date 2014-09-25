#include <p30F6014a.h>
#include "Main.h"
#include "ETM_SCALE.h"
#include "ETM_CAN_UTILITY.h"

_FOSC(ECIO & CSW_FSCM_OFF); 
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_2); 
_FBORPOR(PWRT_OFF & BORV_45 & PBOR_OFF & MCLR_EN);
_FBS(WR_PROTECT_BOOT_OFF & NO_BOOT_CODE & NO_BOOT_EEPROM & NO_BOOT_RAM);
_FSS(WR_PROT_SEC_OFF & NO_SEC_CODE & NO_SEC_EEPROM & NO_SEC_RAM);
_FGS(CODE_PROT_OFF);
_FICD(PGD);



ETMCanMessageBuffer test_dan_1;

ETMCanMessage test_message;

unsigned int global_0 = 0;
unsigned int global_1 = 0;

unsigned int slots_available = 0;
unsigned int rows_available = 0;
unsigned int slots_full = 0;


int main(void) {
  unsigned int n;
  
  
  test_dan_1.message_write_index = 0;
  test_dan_1.message_read_index = 0;
  
  global_0 = 0;
  global_1 = 0;
  
  
  for (n =0; n<20; n++) {
    if (ETMCanBufferRowsAvailable(&test_dan_1)) {
      ETMCanBufferAdd(&test_dan_1, &C2RX0SID);
      global_0 += 1;
    }
  }
  
  for (n =0; n<20; n++) {
    if (ETMCanBufferNotEmpty(&test_dan_1)) {
      ETMCanBufferRead(&test_dan_1, &test_message);
      global_1 += 1;
    }
  }
  
  while (1) {
    
  }
}


