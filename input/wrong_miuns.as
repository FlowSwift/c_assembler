.entry LIST
.extern fn1
MAIN: add r3, LIST
jsr #2
jsr r2
LOOP: prn #48
lea #6, r6
inc #3
mov #5
mov #4, #12, #111
mov r6, r4 r4
mov *r6, L3
sub r1, r4
cmp r3, #-6
bne END
add r7, *r6
clr K
stop r6
sub L3, L3
.entry MAIN
jmp LOOP
END: stop
STR: .string "abcd"
LIST: .data 6, -9
.data -100
K: .data 31
.extern L3