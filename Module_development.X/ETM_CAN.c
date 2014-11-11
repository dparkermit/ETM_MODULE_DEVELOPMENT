#include <p30Fxxxx.h>
#include "ETM_CAN.h"
#include "timer.h"  // DPARKER remove the requirement for this




// Public Buffers
ETMCanMessageBuffer etm_can_rx_message_buffer;
ETMCanMessageBuffer etm_can_tx_message_buffer;
#ifdef __ETM_CAN_MASTER_MODULE
ETMCanMessageBuffer etm_can_rx_data_log_buffer;
ETMCanRamMirrorEthernetBoard     etm_can_ethernet_board_data;
#endif

// Public Variables
unsigned int etm_can_next_pulse_level;
unsigned int etm_can_next_pulse_count;
#ifndef __ETM_CAN_MASTER_MODULE
unsigned int etm_can_high_speed_data_logging_enabled;
#endif

// Public Debug and Status registers
ETMCanSystemDebugData etm_can_system_debug_data;
ETMCanStatusRegister  etm_can_status_register;
ETMCanAgileConfig     etm_can_my_configuration;
ETMCanCanStatus       etm_can_can_status;

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

//local variables

unsigned int etm_can_default_transmit_counter;




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
      etm_can_can_status.can_status_unknown_message_identifier++;
    } 
#else
    if ((next_message.identifier & 0b0000000001111000) != (ETM_CAN_MY_ADDRESS << 3)) {
      // It was not addressed to this board
      etm_can_can_status.can_status_address_error++;
    } else if (next_message.identifier == (ETM_CAN_MSG_CMD_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanExecuteCMD(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_MSG_SET_1_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_MSG_REQUEST_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanReturnValue(&next_message);
    } else if ((next_message.identifier & ETM_CAN_MSG_SLAVE_ADDR_MASK) == (ETM_CAN_MSG_SET_3_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);
    } else {
      etm_can_can_status.can_status_unknown_message_identifier++;
    } 
#endif
  }

  etm_can_can_status.can_status_message_tx_buffer_overflow = etm_can_tx_message_buffer.message_overwrite_count;
  etm_can_can_status.can_status_message_rx_buffer_overflow = etm_can_rx_message_buffer.message_overwrite_count;
#ifdef __ETM_CAN_MASTER_MODULE
  etm_can_can_status.can_status_data_log_rx_buffer_overflow = etm_can_rx_data_log_buffer.message_overwrite_count;
#endif  

}



void ETMCanSetValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  
#ifdef __ETM_CAN_MASTER_MODULE
  if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid set Value ID
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific set value
    ETMCanSetValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // Default Register index
    // This is not valid for the master module
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanSetValueCalibrationUpload(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_can_status.can_status_invalid_index++;
  }
#else
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid set Value ID
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific set value
    ETMCanSetValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // It is a board specific defailt registers
    // These are not implimented at this time because there are no default set values
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanSetValueCalibration(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_can_status.can_status_invalid_index++;
  }    
#endif
}






#ifdef __ETM_CAN_MASTER_MODULE

void ETMCanSetValueCalibrationUpload(ETMCanMessage* message_ptr) {
  // Dparker impliment this
}

void ETMCanSendSync(unsigned int sync_3, unsigned int sync_2, unsigned int sync_1, unsigned int sync_0) {
  ETMCanMessage sync_message;
  sync_message.identifier = ETM_CAN_MSG_SYNC_TX;
  sync_message.word0 = sync_0;
  sync_message.word1 = sync_1;
  sync_message.word2 = sync_2;
  sync_message.word3 = sync_3;
  
  ETMCanTXMessage(&sync_message, &CXTX1CON);
  etm_can_can_status.can_status_tx_1++;
}

void ETMCanEthernetSendPulseSyncOperate(unsigned int operate) {
  ETMCanMessage operate_msg;
  
  operate_msg.identifier = ETM_CAN_MSG_CMD_TX | (ETM_CAN_ADDR_PULSE_SYNC_BOARD << 3);
  if (operate) {
    operate_msg.word3 = ETM_CAN_REGISTER_PULSE_SYNC_CMD_ENABLE_PULSES;
  } else {
    operate_msg.word3 = ETM_CAN_REGISTER_PULSE_SYNC_CMD_DISABLE_PULSES;
  }
  ETMCanTXMessage(&operate_msg, &CXTX2CON);
  etm_can_can_status.can_status_tx_2++;  
}


