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
  test_message.identifier = ETM_CAN_MSG_LVL_TX;
  global_0 = 0x0000;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = ETM_CAN_MSG_SYNC_TX;
  global_0 = 0x0010;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_STATUS_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0020;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);
  
#define ETM_CAN_REGISTER_CALIBRATION_TEST          0x0400
#define ETM_CAN_REGISTER_LOCAL_TEST                0x0100
  
  test_message.identifier = (ETM_CAN_MSG_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0030;
  test_message.word3 = (ETM_CAN_REGISTER_CALIBRATION_TEST | (ETM_CAN_MY_ADDRESS << 12));
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);
  
  
  test_message.identifier = (ETM_CAN_MSG_REQUEST_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0040;
  test_message.word3 = (ETM_CAN_REGISTER_LOCAL_TEST | (ETM_CAN_MY_ADDRESS << 12));
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_CMD_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0050;
  test_message.word3 = ETM_CAN_REGISTER_DEFAULT_CMD_ENABLE_HIGH_SPEED_DATA_LOGGING;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_LVL_TX);
  global_0 = 0x0060;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = ETM_CAN_MSG_SYNC_TX;
  global_0 = 0x0070;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_STATUS_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0080;
  test_message.word3 = global_0;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3));
  global_0 = 0x0090;
  test_message.word3 = ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  test_message.identifier = (ETM_CAN_MSG_SET_1_TX | ((ETM_CAN_MY_ADDRESS+0) << 3));
  global_0 = 0x00A0;
  test_message.word3 = ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  test_message.identifier = (ETM_CAN_MSG_DATA_LOG_TX | ((ETM_CAN_MY_ADDRESS+0) << 3));
  global_0 = 0x00B0;
  test_message.word3 = ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT;
  test_message.word2 = global_0 + 1;
  test_message.word1 = global_0 + 2;
  test_message.word0 = global_0 + 3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);


  _C1IF = 1;

  global_0 = 0;

  while (1) {

    ETMCanProcessMessage();

    
  }
}


