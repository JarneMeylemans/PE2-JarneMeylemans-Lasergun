
/* Include files */
#include "LaserGun.h"
#include "Images.h"
#include <stdio.h>
#include "players.h"


/* Defines */
#define SSID "LaserBoard"	//SSID externe ESP8266
#define PASSWD "12345678"	//Password externe ESP8266

#define true 		1
#define false 		0


#define MAX_KOGELS 			16	//Maximaal aantal kogels in geweer
#define MAX_HP 				100	//Maximale HP waarde
#define ROTATIONS4BULLET 	2	//Aantal keer draaien met rot enc voordat er een kogel bijkomt

// De States gaan definen
#define StartUp_Mode 	0
#define Menu_Mode 		1
#define Game_Mode 		2
#define Play_Mode		3
#define TimeOut_Mode	4
#define GameOver_Mode	5
#define Settings_Mode 	6
#define Error_Mode		8			//Als er zich een Error heeft voorgedaan

/* TIMERS,UART,ADC */
extern TIM_HandleTypeDef htim1;		//Wordt gebruikt voor de Rotary Encoder
extern TIM_HandleTypeDef htim5;		//Het PWM 38kHz signaal
extern TIM_HandleTypeDef htim6;		//Interrupt om de 26?s (om IR UART te gaan ontvangen)
extern UART_HandleTypeDef huart6;	//UART om IR data mee te verzenden
extern ADC_HandleTypeDef hadc1;		//De ADC om batterij percentage te gaan bepalen


volatile uint32_t tim1_cnt = 0;		//variabele gebruikt bij de Rotary encoder
uint32_t rotCnt = 0;				//getal dat gaat vergelijken of rotary encoder al veranderd is
uint8_t SelectedMenu = 0;			//Het geselecteerde menu

extern uint8_t IR_DATA_Byte;
extern uint8_t IR_DATA_Received;

char * StrBuf[50];					//een buffer gebruikt in een sprintf() functie
char * nameOfKiller[20];			//de naam van de killer wordt hier gestockeerd

/* parameters voor het spel */
uint8_t demage = 10; 		//standaard waarde = 10
uint8_t kogels = 15;		//duidt aan hoeveel kogels de speler heeft
uint8_t GunIsHit; 			//duidt aan of speler geraakt is (in timeout is)
uint8_t HP = 40;			//duidt aan hoeveel health points de speler nog heeft
uint8_t kogelCnt;			//om kogels te herladen deze parameter bij
uint8_t DeviceID = 10;		//ID van de lasergun, ook de waarde die wordt gestuurd over IR

/*
 * STATE 0:
 * 		Het geweer gaan opstarten,
 * 		- OLED gaan initialiseren
 * 		- Startscherm tonen
 * 		- encoder gaan starten
 * 		- (ESP8266 gaan initialiseren en connecteren aan AP)
 * 		- screensaver
 *
 */
uint8_t StartUp_Functions(void)
{
	printf("StartUp Begin \r\n");

	/* OLED KLAARZETTEN */
	printf("Looking for Device.");
	int8_t DevCount = 3;			// 3 keer gaan zoeken (anders in error state)

	while(DevCount > 0)
	{
		DevCount--;
		if(OLED_Init())				//OLED gaan initialiseren
		{
			//oled geinitialliseerd en gevonden (loop beindigen)
			printf("\r\nDevice Gevonden!\r\n");
			DevCount = -1;
		}
		else
		{
			//geen device gevonden
			printf(".");
		}
		HAL_Delay(100); //kleine delay
	}

	if(DevCount != -1)
	{
		//er is geen device gevonden
		printf("\r\n No Device Found! \r\n");
		return Error_Mode; //in error mode gaan
	}

	OLED_Clear();
	OLED_SetXY(0,0);
	OLED_PrintStr("OLED ready!", &Font_7x10, OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);

	/* Encoder gaan starten */
	printf("Encoder Start\r\n");
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	OLED_SetXY(0,10);
	OLED_PrintStr("Encoder Ready!", &Font_7x10, OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);

	/* Draadloze verbinding opstellen */
	// Setup_WirelessConnection();			(functie bestaat en werkt min of meer)

	/* WELKOMSCHERM */
	printf("display welcome screen on oled.\r\n");
	OLED_Clear();
	Setup_WelcomeScreen();

	/* 	SCREENSAVER  */
	uint32_t whilecounter = 0;
	while(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port, ROT_SW_Pin) == 1)
	{
		//loop tot er op Start is geduwt
		whilecounter++;
		if(whilecounter > 5000)
		{
			OLED_Clear();
			printf("SleepMode\r\n");
			while(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin) == 1)
			{
				//nog een loop waar het scherm niet zou kunnen inbranden
				HAL_Delay(10);
			}
			whilecounter = 0;
			printf("return from Sleep\r\n");
			Setup_WelcomeScreen();
		}
		HAL_Delay(10);
	}
	HAL_Delay(500);
	printf("NextState -> Menu!\r\n");
	OLED_Clear();
	return Menu_Mode;	//NextState = Menu Mode
}


