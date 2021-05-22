
#include "Tones.h"

extern TIM_HandleTypeDef htim9;


/*
 * Functie gaat op pin C4 een pwm signaal aanleggen, bij een prescaler van 100 kan de
 * frequentie worden gekozen die we willen laten horen door de buzzer.
 * gebruikt Timer 9
 */
void Tone_Tim9( uint16_t frequency, uint8_t prescaler)
{
	uint16_t ARR;
	ARR = ((100000000)/((prescaler + 1) * frequency)) - 1;
	TIM9->ARR = ARR;			//autoreload gaan zetten
	TIM9->PSC = prescaler;		//prescaler gaan zetten
	TIM9->CCR1 = ARR/2;			//duty cycle van 50% nemen
}

/*
 * Het geluidje van een schot met het lasergeweer
 * tijdsduur = +/- 210ms
 */
void Tone_GunShot(void)
{
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);		//starten van de pwm
	for(uint16_t i = 4000 ; i > 800 ; i = i - 150)	//een toon van laag naar hoog laten tonen
	{
		Tone_Tim9(i,100);
		HAL_Delay(10);
	}
	HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);			//pwm gaan stoppen
}

/*
 * Het geluidje van wanneer het geweer geen kogels meer heeft.
 * tijdsduur = +/- 250ms
 */
void Tone_BulletsEmpty(void)
{
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);	//Start PWM
	Tone_Tim9(200,100);
	HAL_Delay(100);
	Tone_Tim9(100,100);
	HAL_Delay(150);
	HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);		//Stop PWM
}

/*
 * Geluidje wanneer Lasergeweer geraakt is door andere persoon
 * tijdsduur = +/- 300ms
 */
void Tone_GunIsHit(void)
{
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);	//Start PWM
	Tone_Tim9(300,100);
	HAL_Delay(100);
	Tone_Tim9(200, 100);
	HAL_Delay(100);
	Tone_Tim9(80,100);
	HAL_Delay(100);
	HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);		//Stop PWM
}

/*
 * GameOver geluidje
 * tijdsduur = +/- 2s
 */
void Tone_GameOver(void)
{
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);	//Start PWM
	Tone_Tim9(300,100);
	HAL_Delay(500);
	Tone_Tim9(200, 100);
	HAL_Delay(500);
	Tone_Tim9(80,100);
	HAL_Delay(1000);
	HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);		//Stop PWM

}

