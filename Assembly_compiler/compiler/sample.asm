DATA B
DATA A
DATA C[4]
DATA D
CONST E = 123
START:
READ AX
READ BX
MOV C[2], AX
MOV B, BX
ADD CX, AX, BX
X:
READ AX
SUB DX, AX, BX
PRINT DX
PRINT CX
IF CX EQ DX THEN
	MOV C[0], CX
	PRINT C[0]
	IF CX EQ DX THEN
		MOV C[0], CX
		PRINT C[0]
	ELSE
		MOV C[1], DX
		PRINT C[1]
		JUMP X
	ENDIF
ELSE
	MOV C[1], DX
	PRINT C[1]
	JUMP X
ENDIF
PRINT E
END
