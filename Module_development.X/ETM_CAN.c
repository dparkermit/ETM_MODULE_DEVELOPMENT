

ETMCanMessageBuffer can_input_message_buffer;

ETMCanMessageBuffer can_output_message_buffer;




#ifdef __MASTER_MODULE
ETMCanMessageBuffer can_data_logging_buffer;
#endif



/*
  Can Resources

  For Master Modules (Ethernet Board)
  CAN1 & CAN2 are used
  Data Logging Information is filtered into CAN1/CAN2 Receive Buffer 0
  All other Can communication is filtered into CAN1/CAN2 Receive Buffer 1


  For Non Master Modules (Everything Else)
  Only CAN1 is used
  Timestamp Broadcast is filtered into CAN1 Receive Buffer 0
  All other Can communication is filtered into CAN1 Receive Buffer 1



*/



/*
  X = Not Implimented
  A = Address
  C = Command Tupe
                                           0bXXXC CCCA AAAA AAX0
*/
// CAN Mask & Filters for Buffer 1
#define ETHERNET_CONTROL_BOARD_ADR_MASK    0b0000 0001 0000 0001   
#define ETHERNET_CONTROL_BOARD_ADR_FILTER  0b0000 0000 0000 0000

#define ION_PUMP_BOARD_ADR_MASK            0b0000 0000 1000 0101
#define ION_PUMP_BOARD_ADR_FILTER          0b0000 0000 0000 0000

#define MAGNETRON_I_MON_BOARD_ADR_MASK     0b0000 0000 0100 0101
#define MAGNETRON_I_MON_BOARD_ADR_FILTER   0b0000 0000 0000 0000


#define ALL_OTHER_BOARD_ADR_MASK           0b0000 0001 1111 1101  

#define PULSE_SYNC_BOARD_ADR_FILTER        0b0000 0001 1100 0000
#define HV_LAMBDA_BOARD_ADR_FILTER         0b0000 0001 1100 1100
#define AFC_CONTROL_BOARD_ADR_FILTER       0b0000 0001 1101 0100
#define COOLING_SYSTEM_BOARD_ADR_FILTER    0b0000 0001 1101 1100
#define HEATER_MAGNET_BOARD_ADR_FILTER     0b0000 0001 1110 0100
#define GUN_DRIVER_BOARD_ADR_FILTER        0b0000 0001 1110 1100


// Message Type Filters for Buffer 1
#define MSG_TYPE_PULSE_JUST_HAPPENDED      0b0000 0000 0000 0000
#define MSG_TYPE_EXECUTE_COMMAND           0b0000 0100 0000 0000
#define MSG_TYPE_SET_VALUE                 0b0000 0110 0000 0000
#define MSG_TYPE_RESPOND_TO_REQUEST        0b0000 1110 0000 0000
#define MSG_TYPE_ERROR                     0b0001 1000 0000 0000



// CAN Mask & Filters for Buffer 0
#define BROADCAST_SET_DATE_TIME_ADR_MASK   0b0001 1110 0000 0001
#define BROADCAST_SET_DATE_TIME_ADR_FILTER 0b0000 0010 0000 0000

#define ETHERNET_BOARD_DATA_LOG_ADR_MASK   0b0001 1100 0000 0001
#define ETHERNET_BOARD_DATA_LOG_ADR_FILTER 0b0001 1100 0000 0000



void ETMCanSendMessage() {
  // First read the message out of the buffer
  
  // Load the 5 words into the register

  // Set the C1TXNDLC Register (data length, remote request, ect)

  C1TXNCONbits.TXREQ = 1;

}

#define ETM_CAN_BOARD_BUFFER_ZERO_MASK
#define ETM_CAN_BOARD_BUFFER_ONE_MASK

#define ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ZER0
#define ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ONE

#define ETM_CAN_BOARD_BUFFER_ONE_FILTER_TWO
#define ETM_CAN_BOARD_BUFFER_ONE_FILTER_THREE
#define ETM_CAN_BOARD_BUFFER_ONE_FILTER_FOUR
#define ETM_CAN_BOARD_BUFFER_ONE_FILTER_FIVE

