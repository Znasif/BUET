.MODEL SMALL
.STACK 100H

.DATA

TEMP DW ?
N DW ?
B DW ?

.CODE

MAIN PROC
    MOV AX,@DATA
    MOV DS,AX
    
    CALL INPUT
    MOV N,DX
    CALL INPUT
    MOV B,DX
    
    MOV AX,N
    PUSH AX
    MOV AX,B
    PUSH AX
    ;CALL CONVERT
    
    ;CALL PRINT
    
    MAIN ENDP
    
INPUT PROC NEAR
    XOR DX,DX
    INPUT_WHILE:
        MOV AH,1
        INT 21H
        CMP AL,0DH
        JE END_INPUT
        SUB AL,48
        MOV BL,AL
        MOV AX,DX
        MOV BH,10
        MUL BH
        MOV BH,0
        ADD AX,BX
        MOV DX,AX
        JMP INPUT_WHILE
    END_INPUT:
        MOV DL,10
        MOV AH,2
        INT 21H
        MOV DL,13
        INT 21H
            
        MOV DL,10
        MOV AH,2
        INT 21H
        MOV DL,13
        INT 21H
        RET
    INPUT ENDP

OUTPUT PROC NEAR
    MOV BX,10
    MOV CX,0
    CMP AX,0
    JNE WHILE_OUTPUT
    JE  FLUSH
    WHILE_OUTPUT:
        CMP AX,0
        JNG GO
        
        CWD
        DIV BX
        PUSH DX
        INC CX
        JMP WHILE
    GO:
        MOV DL,10
        MOV AH,2
        INT 21H
        MOV DL,13
        INT 21H
        

END MAIN            