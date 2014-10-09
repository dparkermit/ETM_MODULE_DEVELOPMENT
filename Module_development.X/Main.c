#include <p30F6014a.h>
#include <libpic30.h>
#include "Main.h"
#include "ETM_CAN_UTILITY.h"
#include "ETM_CAN.h"
#include "ETM_CAN_CONFIG.h"

_FOSC(ECIO & CSW_FSCM_OFF); 
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_2); 
_FBORPOR(PWRT_OFF & BORV_45 & PBOR_OFF & MCLR_EN);
_FBS(WR_PROTECT_BOOT_OFF & NO_BOOT_CODE & NO_BOOT_EEPROM & NO_BOOT_RAM);
_FSS(WR_PROT_SEC_OFF & NO_SEC_CODE & NO_SEC_EEPROM & NO_SEC_RAM);
_FGS(CODE_PROT_OFF);
_FICD(PGD);

extern ETMCanMessageBuffer etm_can_rx_message_buffer;
extern ETMCanMessageBuffer etm_can_tx_message_buffer;


ETMCanMessageBuffer test_dan_1;

ETMCanMessage test_message;

ETMCanMessage test_read_message;

unsigned int global_0 = 0;
unsigned int global_1 = 0;
unsigned int global_2 = 0;

unsigned int slots_available = 0;
unsigned int rows_available = 0;
unsigned int slots_full = 0;


int main(void) {
  unsigned int n;
  
  
  test_dan_1.message_write_index = 0;
  test_dan_1.message_read_index = 0;
  
  global_0 = 0;
  global_1 = 0;
  
  ETMCanInitialize();
  global_0 = C1RXM0SID;
  global_1 = C1RXM1SID;
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

  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  ETMCanReadMessageFromBuffer(&etm_can_tx_message_buffer, &test_read_message);
  global_1 = ETMCanBufferRowsAvailable(&etm_can_tx_message_buffer);
  global_2 = ETMCanBufferNotEmpty(&etm_can_tx_message_buffer);
  global_0 = 0;

  
  for (n =0; n<20; n++) {
    test_message.identifier = n;
    test_message.word0 = 0;
    test_message.word1 = 1;
    test_message.word2 = 2;
    test_message.word3 = 3;

    if (ETMCanBufferRowsAvailable(&test_dan_1)) {
      ETMCanAddMessageToBuffer(&test_dan_1, &test_message);
      global_1 = ETMCanBufferRowsAvailable(&test_dan_1);
      global_2 = ETMCanBufferNotEmpty(&test_dan_1);
    }
    global_0 += 1;
  }
  
  for (n =0; n<20; n++) {
    if (ETMCanBufferNotEmpty(&test_dan_1)) {
      ETMCanReadMessageFromBuffer(&test_dan_1, &test_message);
      global_1 += 1;
    }
  }
  
  while (1) {
    
  }
}


