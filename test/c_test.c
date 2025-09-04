
__attribute__((section(".uart")))
volatile char UART_DATA;

void print(const char *str)
{
    while (*str)
    {
        UART_DATA = *str; // Write character to UART
        str++;
    }
}


int main(void)
{
    print("c test message\n");
    return 0;
}
