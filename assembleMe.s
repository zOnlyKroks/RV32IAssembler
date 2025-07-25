# Debug version - test if call/return works at all
.data
    result: .word 0

.text
# Simple test function that just adds 10 to the input
test_function:
    addi a0, a0, 10        # Add 10 to input (7 + 10 = 17)
    ret                    # Return

main:
    lwi a0, 7              # Load 7
    call test_function     # Should return 17 in a0

    # Store result
    la t0, result
    sw a0, 0(t0)

    # Infinite loop
end:
    j end