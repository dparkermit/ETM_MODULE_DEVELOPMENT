#include <p30Fxxxx.h>
#include "ETM_CAN.h"

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
  
  ETMCanTXMessage(&status_message, &C1TX1CON);
}


void ETMCanDoSync(ETMCanMessage* message_ptr) {
  // Sync data is available in C1RX0B1->C1RX0B4
  // At this time all that happens is that the chip watchdog is reset
  // DPARKER move to assembly and issure W0-W3, SR usage
  ClrWdt();
}

void ETMCanLogData(unsigned char packet_id, unsigned int word0, unsigned int word1, unsigned int word2, unsigned int word3) {
  ETMCanMessage log_message;

  packet_id &= 0x000F;
  packet_id |= (ETM_CAN_MY_ADDRESS << 4);
  packet_id |= 0b00000110000000000;
  packet_id <<= 2;

  log_message.identifier = (packet_id << 3);
  log_message.identifier &= 0xFF00;
  log_message.identifier |= (packet_id & 0x00FF);
  
  log_message.word0 = word0;
  log_message.word1 = word1;
  log_message.word2 = word2;
  log_message.word3 = word3;
  
  ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &log_message);
}

void ETMCanLogDefaultDebug(void) {
  // Sends the debug information up as log data
  ETMCanLogData(0x0, etm_can_system_debug_data.i2c_bus_error_count, etm_can_system_debug_data.spi_bus_error_count, etm_can_system_debug_data.can_bus_error_count, etm_can_system_debug_data.scale_error_count);
  ETMCanLogData(0x1, etm_can_system_debug_data.reset_count, etm_can_system_debug_data.self_test_result_register, 0, 0);
  ETMCanLogData(0x2, etm_can_system_debug_data.debug_0, etm_can_system_debug_data.debug_1, etm_can_system_debug_data.debug_2, etm_can_system_debug_data.debug_3);  
  ETMCanLogData(0x3, etm_can_system_debug_data.debug_4, etm_can_system_debug_data.debug_5, etm_can_system_debug_data.debug_6, etm_can_system_debug_data.debug_7);
  ETMCanLogData(0x4, etm_can_system_debug_data.debug_8, etm_can_system_debug_data.debug_9, etm_can_system_debug_data.debug_A, etm_can_system_debug_data.debug_B);
  ETMCanLogData(0x5, etm_can_system_debug_data.debug_C, etm_can_system_debug_data.debug_D, etm_can_system_debug_data.debug_E, etm_can_system_debug_data.debug_F);



  // DPARKER - add compile time configuration to include (or not include) can data.
  // DPARKER - alterternatively, stagger these so that not every log is updated every time (max of 4 or so each time the function is called)
  ETMCanLogData(0xC, etm_can_system_debug_data.unknown_message_identifier, etm_can_system_debug_data.message_index_does_not_match_board, etm_can_system_debug_data.message_not_addressed_to_this_board, etm_can_system_debug_data.update_status_unknown_board);
  ETMCanLogData(0xD, etm_can_system_debug_data.can_module_error_flag, etm_can_system_debug_data.command_index_not_valid, etm_can_system_debug_data.default_command_index_not_recognized, etm_can_system_debug_data.board_specific_command_unknown);
  ETMCanLogData(0xE,etm_can_system_debug_data.set_value_index_not_valid, etm_can_system_debug_data.board_specific_set_value_unknown, etm_can_system_debug_data.return_value_index_not_valid, etm_can_system_debug_data.board_specific_return_value_unknown);
  ETMCanLogData(0xF, etm_can_system_debug_data.can_commands_processed, 0, 0, 0);
}


void ETMCanLogConfig(void) {
  // Sends the processor configuration up as log data
  ETMCanLogData(6, 0x0000, 0x0001, 0x0002, 0x0003);
  ETMCanLogData(7, 0x0010, 0x0011, 0x0012, 0x0013);
  // DPARKER need to impliment to actual configuration data
}


#endif



