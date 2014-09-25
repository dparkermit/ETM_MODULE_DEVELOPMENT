#ifndef __ETM_CAN_UTILITY_H
#define __ETM_CAN_UTILITY_H


typedef struct {
  unsigned int identifier;
  unsigned int word0;
  unsigned int word1;
  unsigned int word2;
  unsigned int word3;
} ETMCanMessage;


typedef struct {
  unsigned int message_write_index;
  unsigned int message_read_index;
  ETMCanMessage message_data[16];
} ETMCanMessageBuffer;

void ETMCanBufferAdd(ETMCanMessageBuffer* buffer_ptr, volatile unsigned int* data_ptr);
/*
  This stores the data selected by data_ptr (C1RX0SID,C1RX1SID,C2RX0SID,C2RX1SID) into the next available slot in the selected buffer.
  If the buffer is full the data is discarded.
  If you don't want to discard data you need to check the status before calling this function.
*/


void ETMCanBufferRead(ETMCanMessageBuffer* buffer_ptr, ETMCanMessage* message_ptr);
/*
  This stores a copy of the oldest data in the message_ptr location
  If the buffer is empty it returns the error identifier (0b0000111000000000) and fills the data with Zeros.
*/

unsigned int ETMCanBufferRowsAvailable(ETMCanMessageBuffer* buffer_ptr);
/*
  This returns 0 if the buffer is full, otherwise returns the number of available rows
*/

unsigned int ETMCanBufferNotEmpty(ETMCanMessageBuffer* buffer_ptr);
/*
  Returns 0 if the buffer is Empty, otherwise returns the number messages in the buffer
*/










/*
  What functions do we need.

  We need up to two buffers.
  
  * We need a buffer that stores standard commands (that can come in on CAN1 or CAN2, into a single buffer)
     This buffer is 8 (7 available) elements long (80 Bytes) (+ read/write pointers)


  * We need a buffer that stores data logging (that can come in on CAN1 or CAN2, into a single buffer)
     This buffer is 32 (31 available) elements long (320 Bytes) (+ read/write pointers)

  
     


  Functions
 
  ETMCanBufferAdd(&buffer_ptr, &data_ptr)

  &local_data_ptr = ETMCanBufferRead(&buffer_ptr)

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