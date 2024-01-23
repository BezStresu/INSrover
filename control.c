#include "control.h"
#include "uart.h"
	
uint8_t PWM_Enabled = 0;
uint16_t dc = 100;

// ToDo: Real time
int time_calculations(int distance);

void control_Init(void){	// Inicjalizowanie portow do sterowania roverem
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Port A
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		// Port B
	
	// Direction pins
	PORTA->PCR[8] |= PORT_PCR_MUX(1);	// Setting Pin 8 as GPIO
	PTA->PDDR |= (1<<8);		// Setting direction: 1 output, 0 input
	PTA->PCOR |= (1<<8);		// OFF
	
	PORTA->PCR[9] |= PORT_PCR_MUX(1);	// Setting Pin 8 as GPIO
	PTA->PDDR |= (1<<9);		// Setting direction: 1 output, 0 input
	PTA->PCOR |= (1<<9);		// OFF
	
	PORTB->PCR[8] |= PORT_PCR_MUX(1);	// Setting Pin 8 as GPIO
	PTB->PDDR |= (1<<8);		// Setting direction: 1 output, 0 input
	PTB->PCOR |= (1<<8);		// OFF
	
	PORTB->PCR[9] |= PORT_PCR_MUX(1);		// Setting Pin 8 as GPIO
	PTB->PDDR |= (1<<9);		// Setting direction: 1 output, 0 input
	PTB->PCOR |= (1<<9);		// OFF

	//PWM 2
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		// Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // Choose MCGFLLCLK clock source
	PORTB->PCR[6] = PORT_PCR_MUX(2);		// Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)
	TPM0->SC |= TPM_SC_PS(7);  					// Set prescaler to 128
	TPM0->SC |= TPM_SC_CMOD(1);					// For TMP0, select the internal input clock source
	TPM0->MOD = 100; 										// Set modulo value to maximum value from slider
	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		// up counting 
	TPM0->CONTROLS[3].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[3].CnV = dc; 				// Set starting value to 50
	
	//PWM 1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		// Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // Choose MCGFLLCLK clock source
	PORTB->PCR[7] = PORT_PCR_MUX(2);		// Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)
	TPM0->SC |= TPM_SC_PS(7);  					// Set prescaler to 128
	TPM0->SC |= TPM_SC_CMOD(1);					// For TMP0, select the internal input clock source
	TPM0->MOD = dc; 										// Set modulo value to maximum value from slider
	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		// up counting 
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[2].CnV = dc; 				// Set starting value to 0, motors are disabled
	
	PWM_Enabled = 1;
	// PWMs are ON 
}

void set_velocity(uint16_t dece){
	dc = dece;
}

uint16_t get_velocity(void){
	uint16_t dc1 = TPM0->CONTROLS[2].CnV;
	uint16_t dc2 = TPM0->CONTROLS[3].CnV;
	if(dc1 == dc2){
		return dc1;
	}
	else{
		return 9999;
	}
}

void move_foward(uint16_t distance){
	//MOVE
	// LEFT MOTOR FOWARD
	PTB->PSOR |= (1<<8);		// ON PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	// RIGHT MOTOR FOWARD
	PTA->PSOR |= (1<<8);		// ON PTA8
	PTA->PCOR |= (1<<9);		// OFF PTA9
	
	// 35.56 for 4 fully loaded AAs, wheel load and friction were not taken into account, so this variable should be lower
	uint16_t tm = (uint16_t)((100 / 100) * 35.56 * distance);	// 100 is DC of motor's drivers
	DELAY(tm);

	
	// LEFT MOTOR STOP
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	// RIGHT MOTOR STOP
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9	
}

void move_backward(uint16_t distance){
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PSOR |= (1<<9);		// ON PTB9
	
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PSOR |= (1<<9);		// ON PTA9

	uint16_t tm = (uint16_t)((100 / 100) * 35.56 * distance);	// 100 is DC of motor's drivers
	DELAY(tm);
	
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9
	
}
void stop(){
	// LEFT MOTOR STOP
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	// RIGHT MOTOR STOP
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9		
}

void turn_right(uint16_t distance){
	// LEFT MOTOR FOWARD
	PTB->PSOR |= (1<<8);		// ON PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	// RIGHT MOTOR STOP
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9

	//DELAY(time_calculations(1)); // Need to calculate time of working
	uint16_t tm = (uint16_t)((100 / 100) * 35.56 * 7.85);	// 100 is DC of motor's drivers
	DELAY(tm);
	
	// LEFT MOTOR STOP
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	move_foward(distance);
}

void turn_left(uint16_t distance){
	// RIGHT MOTOR FOWARD
	PTA->PSOR |= (1<<8);		// ON PTA8
	PTA->PCOR |= (1<<9);		// OFF PTA9
	
	// LEFT MOTOR STOP
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
	
	uint16_t tm = (uint16_t)((100 / 100) * 35.56 * 7.85);	// 100 is DC of motor's drivers
	DELAY(tm);
	
	// RIGHT MOTOR STOP
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9
	
	move_foward(distance);
	

}

void turn_180(){
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PSOR |= (1<<9);		// ON PTB9
	
	PTA->PCOR |= (1<<8);		// OFF PTA8
	PTA->PCOR |= (1<<9);		// OFF PTB9
	
	//DELAY(1000); // Need to calculate time of working, need to work 2x times longer than standard turn_left()
	DELAY(50);
	PTB->PCOR |= (1<<8);		// OFF PTB8
	PTB->PCOR |= (1<<9);		// OFF PTB9
}

