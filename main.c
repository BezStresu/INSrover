#include "functions.h"

uint16_t cal_count = 0;
#define LF	0xa		// Enter
char rx_buf[99];
char Error[]="Zla komenda";
char Too_Long[]="Zbyt dlugi ciag";
uint8_t rx_buf_pos=0;
char temp,buf;
uint8_t rx_FULL=0;
uint8_t too_long=0;


void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Odczyt wartosci z bufora odbiornika i skasowanie flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=LF)
			{
				if(!too_long)	// Jesli za dlugi ciag, ignoruj reszte znaków
				{
					rx_buf[rx_buf_pos] = temp;	// Kompletuj komende
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// Za dlugi ciag
				}
			}
			else
			{
				if(!too_long)	// Jesli za dlugi ciag, porzuc tablice
					rx_buf[rx_buf_pos] = 0;
				rx_FULL=1;
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}

int main (void) {
	Init_all();
	uint8_t	kal_error;
	kal_error = ADC_Init();		// Inicjalizacja i kalibracja przetwornika A/C
	if(kal_error)
	{
		while(1){							// Klaibracja sie nie powiodla
			kal_error = ADC_Init();
			cal_count++;
			if(!kal_error){
				break;
			}
			else if(cal_count == 10){
				while(TRUE){
					UART_Println("Calibration failed after 10 approaches\n\rRestart device");
					DELAY(5000);
				}
			}
		}
	}
	
	UART_Println("\n-> Starting the main loop. Sleep ...\n\r");
	//control_Init();
	
	//move_foward(99);
	//stop();
	//move_backward(100);
	//turn_right();
	//DELAY(5000);
	//turn_left();
	
	uint8_t i;
	while(1)
	{
		if(rx_FULL)		// Czy dana gotowa?
		{
			if(too_long)
			{
				for(i=0;Too_Long[i]!=0;i++)	// Zbyt dlugi ciag
					{
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
						UART0->D = Too_Long[i];
					}
					while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
					UART0->D = 0xa;		// Nastepna linia
					too_long=0;
			}
			else
			{
				rozpoznajKomendy(rx_buf);
				
			}
			rx_buf_pos=0;
			rx_FULL=0;	// Dana skonsumowana
		}
	}
	
}
