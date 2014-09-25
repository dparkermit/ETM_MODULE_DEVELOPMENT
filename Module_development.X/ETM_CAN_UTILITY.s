.ifdef __dsPIC30F
        .include "p30fxxxx.inc"
.endif


.equ buffer_length, 15
.equ error_identifier, 0b0000111000000000




.global  _ETMCanBufferRowsAvailable
	;; Address of the Buffer Data Structure is in W0
	;; Uses W0,W1,W2,SR
.text
_ETMCanBufferRowsAvailable:
	MOV		[W0+0x2], W2 ; Move read_index to W2
	MOV		[W0], W1     ; Move write_index to W1
	SUB		W2, W1, W0
	DEC		W0, W0
	AND		#buffer_length, W0
Return
	



.global  _ETMCanBufferNotEmpty
	;; Address of the Buffer Data Structure is in W0
	;; Uses W0,W1,W2,SR	
.text
_ETMCanBufferNotEmpty:
	MOV		[W0+0x2], W2 ; Move read_index to W1
	MOV		[W0], W1     ; Move write_index to W1
	SUB		W1,W2,W0
	AND		#buffer_length, W0
Return

	

	
.global  _ETMCanBufferAdd
	;; Uses W0,W1,W2,W3,SR
	;; Address of the Buffer Data Structure is in W0
	;; Address of the Data to add is in W1
.text
_ETMCanBufferAdd:
	;; Check to see if the buffer is full
	;; WO initial points to the write_index
	MOV		[W0+0x2], W3 ; Move read index to W3
	INC             [W0], W2 ; W2 = Write Index + 1
	AND		#buffer_length, W2 ; Wrap the write index
	CP		W2, W3 ; If (write_index +1) = Read Index 
	BRA		Z, _ETMCanBufferAdd_BUFFER_FULL

	;; Calculate where the data should be added
	MOV		[W0], W2
	MUL.UU 		W2,#10,W2 ; W2 is now the offset based on write index 
	ADD		W0,#4, W3 ; Move Start of data to W4
	ADD		W3,W2,W3 ; W3 is not the start address for this data row 

	;; Copy the data from the SFRs to the data buffer
	MOV		[W1], [W3++]   ; Copy SID to Data
	ADD		W1,#6,W1       ; W1 now points to SFR Data 1
	MOV		[W1++], [W3++] ; Copy SFR data 1 to RAM
	MOV		[W1++], [W3++] ; Copy SFR data 2 to RAM
	MOV		[W1++], [W3++] ; Copy SFR data 3 to RAM
	MOV		[W1++], [W3++] ; Copy SFR data 4 to RAM	

	;; Increment the write Index and store
	INC		[W0], W2 ; W2 = Write Index + 1
	AND		#buffer_length, W2 ; Wrap the write index
	MOV		W2, [W0]

_ETMCanBufferAdd_BUFFER_FULL:	

RETURN





	
.global  _ETMCanBufferRead
	;; Address of the Buffer Data Structure is in W0
	;; Address of Message (return data) is in W1
	;; Uses W0,W1,W2,W3,SR
.text
_ETMCanBufferRead:
	;; See if the buffer is empty (the read and write index are the same)
	MOV		[W0+0x2], W2 ; Move read_index to W2
	MOV		[W0], W3     ; Move write_index to W3
	CP		W2,W3
	BRA		NZ, _ETMCanBufferRead_NOT_EMPTY

	;; The Buffer was empty, stuff the return data with error numbers
	;; Do this by setting the ID register to the error_identifier and stuffing the data with Zeros
	MOV		#error_identifier, W3
	MOV             W3, [W1++]
	MOV		#0, W3
	MOV		W3, [W1++]
	MOV		W3, [W1++]
	MOV		W3, [W1++]
	MOV		W3, [W1++]
	BRA		_ETMCanBufferRead_DONE	

_ETMCanBufferRead_NOT_EMPTY:

	;; First calculate the address for the data that we want
	MUL.UU		W2, #10, W2
	ADD		W2, #4, W2
	ADD 		W2, W0, W2 ;W4 is now the base address of the data that we want to copy

	;; Copy the data from can message buffer to the message data
	MOV 		[W2++], [W1++]
	MOV 		[W2++], [W1++]
	MOV 		[W2++], [W1++]
	MOV 		[W2++], [W1++]
	MOV 		[W2++], [W1++]

	;; increment the read pointer
	MOV		[W0+0x2], W2 ; Move read_index to W2
	INC 		W2, W2
	AND		#buffer_length, W2
	MOV		W2, [W0+0x2]
	
_ETMCanBufferRead_DONE:	

RETURN


