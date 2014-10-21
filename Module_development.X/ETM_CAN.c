#include <p30Fxxxx.h>
#include "ETM_CAN.h"


//#define __USE_CAN_1

#ifdef __USE_CAN_1

#define _CXIE                       _C1IE
#define _CXIF                       _C1IF
#define _CXIP                       _C1IP

#define CXTX0CON                    C1TX0CON
#define CXTX1CON                    C1TX1CON
#define CXINTF                      C1INTF

//
#define CXCTRL                      C1CTRL
#define CXCFG1                      C1CFG1
#define CXCFG2                      C1CFG2
#define CXRXM0SID                   C1RXM0SID
#define CXRXM1SID                   C1RXM1SID
#define CXRXF0SID                   C1RXF0SID
#define CXRXF1SID                   C1RXF1SID
#define CXRXF2SID                   C1RXF2SID
#define CXRXF3SID                   C1RXF3SID
#define CXRXF4SID                   C1RXF4SID
#define CXRXF5SID                   C1RXF5SID
#define CXTX2CON                    C1TX2CON
#define CXTX0DLC                    C1TX0DLC
#define CXTX1DLC                    C1TX1DLC
#define CXTX2DLC                    C1TX2DLC
#define CXRX0CON                    C1RX0CON
#define CXRX1CON                    C1RX1CON
#define CXRX1SID                    C1RX1SID
#define CXRX1B1                     C1RX1B1

#define CXRX1CONbits                C1RX1CONbits

#define _CXInterrupt                _C1Interrupt

//

#define CXCTRLbits                  C1CTRLbits
#define CXTX0CONbits                C1TX0CONbits
#define CXTX1CONbits                C1TX1CONbits
#define CXINTFbits                  C1INTFbits
#define CXRX0CONbits                C1RX0CONbits
#define CXINTEbits                  C1INTEbits






#else

#define _CXIE                       _C2IE
#define _CXIF                       _C2IF
#define _CXIP                       _C2IP

#define CXTX0CON                    C2TX0CON
#define CXTX1CON                    C2TX1CON
#define CXINTF                      C2INTF


//
#define CXCTRL                      C2CTRL
#define CXCFG1                      C2CFG1
#define CXCFG2                      C2CFG2
#define CXRXM0SID                   C2RXM0SID
#define CXRXM1SID                   C2RXM1SID
#define CXRXF0SID                   C2RXF0SID
#define CXRXF1SID                   C2RXF1SID
#define CXRXF2SID                   C2RXF2SID
#define CXRXF3SID                   C2RXF3SID
#define CXRXF4SID                   C2RXF4SID
#define CXRXF5SID                   C2RXF5SID
#define CXTX2CON                    C2TX2CON
#define CXTX0DLC                    C2TX0DLC
#define CXTX1DLC                    C2TX1DLC
#define CXTX2DLC                    C2TX2DLC
#define CXRX0CON                    C2RX0CON
#define CXRX1CON                    C2RX1CON
#define CXRX1SID                    C2RX1SID
#define CXRX1B1                     C2RX1B1

#define CXRX1CONbits                C2RX1CONbits

#define _CXInterrupt                _C2Interrupt

//


#define CXCTRLbits                  C2CTRLbits
#define CXTX0CONbits                C2TX0CONbits
#define CXTX1CONbits                C2TX1CONbits
#define CXINTFbits                  C2INTFbits
#define CXRX0CONbits                C2RX0CONbits
#define CXINTEbits                  C2INTEbits

#endif














// Public Buffers
ETMCanMessageBuffer etm_can_rx_message_buffer;
ETMCanMessageBuffer etm_can_tx_message_buffer;
#ifdef __ETM_CAN_MASTER_MODULE
ETMCanMessageBuffer etm_can_rx_data_log_buffer;
#endif

// Public Variables
unsigned int etm_can_next_pulse_level;
unsigned int etm_can_next_pulse_count;
#ifndef __ETM_CAN_MASTER_MODULE
unsigned int etm_can_high_speed_data_logging_enabled;
#endif

// Public Debug and Status registers
ETMCanSystemDebugData etm_can_system_debug_data;
ETMCanStatusRegister etm_can_status_register;


