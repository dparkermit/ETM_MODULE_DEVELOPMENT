#include <p30F6014a.h>
#include <libpic30.h>
//#include "Main.h"
#include "ETM_CAN.h"
#include "timer.h"

_FOSC(ECIO & CSW_FSCM_OFF); 
_FWDT(WDT_OFF & WDTPSA_1 & WDTPSB_2); 
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
  unsigned int loop_counter = 0;
  global_0 = 0;
  global_1 = 0;
  
  ETMCanInitialize();


#define ETM_CAN_REGISTER_CALIBRATION_TEST          0x0400
#define ETM_CAN_REGISTER_LOCAL_TEST                0x0100


  /*

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

  */


  T2CON = (T2_OFF & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT);
  PR2 = 39062;  // 1 Second period
  TMR2 = 0;
  _T2IF = 0;
  _T2IE = 0;
  T2CONbits.TON = 1;
  

  etm_can_status_register.status_word_0 = 0x0000;
  etm_can_status_register.status_word_0 = 0xFFFF;
  etm_can_status_register.data_word_A = 0x0000;
  etm_can_status_register.data_word_B = 0x0001; 
  
  _LATG14 = 0;
  _TRISG14 = 0;
  
  /*
#ifdef __ETM_CAN_MASTER_MODULE

  test_message.identifier = (ETM_CAN_MSG_STATUS_TX | (ETM_CAN_ADDR_HV_LAMBDA_BOARD << 3));
  test_message.word3 = 0x0;
  test_message.word2 = 0x1;
  test_message.word1 = 0x2;
  test_message.word0 = 0x3;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  test_message.identifier = (ETM_CAN_MSG_STATUS_TX | (ETM_CAN_ADDR_HV_LAMBDA_BOARD << 3));
  test_message.word3 = 0x10;
  test_message.word2 = 0x11;
  test_message.word1 = 0x12;
  test_message.word0 = 0x13;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  test_message.identifier = (ETM_CAN_MSG_STATUS_TX | (ETM_CAN_ADDR_HV_LAMBDA_BOARD << 3));
  test_message.word3 = 0x20;
  test_message.word2 = 0x21;
  test_message.word1 = 0x22;
  test_message.word0 = 0x23;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  _C2IF = 1;

#endif
  */
  while (1) {
    if (_T2IF) {
      // should happen once a second
      if (_LATG14) {
	_LATG14 = 0;
      } else {
	_LATG14 = 1;
      }
      _T2IF = 0;
      loop_counter++;
      
#ifdef __ETM_CAN_MASTER_MODULE
      // Send Sync Command
      // DPARKER - Need to write send sync function

    

      // Send HV OFF Command (this will status register data word B to 0xFFFF)
      test_message.identifier = (ETM_CAN_MSG_DATA_LOG_TX | ((ETM_CAN_ADDR_HV_LAMBDA_BOARD) << 3));
      test_message.word3 = ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_OFF;
      test_message.word2 = 0;
      test_message.word1 = 0;
      test_message.word0 = 0;
      ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

      // Send Set Value
      test_message.identifier = (ETM_CAN_MSG_SET_1_TX | (ETM_CAN_ADDR_HV_LAMBDA_BOARD << 3));
      test_message.word3 = ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT;
      test_message.word2 = 0;
      test_message.word1 = 0;
      test_message.word0 = loop_counter;
      ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);
      

      _C2IF = 1;
    
#else


      // The slave module needs to send out a status command and log data
      ETMCanSendStatus();
      
      // This will send out the first for log data commands
      
      etm_can_system_debug_data.debug_0 = etm_can_tx_message_buffer.message_write_count;
      etm_can_system_debug_data.debug_1 = etm_can_tx_message_buffer.message_overwrite_count;
      etm_can_system_debug_data.debug_2 = etm_can_rx_message_buffer.message_write_count;
      etm_can_system_debug_data.debug_3 = etm_can_rx_message_buffer.message_overwrite_count;

      ETMCanLogDefaultDebug();
      //ETMCanLogData(0x2, 0, 1, 2, 3);
      //ETMCanLogData(0x3, 4, 5, 6, 7);


#endif
    }
    
    ETMCanProcessMessage();

#ifdef __ETM_CAN_MASTER_MODULE
    ETMCanProcessLogData();
#endif

  }
}


