#include "ETM_CAN.h"


void ETMCanSetValueBoardSpecific(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
  index_word &= 0x0FFF;
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



#ifndef __MASTER_MODULE
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
  
}


void ETMCanResetFaults(void) {
  // Reset faults associated with this board
}

  
#endif