void ETMCanInitialize(void) {
  _C1IE = 0;
  _C1IF = 0;
  _C1IP = ETM_CAN_INTERRUPT_PRIORITY;
  
  C1INTF = 0;
  
  C1INTEbits.RX0IE = 1; // Enable RXB0 interrupt
  C1INTEbits.RX1IE = 1; // Enable RXB1 interrupt
  C1INTEbits.TX0IE = 1; // Enable TXB0 interrupt
  C1INTEbits.ERRIE = 1; // Enable Error interrupt
  
  ETMCanBufferInitialize(&etm_can_rx_message_buffer);
  ETMCanBufferInitialize(&etm_can_tx_message_buffer);

  // DPARKER - Zero all the counters in the error structure.

#ifdef __ETM_CAN_MASTER_MODULE
  ETMCanBufferInitialize(&etm_can_rx_data_log_buffer);
#endif
  
  // ---------------- Set up CAN Control Registers ---------------- //
  
  // Set Baud Rate
  C1CTRL = CXCTRL_CONFIG_MODE_VALUE;
  while(C1CTRLbits.OPMODE != 4);
  
  C1CFG1 = ETM_CAN_CXCFG1_VALUE;
  C1CFG2 = CXCFG2_VALUE;
  
  
  // Load Mask registers for RX0 and RX1
#ifdef __ETM_CAN_MASTER_MODULE
  C1RXM0SID = ETM_CAN_MASTER_RX0_MASK;
  C1RXM1SID = ETM_CAN_MASTER_RX1_MASK;
#else
  C1RXM0SID = ETM_CAN_SLAVE_RX0_MASK;
  C1RXM1SID = ETM_CAN_SLAVE_RX1_MASK;
#endif

  // Load Filter registers
#ifdef __ETM_CAN_MASTER_MODULE
  C1RXF0SID = ETM_CAN_MSG_LVL_FILTER;
  C1RXF1SID = ETM_CAN_MSG_DATA_LOG_FILTER;
  C1RXF2SID = ETM_CAN_MSG_MASTER_FILTER;
  C1RXF3SID = ETM_CAN_MSG_FILTER_OFF;
  C1RXF4SID = ETM_CAN_MSG_FILTER_OFF;
  C1RXF5SID = ETM_CAN_MSG_FILTER_OFF;
#else
  C1RXF0SID = ETM_CAN_MSG_LVL_FILTER;
  C1RXF1SID = ETM_CAN_MSG_SYNC_FILTER;
  C1RXF2SID = (ETM_CAN_MSG_SLAVE_FILTER | (ETM_CAN_MY_ADDRESS << 3));
  C1RXF3SID = ETM_CAN_MSG_FILTER_OFF;
  C1RXF4SID = ETM_CAN_MSG_FILTER_OFF;
  C1RXF5SID = ETM_CAN_MSG_FILTER_OFF;
#endif

  // Set Transmitter Mode
  C1TX0CON = CXTXXCON_VALUE_LOW_PRIORITY;
  C1TX1CON = CXTXXCON_VALUE_MEDIUM_PRIORITY;
  C1TX2CON = CXTXXCON_VALUE_HIGH_PRIORITY;

  C1TX0DLC = CXTXXDLC_VALUE;
  C1TX1DLC = CXTXXDLC_VALUE;
  C1TX2DLC = CXTXXDLC_VALUE;

  
  // Set Receiver Mode
  C1RX0CON = CXRXXCON_VALUE;
  C1RX1CON = CXRXXCON_VALUE;
  
  // Switch to normal operation
  // DPARKER we are in loopback for testing
  C1CTRL = CXCTRL_OPERATE_MODE_VALUE;
  while(C1CTRLbits.OPMODE != 0);
  
  // Enable Can interrupt
  _C1IE = 1;
}







void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void) {
  ETMCanMessage can_message;
  
  _C1IF = 0;
  
  if(C1RX0CONbits.RXFUL) {
    /*
      A message has been received in Buffer Zero
    */
    if (!C1RX0CONbits.FILHIT0) {
      // The command was received by Filter 0
      // It is a Next Pulse Level Command
      ETMCanRXMessage(&can_message, &C1RX0CON);
      // DPARKER set next pulse level to data
      etm_can_next_pulse_level = can_message.word2;
      etm_can_next_pulse_count = can_message.word3;
      etm_can_system_debug_data.can_commands_processed++;
    } else {
      // The commmand was received by Filter 1
#ifdef __ETM_CAN_MASTER_MODULE
      // The command is a data log.  Add it to the data log buffer
      ETMCanRXMessageBuffer(&etm_can_rx_data_log_buffer, &C1RX0CON);
      etm_can_system_debug_data.can_commands_processed++;
#else
      // The command is a sync command.
      ETMCanRXMessage(&can_message, &C1RX0CON);
      etm_can_system_debug_data.can_commands_processed++;
      ETMCanDoSync(&can_message);
#endif
    }
    C1INTFbits.RX0IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if(C1RX1CONbits.RXFUL) {
    /* 
       A message has been recieved in Buffer 1
       This command gets pushed onto the command message buffer
    */
    ETMCanRXMessageBuffer(&etm_can_rx_message_buffer, &C1RX1CON);
    etm_can_system_debug_data.can_commands_processed++; 
#ifdef __ETM_CAN_MASTER_MODULE
    if (((C1RX1SID & ETM_CAN_MSG_MASTER_ADDR_MASK) == ETM_CAN_MSG_STATUS_RX) && (C1RX1B1 & 0x0001))  {
      // The message is a status command that indicates a fault
      // DPARKER set global error identifier
      // DPARKER send out a disable command to the pulse sync board
    }
#endif
    C1INTFbits.RX1IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if ((!C1TX0CONbits.TXREQ) && (ETMCanBufferNotEmpty(&etm_can_tx_message_buffer))) {
    /*
      TX0 is empty and there is a message waiting in the transmit message buffer
      Load the next message into TX0
    */
    ETMCanTXMessageBuffer(&etm_can_tx_message_buffer, &C1TX0CON);
    etm_can_system_debug_data.can_commands_processed++;
    C1INTFbits.TX0IF = 0;
  }
  
  
  if (C1INTFbits.ERRIF) {
    
    // There was some sort of CAN Error
    // DPARKER - figure out which error and fix/reset
    etm_can_system_debug_data.can_module_error_flag++;
    C1INTFbits.ERRIF = 0;
  }
}

