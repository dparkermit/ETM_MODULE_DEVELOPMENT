#include "ETM_CAN.h"


void ETMCanSetValueBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  switch (index_word) {
    /*
      Place all board specific set values here
    */
  case ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT:
    break;

  default:
    etm_can_can1_errors.board_specific_set_value_unknown++;
    break;
  }
}



#ifdef __ETM_CAN_MASTER_MODULE
void ETMCanUpdateStatusBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int source_board;
  source_board = (message_ptr->identifier >> 3);
  source_board &= 0x000F;
  switch (source_board) {
    /*
      Place all board specific status updates here
    */
  
  default:
    etm_can_can1_errors.update_status_unknown_board++;
    break;

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
    break;

  case ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_OFF:
    break;
      
  default:
    etm_can_can1_errors.board_specific_command_unknown++;
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
    etm_can_can1_errors.board_specific_return_value_unknown++;
    break;
  }
}


void ETMCanResetFaults(void) {
  // Reset faults associated with this board
}

  
#endif
