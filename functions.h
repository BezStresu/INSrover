#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "i2c.h"
#include "pit.h"
#include "ADC.h"
#include <string.h>
#include "control.h"
#include "frdm_bsp.h" 
#include "led.h" 
#include "uart.h" 
#include "stdio.h"


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern uint8_t result_adc0_ok;
extern uint8_t result_light_ok;
extern uint8_t result_temp_ok;
extern uint8_t end_flag;
uint16_t rozpoznajKomendy(char *komendy);
uint16_t czyZawieraLitere(const char* tablica, char litera);
uint16_t wyodrebnijLiczbe(const char* tekst);
void Init_all(void);
void set_comand(void);
void measurement(void);
void measurement2(void);
void send_result(void);


#endif
