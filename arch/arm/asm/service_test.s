.global service_test
service_test:
    // try to write into test
    ldr r1, test
    mov r0, #423
    str r0, [r1]
    mov r0, #'A'
    mov r1, #0
    svc #0
    b service_test

test:
.int 0

uart_base:
.int 0x1c090000

.global service_test_size
service_test_size:
.int . - service_test