/*
 * Een functie die via AT COMMANDs een ESP8266 gaat klaarzetten en gaan connecteren aan een AP
 */
void Setup_WirelessConnection(void)
{
	uint8_t error;
	printf("connecting to AP...\r\n\r\n");
	error = ESP8266_Init("LaserBoard","12345678");
	OLED_SetXY(0,20);
	if(error == 0)
	{
		OLED_PrintStr("WIFI GOT CONNECTED!", &Font_7x10, OLED_BLUE);
	}
	else if(error == 1)
	{
		OLED_PrintStr("WIFI: LARGE ERROR!", &Font_7x10, OLED_BLUE);
	}
	else if(error == 3)
	{
		OLED_PrintStr("WIFI SMALL ERROR", &Font_7x10, OLED_BLUE);
	}
	else
	{
		OLED_PrintStr("non expected error", &Font_7x10, OLED_BLUE);
	}
	OLED_UpdateScreen();
	HAL_Delay(1000);

	printf("\r\n\r\nSend Data To Host \r\n"
		   "----------------- \r\n");
	error = ESP_SendDataToIP("192.168.10.1","test");
	OLED_SetXY(0,30);
	if(error == 0)
	{
		OLED_PrintStr("DATA TRANSMITTED", &Font_7x10, OLED_BLUE);
	}
	else if(error == 1)
	{
		OLED_PrintStr("*TRANSMIT ERROR*", &Font_7x10, OLED_BLUE);

	}
	else
	{
		OLED_PrintStr("non expected error", &Font_7x10, OLED_BLUE);
	}
	OLED_UpdateScreen();

	/*while(1)
	{
		printf("string: '%s'\r\n\r\n",g_uartBuffer);
		HAL_Delay(1000);
	}*/
	HAL_Delay(5000);


}

/*
 * Het welkomscherm in startup mode
 */
void Setup_WelcomeScreen(void)
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


/* STATE 1:
 * 	- toont een menu waar we verder kunnen gaan naar andere states:
 * 		* Settings 	(state 6)
 * 		* Play 		(state 3)
 * 		* Exit 		(state 0)
 */
uint8_t Menu_Functions(void)
{
	tim1_cnt = (htim1.Instance->CNT);	//haal variable van rotary encoder op , RotPerSelect = aantal kliks per rotatie

	if(rotCnt != tim1_cnt)
	{
		if(rotCnt > tim1_cnt)
		{
			SelectedMenu++;
			if(SelectedMenu > 3)
			{
				SelectedMenu = 3;
			}
		}
		else
		{
			SelectedMenu--;
			if(SelectedMenu < 0)
			{
				SelectedMenu = 0;
			}
		}

		rotCnt = tim1_cnt;
		/*Update het scherm met nieuwe gegevens*/
		OLED_Fill(OLED_BLACK);
		DisplaySelectedMenu(SelectedMenu);
		OLED_UpdateScreen();

	}


	/*Knop gaan testen*/
	/* als knop ingedrukt wordt gaan we de nieuwe state moeten returnen */
	if(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin) == 0)
	{
		switch (SelectedMenu)
		{
		case 1:
			return Settings_Mode;
			break;
		case 2:
			printf("op knop gedrukt!\r\n");
			return Game_Mode;
			break;
		case 3:
			return StartUp_Mode; //exit terug gaan opstarten
			break;
		default:
			break;
		}
	}

	return Menu_Mode;
}

