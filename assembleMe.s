lwi x1, 0xbdcccccd
lwi x2, 0xc0000000
lwi x3, 2
lwi x4, 0x01000000

sw x1, 0(x4)
sw x2, 1(x4)
sw x3, 2(x4)

lw x5, 3(x4)