void ETMCanMaster100msCommunication(void) {
  /*
    One command is schedule to be sent every 25ms
    This loops through 8 times so each command is sent once every 200mS (5Hz)
    The sync command and Pulse Sync enable command are each sent twice for an effecive rate of 100ms (10Hz)
  */

  ETMCanMessage test_message;
  
  if (_T2IF) {
    // should be true once every 25mS
    _T2IF = 0;
    
    etm_can_default_transmit_counter++;
    etm_can_default_transmit_counter &= 0x7;

    
    switch (etm_can_default_transmit_counter) 
      {
      case 0x0:
	// DPARKER change to the "operate LED"
	if (_LATG14) {
	  _LATG14 = 0;
	} else {
	  _LATG14 = 1;
	}
	// Send Sync Command (this is on TX1)
	ETMCanSendSync(0,0,0,0);
	break;

      case 0x1:
	// Send Enable/Disable command to Pulse Sync Board  (this is on TX2)		
	break;
	
      case 0x2:
	// Send High/Low Energy Program voltage to Lambda Board 
	
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
	test_message.word2 = 16000;
	test_message.word1 = 18000;
	test_message.word0 = 0;
	ETMCanAddMessageToBuffer(&etm_can_tx_message_buffer, &test_message);
	
	MacroETMCanCheckTXBuffer();
	break;
	
      case 0x3:
	// Send Heater/Magnet Current to Heater Magnet Board
	break;
	
      case 0x4:
	// Send Sync Command (this is on TX1)
	ETMCanSendSync(0,0,0,0);
	break;
	
      case 0x5:
	// Send Enable/Disable command to Pulse Sync Board  (this is on TX2)
	break;
	
      case 0x6:
	// Send High/Low Energy Pulse top voltage to Gun Driver
	break;
	
      case 0x7:
	// Send Heater/Cathode set points to Gun Driver
	break;

      }
  }
}

#else

void ETMCanExecuteCMD(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x007F) {
    // It is a default command
    ETMCanExecuteCMDDefault(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x0FF) {
    // It is a board specific command
    ETMCanExecuteCMDBoardSpecific(message_ptr);
  } else {
    // It was not a command ID
    etm_can_can_status.can_status_invalid_index++;
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

    /*
    // DPARKER this command has been removed because the configuration is constantly sent so this command is no longer needed
    case ETM_CAN_REGISTER_DEFAULT_CMD_RESEND_CONFIG:
    //ETMCanLogConfig();
    
    break;
    */


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
    etm_can_can_status.can_status_invalid_index++;
    break;
  }
}



void ETMCanReturnValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  if ((index_word & 0xF000) != (ETM_CAN_MY_ADDRESS << 12)) {
    // The index is not addressed to this board
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x00FF) {
    // It is not a valid return Value ID
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x2FF) {
    // It is a board specific return value
    ETMCanReturnValueBoardSpecific(message_ptr);
  } else if ((index_word & 0x0FFF) <= 0x3FF) {
    // It is a board specific default registers
    // These are not implimented at this time because there are no default set values
    etm_can_can_status.can_status_invalid_index++;
  } else if ((index_word & 0x0FFF) <= 0x4FF) {
    ETMCanReturnValueCalibration(message_ptr);
  } else {
    // It was not a set value index 
    etm_can_can_status.can_status_invalid_index++;
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
  etm_can_can_status.can_status_tx_1++;
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
  MacroETMCanCheckTXBuffer()
}