/*
 * Gaat afhankelijk welk menu is geselecteerd een scherm tonen met dat menu geselecteerd
 */
void DisplaySelectedMenu(uint8_t Menu)
{
	OLED_SetXY(5,10);
	switch ( Menu )
	{
	// Optie 1
	case 1:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Settings  ", &Font_11x18,OLED_BLACK);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Play  ", &Font_11x18,OLED_BLUE );
		OLED_SetXY(5,40);
		OLED_PrintStr("> Exit  ", &Font_11x18, OLED_BLUE);
		break;
	// Optie 2
	case 2:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Settings  ", &Font_11x18,OLED_BLUE);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Play  ", &Font_11x18,OLED_BLACK);
		OLED_SetXY(5,40);
		OLED_PrintStr("> Exit   ", &Font_11x18, OLED_BLUE);
		break;
	// Optie 3
	case 3:
		OLED_SetXY(5,0);
		OLED_PrintStr("> Settings  ", &Font_11x18,OLED_BLUE);
		OLED_SetXY(5,20);
		OLED_PrintStr("> Play  ", &Font_11x18,OLED_BLUE );
		OLED_SetXY(5,40);
		OLED_PrintStr("> Exit  ", &Font_11x18, OLED_BLACK);
		break;
	// Geen optie geselecteerd
	default:
			OLED_SetXY(5,0);
			OLED_PrintStr("> Settings  ", &Font_11x18,OLED_BLUE);
			OLED_SetXY(5,20);
			OLED_PrintStr("> Play  ", &Font_11x18,OLED_BLUE );
			OLED_SetXY(5,40);
			OLED_PrintStr("> Exit  ", &Font_11x18, OLED_BLUE);

			break;
	}
}


/* STATE 2:
 * 	 gaat alles klaarzetten om in het spel te gaan gebruiken
 * 		- parameters gaan klaarzetten (moest ESP8266 werken was dit een heel belangrijke state)
 * 		# gaat naar ESP 'ready' sturen en moet dan wachten tot AP een 'ready' terugstuurd
 * 		- interrupt gaan starten om ir te gaan ontvangen
 */
uint8_t Game_Functions(void)
{
	/* -> zeggen dat je ready bent
	 * -> wacht op access point om te vermelden dat er mag gestart worden
	 */
	OLED_Clear();
	//Send To AP -> ready + name
	OLED_SetXY(0,0);
	OLED_PrintStr("Send READY ",&Font_7x10, OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);

	//wait for start signal from AP
	OLED_SetXY(0,10);
	OLED_PrintStr("Waiting for start...",&Font_7x10,OLED_BLUE);
	OLED_UpdateScreen();
	HAL_Delay(500);

	//starten van de timer die IR data gaat mee helpen ontvangen
	IR_DATA_Received = false;
	HAL_TIM_Base_Start_IT(&htim6);
	TIM6->ARR = 2700;

	/*PARAMETERS VOOR STARTEN VAN HET SPEL GAAN KLAARZETTEN */
	rotCnt = htim1.Instance->CNT;	//rotCnt weer op zelfde waarde zetten
	HP = MAX_HP;					//hp op maximum zetten (100)
	kogels = MAX_KOGELS; 			//Kogels op een waarde gaan zetten
	GunIsHit = false;				//timeout zal nog niet getriggerd zijn in het begin
	kogelCnt = 0;

	return Play_Mode;
}

/*ShootCode:
 * deze functie gaat een code doorsturen over de IR led
 *
 * parameters:
 * 	-> uint8_t code : de byte die we gaan sturen over de uart IR
 *
 */
void ShootCode(uint8_t code)
{
	//TODO: code nog gaan testen in een ander project

	/* Starten van de PWM van 38kHz  */
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_UART_Transmit(&huart6,&code,1,100);
	HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);

}

