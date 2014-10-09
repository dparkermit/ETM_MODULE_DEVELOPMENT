#include <p30Fxxxx.h>
#include "ETM_CAN.h"
#include "ETM_CAN_UTILITY.h"
#include "ETM_CAN_CONFIG.h"


ETMCanMessageBuffer etm_can_rx_message_buffer;
ETMCanMessageBuffer etm_can_tx_message_buffer;

#ifdef __MASTER_MODULE
ETMCanMessageBuffer etm_can_rx_data_log_buffer;
#endif

unsigned int etm_can_next_pulse_level;
unsigned int etm_can_next_pulse_count;



void ETMCanSetValue(ETMCanMessage* message_ptr) {
  unsigned int index_word;
  index_word = message_ptr->word3;
}

#ifdef __MASTER_MODULE
void ETMCanUpdateStatus(ETMCanMessage* message_ptr) {
  
}
#endif

#ifndef __MASTER_MODULE
void ETMCanExecuteCMD(ETMCanMessage* message_ptr) {
  
}
#endif


#ifndef __MASTER_MODULE
void ETMCanReturnValue(ETMCanMessage* message_ptr) {

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

#ifdef __MASTER_MODULE
  ETMCanBufferInitialize(&etm_can_rx_data_log_buffer);
#endif
  
  // ---------------- Set up CAN Control Registers ---------------- //
  
  // Set Baud Rate
  C1CTRL = CXCTRL_CONFIG_MODE_VALUE;
  while(C1CTRLbits.OPMODE != 4);
  
  C1CFG1 = CXCFG1_VALUE;
  C1CFG2 = CXCFG2_VALUE;
  
  
  // Load Mask registers for RX0 and RX1
#ifdef __MASTER_MODULE
  C1RXM0SID = ETM_CAN_MASTER_RX0_MASK;
  C1RXM1SID = ETM_CAN_MASTER_RX1_MASK;
#else
  C1RXM0SID = ETM_CAN_SLAVE_RX0_MASK;
  C1RXM1SID = ETM_CAN_SLAVE_RX1_MASK;
#endif

  //C1RXM0SID = 0; // DPARKER testing receive all
  //C1RXM1SID = 0xFFFF; // DPARKER testing receive all
  
  // Load Filter registers
  C1RXF0SID = ETM_CAN_CMD_LVL_FILTER;
  C1RXF3SID = ETM_CAN_CMD_ERROR_RX;
  C1RXF4SID = ETM_CAN_CMD_ERROR_RX;
  C1RXF5SID = ETM_CAN_CMD_ERROR_RX;

#ifdef __MASTER_MODULE
  C1RXF1SID = ETM_CAN_CMD_DATA_LOG_FILTER;
  C1RXF2SID = ETM_CAN_CMD_MASTER_FILTER;
#else
  C1RXF1SID = ETM_CAN_CMD_SYNC_FILTER;
  C1RXF2SID = (ETM_CAN_CMD_SLAVE_FILTER | (ETM_CAN_MY_ADDRESS << 3));
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
  C1CTRL = CXCTRL_LOOPBACK_MODE_VALUE;
  while(C1CTRLbits.OPMODE != 2);
  
  // Enable Can interrupt
  //_C1IE = 1; //DPARKER
}





void ETMCanDoSync(void) {
  // Sync data is available in C1RX0B1->C1RX0B4
  // At this time all that happens is that the chip watchdog is reset
}




void ETMCanProcessCmd(void) {
  ETMCanMessage next_message;
  while (ETMCanBufferNotEmpty(&etm_can_rx_message_buffer)) {
    
    ETMCanReadMessageFromBuffer(&etm_can_rx_message_buffer, &next_message);
    
#ifdef __MASTER_MODULE
    if ((next_message.identifier & ETM_CAN_SET_2_MASK) == ETM_CAN_CMD_SET_2) {
      ETMCanSetValue(&next_message);      
    } else if ((next_message.identifier & ETM_CAN_STATUS_MASK) == ETM_CAN_CMD_STATUS) {
      ETMCanUpdateStatus(&next_message);
    } else {
      // DPARKER there was an unrecognized command.  Update some error counter
    } 
#else
    if (next_message.identifier == (ETM_CAN_CMD_CMD_TX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanExecuteCMD(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_CMD_SET_1_TX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);      
    } else if (next_message.identifier == (ETM_CAN_CMD_REQUEST_TX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanReturnValue(&next_message);
    } else if ((next_message.identifier & ETM_CAN_CMD_SET_3_RX_MASK) == (ETM_CAN_CMD_SET_3_RX | (ETM_CAN_MY_ADDRESS << 3))) {
      ETMCanSetValue(&next_message);
    } else {
      // DPARKER there was an unrecognized command.  Update some error counter
    } 
#endif

  }
}


void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void) {
  _C1IF = 0;
  
  
  if(C1RX0CONbits.RXFUL) {
    /*
      A message has been received in Buffer Zero
    */
    if (C1RX0CONbits.FILHIT0) {
      // The command was received by Filter 0
      // It is a Next Pulse Level Command
      
      // DPARKER set next pulse level to data
      etm_can_next_pulse_level = C1RX0B1;
      etm_can_next_pulse_count = C1RX0B2;
      C1RX0CONbits.RXFUL = 0; // Indicate that the buffer is now free
    } else {
      // The commmand was received by Filter 1
#ifdef __MASTER_MODULE
      // The command is a data log.  Add it to the data log buffer
      ETMCanRXMessageBuffer(&etm_can_rx_data_log_buffer, &C1RX0SID);
#else
      // The command is a sync command.
      ETMCanDoSync();
      C1RX0CONbits.RXFUL = 0; // Indicate that the buffer is now free
      // DPARKER figure out how this works
#endif
    }
    C1INTFbits.RX0IF = 0; // Clear the Interuppt Status bit
  }
  
  
  if(C1RX1CONbits.RXFUL) {
    /* 
       A message has been recieved in Buffer 1
       This command gets pushed onto the command message buffer
    */
    ETMCanRXMessageBuffer(&etm_can_rx_message_buffer, &C1RX1SID); 
    C1INTFbits.RX1IF = 0; // Clear the Interuppt Status bit
    C1RX1CONbits.RXFUL = 0; // Indicate that the buffer is now free
  }
  
  
  if ((!C1TX0CONbits.TXREQ) && (ETMCanBufferNotEmpty(&etm_can_tx_message_buffer))) {
    /*
      TX0 is empty and there is a message waiting in the transmit message buffer
      Load the next message into TX0
    */
    ETMCanTXMessageBuffer(&etm_can_tx_message_buffer, &C1TX0SID);
    C1INTFbits.TX0IF = 0;
    C1TX0CONbits.TXREQ = 1;
  }
    
  
  if (C1INTFbits.ERRIF) {
    // There was some sort of CAN Error
    // DPARKER - figure out which error and fix/reset
    C1INTFbits.ERRIF = 0;
  }
}

