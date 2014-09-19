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


#define __MASTER_NODE    // This is the master board.  It sends out timestamp pulses and uses receive buffer 1 to receive logging data
                         // Else this is a slave board.  It sends out logging data and uses receive buffer 1 to receive timestamp pulses 

#ifdef __MASTER_NODE

#else

#endif



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
  _C1IF = 0;         //Clear interrupt flag


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
