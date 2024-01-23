/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Tutorial (C).            *
 ******************************************************************************/

/**
 * @file uart.c
 * @author Koryciak
 * @date Sep 2020
 * @brief File containing definitions.
 * @ver 0.1
 */

#include "uart.h"

/******************************************************************************\
* Private prototypes
\******************************************************************************/
void uart_send(uint8_t data);

void UART_Init(uint32_t baud_rate)
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;							//UART0 dolaczony do zegara
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;							//Port B dolaczony do zegara
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);		//Zegar MCGFLLCLK=41943040Hz (CLOCK_SETUP=0)
	PORTB->PCR[1] = PORT_PCR_MUX(2);								//PTB1=TX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2);								//PTB2=RX_D
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika

	UART0->BDH = 0;
	UART0->BDL = 137;		//Dla CLOCK_SETUP=0 BR=9600	BDL=137	:	CLOCK_SETUP=1	BR=230400		BDL=13	136 lub 137
	UART0->C4 &= ~UART0_C4_OSR_MASK;
	UART0->C4 |= UART0_C4_OSR(31);	//Dla CLOCK_SETUP=0 BR=9600	OSR=31	:	CLOCK_SETUP=1	

	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;	//Próbkowanie odbiornika na obydwu zboczach zegara
	UART0->C2 |= UART0_C2_RIE_MASK;		// Wlacz przerwania od odbiornika
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		//Wlacz nadajnik i odbiornik
	
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	
}

void UART_Print(const char *str) {
	
	uint8_t str_len = 0;

  while (str[str_len] != '\0') {              /* until end of string */
    uart_send(str[str_len]);
    ++str_len;
  }
}

void UART_Println(const char *str) {
	
	UART_Print(str);
	UART_Print("\n\r");
}

/**
 * @brief UART send data
 */
void uart_send(uint8_t data) {
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = data;
}
