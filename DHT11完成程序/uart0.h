void InitUART(void);
void UART_Init_38400(void);
void Send1Char(unsigned char sendchar);
void PutString(char *ptr);
unsigned char Get1Char(void);
void DisplayConfigMenu(void);
void HandleConfigMenu(unsigned char inputvalue);
#define CPU          (8000000)
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))
