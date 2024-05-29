.globl main

.data
m1:	 .word 6
m2:	 .word 5
m2:	 .word 4

.text
main:
	la	 $t0, -ERROR-
	lw	 $t1, -ERROR-, ($t1)
	la	 $t0, -ERROR-
	lw	 $t0, -ERROR-, ($t0)
	add	 $t0, $t1, $t0