// Private Functions
void ETMCanSetValue(ETMCanMessage* message_ptr);
void ETMCanSetValueCalibration(ETMCanMessage* message_ptr);
#ifdef __ETM_CAN_MASTER_MODULE
void ETMCanSetValueCalibrationUpload(ETMCanMessage* message_ptr);
#else
void ETMCanExecuteCMD(ETMCanMessage* message_ptr);
void ETMCanExecuteCMDDefault(ETMCanMessage* message_ptr);
void ETMCanReturnValue(ETMCanMessage* message_ptr);
void ETMCanReturnValueCalibration(ETMCanMessage* message_ptr);
void ETMCanDoSync(ETMCanMessage* message_ptr);
#endif







void ETMCanProcessMessage(void) {
  ETMCanMessage next_message;
  while (ETMCanBufferNotEmpty(&etm_can_rx_message_buffer)) {
    ETMCanReadMessageFromBuffer(&etm_can_rx_message_buffer, &next_message);
    
#ifdef __ETM_CAN_MASTER_MODULE
    if ((next_message.identifier & ETM_CAN_MSG_MASTER_ADDR_MASK) == ETM_CAN_MSG_SET_2_RX) {
      ETMCanSetValue(&next_message);      
    } else if ((next_message.identifier & ETM_CAN_MSG_MASTER_ADDR_MASK) == ETM_CAN_MSG_STATUS_RX) {
      ETMCanUpdateStatusBoardSpecific(&next_message);
    } else {
      etm_can_system_debug_data.unknown_message_identifier++;
    } 
#else
    if ((next_message.identifier & 0b0000000001111000) != (ETM_CAN_MY_ADDRESS << 3)) {
      // It was not addressed to this board
      etm_can_system_debug_data.message_not_addressed_to_this_board++;
    } else if (next_message.identifier == (ETM_CAN_MSG_CMD_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanExecuteCMD(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_MSG_SET_1_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_MSG_REQUEST_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanReturnValue(&next_message);
    } else if ((next_message.identifier & ETM_CAN_MSG_SLAVE_ADDR_MASK) == (ETM_CAN_MSG_SET_3_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);
    } else {
      etm_can_system_debug_data.unknown_message_identifier++;
    } 
#endif
    
  }
}



void ETMCanSetValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  
#ifdef __ETM_CAN_MASTER_MODULE
  if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid set Value ID
    etm_can_system_debug_data.set_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific set value
    ETMCanSetValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // Default Register index
    // This is not valid for the master module
    etm_can_system_debug_data.set_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanSetValueCalibrationUpload(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_system_debug_data.set_value_index_not_valid++;
  }
#else
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_system_debug_data.message_index_does_not_match_board++;
  } else if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid set Value ID
    etm_can_system_debug_data.set_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific set value
    ETMCanSetValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // It is a board specific defailt registers
    // These are not implimented at this time because there are no default set values
    etm_can_system_debug_data.set_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanSetValueCalibration(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_system_debug_data.set_value_index_not_valid++;
  }    
#endif
}



#ifdef __ETM_CAN_MASTER_MODULE

void ETMCanSetValueCalibrationUpload(ETMCanMessage* message_ptr) {
  // Dparker impliment this
}

#else

void ETMCanExecuteCMD(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_system_debug_data.message_index_does_not_match_board++;
  } else if ((index_word & 0x0FFF) <= 0x007F) {
    // It is a default command
    ETMCanExecuteCMDDefault(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x0FF) {
    // It is a board specific command
    ETMCanExecuteCMDBoardSpecific(message_ptr);
  } else {
    // It was not a command ID
    etm_can_system_debug_data.command_index_not_valid++;
  }
}


