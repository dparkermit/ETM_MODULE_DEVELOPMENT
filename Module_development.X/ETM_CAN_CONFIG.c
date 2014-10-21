#include "ETM_CAN.h"


void ETMCanSetValueBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  switch (index_word) {
    /*
      Place all board specific set values here
    */
  case ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT:
    etm_can_status_register.data_word_A = message_ptr->word0;
    break;

  default:
    etm_can_system_debug_data.board_specific_set_value_unknown++;
    break;
  }
}



#ifdef __ETM_CAN_MASTER_MODULE
ETMCanDataStructure etm_can_slave_data;
ETMCanHighSpeedData etm_can_high_speed_data_test;

void ETMCanUpdateStatusBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int source_board;
  source_board = (message_ptr->identifier >> 3);
  source_board &= 0x000F;
  switch (source_board) {
    /*
      Place all board specific status updates here
    */
  
  default:
    etm_can_system_debug_data.update_status_unknown_board++;
    break;

  }
}


void ETMCanProcessLogData(void) {
  ETMCanMessage next_message;
  unsigned int data_log_index;
  ETMCanSystemDebugData* debug_data_ptr; 
  while (ETMCanBufferNotEmpty(&etm_can_rx_data_log_buffer)) {
    ETMCanReadMessageFromBuffer(&etm_can_rx_data_log_buffer, &next_message);
    data_log_index = next_message.identifier;
    data_log_index >>= 3;
    data_log_index &= 0x00FF;
    
    if ((data_log_index & 0x000F) <= 0x000B) {
      // It is a common data logging buffer, load into the appropraite ram location
      
      switch (data_log_index >> 4) 
	{
	case ETM_CAN_ADDR_ION_PUMP_BOARD:
	  debug_data_ptr = &etm_can_slave_data.ionpump_debug_data;
	  break;
	  
	case ETM_CAN_ADDR_MAGNETRON_CURRENT_BOARD:
	  debug_data_ptr = &etm_can_slave_data.magmon_debug_data;
	  break;
	  
	case ETM_CAN_ADDR_HV_LAMBDA_BOARD:
	  debug_data_ptr = &etm_can_slave_data.hvlambda_debug_data;
	  break;
	  
	  
	  // DPARKER add in all the other boards
	  
	default:
	  //DPARKER add in some error counter
	  break;
	  
	}

      
      switch (data_log_index & 0x000F)
	{
	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_0:
	  debug_data_ptr->i2c_bus_error_count        = next_message.word3;
	  debug_data_ptr->spi_bus_error_count        = next_message.word2;
	  debug_data_ptr->can_bus_error_count        = next_message.word1;
	  debug_data_ptr->scale_error_count          = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_1:
	  debug_data_ptr->reset_count                = next_message.word3;
	  debug_data_ptr->self_test_result_register  = next_message.word2;
	  debug_data_ptr->reserved_0                 = next_message.word1;
	  debug_data_ptr->reserved_1                 = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_0:
	  debug_data_ptr->debug_0                    = next_message.word3;
	  debug_data_ptr->debug_1                    = next_message.word2;
	  debug_data_ptr->debug_2                    = next_message.word1;
	  debug_data_ptr->debug_3                    = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_1:
	  debug_data_ptr->debug_4                    = next_message.word3;
	  debug_data_ptr->debug_5                    = next_message.word2;
	  debug_data_ptr->debug_6                    = next_message.word1;
	  debug_data_ptr->debug_7                    = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_2:
	  debug_data_ptr->debug_8                    = next_message.word3;
	  debug_data_ptr->debug_9                    = next_message.word2;
	  debug_data_ptr->debug_A                    = next_message.word1;
	  debug_data_ptr->debug_B                    = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_3:
	  debug_data_ptr->debug_C                    = next_message.word3;
	  debug_data_ptr->debug_D                    = next_message.word2;
	  debug_data_ptr->debug_E                    = next_message.word1;
	  debug_data_ptr->debug_F                    = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_0:
	  // DPARKER add configuration data
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_1:
	  // DPARKER add configuration data
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_0:
	  debug_data_ptr->unknown_message_identifier           = next_message.word3;
	  debug_data_ptr->message_index_does_not_match_board   = next_message.word2;
	  debug_data_ptr->message_not_addressed_to_this_board  = next_message.word1;
	  debug_data_ptr->update_status_unknown_board          = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_1:
	  debug_data_ptr->can_module_error_flag                = next_message.word3;
	  debug_data_ptr->command_index_not_valid              = next_message.word2;
	  debug_data_ptr->default_command_index_not_recognized = next_message.word1;
	  debug_data_ptr->board_specific_command_unknown       = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_2:
	  debug_data_ptr->can_module_error_flag                = next_message.word3;
	  debug_data_ptr->command_index_not_valid              = next_message.word2;
	  debug_data_ptr->default_command_index_not_recognized = next_message.word1;
	  debug_data_ptr->board_specific_command_unknown       = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_3:
	  debug_data_ptr->can_commands_processed               = next_message.word3;
	  debug_data_ptr->can_rx_0_count                       = next_message.word2;
	  debug_data_ptr->can_rx_1_count                       = next_message.word1;
	  debug_data_ptr->can_tx_0_count                       = next_message.word0;

	  break;
	  
	} 

    } else {
      // It is board specific logging data
      switch (data_log_index) 
	{
	case ETM_CAN_DATA_LOG_REGISTER_HV_LAMBDA_FAST_PROGRAM_VOLTAGE:
	  etm_can_high_speed_data_test.pulse_count = next_message.word3;
	  etm_can_high_speed_data_test.hvlambda_readback_high_energy_lambda_program_voltage = next_message.word2;
	  etm_can_high_speed_data_test.hvlambda_readback_low_energy_lambda_program_voltage = next_message.word1;
	  etm_can_high_speed_data_test.hvlambda_readback_peak_lambda_voltage = next_message.word0;
	  break;

	case ETM_CAN_DATA_LOG_REGISTER_HV_LAMBDA_SLOW_SET_POINT:
	  etm_can_slave_data.hvlambda_readback_high_energy_set_point = next_message.word2;
	  etm_can_slave_data.hvlambda_readback_low_energy_set_point = next_message.word1;
	  etm_can_slave_data.hvlambda_readback_base_plate_temp = next_message.word0;
	  break;



	default:
	  //DPARKER add some error
	  break;
	}
    }
  }
}

#else

void ETMCanExecuteCMDBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  index_word &= 0x0FFF;
  switch (index_word) {
    /*
      Place all board specific commands here
    */
    
    
  case ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_ON:
    etm_can_status_register.data_word_B = 0x0000;
    break;

  case ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_OFF:
    etm_can_status_register.data_word_B = 0xFFFF;
    break;
      
  default:
    etm_can_system_debug_data.board_specific_command_unknown++;
    break;
  }
}


void ETMCanReturnValueBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  index_word &= 0x0FFF;
  switch (index_word) {
    
    /*
      Place all board specific return value commands here
    */
  default:
    etm_can_system_debug_data.board_specific_return_value_unknown++;
    break;
  }
}


void ETMCanResetFaults(void) {
  // Reset faults associated with this board
}

  
#endif