void ETMCanSlaveLog100ms(void) {
  // Sends the debug information up as log data  
  if (_T2IF) {
    // should be true once every 100mS
    _T2IF = 0;
    
    etm_can_default_transmit_counter++;
    etm_can_default_transmit_counter &= 0xF;
    
    // DPARKER change to the "operate LED"
    if (_LATG14) {
      _LATG14 = 0;
    } else {
      _LATG14 = 1;
    }
    
    ETMCanSendStatus(); // Send out the status every 100mS
    
    switch (etm_can_default_transmit_counter) 
      {
      case 0x0:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_0, etm_can_system_debug_data.i2c_bus_error_count, etm_can_system_debug_data.spi_bus_error_count, etm_can_system_debug_data.can_bus_error_count, etm_can_system_debug_data.scale_error_count);      
	break;
	
      case 0x1:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_1, etm_can_system_debug_data.reset_count, etm_can_system_debug_data.self_test_result_register, 0, 0);
	break;
	
      case 0x2:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_0, etm_can_system_debug_data.debug_0, etm_can_system_debug_data.debug_1, etm_can_system_debug_data.debug_2, etm_can_system_debug_data.debug_3);  
	break;
	
      case 0x3:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_1, etm_can_system_debug_data.debug_4, etm_can_system_debug_data.debug_5, etm_can_system_debug_data.debug_6, etm_can_system_debug_data.debug_7);
	break;
	
      case 0x4:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_2, etm_can_system_debug_data.debug_8, etm_can_system_debug_data.debug_9, etm_can_system_debug_data.debug_A, etm_can_system_debug_data.debug_B);
	break;
	
      case 0x5:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_3, etm_can_system_debug_data.debug_C, etm_can_system_debug_data.debug_D, etm_can_system_debug_data.debug_E, etm_can_system_debug_data.debug_F);
	break;
	
      case 0x6:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_0, etm_can_can_status.can_status_CXEC_reg, etm_can_can_status.can_status_error_flag, etm_can_can_status.can_status_tx_1, etm_can_can_status.can_status_tx_2);
	break;
	
      case 0x7:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_1, etm_can_can_status.can_status_rx_0_filt_0, etm_can_can_status.can_status_rx_0_filt_1, etm_can_can_status.can_status_rx_1_filt_2, etm_can_can_status.can_status_isr_entered);
	break;
	
      case 0x8:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_2, etm_can_can_status.can_status_unknown_message_identifier, etm_can_can_status.can_status_invalid_index, etm_can_can_status.can_status_address_error, etm_can_can_status.can_status_tx_0);
	break;

      case 0x9:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_3, etm_can_can_status.can_status_message_tx_buffer_overflow, etm_can_can_status.can_status_message_rx_buffer_overflow, etm_can_can_status.can_status_data_log_rx_buffer_overflow, etm_can_can_status.can_status_unused_4);
	break;
	
      case 0xA:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_0, etm_can_my_configuration.agile_number_high_word, etm_can_my_configuration.agile_number_low_word, etm_can_my_configuration.agile_dash, etm_can_my_configuration.agile_rev_ascii);
	break;

      case 0xB:
	ETMCanLogData(ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_1, etm_can_my_configuration.serial_number, etm_can_my_configuration.firmware_branch, etm_can_my_configuration.firmware_major_rev, etm_can_my_configuration.firmware_minor_rev);
	break;
      
      case 0xC:
	ETMCanLogCustomPacketC();
	break;
	
      case 0xD:
	ETMCanLogCustomPacketD();
	break;

      case 0xE:
	ETMCanLogCustomPacketE();
	break;

      case 0xF:
	ETMCanLogCustomPacketF();
	break;
      }
  }
}

#endif





//------------------------- For use with pulse sync board only ----------------------- //

void ETMCanPulseSyncSendNextPulseLevel(unsigned int next_pulse_level, unsigned int next_pulse_count) {
  ETMCanMessage lvl_msg;

  lvl_msg.identifier = ETM_CAN_MSG_LVL_TX;
  lvl_msg.word1 = next_pulse_count;
  lvl_msg.word0 = next_pulse_level;
  ETMCanTXMessage(&lvl_msg, &CXTX2CON);
  etm_can_can_status.can_status_tx_2++;
}


//---------------------- For use with Ion Pump board only -------------------------- //
void ETMCanIonPumpSendTargetCurrentReading(unsigned int target_current_reading, unsigned int energy_level, unsigned int pulse_count) {
  ETMCanMessage can_msg;
  
  can_msg.identifier = ETM_CAN_MSG_SET_2_TX | (ETM_CAN_MY_ADDRESS << 3);
  can_msg.word0 = pulse_count;
  can_msg.word1 = target_current_reading;
  if (energy_level == ETM_CAN_HIGH_ENERGY) {
    can_msg.word3 = ETM_CAN_REGISTER_ECB_SET_2_HIGH_ENERGY_TARGET_CURRENT_MON;
  } else {
    can_msg.word3 = ETM_CAN_REGISTER_ECB_SET_2_LOW_ENERGY_TARGET_CURRENT_MON;
  }
  ETMCanTXMessage(&can_msg, &CXTX2CON);
  etm_can_can_status.can_status_tx_2++;
  
}












