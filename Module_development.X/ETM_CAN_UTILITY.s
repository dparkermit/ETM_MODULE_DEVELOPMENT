.ifdef __dsPIC30F
        .include "p30fxxxx.inc"
.endif


.section .nbss, bss, near    	
	_can1_buffer0_input_message_write_index:	.space 2
	_can1_buffer0_input_message_read_index:	        .space 2
	_can1_buffer0_input_message_buffer:	  	.space 80
.text


	
	;; ----------------------------------------------------------

.global  _ETMCan1RX0BufferNotEmpty
	;; uses and does not restore W0
.text
_ETMCan1RX0BufferNotEmpty:
	MOV 		_can1_buffer0_input_message_read_index, W0
	SUB             _can1_buffer0_input_message_write_index, WREG
	AND 		#7, W0
Return

.global  _ETMCan1RX0BufferSlotsAvailable
	;; uses and does not restore W0
.text
_ETMCan1RX0BufferSlotsAvailable:
	MOV  		_can1_buffer0_input_message_write_index, W0
	SUB		_can1_buffer0_input_message_read_index, WREG
	DEC		W0,W0
	AND		#7,W0
Return
	

	
	
.global  _ETMCan1RX0BufferAdd
	;; uses and does not restore W0,W1
.text
_ETMCan1RX0BufferAdd:	
	;; If the buffer is full, do not do anything
	INC		_can1_buffer0_input_message_write_index, WREG
	AND		#7, W0
	CP		_can1_buffer0_input_message_read_index
	BRA		Z, _ETMCan1RX0BufferAdd_BUFFER_FULL
	
	;; Calculate the address of this entry into the buffer (store in W0)
	MOV		_can1_buffer0_input_message_write_index, W1
	MUL.UU		W1,#10,W0
	MOV		#_can1_buffer0_input_message_buffer, W1
	ADD             W0,W1,W0

	;; Copy the data from the can registers to the buffer
	MOV             C1RX0SID, W1 
	MOV		W1,[W0++]

	MOV		C1RX0B1, W1
	MOV             W1,[W0++]

	MOV		C1RX0B2, W1
	MOV             W1,[W0++]

	MOV		C1RX0B3, W1
	MOV             W1,[W0++]

	MOV		C1RX0B4, W1	
	MOV             W1,[W0++]

	;; Increment the write location
	INC             _can1_buffer0_input_message_write_index, WREG
	AND 		#7, W0
	MOV		W0, _can1_buffer0_input_message_write_index
_ETMCan1RX0BufferAdd_BUFFER_FULL:	
	
RETURN



	
.global  _ETMCan1RX0BufferReadData
	;; uses and does not restore W0,W1,W2,W3
.text
_ETMCan1RX0BufferReadData:
	;; Address of Return Data
	MOV		W0,W4

	;; Caclulate our Base Address
	MOV		_can1_buffer0_input_message_read_index, W0
	MUL.UU		W0,#10,W2
	MOV		#_can1_buffer0_input_message_buffer, W1
	ADD             W2,W1,W1                                      	;; W1 is the base address of this Read

	;; See if the buffer is empty
	CP		_can1_buffer0_input_message_write_index
	BRA             NZ, _ETMCan1RX0BufferReadData_NOT_EMPTY
	;; If we got here, the buffer was EMPTY, Need to Fudge with the return data
	;; Do this by setting the ID register to an invalid number, and stuffing the data with Zeros
	MOV		#0b0000111000000000, W2
	MOV             W2, [W4++]
	MOV		#0, W2
	MOV		W2, [W4++]
	MOV		W2, [W4++]
	MOV		W2, [W4++]
	MOV		W2, [W4++]
	BRA		_ETMCan1RX0BufferReadData_DONE	
_ETMCan1RX0BufferReadData_NOT_EMPTY:	
	;; Load the data
	MOV             [W1++], [W4++]
	MOV             [W1++], [W4++]
	MOV             [W1++], [W4++]
	MOV             [W1++], [W4++]
	MOV             [W1++], [W4++]
	;;  Iincrement to read pointer	
	INC		W0,W0
	AND 		#7, W0
	MOV		W0, _can1_buffer0_input_message_read_index
_ETMCan1RX0BufferReadData_DONE:	

RETURN


	
