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


unsigned int unit_counter;
void Do100mSDataLog(void);




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


#define ETM_CAN_REGISTER_CALIBRATION_TEST          0x0400
#define ETM_CAN_REGISTER_LOCAL_TEST                0x0100




#ifdef __ETM_CAN_MASTER_MODULE
  // DPARKER read configuration from EEPROM
  etm_can_my_configuration.agile_number_high_word = 0;
  etm_can_my_configuration.agile_number_low_word  = 34760;
  etm_can_my_configuration.agile_dash             = 000;
  etm_can_my_configuration.agile_rev_ascii        = 'G';

  etm_can_my_configuration.serial_number          = 107;
  etm_can_my_configuration.firmware_branch        = 0;
  etm_can_my_configuration.firmware_major_rev     = 10;
  etm_can_my_configuration.firmware_minor_rev     = 2;
#else
  etm_can_my_configuration.agile_number_high_word = 0;
  etm_can_my_configuration.agile_number_low_word  = 36224;
  etm_can_my_configuration.agile_dash             = 000;
  etm_can_my_configuration.agile_rev_ascii        = 'B';

  etm_can_my_configuration.serial_number          = 102;
  etm_can_my_configuration.firmware_branch        = 0;
  etm_can_my_configuration.firmware_major_rev     = 1;
  etm_can_my_configuration.firmware_minor_rev     = 5;
#endif
  

  
  T2CON = (T2_OFF & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT);
  PR2 = 3906;  // 100mS period
  TMR2 = 0;
  _T2IF = 0;
  _T2IE = 0;
  T2CONbits.TON = 1;


  etm_can_status_register.status_word_0 = 0xFFFF;
  etm_can_status_register.status_word_1 = 0x0000;
  etm_can_status_register.data_word_A = 0x0000;
  etm_can_status_register.data_word_B = 0x0001; 
  
  _LATG14 = 0;
  _TRISG14 = 0;
  
  while (1) {
    

    
    
    ETMCanProcessMessage();
    
#ifdef __ETM_CAN_MASTER_MODULE
    ETMCanProcessLogData();
    ETMCanMaster100msCommunication();
#else
    ETMCanSlaveLog100ms();
#endif
  }
}



void Do100mSDataLog(void) { 


#ifdef __ETM_CAN_MASTER_MODULE

  
  // Send Sync Command
  // DPARKER - Need to write send sync function
  
  
  
  // Send HV OFF Command (this will status register data word B to 0xFFFF)
  test_message.identifier = (ETM_CAN_MSG_CMD_TX | ((ETM_CAN_ADDR_HV_LAMBDA_BOARD) << 3));
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
  test_message.word0 = 0;
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);

  MacroETMCanCheckTXBuffer()  


  /*
    DPARKER, here is the problem to solve
   
    When we write to a buffer that backs up a TX buffer (etm_can_tx_message_buffer, etm_can_rx_data_log_buffer), the data will not get transfered to the TX register unless the ISR is called.
    One work around is to the schedule to ISR after every time we add to the buffer, an alternative is to check and see if the TX register is empty and if it is initiate the transfer
 
  */
  


#else
  

  




  
#endif

}
