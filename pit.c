#include "MKL05Z4.h"
#include	"pit.h"

uint32_t tsv;
//ToDo: UStawić PIT aby zgłaszał przerwanie co 0.5s(potem może to zmienimy na częściej) przy każdym przerwaniu będzie zapalał jakąś(jeszcze do wybrania) diodę tak aby potwierdzić działanie
void PIT_Init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			// Włączenie sygnału zegara do modułu PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Włączenie modułu PIT
	//tsv=BUS_CLOCK * 2;		// 2 s
	//tsv=BUS_CLOCK;											// Przerwanie co 1s
	//tsv=BUS_CLOCK/4;											// Przerwanie co 0.5s
	tsv=BUS_CLOCK/20;										// Przerwanie co 100ms
	//tsv=BUS_CLOCK/100;									// Przerwanie co 10ms
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);		// Załadowanie wartości startowej
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Odblokowanie przerwania i wystartowanie licznika
	//NVIC_SetPriority(PIT_IRQn, 2); // 0, 1, 2 or 3 /* PIT interrupt priority level  */
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);	
}

// ToDo: Dodać mruganie diodą
void PIT_IRQHandler(void)
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		// Skasuj flagę żądania przerwania
	NVIC_ClearPendingIRQ(PIT_IRQn);
}

// ToDo: Ta funkcja jest do przeróbki żeby można było nią ustawiać okres przerwań

//void PIT_SetTSV(uint32_t period) {

//	if (pitEnabled) PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(40000*period); /* count down from */
//}