/* STATE 3
 * Play_Functions:
 * de functie die de laserboard zal uitvoeren als we het spel aan het spelen zijn
 *
 *
 * -> refreshed altijd het scherm en update de parameters op het scherm
 * -> controlleert ofdat we geraakt zijn door een andere lasergun
 * 		wanneer geraakt dan moet de Gun voor een bepaalde tijd in een time out zitten
 * -> wanneer Trigger wordt ingedrukt dan via IR led data gaan sturen
 */
uint8_t Play_Functions(void)
{

	/* Scherm gaan refreshen met alle parameters
	 * parameters:
	 * 	-> kogels = aantal kogels nog in het magazijn (uint8_t)
	 * 	-> HP = aantal Heilth Points van Lasergun (uint8_t)
	 * 	-> Batterypercentage = waarde van hoeveel de batterij is gevuld (uint8_t) (0-100%)
	 * 	-> GunIsHit = boolean waarde die weergeeft ofdat Gun in timeout is of niet
	 */

	/* Testen of Lasergun geraakt is */
	if(IR_DATA_Received == true)
	{
		printf("Data Received = %d \r\n",IR_DATA_Byte);

		//bepaalde code's gaan negeren

		//Controlleren of ontvangen byte een herkend geweer is
		if( CheckIR_Byte(IR_DATA_Byte) && IR_DATA_Byte != DeviceID )
		{
			//killer is herkend
			if(NameOfKillerInString(nameOfKiller,IR_DATA_Byte))
			{
				//alles goed verlopen
				printf("Name found\r\n");

			}
			else {
				//geen naam gevonden (wel id)
				printf("No Name Found!\r\n");
			}
			//printf("killer gevonden");
			GunIsHit = true;	//speler is geraakt
			IR_DATA_Received = false;
		}
		else
		{
			//byte is niet herkend
			printf("No PLAYER or FALSE data!\r\n");
			IR_DATA_Received = false;
		}
	}

	if(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin) == 0)
	{
		HAL_Delay(500);
		return 1;
	}

	/*Batterij value gaan ophalen */
	uint8_t batval = ADC_BatteryStatus();


	/*Controlleren of Gun is geraakt met de juiste IR code */
	if(GunIsHit == true )
	{
		// Gun in timeout gaan plaatsen
		printf("LaserBoard is geraakt \r\n");
		/* HP gaan verkleinen */
		HP = HP - demage;	//Lasergun demage gaan geven
		return 4; 			//TimeOut mode
	}

	/* Trigger gaan controlleren (schietknop) */
	if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port, TRIGGER_Pin) == 0 && kogels > 0)
	{
		printf("Shot fired!\r\n");
		kogels--;
		HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,1);
		Tone_GunShot();
		ShootCode(DeviceID);	//Schieten met de code
		HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,0);

	}
	else if (HAL_GPIO_ReadPin(TRIGGER_GPIO_Port, TRIGGER_Pin) == 0 && kogels == 0)
	{
		printf("Magazijn leeg!!! \r\n");
		Tone_BulletsEmpty();
	}


	/* De rotary encoder gaan bekijken of die al gedraait is -> kogels herladen*/
	tim1_cnt = (htim1.Instance->CNT);
	if(rotCnt > tim1_cnt)
	{
		rotCnt = tim1_cnt;
		kogelCnt++;
		if(kogelCnt == ROTATIONS4BULLET )
		{
			kogelCnt = 0;
			if(kogels < 16)
			{
				kogels++;
			}
		}
	}
	else if(rotCnt < tim1_cnt)
	{
		rotCnt = tim1_cnt;
	}

	/* Scherm gaan updaten */
	OLED_Fill(OLED_BLACK);
	DisplayPlayMenu(kogels,HP,batval);
	OLED_UpdateScreen();

	return Play_Mode;
}


/*
 * De interface gaan tonen met de nieuwe parameters
 */
