#include "functions.h"

static uint8_t msTicks = 0;
static char temp[99];
static uint8_t arrayXYZ[6];

uint8_t result_adc0_ok = 0;
uint8_t result_light_ok = 0;
uint8_t result_temp_ok = 0;

uint16_t count_light = 0;
uint16_t count_temp = 0;

float	m = 162;		// /100
float u25 = 716;
float T;
float L;
float s25 = 25;

uint16_t temp1;	// temporary
float	adc0_light;
float	adc0_temp;
float coeff = ((float)(((float)2.91) / 4095) );	// 2.91[V] to napiecie referencyjne przetwornika, 4095 to jego rozdzielczosc
// Do przemyslenia: Obsluge przerwania adc przeniesc do plików adc

double temp2;
float lr;
float fb;
float gr;
double ga_earth = 9.81;
double ga_mars = 3.70;
double ga_moon = 1.62;
double ga_venus = 8.90;

uint8_t end_flag = 0;

uint16_t czyZawieraLitere(const char* tablica, char litera);
uint16_t rozpoznajKomendy(char *komendy);
void send_result(void);
void measurement(void);

void set_comand(void){
	UART_Println("\n\r ||| Please enter the comand string.. |||");
}

void Init_all(void){
	PIT_Init();
	control_Init();
	I2C_Init();
  I2C_Write(0x3F, 0x8);
  I2C_WriteReg(0x1D, 0x2A, 0x1);
	UART_Init(9600);	// 9600 bcs of BT
	UART_Println("\n\r### Welcom in INSrover, this device allow you to ###\n\r### drive celestial body on the surface          ###\n\r### and collect data about it                    ###");
	UART_Println("\n\r Enter the route along which the device will move, keep this convention: ");
	UART_Println("\n\r The example command is 'f50' where the first letter indicates the direction\n\r and the next two numbers indicate the distance\n\r expressed in centimeters.");
	UART_Println("\n\r f - forward\n\r b - backward\n\r r - right\n\r l - left");
	UART_Println("\n\r The distance that the device can cover in one command is from 10 to 99 [cm]");
	UART_Println("\n\r An example command string looks like this:\n\r f50r85f30l74b10 \n\n\r In case of an incorrectly formulated command string, the system will inform you about it.");
	set_comand();
}
uint16_t rozpoznajKomendy(char *komendy) {
	//UART_Init(28800);
	end_flag = 0;
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0);
	result_light_ok = 1;			// wybranie pierwszego pomiaru jako pomiar natezenia swiatla
	int lenght = strlen(komendy);
	sprintf(temp, "Lenght of comand's string: %i", lenght);
	UART_Println(temp);
	UART_Println("@@@ Beginning of the route @@@");
	if(lenght % 3 == 0){
		int max_slow = lenght/3;
		char slowa[50][3];
		for(int i = 0; i < max_slow; i++){
			slowa[i][0] = komendy[i*3];
			slowa[i][1] = komendy[i*3 + 1];
			slowa[i][2] = komendy[i*3 + 2];
			char element[3];
			element[0] = slowa[i][0];
			element[1] = slowa[i][1];
			element[2] = slowa[i][2];
			if(czyZawieraLitere(element, 'f')){
				UART_Println("Foward");
				
				//return 1;
				uint16_t temp3 = wyodrebnijLiczbe(element);
				move_foward(temp3);
				sprintf(temp, "Odleglosc: %i",temp3);
				UART_Println(temp);
				UART_Println("Przejazd do przodu zakonczony");
			}
			else if(czyZawieraLitere(element, 'b')){
				UART_Println("Backward");
				//return 2;
				uint16_t temp3 = wyodrebnijLiczbe(element);
				move_backward(temp3);
				sprintf(temp, "Odleglosc: %i",temp3);
				UART_Println(temp);
				UART_Println("Przejazd do tylu zakonczony");
			}
			else if(czyZawieraLitere(element, 'r')){
				UART_Println("Right");
				//return 3;
				int temp3 = wyodrebnijLiczbe(element);
				turn_right(temp3);
				sprintf(temp, "Odleglosc: %i",temp3);
				UART_Println(temp);
				UART_Println("Przejazd w prawo zakonczony");
			}
			else if(czyZawieraLitere(element, 'l')){
				UART_Println("Left");
				//return 4;
				int temp3 = wyodrebnijLiczbe(element);
				turn_left(temp3);
				sprintf(temp, "Odleglosc: %i",temp3);
				UART_Println(temp);
				UART_Println("Przejazd w lewo zakonczony");
			}
			else{
			}
		}
	}
	else{
		UART_Println("Nie jest podzielna przez 3");
		//return 0;
	}
	stop();
	UART_Println("@@@ Koniec jazdy kowboju @@@");
	end_flag = 1;
	ADC0->SC1[0] &= ~(ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0));
	return 0;
}	// end fun

