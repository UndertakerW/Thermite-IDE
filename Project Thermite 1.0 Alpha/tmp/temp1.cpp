.data
.text
j mainfunc
mainfunc: 
lui $gp, 96
lui $at, 112
addi $v0, $zero, 5
syscall
add $t0, $zero, $v0
add $t1, $zero, $t0
addi $v0, $zero, 1
add $a0, $zero, $t1
syscall
addi $t2, $zero , 0
add $k0, $zero, $t2
j label0
label0:
addi $v0, $zero, 10
syscall

