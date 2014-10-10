#include <p30F6014a.h>
#include <libpic30.h>
//#include "Main.h"
#include "ETM_CAN.h"

_FOSC(ECIO & CSW_FSCM_OFF); 
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_2); 
_FBORPOR(PWRT_OFF & BORV_45 & PBOR_OFF & MCLR_EN);
_FBS(WR_PROTECT_BOOT_OFF & NO_BOOT_CODE & NO_BOOT_EEPROM & NO_BOOT_RAM);
_FSS(WR_PROT_SEC_OFF & NO_SEC_CODE & NO_SEC_EEPROM & NO_SEC_RAM);
_FGS(CODE_PROT_OFF);
_FICD(PGD);




ETMCanMessage test_message;

ETMCanMessage test_read_message;

unsigned int global_0 = 0;
unsigned int global_1 = 0;
unsigned int global_2 = 0;

unsigned int slots_available = 0;
unsigned int rows_available = 0;
unsigned int slots_full = 0;


int main(void) {
  
  global_0 = 0;
  global_1 = 0;
  
  ETMCanInitialize();

  test_message.identifier = (ETM_CAN_CMD_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0;
  test_message.word1 = 1;
  test_message.word2 = 2;
  test_message.word3 = 3;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  test_message.identifier = (ETM_CAN_CMD_REQUEST_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x10;
  test_message.word1 = 0x11;
  test_message.word2 = 0x12;
  test_message.word3 = 0x13;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  test_message.identifier = (ETM_CAN_CMD_CMD_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x20;
  test_message.word1 = 0x21;
  test_message.word2 = 0x22;
  test_message.word3 = 0x23;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);



  test_message.identifier = (ETM_CAN_CMD_LVL_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x30;
  test_message.word1 = 0x31;
  test_message.word2 = 0x32;
  test_message.word3 = 0x33;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_CMD_SYNC_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x40;
  test_message.word1 = 0x41;
  test_message.word2 = 0x42;
  test_message.word3 = 0x43;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);



  test_message.identifier = (ETM_CAN_CMD_STATUS_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x50;
  test_message.word1 = 0x51;
  test_message.word2 = 0x52;
  test_message.word3 = 0x53;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_CMD_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x60;
  test_message.word1 = 0x61;
  test_message.word2 = 0x62;
  test_message.word3 = 0x63;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_CMD_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3));
  test_message.word0 = 0x70;
  test_message.word1 = 0x71;
  test_message.word2 = 0x72;
  test_message.word3 = 0x73;

  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  _C1IF = 1;

  global_0 = 0;

  while (1) {
    
  }
}


