TITLE OFFLINE 2
.MODEL  SMALL
.STACK  100H
    
.DATA 

TEMP DW ?
SUM DW ?
I1 DW ?
J1 DW ?
K1 DW ?
  
.CODE   

MAIN PROC
    ;MOV AX,@DATA
    INPUT:
        MOV DX,0
        INPUT_L:
            MOV AH,1
            INT 21H
            CMP AL,10
            JE  SUMMATION
            SUB AL,30H
            MOV BL,AL
            MOV AX,DX
            MOV BH,10
            MUL BH
            MOV BH,0
            ADD AX,BX
            MOV DX,AX
            JMP INPUT_L
         SUMMATION:
            CMP DX,0
            JE  EXIT
            MOV CX,DX
            MOV SUM,0
            MOV I1,0
            I_WHILE:
                INC I1
                CMP I1,CX
                JG  OUTPUT
                INC J1
                J_WHILE:
                    INC J1
                    CMP J1,CX
                    JGE I_WHILE
                    JMP GCD
                    ADD SUM;,TEMP
                    JMP J_WHILE
            
    GCD:
      MOV K1,CX
      GCD_L:  
        SUB K1,1
        CMP K1,1
        JL  J_WHILE
        MOV DX,0
        MUL I1
        CMP AX,J1
        JGE OUT
        
        JMP GCD_L
      OUT:
        
    OUTPUT:
        ;PRINT ANSWER
        JMP INPUT
      
      
    EXIT:
        MOV AH,4CH
        INT 21H  
    

MAIN ENDP


END MAIN