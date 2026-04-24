unsigned int get_sysclk(void); 
unsigned int get_tim2_clock(void); 
void tim2_init(void) ;
void delay_ms(unsigned int ms) ;
void systick_init(void);
void delay_ms_systick(unsigned int ms);
unsigned int measure_sysclk_hz(unsigned int seconds);
void dwt_init(void);