void ETMCanExecuteCMDDefault(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  index_word &= 0x0FFF;
  
  switch (index_word) {
    
  case ETM_CAN_REGISTER_DEFAULT_CMD_RESET_FAULTS:
    ETMCanResetFaults();
    break;
    
  case ETM_CAN_REGISTER_DEFAULT_CMD_RESET_MCU:
    __asm__ ("Reset");
    break;
    
  case ETM_CAN_REGISTER_DEFAULT_CMD_RESEND_CONFIG:
    ETMCanLogConfig();
    break;
    
  case ETM_CAN_REGISTER_DEFAULT_CMD_WRITE_EEPROM_PAGE:
    // DPARKER implement this
    break;
 
  case ETM_CAN_REGISTER_DEFAULT_CMD_DISABLE_HIGH_SPEED_DATA_LOGGING:
    etm_can_high_speed_data_logging_enabled = 0;
    break;

  case ETM_CAN_REGISTER_DEFAULT_CMD_ENABLE_HIGH_SPEED_DATA_LOGGING:
    etm_can_high_speed_data_logging_enabled = 1;
    break;
   
  default:
    // The default command was not recognized 
    etm_can_system_debug_data.default_command_index_not_recognized++;
    break;
  }
}



void ETMCanReturnValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_system_debug_data.message_index_does_not_match_board++;
  } else if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid return Value ID
    etm_can_system_debug_data.return_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific return value
    ETMCanReturnValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // It is a board specific default registers
    // These are not implimented at this time because there are no default set values
    etm_can_system_debug_data.return_value_index_not_valid++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanReturnValueCalibration(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_system_debug_data.set_value_index_not_valid++;
  }
}


void ETMCanSetValueCalibration(ETMCanMessage* message_ptr) {
  // DPARKER need to impliment calibration system
}

void ETMCanReturnValueCalibration(ETMCanMessage* message_ptr) {
  // DPARKER need to impliment calibration system
}

void ETMCanSendStatus(void) {
  ETMCanMessage status_message;
  status_message.identifier = ETM_CAN_MSG_STATUS_TX | (ETM_CAN_MY_ADDRESS << 3);
  status_message.word0 = etm_can_status_register.status_word_0;
  status_message.word1 = etm_can_status_register.status_word_1;
  status_message.word2 = etm_can_status_register.data_word_A;
  status_message.word3 = etm_can_status_register.data_word_B;
  
  ETMCanTXMessage(&status_message, &CXTX1CON);
}


void ETMCanDoSync(ETMCanMessage* message_ptr) {
  // Sync data is available in CXRX0B1->CXRX0B4
  // At this time all that happens is that the chip watchdog is reset
  // DPARKER move to assembly and issure W0-W3, SR usage
  ClrWdt();
}

void ETMCanLogData(unsigned int packet_id, unsigned int word3, unsigned int word2, unsigned int word1, unsigned int word0) {
  ETMCanMessage log_message;

  packet_id &= 0x000F;
  packet_id |= (ETM_CAN_MY_ADDRESS << 4);
  packet_id <<= 1;
  packet_id |= 0b0000011000000000;
  packet_id <<= 2;

  log_message.identifier = packet_id;
  log_message.identifier &= 0xFF00;
  log_message.identifier <<= 3;
  log_message.identifier |= (packet_id & 0x00FF);
  
  log_message.word0 = word0;
  log_message.word1 = word1;
  log_message.word2 = word2;
  log_message.word3 = word3;
  
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &log_message);
}