void ETMCanInitialize() {
  _C1IE = 0;
  _C1IF = 0;
  _C1IP = ETM_CAN_INTERRUPT_PRIORITY;

  C1INTF = 0;

  C1INTEbits.RX0IE = 1; // Enable RXB0 interrupt
  C1INTEbits.RX1IE = 1; // Enable RXB1 interrupt
  C1INTEbits.TX0IE = 1; // Enable TXB0 interrupt
  C1INTEbits.ERRIE = 1; // Enable Error interrupt
  

  can_input_message_buffer.write_index = 0;
  can_input_message_buffer.read_index = 0;

  can_output_message_buffer.write_index = 0;
  can_output_message_buffer.read_index = 0;


  // -------- Set up Control Registers ------------- //
  C1CTRLbits.TSTAMP = 0;
  C1CTRLbits.CSIDL = 0;
  C1CTRLbits.CANCKS = 1;
  C1CTRLbits.REQOP = 4; // Request Configuration Mode
  while(C1CTRLbits.OPMODE != 4);
  
  C1CFG1bits.SJW = 0;
  C1CFG1bits.BRP = 0;
  
  C1CFG2bit.WAKEFIL = 0;
  C1CFG2bit.SEG2PH = 3;
  C1CFG2bit.SEG2PHTS = 1;
  C1CFG2bit.SAM = 0; 
  C1CFG2bit.SEG1PH = 2;
  C1CFG2bit.PRSEG = 1;

  // Load Mask registers
  C1RXM0SID = ETM_CAN_BOARD_BUFFER_ZERO_MASK;
  C1RXM1SID = ETM_CAN_BOARD_BUFFER_ONE_MASK;
  

  // Load Filter registers
  C1RXF0SID = ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ZER0;
  C1RXF1SID = ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ONE;
  C1RXF2SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_TWO;
  C1RXF3SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_THREE
  C1RXF4SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_FOUR;
  C1RXF5SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_FIVE;

  // Set Transmitter Mode
  C1TX0CONbits.TXREG = 0;
  C1TX0CONbits.TXPRI = 3;
  
  // Set Receiver Mode
  C1RX0CONbits.RXFUL = 0;
  C1RX0CONbits.DBEN = 0;

  C1RX1CONbits.RXFUL = 0;

  // Switch to normal operation
  C1CTRLbits.REQOP = 0; // Request Configuration Mode
  while(C1CTRLbits.OPMODE != 0);

  _C1IE = 1;

#ifdef __MASTER_MODULE
  // Enable C2

  _C2IE = 0;
  _C2IF = 0;
  _C2IP = ETM_CAN_INTERRUPT_PRIORITY;

  C2INTF = 0;

  C2INTEbits.RX0IE = 1; // Enable RXB0 interrupt
  C2INTEbits.RX1IE = 1; // Enable RXB1 interrupt
  C2INTEbits.TX0IE = 1; // Enable TXB0 interrupt
  C2INTEbits.ERRIE = 1; // Enable Error interrupt
  

  can_input_message_buffer.write_index = 0;
  can_input_message_buffer.read_index = 0;

  can_output_message_buffer.write_index = 0;
  can_output_message_buffer.read_index = 0;


  // -------- Set up Control Registers ------------- //
  C2CTRLbits.TSTAMP = 0;
  C2CTRLbits.CSIDL = 0;
  C2CTRLbits.CANCKS = 1;
  C2CTRLbits.REQOP = 4; // Request Configuration Mode
  while(C2CTRLbits.OPMODE != 4);
  
  C2CFG1bits.SJW = 0;
  C2CFG1bits.BRP = 0;
  
  C2CFG2bit.WAKEFIL = 0;
  C2CFG2bit.SEG2PH = 3;
  C2CFG2bit.SEG2PHTS = 1;
  C2CFG2bit.SAM = 0; 
  C2CFG2bit.SEG1PH = 2;
  C2CFG2bit.PRSEG = 1;

  // Load Mask registers
  C2RXM0SID = ETM_CAN_BOARD_BUFFER_ZERO_MASK;
  C2RXM1SID = ETM_CAN_BOARD_BUFFER_ONE_MASK;
  

  // Load Filter registers
  C2RXF0SID = ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ZER0;
  C2RXF1SID = ETM_CAN_BOARD_BUFFER_ZERO_FILTER_ONE;
  C2RXF2SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_TWO;
  C2RXF3SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_THREE
  C2RXF4SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_FOUR;
  C2RXF5SID = ETM_CAN_BOARD_BUFFER_ONE_FILTER_FIVE;

  // Set Transmitter Mode
  C2TX0CONbits.TXREG = 0;
  C2TX0CONbits.TXPRI = 3;
  
  // Set Receiver Mode
  C2RX0CONbits.RXFUL = 0;
  C2RX0CONbits.DBEN = 0;

  C2RX1CONbits.RXFUL = 0;

  // Switch to normal operation
  C2CTRLbits.REQOP = 0; // Request Configuration Mode
  while(C2CTRLbits.OPMODE != 0);

  _C2IE = 1;
#endif

}


