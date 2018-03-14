TITLE OFFLINE_B1: CRICKET

  .MODEL  SMALL
  .STACK  100H    
  .DATA

  MSG1 DW "Enter the Score team obtained batting first: $"
  MSG2 DW "My Prediction:VICTORY $"
  MSG3 DW "My Prediction:DEFEAT $" 
  VAR1 DB ?
  
  .CODE   
MAIN PROC
        MOV AX,@DATA
        MOV DS,AX
        LEA DX,MSG1
        MOV AH,9H
        INT 21H

    INPUT1:
        MOV AH,1H
        INT 21H
        MOV VAR1,AL
        
    PROCESS1:
        CMP VAR1,'1'
        JE  INPUT2
        INT 21H
        CMP VAR1,'1'
        JG  OUTPUT1
        JL  OUTPUT2
        
    
    INPUT2:
        INT 21H
        MOV VAR1,AL
        CMP VAR1,'5'
        JNL OUTPUT1
        JMP OUTPUT2
        
        
    OUTPUT1:
        INT 21H
        MOV DL,10
        MOV AH,2H
        INT 21H
        MOV DL,13
        INT 21H
        LEA DX,MSG2
        JMP EXIT
    OUTPUT2:
        INT 21H
        MOV DL,10
        MOV AH,2H
        INT 21H
        MOV DL,13
        INT 21H
        LEA DX,MSG3
    EXIT:
        MOV AH,9H
        INT 21H
        MOV AH,4CH
        INT 21H
MAIN ENDP


END MAIN