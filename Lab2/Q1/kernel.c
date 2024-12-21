char stack0[4096];
extern void uartputc_sync(int c);
int kmain(){
    char* str = "Hello World\n";
    while (*str){
        uartputc_sync(*str++);
    }
    return 0;
}