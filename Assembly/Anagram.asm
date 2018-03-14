.MODEL SMALL

.STACK 100H

.DATA

WORD1 DB 80 DUP(?)
WORD2 DB 80 DUP(?)

A DW 26 DUP(0)
B DW 26 DUP(0)

DOWN DB 0DH,0AH,'$'
NO DB "NO",'$'
YES DB "YES",'$'

L1 DW 0
L2 DW 0
VAR DW 0
TEMP DW 0

.CODE

MAIN PROC
    MOV AX,@DATA
    MOV DS,AX
    MOV ES,AX
    
    LEA DI,WORD1
    CALL read_str
    MOV L1,BX
    LEA DX,DOWN
    MOV AH,9
    INT 21H
    
    
    LEA DI,WORD2
    CALL read_str
    MOV L2,BX
    LEA DX,DOWN
    MOV AH,9
    INT 21H
    
    ;PUSH AX
    MOV AX,L1
    CMP AX,L2
    ;POP AX
    JNE OUTPUT1
    
    MOV CX,L1
    LEA SI,WORD1
    LEA DI,A
    CLD
    
    LOOP_WORD1:
        LODSB
        LEA DI,A
        WHILE1:
            CMP AL,'a'
            JE BREAK1
            ADD DI,2
            SUB AL,1
            JMP WHILE1
            
        BREAK1:
            MOV AX,[DI]
            INC AX
            MOV [DI],AX
    LOOP LOOP_WORD1
    
    MOV CX,L2
    LEA SI,WORD2
    LEA DI,B
    CLD
    
    LOOP_WORD2:
        LODSB
        LEA DI,B
        WHILE2:
            CMP AL,'a'
            JE BREAK2
            ADD DI,2
            SUB AL,1
            JMP WHILE2
            
        BREAK2:
            MOV AX,[DI]
            INC AX
            MOV [DI],AX
    LOOP LOOP_WORD2
    
    MOV CX,26
    LEA SI,A
    LEA DI,B
    
    LOOP_CHECK:
        MOV AX, [SI]
        CMP AX, [DI]
        
        ;MOV TEMP,CX
        JNE OUTPUT1
        ;MOV CX,TEMP
        ADD SI,2
        ADD DI,2
        
    LOOP LOOP_CHECK 
    
    OUTPUT2:
        LEA DX,DOWN
        MOV AH,9
        INT 21H
        LEA DX,YES
        JMP EXIT
    
    OUTPUT1:
        LEA DX,DOWN
        MOV AH,9
        INT 21H
        LEA DX,NO
        JMP EXIT
    
    EXIT:
        INT 21H
        MOV AH,4CH
        INT 21H
    
    

read_str proc
    
    push ax
    push di
    
    cld
    xor bx,bx  
    mov ah,1
    int 21h
    while:
    cmp al,0dh
    je end_while
    cmp al,8h
    jne else
    dec di
    dec bx
    jmp read
    else:
    stosb
    inc bx
    read:
    int 21h
    jmp while
    end_while:
    pop di
    pop ax
    ret
    read_str endp
    



disp_str proc
    push ax
    push bx
    push si
    push cx
    push dx
    mov cx,bx
    jcxz p_exit
    cld
    top:
    lodsb
    mov dl,al
    mov ah,2
    int 21h
    loop top
    p_exit:
    pop ax
    pop bx
    pop si
    pop cx
    pop dx
    ret
    disp_str endp 

end main