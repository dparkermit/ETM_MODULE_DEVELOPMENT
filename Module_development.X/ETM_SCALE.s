.ifdef __dsPIC30F
        .include "p30fxxxx.inc"
.endif
.ifdef __dsPIC33F
        .include "p33Fxxxx.inc"
.endif

.section .nbss, bss, near    	
	_saturation_etmscalefactor2_count:	.space 2
	.global _saturation_etmscalefactor2_count
	_saturation_etmscalefactor16_count:	.space 2
	.global _saturation_etmscalefactor16_count
.text	



	;; ----------------------------------------------------------

	
	.global  _EtmScaleFactor2
	;; uses and does not restore W0->W3
	.text
_EtmScaleFactor2:
	;; Value is stored in w0
	;; Scale is stored in w1

	MUL.UU		W0,W1,W2 		; Multiply W0 by W1 and store in W2:W3, MSW is stored in W3
	MUL.UU		W3,#2,W0		; Multiply W3 by 2 and store the results in W0:W1 - W0(LSW) is the result we care about
						
	CP0		W1			; If W1 is Zero, then there was NOT an overflow
	BRA		Z, _EtmScaleFactor2_no_overflow
	;; There was an overflow in the multiply opertion
	;; Increment the overflow counter and set the result to 0xFFFF
	MOV		#0xFFFF, W0
	INC		_saturation_etmscalefactor2_count
_EtmScaleFactor2_no_overflow:	
	;; OR together W0, W1 into W0 to give the final results
	LSR		W2, #15, W1		; Take the 1 MSbits of W2 and store then as the 1 LSB of W1
	IOR		W0, W1, W0		; Add W1 to W0 (using bitwise or in this case)
	RETURN

	
	;; ----------------------------------------------------------

	
	.global  _EtmScaleFactor16
	;; uses and does not restore W0->W3
	.text
_EtmScaleFactor16:
	;; Value is stored in w0
	;; Scale is stored in w1

	MUL.UU		W0,W1,W2 		; Multiply W0 by W1 and store in W2:W3, MSW is stored in W3
	MUL.UU		W3,#16,W0		; Multiply W3 by 16 and store the results in W0:W1 - W0(LSW) is the result we care about
						
	CP0		W1			; If W1 is Zero, then there was NOT an overflow
	BRA		Z, _EtmScaleFactor16_no_overflow
	;; There was an overflow in the multiply opertion
	;; Increment the overflow counter and set the result to 0xFFFF
	MOV		#0xFFFF, W0
	INC		_saturation_etmscalefactor16_count
_EtmScaleFactor16_no_overflow:	
	;; OR together W0, W1 into W0 to give the final results
	LSR		W2, #12, W1		; Take the 4 MSbits of W2 and store then as the 4 LSB of W1
	IOR		W0, W1, W0		; Add W1 to W0 (using bitwise or in this case)
	RETURN

	
