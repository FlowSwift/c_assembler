END: stop
.entry LIST
STR: .string "abcd"
.data 1, 7
K: .data 31
.extern fn1
rts
.entry L
add r3, r6
.extern L3
.data +6,5,-9,-7