END: stop
.entry LIST
STR: .string "abcd"
.extern LIST
.data 1, 7
K: .data 31
rts
.extern fn1
add r3, r6
.extern L3
.data +6,5,-9,-7