void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void) {
  _C1IF = 0;

  if(C1INTFbits.RX0IF) {
#ifdef __MASTER_MODULE
    // The Ethernet Board has received a data logging packet
    ETMCanBufferAdd(&can_data_logging_buffer, &C1RX0SID);
#else
    // A board has received a data time packed
    // DPARKER sync to local date and time
    // ETMCanSetDateTime();
#endif
    C1INTFbits.RX0IF = 0; // Clear the Interuppt Status bit
    C1RX0CONbits.RXFUL = 0;
  }
    
  if(C1INTFbits.RX1IF) {      
    /* 
       A message has been recieved in Buffer 1
       This is a standard command
       Add it to the input buffer
    */
    ETMCanBufferAdd(&can_input_message_buffer, &C1RX1SID); 
    C1INTFbits.RX1IF = 0; // Clear the Interuppt Status bit
    C1RX1CONbits.RXFUL = 0;
  }

  if (C1INTFbits.TX0IF) {
    /*
      A message has been sent from Buf 0
      Load the next message into Buf 0
    */
    C1INTFbits.TX0IF = 0;
  }

  

  if (C1INTFbits.ERRIF) {
    // There was some sort of CAN Error
    // DPARKER - figure out which error and fix/reset
    C1INTFbits.ERRIF = 0;
  }
  
}






void DoCanCmd() {
  ETMCanMessage can_message;
  unsigned int message_id;
  if (ETMCanBufferNotEmpty(&can_input_message_buffer)) {
    ETMCanBufferRead(&can_input_message_buffer, &can_message);
    
    message_id = can_message.identifier & 0b0001111111111100;
    message_id >>= 2;

    switch (can_message.identifier) {

    case CAN_TYPE_SET_DATE_TIME:
      break;



    }
    
  }

  

  message_type = CXRX0SID & 0b0001111000000000;
  
  switch (message_type) {
  
  case CAN_TYPE_BROADCAST_SET_DATE_TIME:
    break;
    
  case CAN_TYPE_EXECUTE_COMMAND:
    break;
    
  case CAN_TYPE_SET_REMOTE_VALUE:
    break;
    
  case CAN_TYPE_REQUEST_REMOTE_VALUE:
    break;
    
  case CAN_TYPE_REQUEST_RESPONSE:
    break;
  }
}












































#ifdef __USE_CAN_1

#define _CXIE                       _C1IE
#define _CXIF                       _C1IF
#define _CXIP                       _C1IP

#define CXTX0CON                    C1TX0CON
#define CXTX1CON                    C1TX1CON
#define CXINTF                      C1INTF

#define CXCTRLbits                  C1CTRLbits
#define CXTX0CONbits                C1TX0CONbits
#define CXTX1CONbits                C1TX1CONbits
#define CXINTFbits                  C1INTFbits
#define CXRX0CONbits                C1RX0CONbits
#define CXINTEbits                  C1INTEbits

#define CANXSetOperationMode        CAN1SetOperationMode
#define CANXInitialize              CAN1Initialize
#define CANXSetFilter               CAN1SetFilter
#define CANXSetMask                 CAN1SetMask
#define CANXSetTXMode               CAN1SetTXMode
#define CANXSetRXMode               CAN1SetRXMode
#define CANXIsTXReady               CAN1IsTXReady
#define CANXSendMessage             CAN1SendMessage
#define CANXReceiveMessage          CAN1ReceiveMessage

#else

#define _CXIE                       _C2IE
#define _CXIF                       _C2IF
#define _CXIP                       _C2IP

#define CXTX0CON                    C2TX0CON
#define CXTX1CON                    C2TX1CON
#define CXINTF                      C2INTF