void DisplayPlayMenu(uint8_t kogels, uint8_t health, uint8_t battery)
{
	/* Alles op het scherm gaan verwijderen */
	OLED_Fill(OLED_BLACK);

	/* Rechthoeken van de Menu gaan tekenen */
	uint8_t StrBuf[20];
	//OLED_DrawRectangle(0,0,128,64,OLED_BLUE);
	OLED_DrawRectangle(0,50,128,14,OLED_BLUE);


	/* Health bar gaan tekenen  */
	OLED_DrawRectangle(2,52,100,10,OLED_BLUE);
	OLED_DrawFillRectangle(2,52,health,10,OLED_BLUE);
	/* Health Points afdrukken */
	sprintf(StrBuf,"%d",health);
	OLED_SetXY(104,53);
	OLED_PrintStr(StrBuf,&Font_7x10,OLED_BLUE);

	/* Kogels gaan afdrukken op het scherm */
	sprintf(StrBuf,"%d",kogels);
	if(kogels >= 10)
	{
		OLED_SetXY(1,2);
		OLED_PrintStr(StrBuf,&Font_16x26,OLED_BLUE);
	}
	else
	{
		OLED_SetXY(17,2);
		OLED_PrintStr(StrBuf,&Font_16x26,OLED_BLUE);
	}

	sprintf(StrBuf,"/%d",MAX_KOGELS);
	OLED_SetXY(34,17);
	OLED_PrintStr(StrBuf,&Font_11x18,OLED_BLUE);

	OLED_DrawRectangle(0,0,70,40,OLED_BLUE);

	/* BatteryCapacity gaan afdrukken */
	if(battery == 3 )
	{
		/* Battery groter dan 75% */
		OLED_DrawBitmap(106,2,FullBattery,20,10,OLED_BLUE);
	}
	else if( battery == 2 )
	{
		/* Battery groter dan 40% */
		OLED_DrawBitmap(106,2,MidBattery,20,10,OLED_BLUE);
	}
	else if (battery == 1 )
	{
		/*Battery groter dan 25% */
		OLED_DrawBitmap(106,2,LowBattery,20,10,OLED_BLUE);
	}
	else //0
	{
		/* Battery lager dan 25% en groter dan 0% */
		OLED_DrawBitmap(106,2,EmptyBattery,20,10,OLED_BLUE);
	}

}

/* STATE 4:
 * 	Wanneer speler geraakt is gaan we in deze state gaan
 * 		- terug stoppen met de interrupt tim6 (geen data kunnen ontvangen)
 * 		- (esp zou nu moeten sturen naar ap dat het geweer geraakt is en door wie)
 * 		-
 *
 */
uint8_t TimeOut_Functions(void)
{
	//Timer 6 afzetten (geen ir data gaan ontvangen)
	HAL_TIM_Base_Stop_IT(&htim6);
	Tone_GunIsHit(); //geluidje van 1.5s

	printf("Speler is geraakt\r\n");
	/*TimeOut_Functions gaat de timeout van een gehit laserboard gaan afhandelen
	 * namelijk:
	 * -> op scherm tonen dat persoon geraakt is,
	 * -> aantal seconden nog te wachten
	 * -> door wie hij geraakt is
	 * -> Hoeveel HP hij over heeft
	 * -> (als dood Game Over weergeven en spel beiindigen)
	 */



	int8_t counter;

	/* HP gaan verlagen

	/* YOU ARE HIT */
	OLED_Fill(OLED_BLACK);
	OLED_SetXY(10,0);
	OLED_PrintStr("YOU ARE", &Font_16x26,OLED_BLUE);
	OLED_SetXY(40,23);
	OLED_PrintStr("HIT!", &Font_16x26,OLED_BLUE);

	/* Hit By: ... */
	OLED_SetXY(0,53);
	sprintf(StrBuf,"Hit By: %s ",nameOfKiller);
	OLED_PrintStr(StrBuf,&Font_7x10,OLED_BLUE);

	/* HP left */
	OLED_SetXY(0,35);
	sprintf(StrBuf,"HP:%d",HP);
	OLED_PrintStr(StrBuf,&Font_7x10,OLED_BLUE);

	/* Checken of HP al 0 is */
	if(HP <= 0 || HP > 100)
	{
		return GameOver_Mode; 	//Game over!!
	}


	/* display updaten */
	OLED_UpdateScreen();
	HAL_Delay(2000);

	for(counter = 9 ; counter >= 0 ; counter-- )
	{
		/* Loop van 10s timeout / speler kan hier niets doen */

		if(counter > 0)
		{
			OLED_SetXY(110,25);
			sprintf(StrBuf,"%d",counter);
			OLED_PrintStr(StrBuf,&Font_16x26,OLED_BLUE);
		}
		else
		{
			OLED_Fill(OLED_BLUE);
			//OLED_SetXY(45,25);
			//OLED_PrintStr("GO!",&Font_16x26,OLED_BLACK);
			OLED_SetXY(5,10);
			OLED_PrintStr("BACK IN THE",&Font_11x18,OLED_BLACK);
			OLED_SetXY(33,25);
			OLED_PrintStr("GAME",&Font_16x26,OLED_BLACK);
		}

		OLED_UpdateScreen();

		//delay van 1s
		HAL_Delay(1000);
	}

	/* Scherm weer gaan clearen */
	OLED_Clear();

	/*GunIsHit = false*/
	GunIsHit = false;

	//Timer 6 weer gaan aanzetten
	HAL_TIM_Base_Start_IT(&htim6);
	IR_DATA_Received = false;

	return Play_Mode;//playmode
}

