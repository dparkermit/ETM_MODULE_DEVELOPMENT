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


unsigned int global_0;
unsigned int global_1;
unsigned int global_2;
unsigned int global_3;
unsigned int global_4;

unsigned int slots_available;
unsigned int slots_full;

ETMCanMessage message_0;
ETMCanMessage message_1;
ETMCanMessage message_2;
ETMCanMessage message_3;
ETMCanMessage message_4;
ETMCanMessage message_5;


ETMCanMessage *can_message_ptr;

int main(void) {
  unsigned int local_0;
  unsigned int local_1;
  unsigned int n;



  
  global_0 = 0;



  while (1) {

    for (n =0; n<10; n++) {
      ETMCan1RX0BufferAdd();
      slots_available = ETMCan1RX0BufferSlotsAvailable();
      slots_full = ETMCan1RX0BufferNotEmpty();
      global_0 += 0x0100;
    }

    for (n =0; n<10; n++) {
      ETMCan1RX0BufferReadData(&message_0);
      slots_available = ETMCan1RX0BufferSlotsAvailable();
      slots_full = ETMCan1RX0BufferNotEmpty();
      global_0 += 0x0100;
    }
  }
}