uint16_t wyodrebnijLiczbe(const char* tekst) {
    uint16_t liczba = 0;
    while (*tekst != '\0') {
        if (*tekst >= '0' && *tekst <= '9') {
            liczba = liczba * 10 + (*tekst - '0');
        }
        tekst++;
    }

    return liczba;
}

uint16_t czyZawieraLitere(const char* tablica, char litera) {
    while (*tablica != '\0') {
        if (*tablica == litera) {
            return 1;
        }
        tablica++;
    }
    return 0;
} // end fun

void measurement(void){
	if(result_adc0_ok && result_light_ok && !result_temp_ok)
		{
			ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0);
			adc0_light = adc0_light*coeff* 500;		// Dostosowanie wyniku do zakresu napieciowego, wynik w luxach, obliczenia w notesie
			
			result_adc0_ok = 0;
			result_light_ok = 0;
			result_temp_ok = 1;
		}
		else if(result_adc0_ok && result_temp_ok && !result_light_ok)
		{
			ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(26);
			T = s25 - ((float)adc0_temp- u25)/m;		// temperatura w stopniach celcjusza
			result_adc0_ok = 0;
			result_temp_ok = 0;
			result_light_ok = 1;
		}
		
//					The sensitivity is represented in counts/g. In 2g mode the sensitivity is 4096 counts/g. In 4g mode the sensitivity is 2048 counts/
//					g and in 8g mode the sensitivity is 1024 counts/g, from dataheet
}	// end measurement

void measurement2(void){
	I2C_ReadRegBlock(0x1D, 0x01, 8, arrayXYZ);
	lr = ((double)((int16_t)((arrayXYZ[0]<<8)|arrayXYZ[1])>>2)/4096);
	fb = ((double)((int16_t)((arrayXYZ[2]<<8)|arrayXYZ[3])>>2)/4096);
	gr = ((double)((int16_t)((arrayXYZ[4]<<8)|arrayXYZ[5])>>2)/4096) * ga_earth;
//	The sensitivity is represented in counts/g. In 2g mode the sensitivity is 4096 counts/g. In 4g mode the sensitivity is 2048 counts/
//	g and in 8g mode the sensitivity is 1024 counts/g, from dataheet
// 	end measurement
	
}

void send_result(void){
	measurement2();
	sprintf(temp,"Left/Right: %+1.6fg", lr); // default 4096 counts/g sensitivity
	UART_Println(temp);
	
	sprintf(temp,"Foward/Backward: %+1.6fg", fb); // default 4096 counts/g sensitivity
	UART_Println(temp);
	
	sprintf(temp,"Gravity: %+1.6fm/s^2", gr); // default 4096 counts/g sensitivity
	UART_Println(temp);
	
	sprintf(temp,"Light: %f Lux", adc0_light);
	UART_Println(temp);
	
	sprintf(temp,"Temperature: %f Celcius", T);
	UART_Println(temp);
}

void ADC0_IRQHandler(){
	msTicks++;
	temp1 = ADC0->R[0];		// Odczyt danej i skasowanie flagi COCO
	if(!result_adc0_ok)					// Sprawdz, czy wynik skonsumowany przez petle glówna
	{
		if(result_temp_ok)
		{
			adc0_light = temp1;				// Wyslij nowa dana do petli glównej
			count_light++;
		}
		else if(result_light_ok)
		{
			adc0_temp = temp1;
			count_temp++;
		}
		result_adc0_ok = 1;
	}
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	//UART_Println("/// Pomiar ///");
	measurement();
	if( msTicks%50 == 0 ) {
		__NOP;
	}
	if( msTicks%20 == 0) {
		send_result();
	}
	
}
