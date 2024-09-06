macr .data
4
5
6
end_macr
aa
END: stop
.entry LIST
STR: .string "abcd"
LIST: .data 6, -9, +7
.data -100
K: .data 31
rts
.extern fn1
add r3, r6
.extern L3
.data +6,5,-9,-7