.global service_test
service_test:
    mov r0, #'A'
    mov r1, #0
    svc #0
    b service_test

uart_base:
.int 0x1c090000

.global service_test_size
service_test_size:
.int . - service_test
