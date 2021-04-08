

/* Include Files */
#include "main.h"
#include "StartUp.h"
#include "OLED.h"
#include "Images.h"



#define MaxKogels 10
/* Private Variabelen */

TIM_HandleTypeDef htim1;

uint8_t Change = 1;
uint8_t Kogels = 10;

uint8_t rot_cnt = 0;


/* Functies */

void WelcomeScreen(void)
{
	OLED_DrawBitmap(10,5,Gun,112,47,OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(1000);
	OLED_DrawBitmap(10,5,ShootingGun,112,47,OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);
	OLED_Fill(OLED_BLACK);
	OLED_DrawBitmap(10,5,Gun,112,47,OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(1000);

	OLED_SetXY(10,45);
	OLED_PrintStr("  Welcome!  ", &Font_11x18,OLED_BLUE);
	OLED_DrawBitmap(10,5,ShootingGun,112,47,OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);

	OLED_Fill(OLED_BLACK);
	OLED_SetXY(10,45);
	OLED_PrintStr("  Welcome!  ", &Font_11x18,OLED_BLUE);
	OLED_DrawBitmap(10,5,Gun,112,47,OLED_BLUE);


	OLED_SetXY(40,35);
	OLED_PrintStr("Press Start", &Font_7x10,OLED_BLUE);
	OLED_UpdateScreen();
}

void DisplayMenu( uint8_t Selected )
{
	OLED_SetXY(5,10);
	switch (Selected)
	{
	case 1:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Optie1  ", &Font_11x18,OLED_BLACK);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Optie2  ", &Font_11x18,OLED_BLUE );
		OLED_SetXY(5,40);
		OLED_PrintStr("> Optie3  ", &Font_11x18, OLED_BLUE);
		break;
	case 2:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Optie1  ", &Font_11x18,OLED_BLUE);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Optie2  ", &Font_11x18,OLED_BLACK);
		OLED_SetXY(5,40);
		OLED_PrintStr("> Optie3  ", &Font_11x18, OLED_BLUE);
		break;
	case 3:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Optie1  ", &Font_11x18,OLED_BLUE);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Optie2  ", &Font_11x18,OLED_BLUE );
		OLED_SetXY(5,40);
		OLED_PrintStr("> Optie3  ", &Font_11x18, OLED_BLACK);
		break;

	default:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Optie1  ", &Font_11x18,OLED_BLUE);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Optie2  ", &Font_11x18,OLED_BLUE );
		OLED_SetXY(5,40);
		OLED_PrintStr("> Optie3  ", &Font_11x18, OLED_BLUE);
		break;

	}
}


void ShowGamePlayMenu(void)
{
	char *str[10];
	if(Change == 1)
	{
		//Als Change => 0 dan wilt dat zeggen dat het scherm in deze loop moet aangepast worden
		OLED_Fill(OLED_BLACK);
		sprintf(&str,"%d/%d",Kogels,MaxKogels);
		OLED_SetXY(0,0);
		OLED_PrintStr(&str,&Font_16x26,OLED_BLUE);
		OLED_SetXY(0,50);
		OLED_PrintStr("kogels",&Font_7x10,OLED_BLUE);
		OLED_UpdateScreen();

	}

	if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin) == 0)
	{
		//Trigger is ingedrukt kogels gaan verminderen
		if(Kogels > 0)
		{
			Kogels--;
		}
		Change = 1;
	}
	else
	{
		Change = 0;
	}

	if(IsEncoderTurnedPos())	// 0 => niets / 1 => counter is positief gedraait
	{
		//Encoder counter is verhoogt
		if(Kogels < MaxKogels)
		{
			Kogels++;
		}
		Change = 1;
	}
	else if (IsEncoderTurnedNeg())	// 0 => niets / 1 => counter is negatief gedraait
	{
		//Encoder counter is verlaagt
		//In dit geval niets doen
		///Change = 1;
	}
	else
	{
		Change = 0;
	}
}

/*
 * Deze functie gaat kijken of de waarde van de encoder is verhoogt
 * Zo ja returned hij een 1
 * Zo nee dan een 0
 */
uint8_t IsEncoderTurnedPos( void )
{
	uint8_t Rotations = 5;
	volatile uint32_t EncCount = htim1.Instance->CNT / Rotations ;	//per 5 klikken gaat de Encount omhoog

	if(rot_cnt != EncCount)
	{
		if(rot_cnt > EncCount)
		{
			return 1;
		}
	}
	return 0;

}

uint8_t IsEncoderTurnedNeg( void )
{
	uint8_t Rotations = 5;
	volatile uint32_t EncCount = htim1.Instance->CNT / Rotations ;	//per 5 klikken gaat de Encount omhoog

	if(rot_cnt != EncCount)
	{
		if(rot_cnt < EncCount)
		{
			return 1;
		}
	}
	return 0;

}

