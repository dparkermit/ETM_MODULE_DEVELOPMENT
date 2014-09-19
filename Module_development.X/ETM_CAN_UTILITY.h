#ifndef __ETM_CAN_UTILITY_H
#define __ETM_CAN_UTILITY_H


typedef struct {
  unsigned int identifier;
  unsigned int word0;
  unsigned int word1;
  unsigned int word2;
  unsigned int word3;
} ETMCanMessage;



void ETMCan1RX0BufferAdd();
/*
  This stores the data in the next available buffer slot
  If the buffer is full the data is discarded
  If you don't want to discard data you need to check the status before calling this function
*/

void ETMCan1RX0BufferReadData(ETMCanMessage* message);
/*
  This returns a copy of the oldest data in the buffer to the message
  If the buffer is empty it returns the error identifier (0b0000111000000000) and fills the data with Zeros.
*/

unsigned int ETMCan1RX0BufferNotEmpty();
/*
  Returns 0 if the buffer is empty.
  If the buffer is not empty it returns the number of slots with data
*/

unsigned int ETMCan1RX0BufferNotFull();
/*
  Returns 0 if the buffer is Full.
  If the buffer is not full, it returns the number of empty slots
*/




/*
  What functions do we need.

  We need up to two buffers.
  
  * We need a buffer that stores standard commands (that can come in on CAN1 or CAN2, into a single buffer)
     This buffer is 8 (7 available) elements long (80 Bytes) (+ read/write pointers)


  * We need a buffer that stores data logging (that can come in on CAN1 or CAN2, into a single buffer)
     This buffer is 32 (31 available) elements long (320 Bytes) (+ read/write pointers)

  
  Functions
 

  *** For the standard command ***
  ETMCanBufferAddCan1RX0
  ETMCanBufferAddCan1RX1
  ETMCanBufferAddCan2RX0
  ETMCanBufferAddCan2RX1

  ETMCanBufferNotFull()
  ETMCanBufferNotEmpty()

  ETMCanBufferRead(ETMCanMessage* message)



  *** For the Data Logging Commands ***
  ETMCanLoggingBufferAddCan1RX0
  ETMCanLoggingBufferAddCan1RX1
  ETMCanLoggingBufferAddCan2RX0
  ETMCanLoggingBufferAddCan2RX1

  ETMCanLoggingBufferNotFull
  ETMCanLoggingBufferNotEmpty
  
  ETMCanLoggingBufferRead(ETMCanMessage* message)

*/


#endif