/* STATE = 5
 * 	speler ligt uit het spel
 */
uint8_t GameOver_Functions(void)
{
	//printf("GAME OVER\r\n");
	OLED_Fill(OLED_BLUE);
	OLED_SetXY(15,25);
	OLED_PrintStr("GAME OVER",&Font_11x18,OLED_BLACK);
	OLED_UpdateScreen();
	HAL_Delay(100);
	if(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin) == 0)
	{
		printf("Next\r\n");
		return 0; //startup mode
	}
	return 5;
}

/*
 * de batterij status gaan ophalen
 */
uint8_t ADC_BatteryStatus(void)
{
	uint16_t AVG_Batval = 0;
	uint16_t Batval;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,10);
	for(uint8_t i = 0; i < 10; i++)
	{
		 Batval = HAL_ADC_GetValue(&hadc1);

		 AVG_Batval = AVG_Batval + Batval;
	}
	HAL_ADC_Stop(&hadc1);
	AVG_Batval = AVG_Batval/10;


	if(AVG_Batval <= 2825)
	{
		//battery empty
		return 0;
	}
	else if(AVG_Batval > 2825 && AVG_Batval <= 2850 )
	{
		//battery low
		return 1;
	}
	else if(AVG_Batval > 2850 && AVG_Batval <= 2900 )
	{
		//battery medium
		return 2;
	}
	else //(AVG_Batval > 3075)
	{
		//battery high
		return 3;
	}

}

/*  STATE = 6
 * 		- hetzelfde als menu-mode maar nu met parameters ipv states
 * 		- demage en code gaan aanpassen als Trigger ook wordt ingedrukt
 */