void ETMCanInitialize(void) {
  _CXIE = 0;
  _CXIF = 0;
  _CXIP = ETM_CAN_INTERRUPT_PRIORITY;
  
  CXINTF = 0;
  
  CXINTEbits.RX0IE = 1; // Enable RXB0 interrupt
  CXINTEbits.RX1IE = 1; // Enable RXB1 interrupt
  CXINTEbits.TX0IE = 1; // Enable TXB0 interrupt
  CXINTEbits.ERRIE = 1; // Enable Error interrupt

  // DPARKER - Zero all the counters in the error structure.
  
  ETMCanBufferInitialize(&etm_can_rx_message_buffer);
  ETMCanBufferInitialize(&etm_can_tx_message_buffer);

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
  CXCTRL = CXCTRL_OPERATE_MODE_VALUE;
  while(CXCTRLbits.OPMODE != 0);
  
#ifdef __ETM_CAN_MASTER_MODULE
  // Set up data structure for ethernet board
  etm_can_ethernet_board_data.can_status    = &etm_can_can_status;
  etm_can_ethernet_board_data.debug_data    = &etm_can_system_debug_data;
  etm_can_ethernet_board_data.configuration = &etm_can_my_configuration;
  etm_can_ethernet_board_data.status_data   = &etm_can_status_register;
#endif


  
  // Enable Can interrupt
  _CXIE = 1;




#ifdef __ETM_CAN_MASTER_MODULE
  T2CON = (T2_OFF & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT);
  PR2 = 976;  // 25mS Period
#else
  T2CON = (T2_OFF & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT);
  PR2 = 3906;  // 100mS period
#endif


  TMR2 = 0;
  _T2IF = 0;
  _T2IE = 0;
  T2CONbits.TON = 1;
}







void __attribute__((interrupt, no_auto_psv)) _CXInterrupt(void) {
  ETMCanMessage can_message;
  
  _CXIF = 0;
  etm_can_can_status.can_status_isr_entered++;

  if(CXRX0CONbits.RXFUL) {
    /*
      A message has been received in Buffer Zero
    */
    if (!CXRX0CONbits.FILHIT0) {
      // The command was received by Filter 0
      etm_can_can_status.can_status_rx_0_filt_0++;
      // It is a Next Pulse Level Command
      ETMCanRXMessage(&can_message, &CXRX0CON);
      // DPARKER set next pulse level to data
      etm_can_next_pulse_level = can_message.word2;
      etm_can_next_pulse_count = can_message.word3;
    } else {
      // The commmand was received by Filter 1
      etm_can_can_status.can_status_rx_0_filt_1++;
#ifdef __ETM_CAN_MASTER_MODULE
      // The command is a data log.  Add it to the data log buffer
      ETMCanRXMessageBuffer(&etm_can_rx_data_log_buffer, &CXRX0CON);
#else
      // The command is a sync command.
      ETMCanRXMessage(&can_message, &CXRX0CON);
      ETMCanDoSync(&can_message);
#endif
    }
    CXINTFbits.RX0IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if(CXRX1CONbits.RXFUL) {
    /* 
       A message has been recieved in Buffer 1
       This command gets pushed onto the command message buffer
    */
    etm_can_can_status.can_status_rx_1_filt_2++;
    ETMCanRXMessageBuffer(&etm_can_rx_message_buffer, &CXRX1CON);
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
    /*
      TX0 is empty and there is a message waiting in the transmit message buffer
      Load the next message into TX0
    */
    ETMCanTXMessageBuffer(&etm_can_tx_message_buffer, &CXTX0CON);
    CXINTFbits.TX0IF = 0;
    etm_can_can_status.can_status_tx_0++;
  }
  
  
  if (CXINTFbits.ERRIF) {
    // There was some sort of CAN Error
    // DPARKER - figure out which error and fix/reset
    etm_can_can_status.can_status_error_flag++;
    CXINTFbits.ERRIF = 0;
  }

  etm_can_can_status.can_status_CXEC_reg = CXEC;
}