#define CXCTRLbits                  C2CTRLbits
#define CXTX0CONbits                C2TX0CONbits
#define CXTX1CONbits                C2TX1CONbits
#define CXINTFbits                  C2INTFbits
#define CXRX0CONbits                C2RX0CONbits
#define CXINTEbits                  C2INTEbits

#define CANXSetOperationMode        CAN2SetOperationMode
#define CANXInitialize              CAN2Initialize
#define CANXSetFilter               CAN2SetFilter
#define CANXSetMask                 CAN2SetMask
#define CANXSetTXMode               CAN2SetTXMode
#define CANXSetRXMode               CAN2SetRXMode
#define CANXIsTXReady               CAN2IsTXReady
#define CANXSendMessage             CAN2SendMessage
#define CANXReceiveMessage          CAN2ReceiveMessage

#endif




// Receive Buffer 1 - Mask and Filter Registers
#define RB1_MESSAGE_ACCEPTANCE_MASK_DATA_LOG       0b10000000000
#define RB1_MESSAGE_ACCEPTANCE_FILTER_DATA_LOG     0b10000000000
#define RB1_MESSAGE_ACCEPTANCE_MASK_TIMESTAMP      0b11110000000
#define RB1_MESSAGE_ACCEPTANCE_FILTER_TIMESTAMP    0b00000000000

// Receive Buffer 0 - Mask and Filter Registers
#define RB0_MESSAGE_ACCEPTANCE_STANDARD            0b10000001111
#define RB0_MESSAGE_ACCEPTANCE_FILTER_STANDARD_0  (0b00000000000 + CAN_LOCAL_ADDRESS)
#define RB0_MESSAGE_ACCEPTANCE_FILTER_STANDARD_1  (0b00000000000 + CAL_ALTERNATE_LOCAL_ADDRESS) 

#define CAN_TYPE_EXECUTE_COMMAND                   0b0000001000000000
#define CAN_TYPE_SET_REMOTE_VALUE                  0b0000010000000000
#define CAN_TYPE_REQUEST_REMOTE_VALUE              0b0000101000000000
#define CAN_TYPE_REQUEST_RESPONSE                  0b0000100000000000


void DoCanCmd() {
  unsigned int message_type;
  message_type = CXRX0SID & 0b0001111000000000;
  
  switch (message_type) {
  
  case CAN_TYPE_BROADCAST_SET_DATE_TIME:
    break;
    
  case CAN_TYPE_EXECUTE_COMMAND:
    break;
    
  case CAN_TYPE_SET_REMOTE_VALUE:
    break;
    
  case CAN_TYPE_REQUEST_REMOTE_VALUE:
    break;
    
  case CAN_TYPE_REQUEST_RESPONSE:
    break;
  }
}




ETMCanSetDateTime();

ETMCanSendDateTime();

ETMCanExecuteCmd();

ETMCanSetValue();

ETMCanRequestData();

ETMCanDoDataRequest();

ETMCanSendLogData();

ETMCanRecieveLogData();



unsigned int  can1_buffer0_input_message_buffer[5][8];
unsigned char can1_buffer0_input_message_read_index = 0;
unsigned char can1_buffer0_input_message_write_index = 1;


void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void) {
  _C1IF = 0;


  if(C1INTFbits.RX0IF) {      
    // A message has been recieved in Buffer 0
    // This is a standard command
    // Add it to the input buffer
    ETMCan1ReceiveRX0MessageWithIdentifier(&can1_buffer0_input_message_buffer[can1_buffer0_input_message_write_index][0]); 
    can1_buffer0_input_message_write_index++;
    can1_buffer0_input_message_write_index &= 0x07;
  }

  if(CXINTFbits.RX0IF) {      
    CXINTFbits.RX0IF = 0; 	//If the Interrupt is due to Receive0 of CAN2 Clear the Interrupt
    
    CANXReceiveMessage(&can_input_message_buffer[can_input_message_write_index][0], 8, 0);
    CXRX0CONbits.RXFUL = 0;
    can_input_message_write_index++;
    can_input_message_write_index &= 0x0F;
    if (can_input_message_write_index == can_input_message_read_index) {
      can_input_message_read_index++;
      can_input_message_read_index &= 0x0F;
    }
  }
}
