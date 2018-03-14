                                .model small
.stack 100h
.data
i dw ?
j dw ?
tmp dw ?
s dw ?
N dw ?
.code
main proc
    while:
        mov ah,1
        int 21h
        cmp al,0dh
        je end_while
        sub al,48
        mov bl,al
        mov ax,dx
        mov bh,10
        mul bh
        mov bh,0
        add ax,bx
        mov dx,ax
        jmp while
     end_while:
         mov cx,dx
         mov s,0
         mov
         loop1:
            mov bx,cx
            sub bx,1
            l2:
                mov ax,bx
                call gcd
                add s,bx
                cmp j,dx
                inc j
                jle l2
            cmp i,dx
            inc i
            jle loop1
            
         mov bx,s
         call DECIMAL_OUTPUT 
main endp
                
            
gcd proc
    
    
    xor ax,ax
    mov ax,j
    
    l1: 
        mov bx,i
        div i
        mov i,dx
        xor dx,dx
        mov ax,bx
        cmp i,0
        je fin
        jne l1
     fin:
        ret
        
gcd endp




DECIMAL_OUTPUT PROC

   CMP BX, 0                     
   JGE @START                     
   MOV AH, 2                                          
   INT 21H                        

                            

   @START:                        

   MOV AX, BX                     
   XOR CX, CX                     
   MOV BX, 10                     

   @REPEAT:                      
     XOR DX, DX                  
     DIV BX                       
     PUSH DX                      
     INC CX                       
     OR AX, AX                    
     JNE @REPEAT                    

   MOV AH, 2                      

   @DISPLAY:                      
     POP DX                       
     OR DL,30h                    
     INT 21H                      
   LOOP @DISPLAY                  ; jump to label @DISPLA

   RET                             
 DECIMAL_OUTPUT ENDP