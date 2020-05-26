.data
teststring: .asciiz "///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
prompt: .asciiz "The Fibonacii at "
prompt2: .asciiz "is"
.text
j mainfunc
label0: 
addi $gp, $gp, 4
sw $ra, 0($gp)
sw $t0, 0($at)
addi $at, $at, 4
sw $t1, 0($at)
addi $at, $at, 4
sw $t2, 0($at)
addi $at, $at, 4
sw $t3, 0($at)
addi $at, $at, 4
sw $t4, 0($at)
addi $at, $at, 4
sw $t5, 0($at)
addi $at, $at, 4
sw $t6, 0($at)
addi $at, $at, 4
sw $t7, 0($at)
addi $at, $at, 4
sw $t8, 0($at)
addi $at, $at, 4
sw $t9, 0($at)
addi $at, $at, 4
sw $s0, 0($at)
addi $at, $at, 4
sw $s1, 0($at)
addi $at, $at, 4
sw $s2, 0($at)
addi $at, $at, 4
sw $s3, 0($at)
addi $at, $at, 4
sw $s4, 0($at)
addi $at, $at, 4
sw $s5, 0($at)
addi $at, $at, 4
sw $s6, 0($at)
addi $at, $at, 4
sw $s7, 0($at)
addi $at, $at, 4
sw $k1, 0($at)
addi $at, $at, 4
sw $sp, 0($at)
addi $at, $at, 4
sw $a3, 0($at)
addi $at, $at, 4
addi $t2, $zero , 1
slt $t0, $sp, $t2
xori $t0, $t0, 1
slt $t1, $t2, $sp
xori $t1, $t1, 1
and $t0, $t1, $t0
beq $t0, $zero ,label1
addi $t2, $zero , 1
add $k0, $zero, $t2
j label3
label1: 
addi $t2, $zero , 2
slt $t0, $sp, $t2
xori $t0, $t0, 1
slt $t1, $t2, $sp
xori $t1, $t1, 1
and $t0, $t1, $t0
beq $t0, $zero ,label2
addi $t2, $zero , 1
add $k0, $zero, $t2
j label3
label2: 
addi $t3, $zero, 2
sub $t2, $sp, $t3
add $t0, $zero, $t2
add $t4, $zero, $t0
add $t5, $zero, $k1
add $t6, $zero, $sp
add $t7, $zero, $a3
add $sp, $zero, $t4
jal label0
add $k1, $zero, $t5
add $sp, $zero, $t6
add $a3, $zero, $t7
addi $t8, $k0, 0
addi $t3, $zero, 1
sub $s0, $sp, $t3
add $t0, $zero, $s0
add $s1, $zero, $t0
add $t5, $zero, $k1
add $t6, $zero, $sp
add $t7, $zero, $a3
add $sp, $zero, $s1
jal label0
add $k1, $zero, $t5
add $sp, $zero, $t6
add $a3, $zero, $t7
addi $s2, $k0, 0
add $s3, $t8, $s2
add $k0, $zero, $s3
j label3
label3:
addi $at, $at, -4
lw $a3, 0($at)
addi $at, $at, -4
lw $sp, 0($at)
addi $at, $at, -4
lw $k1, 0($at)
addi $at, $at, -4
lw $s7, 0($at)
addi $at, $at, -4
lw $s6, 0($at)
addi $at, $at, -4
lw $s5, 0($at)
addi $at, $at, -4
lw $s4, 0($at)
addi $at, $at, -4
lw $s3, 0($at)
addi $at, $at, -4
lw $s2, 0($at)
addi $at, $at, -4
lw $s1, 0($at)
addi $at, $at, -4
lw $s0, 0($at)
addi $at, $at, -4
lw $t9, 0($at)
addi $at, $at, -4
lw $t8, 0($at)
addi $at, $at, -4
lw $t7, 0($at)
addi $at, $at, -4
lw $t6, 0($at)
addi $at, $at, -4
lw $t5, 0($at)
addi $at, $at, -4
lw $t4, 0($at)
addi $at, $at, -4
lw $t3, 0($at)
addi $at, $at, -4
lw $t2, 0($at)
addi $at, $at, -4
lw $t1, 0($at)
addi $at, $at, -4
lw $t0, 0($at)
lw $ra, 0($gp)
addi $gp, $gp, -4
jr $ra
mainfunc: 
lui $gp, 96
lui $at, 112
addi $t1, $zero , 0
add $t0, $zero, $t1
addi $v0, $zero, 8
lui $a0, 80
addi $a0, $a0, 0
addi $a1, $zero, 400
syscall
addi $v0, $zero, 4
lui $a0, 80
addi $a0, $a0, 0
syscall
addi $v0, $zero, 4
lui $a0, 80
addi $a0, $a0, 400
syscall
addi $v0, $zero, 5
syscall
add $t2, $zero, $v0
addi $v0, $zero, 4
lui $a0, 80
addi $a0, $a0, 420
syscall
add $t4, $zero, $t2
add $t2, $zero, $t4
add $t5, $zero, $k1
add $t6, $zero, $sp
add $t7, $zero, $a3
add $sp, $zero, $t2
jal label0
add $k1, $zero, $t5
add $sp, $zero, $t6
add $a3, $zero, $t7
addi $t8, $k0, 0
add $t0, $zero, $t8
addi $v0, $zero, 1
add $a0, $zero, $t0
syscall
addi $t1, $zero , 0
add $k0, $zero, $t1
j label4
label4:
addi $v0, $zero, 10
syscall

