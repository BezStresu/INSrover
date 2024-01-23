#ifndef pit_h
#define pit_h
#define BUS_CLOCK SystemCoreClock/2

void PIT_Init(void);
void PIT_IRQHandler(void);

#endif