uint8_t Settings_Functions(void)
{
	/* Toon een menu waar we kunnen kiezen tussen 3 opties */

		tim1_cnt = (htim1.Instance->CNT);	//haal variable van rotary encoder op , RotPerSelect = aantal kliks per rotatie

		if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin) == 0 && SelectedMenu == 1)
		{
			//De demage gaan aanpassen als rotary encoder draait
			if(rotCnt != tim1_cnt)
			{
				if(rotCnt > tim1_cnt)
				{
					demage++;
					if(demage > 50)
					{
						demage = 50;
					}
				}
				else
				{
					demage--;
					if(demage < 1)
					{
						demage = 1;
					}
				}
				rotCnt = tim1_cnt;
			}

			//scherm gaan updaten
			OLED_Fill(OLED_BLACK);
			DisplaySettings(SelectedMenu,demage,DeviceID);
			OLED_UpdateScreen();
		}
		else if (HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin) == 0 && SelectedMenu == 2)
		{
			//Device ID gaan aanpassen
			if(rotCnt != tim1_cnt)
			{
				if(rotCnt > tim1_cnt)
				{
					DeviceID = DeviceID + 10;
					if(DeviceID > 90)
					{
						DeviceID = 90;
					}
				}
				else
				{
					DeviceID = DeviceID - 10;
					if(DeviceID < 10)
					{
						DeviceID = 10;
					}
				}
				rotCnt = tim1_cnt;
			}

			//scherm gaan updaten
			OLED_Fill(OLED_BLACK);
			DisplaySettings(SelectedMenu,demage,DeviceID);
			OLED_UpdateScreen();
		}
		else
		{
			//trigger niet ingedrukt, dan settings gaan selecteren
			if(rotCnt != tim1_cnt)
			{
				if(rotCnt > tim1_cnt)
				{
					SelectedMenu++;
					if(SelectedMenu > 3)
					{
						SelectedMenu = 3;
					}
				}
				else
				{
					SelectedMenu--;
					if(SelectedMenu < 0)
					{
						SelectedMenu = 0;
					}
				}

				rotCnt = tim1_cnt;
			}
			/*Update het scherm met nieuwe gegevens*/
			OLED_Fill(OLED_BLACK);
			DisplaySettings(SelectedMenu,demage,DeviceID);
			OLED_UpdateScreen();
		}

		/*Knop gaan testen*/
		/* als trigger ingedrukt wordt gaan we een setting kunnen aanpassen */
		if(HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin) == 0)
		{
			switch (SelectedMenu)
			{
			case 1:
				//Niets doen,

				break;
			case 2:
				//niets doen,

				break;
			case 3:
				HAL_Delay(500);
				return 1; //terug naar Menu_Mode

				break;
			default:
				break;
			}
		}

		return Settings_Mode; //loop door de settings menu
}

/*
 * Displayen van het settingsmenu met aangapaste parameters
 */
void DisplaySettings(uint8_t Selected, uint8_t dem, uint16_t code )
{
	switch ( Selected )
	{
	// Optie 1
	case 1:
				OLED_SetXY(5,0);
				sprintf(StrBuf,">Demage:%d",dem);
				OLED_PrintStr(StrBuf,&Font_11x18,OLED_BLUE);
				OLED_SetXY(5,20);
				sprintf(StrBuf," IR: %d",code);
				OLED_PrintStr(StrBuf, &Font_11x18,OLED_BLUE);
				OLED_SetXY(5,40);
				OLED_PrintStr(" Exit", &Font_11x18, OLED_BLUE);
				break;
	// Optie 2
	case 2:
				OLED_SetXY(5,0);
				sprintf(StrBuf," Demage:%d",dem);
				OLED_PrintStr(StrBuf,&Font_11x18,OLED_BLUE);
				OLED_SetXY(5,20);
				sprintf(StrBuf,">IR: %d",code);
				OLED_PrintStr(StrBuf, &Font_11x18,OLED_BLUE);
				OLED_SetXY(5,40);
				OLED_PrintStr(" Exit", &Font_11x18, OLED_BLUE);
				break;
	// Optie 3
	case 3:
				OLED_SetXY(5,0);
				sprintf(StrBuf," Demage:%d",dem);
				OLED_PrintStr(StrBuf,&Font_11x18,OLED_BLUE);
				OLED_SetXY(5,20);
				sprintf(StrBuf," IR: %d",code);
				OLED_PrintStr(StrBuf, &Font_11x18,OLED_BLUE);
				OLED_SetXY(5,40);
				OLED_PrintStr(">Exit", &Font_11x18, OLED_BLUE);
				break;
	// Geen optie geselecteerd
	default:
				OLED_SetXY(5,0);
				sprintf(StrBuf," Demage:%d",dem);
				OLED_PrintStr(StrBuf,&Font_11x18,OLED_BLUE);
				OLED_SetXY(5,20);
				sprintf(StrBuf," IR: %d",code);
				OLED_PrintStr(StrBuf, &Font_11x18,OLED_BLUE);
				OLED_SetXY(5,40);
				OLED_PrintStr(" Exit", &Font_11x18, OLED_BLUE);

				break;
			}



}