void ETMCanLogDefaultDebug(void) {
  // Sends the debug information up as log data
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_0, etm_can_system_debug_data.i2c_bus_error_count, etm_can_system_debug_data.spi_bus_error_count, etm_can_system_debug_data.can_bus_error_count, etm_can_system_debug_data.scale_error_count);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_1, etm_can_system_debug_data.reset_count, etm_can_system_debug_data.self_test_result_register, 0, 0);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_0, etm_can_system_debug_data.debug_0, etm_can_system_debug_data.debug_1, etm_can_system_debug_data.debug_2, etm_can_system_debug_data.debug_3);  
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_1, etm_can_system_debug_data.debug_4, etm_can_system_debug_data.debug_5, etm_can_system_debug_data.debug_6, etm_can_system_debug_data.debug_7);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_2, etm_can_system_debug_data.debug_8, etm_can_system_debug_data.debug_9, etm_can_system_debug_data.debug_A, etm_can_system_debug_data.debug_B);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_3, etm_can_system_debug_data.debug_C, etm_can_system_debug_data.debug_D, etm_can_system_debug_data.debug_E, etm_can_system_debug_data.debug_F);



  // DPARKER - add compile time configuration to include (or not include) can data.
  // DPARKER - alterternatively, stagger these so that not every log is updated every time (max of 4 or so each time the function is called)
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_0, etm_can_system_debug_data.unknown_message_identifier, etm_can_system_debug_data.message_index_does_not_match_board, etm_can_system_debug_data.message_not_addressed_to_this_board, etm_can_system_debug_data.update_status_unknown_board);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_1, etm_can_system_debug_data.can_module_error_flag, etm_can_system_debug_data.command_index_not_valid, etm_can_system_debug_data.default_command_index_not_recognized, etm_can_system_debug_data.board_specific_command_unknown);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_2,etm_can_system_debug_data.set_value_index_not_valid, etm_can_system_debug_data.board_specific_set_value_unknown, etm_can_system_debug_data.return_value_index_not_valid, etm_can_system_debug_data.board_specific_return_value_unknown);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_3, etm_can_system_debug_data.can_commands_processed, etm_can_system_debug_data.can_rx_0_count, etm_can_system_debug_data.can_rx_1_count, etm_can_system_debug_data.can_tx_0_count);
}


void ETMCanLogConfig(void) {
  // Sends the processor configuration up as log data
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_0, 0x0000, 0x0001, 0x0002, 0x0003);
  ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_1, 0x0010, 0x0011, 0x0012, 0x0013);
  // DPARKER need to impliment to actual configuration data
}


#endif



void ETMCanInitialize(void) {
  _CXIE = 0;
  _CXIF = 0;
  _CXIP = ETM_CAN_INTERRUPT_PRIORITY;
  
  CXINTF = 0;
  
  CXINTEbits.RX0IE = 1; // Enable RXB0 interrupt
  CXINTEbits.RX1IE = 1; // Enable RXB1 interrupt
  CXINTEbits.TX0IE = 1; // Enable TXB0 interrupt
  CXINTEbits.ERRIE = 1; // Enable Error interrupt
  
  ETMCanBufferInitialize(&etm_can_rx_message_buffer);
  ETMCanBufferInitialize(&etm_can_tx_message_buffer);

  // DPARKER - Zero all the counters in the error structure.

#ifdef __ETM_CAN_MASTER_MODULE
  ETMCanBufferInitialize(&etm_can_rx_data_log_buffer);
#endif
  
  // ---------------- Set up CAN Control Registers ---------------- //
  
  // Set Baud Rate
  CXCTRL = CXCTRL_CONFIG_MODE_VALUE;
  while(CXCTRLbits.OPMODE != 4);
  
  CXCFG1 = ETM_CAN_CXCFG1_VALUE;
  CXCFG2 = CXCFG2_VALUE;
  
  
  // Load Mask registers for RX0 and RX1
#ifdef __ETM_CAN_MASTER_MODULE
  CXRXM0SID = ETM_CAN_MASTER_RX0_MASK;
  CXRXM1SID = ETM_CAN_MASTER_RX1_MASK;
#else
  CXRXM0SID = ETM_CAN_SLAVE_RX0_MASK;
  CXRXM1SID = ETM_CAN_SLAVE_RX1_MASK;
#endif

  // Load Filter registers
#ifdef __ETM_CAN_MASTER_MODULE
  CXRXF0SID = ETM_CAN_MSG_LVL_FILTER;
  CXRXF1SID = ETM_CAN_MSG_DATA_LOG_FILTER;
  CXRXF2SID = ETM_CAN_MSG_MASTER_FILTER;
  CXRXF3SID = ETM_CAN_MSG_FILTER_OFF;
  CXRXF4SID = ETM_CAN_MSG_FILTER_OFF;
  CXRXF5SID = ETM_CAN_MSG_FILTER_OFF;
#else
  CXRXF0SID = ETM_CAN_MSG_LVL_FILTER;
  CXRXF1SID = ETM_CAN_MSG_SYNC_FILTER;
  CXRXF2SID = (ETM_CAN_MSG_SLAVE_FILTER | (ETM_CAN_MY_ADDRESS << 3));
  CXRXF3SID = ETM_CAN_MSG_FILTER_OFF;
  CXRXF4SID = ETM_CAN_MSG_FILTER_OFF;
  CXRXF5SID = ETM_CAN_MSG_FILTER_OFF;
#endif

  // Set Transmitter Mode
  CXTX0CON = CXTXXCON_VALUE_LOW_PRIORITY;
  CXTX1CON = CXTXXCON_VALUE_MEDIUM_PRIORITY;
  CXTX2CON = CXTXXCON_VALUE_HIGH_PRIORITY;

  CXTX0DLC = CXTXXDLC_VALUE;
  CXTX1DLC = CXTXXDLC_VALUE;
  CXTX2DLC = CXTXXDLC_VALUE;

  
  // Set Receiver Mode
  CXRX0CON = CXRXXCON_VALUE;
  CXRX1CON = CXRXXCON_VALUE;
  
  // Switch to normal operation
  // DPARKER we are in loopback for testing
  CXCTRL = CXCTRL_OPERATE_MODE_VALUE;
  while(CXCTRLbits.OPMODE != 0);
  
  // Enable Can interrupt
  _CXIE = 1;
}







