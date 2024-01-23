#include "MKL05Z4.h"
#include "frdm_bsp.h"

#ifndef CONTROL_H
#define CONTROL_H


void control_Init(void);	// Inicjalizowanie portow do sterowania roverem
void move_foward(uint16_t distance);
void move_backward(uint16_t distance);
void stop(void);
void turn_right(uint16_t distance);
void turn_left(uint16_t distance);
void turn_180(void);
void set_velocity(uint16_t dece);	// Zmiana wypelnienia sygnalu PWM sterujacego silinkami
uint16_t get_velocity(void);

int time_calculations(int distance);





#endif
