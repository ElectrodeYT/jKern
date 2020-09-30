.global service_test
service_test:
    // Write char to uart
    mov r0, #0
    mov r1, #'A'
    svc #0
    mov r0, #0
    bx r0 // cant really set origin here

return_value_a:
.int 0

.global service_test_size
service_test_size:
.int . - service_test

.global service_test_2
service_test_2:
    // Write char to uart
    mov r0, #0
    mov r1, #'B'
    svc #0
    mov r0, #0
    bx r0

return_value_b:
.int 0

.global service_test_size_2
service_test_size_2:
.int . - service_test_2