void __attribute__((interrupt, no_auto_psv)) _CXInterrupt(void) {
  ETMCanMessage can_message;
  
  _CXIF = 0;
  
  if(CXRX0CONbits.RXFUL) {
    etm_can_system_debug_data.can_rx_0_count++;
    /*
      A message has been received in Buffer Zero
    */
    if (!CXRX0CONbits.FILHIT0) {
      // The command was received by Filter 0
      // It is a Next Pulse Level Command
      ETMCanRXMessage(&can_message, &CXRX0CON);
      // DPARKER set next pulse level to data
      etm_can_next_pulse_level = can_message.word2;
      etm_can_next_pulse_count = can_message.word3;
      etm_can_system_debug_data.can_commands_processed++;
    } else {
      // The commmand was received by Filter 1
#ifdef __ETM_CAN_MASTER_MODULE
      // The command is a data log.  Add it to the data log buffer
      ETMCanRXMessageBuffer(&etm_can_rx_data_log_buffer, &CXRX0CON);
      etm_can_system_debug_data.can_commands_processed++;
#else
      // The command is a sync command.
      ETMCanRXMessage(&can_message, &CXRX0CON);
      etm_can_system_debug_data.can_commands_processed++;
      ETMCanDoSync(&can_message);
#endif
    }
    CXINTFbits.RX0IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if(CXRX1CONbits.RXFUL) {
    etm_can_system_debug_data.can_rx_1_count++;
    /* 
       A message has been recieved in Buffer 1
       This command gets pushed onto the command message buffer
    */
    ETMCanRXMessageBuffer(&etm_can_rx_message_buffer, &CXRX1CON);
    etm_can_system_debug_data.can_commands_processed++; 
#ifdef __ETM_CAN_MASTER_MODULE
    if (((CXRX1SID & ETM_CAN_MSG_MASTER_ADDR_MASK) == ETM_CAN_MSG_STATUS_RX) && (CXRX1B1 & 0x0001))  {
      // The message is a status command that indicates a fault
      // DPARKER set global error identifier
      // DPARKER send out a disable command to the pulse sync board
    }
#endif
    CXINTFbits.RX1IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if ((!CXTX0CONbits.TXREQ) && (ETMCanBufferNotEmpty(&etm_can_tx_message_buffer))) {
    etm_can_system_debug_data.can_tx_0_count++;
    /*
      TX0 is empty and there is a message waiting in the transmit message buffer
      Load the next message into TX0
    */
    ETMCanTXMessageBuffer(&etm_can_tx_message_buffer, &CXTX0CON);
    etm_can_system_debug_data.can_commands_processed++;
    CXINTFbits.TX0IF = 0;
  }
  
  
  if (CXINTFbits.ERRIF) {
    
    // There was some sort of CAN Error
    // DPARKER - figure out which error and fix/reset
    etm_can_system_debug_data.can_module_error_flag++;
    CXINTFbits.ERRIF = 0;
  }
}

