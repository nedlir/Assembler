; tester for input file


A: .data 4, 8, 15, 16, 23, 42
cmp r1, r3
.entry A
prn A
clr A
prn A
jmp %YES
prn A
prn #48
YES: cmp A, X
X: .data 2047   , 0, 0  , 0   ,  0 , -2047
dec r5
dec r5
dec r5

; This is a comment

clr r1
cmp r1, r2
bne ENOUGH
not r2
not r1
cmp r1, r2
bne ENOUGH
lea myString, r3
lea myString, r2
.extern swag22
ENOUGH: stop
GOBACK: rts
cmp swag22 , #-2047
      

   

.string ".data"
myString: .string "r2"
yourString: .string "r2"
cmp myString, yourString
bne %ENOUGH
dec swag22
dec swag22
inc r1
sub r2, r6
prn swag22
jmp %YES
.data -1
stop