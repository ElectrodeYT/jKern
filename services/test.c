// jKern services/test.c
// Test Services

int test_service1() {
    while(true) {
        WRITE_REG(UART0_BASE, 'A');
    }
}

int test_service2() {
    while(true) {
        WRITE_REG(UART0_BASE, 'B');        
    }
}
