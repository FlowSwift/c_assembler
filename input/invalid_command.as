END: stop
.entry LIST
STR: .string "abcd"
LIST: .data 6, -9, +7
.data -100000000000000000, 7
K: .data 31
rt
.extern fn1
add r3, r6
.extern L3
.data +6,5,-9,-7