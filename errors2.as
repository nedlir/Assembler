; non-existant label reference (could be found post first-pass)
EXIST: .string "this label will used for comparison"

mov notHere, r1
mov r1, notHere
cmp notHere, notThere
add notHere, notThere
sub notHere, notThere
lea notHere, notThere
cmp notHere, EXIST
add notHere, EXIST
sub notHere, EXIST
lea notHere, EXIST
cmp EXIST, notThere
add EXIST, notThere
sub EXIST, notThere
lea EXIST, notThere
clr notHere
not notHere 
inc notHere
dec notHere
jmp notHere
bne notHere
jsr notHere
red notHere
prn notHere


someLabel1: mov notHere, r1
someLabel2: mov r1, notHere
someLabel3: cmp notHere, notThere
someLabel4: add notHere, notThere
someLabel5: sub notHere, notThere
someLabel6: lea notHere, notThere
someLabel7: cmp notHere, EXIST
someLabel8: add notHere, EXIST
someLabel9: sub notHere, EXIST
someLabel10: lea notHere, EXIST
someLabel11: cmp EXIST, notThere
someLabel12: add EXIST, notThere
someLabel13: sub EXIST, notThere
someLabel14: lea EXIST, notThere
someLabel15: clr notHere
someLabel16: not notHere 
someLabel17: inc notHere
someLabel18: dec notHere
someLabel19: jmp notHere
someLabel20: bne notHere
someLabel21: jsr notHere
someLabel22: red notHere
someLabel23: prn notHere


; entry and extern simultaneous deceleration (could be found post first-pass)
.entry HENSHIN
.extern HENSHIN
someLabel24: .entry KAMEHAMEHA
.extern KAMEHAMEHA
someLabel24: .extern HOPA
.entry HOPA
HOPA: .data -21

;relative addressing errors
.extern FRIEND
jsr %FRIEND

DJ: .data 123
jmp %DJ

bne %ledZeppelin
ledZeppelin: .string "Stairway to heaven"

;