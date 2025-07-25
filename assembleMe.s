.data
val:    .word 0xc
vall:    .word 0xd

.text
main:
    lw   x1, 0(val)
    lw   x3, 0(vall)

    addi x2, x1, 1
    addi x4, x3, 1