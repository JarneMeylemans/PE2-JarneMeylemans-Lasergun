/*Library voor het OLED aan te sturen via I2C
 * Jarne Meylemans
 * Aanmaak Datum: 18/3/2021
 *
 * Updates:
 * -------------------------------------------------------------
 *
 */

/* Include Files */
#include "OLED.h"


extern I2C_HandleTypeDef hi2c1;

#define SENDCOMMAND(command)	OLED_I2C_WriteTo(OLED_ADDR, 0x00, (command))
#define SENDDATA(data)  		OLED_I2C_WriteTo(OLED_ADDR, 0x40, (data))

static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8 ]; /* Aantal bytes dat het scherm groot is */

typedef struct OLED_t {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} OLED_t;

static OLED_t OLED;


/* Functions */

/*
 * OLED_Init gaat alles klaarzetten om het scherm te gaan gebruiken
 * 		* checkt of er een device aanwezig is op de bus
 * 		* initialiseerd de OLED zelf via I2C commands
 */
uint8_t OLED_Init(void)
{
	HAL_Delay(10);
	/* Gaan kijken of er een device aanwezig is */
	if(HAL_I2C_IsDeviceReady(&hi2c1, OLED_ADDR, 1, 20000) != HAL_OK)
	{
		// Als er geen device is aangekoppeld gaan we meteen een 0 terugsturen
		return 0;
	}

	HAL_Delay(10);

	/* Initialiseren van de OLED */

	//Fundamental Command Table
	SENDCOMMAND(0xAE); 		// Display afzetten (sleep mode)
	//Addressing Setting Command Table
	SENDCOMMAND(0x20); 		// Set Memory Addressing Mode*
	SENDCOMMAND(0x10);		// 00b => Horizontal Addressing Mode
	SENDCOMMAND(0xB0); 		// Set GDDRAM Page Start Address
	//Hardware Configuration Command Table
	SENDCOMMAND(0xC8); 		// Set COM Output Scan Direction (remapped mode)
	//Addressing Setting Command Table
	SENDCOMMAND(0x00);  	// Set Lower Column start address
	SENDCOMMAND(0x10); 		// Set Higher Column Start Address
	//Hardware Configuration Command Table
	SENDCOMMAND(0x40); 		// Set Display Start Line
	//Fundamental Command Table
	SENDCOMMAND(0x81);		// Set Contrast Control*
	SENDCOMMAND(0xFF); 		// volledig contrast
	//Hardware
	SENDCOMMAND(0xA1); 		// Set Segment Re-Map A1=colomn addr 127
	//Fundamental
	SENDCOMMAND(0xA6);		// Set Normal/Inverse Display A6 = normal
	//Hardware
	SENDCOMMAND(0xA8); 		// Set Multiplex Ratio*
	SENDCOMMAND(0x3F); 		//
	//Fundamental
	SENDCOMMAND(0xA4);		//Entire Display ON , resume to RAM content display
	//Hardware
	SENDCOMMAND(0xD3); 		// Set Display Offset*
	SENDCOMMAND(0x00); 		//not offset
	//Timing & Driving
	SENDCOMMAND(0xD5); 		//*Set Display Clock Divide Ratio/Oscillator Frequency
	SENDCOMMAND(0xF0); 		//
	SENDCOMMAND(0xD9); 		//*Set Pre-charge Period
	SENDCOMMAND(0x22); 		//
	//Hardware
	SENDCOMMAND(0xDA); 		//*Set COM Pins Hardware Configuration
	SENDCOMMAND(0x12);
	//Timing & Driving
	SENDCOMMAND(0xDB); 		//Set VCOMH Deselect Level *
	SENDCOMMAND(0x20);
	SENDCOMMAND(0x8D);		//Set DC-DC enable
	SENDCOMMAND(0x14);
	//Fundamentals
	SENDCOMMAND(0xAF);

	/* bij de commands waar een '*' staat, moet een commando, dat er bij hoort achter worden geplaatst*/

	SENDCOMMAND(0x2E);		//Zet het scrollen af!

	/* Clear Screen */
	OLED_Fill(OLED_BLACK);

	/* Update Screen */
	OLED_UpdateScreen();

	/*zet standaard waarden*/
	OLED.CurrentX = 0;	/*cursor op 0 gezet */
	OLED.CurrentY = 0;	/*cursor op 0 gezet */

	/* confirm dat de OLED is geinitialiseerd */
	OLED.Initialized = 1;

	/*OK*/
	return 1;
}

/* OLED_UpdateScreen
 * 		- gaat het scherm updaten; gaat de data die in de OLED_Buffer zit doorsturen naar
 * 		het scherm over I2C
 */
void OLED_UpdateScreen(void)
{
	uint8_t m;
	for(m = 0 ;m < 8 ; m++)
	{
		SENDCOMMAND(0xB0 + m); 	//Set Page Start (page 0 tot 7)
		SENDCOMMAND(0x00); 		//Set Lower Column
		SENDCOMMAND(0x10); 		//Set Higher Column

		//OLED_I2C_WriteMultiple(OLED_ADDR, 0x40, &OLED_Buffer[OLED_WIDTH * m], (OLED_WIDTH/2));
		OLED_I2C_WriteMultiple(0x78,0x40,&OLED_Buffer[128 * m], 64);
		OLED_I2C_WriteMultiple(0x78,0x40,&OLED_Buffer[(128 * m) + 64], 64);

		/* OPMERKING:
		 * Het bovenstaande is gedaan omdat op een of ander manier de volledige OLED_Buffer niet
		 * in 1 keer wou updaten, deze manier (opgedeeld in 2 delen werkt wel)
		 */
	}
}


/* OLED_Fill
 * 		- gaat het hele scherm een bepaalde kleur geven (zwart of blauw)
 *	par:
 *		- color -> de kleur die het scherm moet krijgen
 */
void OLED_Fill( OLED_COLOR color ){
	/*Vul de buffer met allemaal bits van dezelfde kleur*/
	/*uint8_t kleur = 0x00; // zwarte kleur
	if(color == OLED_BLUE)
	{
		kleur = 0xFF; //blauwe kleur
	}
	for(int i = 0; i < sizeof(OLED_Buffer); i++)
	{
		OLED_Buffer[i] = kleur;
	}*/
	memset(OLED_Buffer,( color == OLED_BLACK ) ? 0x00 : 0xFF, sizeof(OLED_Buffer));
}

/* OLED_DrawPixel:
 * 		- gaat een pixel op een bepaalde plaats printen op het scherm
 * par:
 * 		- x -> locatie van de pixel (x)
 * 		- y -> locatie van de pixel (y)
 * 		- color -> de kleur die de pixel moet hebben (zwart of blauw)
 */
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color)
{
	if( y > OLED_HEIGHT || x > OLED_WIDTH )
	{
		//ERROR (niet op het scherm)
		return;
	}

	if(color == OLED_BLUE)
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
}

/* OLED_Clear:
 * 		- gaat scherm volledig wissen naar een zwart scherm
 */
void OLED_Clear(void)
{
	OLED_Fill(OLED_BLACK);
	OLED_UpdateScreen();
}

/* OLED_SetXY:
 * 		gaat de cursor op een nieuwe plaats op het scherm zetten
 *  parameters: x -> nieuwe locatie van de cursor in de x richting
 *  			y -> nieuwe locatie van de cursor in de y richting
 */
void OLED_SetXY(uint8_t x, uint8_t y)
{
	OLED.CurrentX = x;
	OLED.CurrentY = y;
}

/* OLED_PrintChar:
 * 		- gaat een enkel karakter gaan afdrukken afhankelijk van de font (zie fonts.c)
 * 	Parameters:
 * 		- ch 	-> het karakter dat we willen gaan sturen
 * 		- *Font -> pointer naar de font array
 * 		- color -> de kleur die
 * 	return
 * 		- returned een char waarde die werd gestuurd, wanneer niet gelukt wordt er een 0 teruggestuurd
 */
char OLED_PrintChar( char ch, FontDef_t* Font, OLED_COLOR color)
{
	/* Kijken of er genoeg plaats aanwezig is */
	if(OLED_WIDTH <= (OLED.CurrentX + Font->FontWidth) || OLED_HEIGHT <= (OLED.CurrentY + Font->FontHeight))
	{
		return 0; //Error
	}
	/* Font Printen */
	for(uint8_t i = 0; i < Font->FontHeight; i++)
	{
		uint32_t byte = Font->data[(ch - 32) * Font->FontHeight + i];
		for (uint8_t j = 0; j < Font->FontWidth ; j++)
		{
			// De bits gaan printen van links naar rechts
			if((byte << j ) & 0x8000) //0b1000 0000 0000 0000 : eerst MSB gaan sturen in fonts
			{
				OLED_DrawPixel(OLED.CurrentX + j, OLED.CurrentY + i, color); //pixel gaan kleuren
			}
			else
			{
				OLED_DrawPixel(OLED.CurrentX + j, OLED.CurrentY + i, !color); //pixel gaan verwijderen
			}
		}
	}
	//Cursor gaan verschuiven over de x richting.
	OLED.CurrentX = OLED.CurrentX + Font->FontWidth;

	return ch;
}

/* OLED_PrintStr:
 * 		- gaat een volledige zin afdrukken op het scherm
 * Parameters:
 * 		- str[] -> array waar string in zit
 * 		- *Font -> pointer naar font
 * 		- color -> kleur waarin string moet worden weergegeven
 */
char OLED_PrintStr( char* str , FontDef_t* Font, OLED_COLOR color)
{
	/*Zin gaan printen */
	while(*str)
	{
		if(OLED_PrintChar(*str, Font, color) != *str)
		{
			//wanneer er zich een error heeft voorgedaan
			return *str;
		}

		/*verhoog de str pointer */
		str++;
	}
	/* als alles oke is dan zou normaal 0 moeten worden gereturned */
	return *str;
}

void OLED_DrawBitmap(int8_t x, int8_t y, const unsigned char* bitmap, int8_t w, int8_t h, uint8_t color)
{
	int8_t byteWidth = (w + 7) / 8;	//breedte van de bitmap in bytes
	uint8_t byte = 0;

	for(int16_t j = 0; j < h ; j++, y++)
	{
		for(int16_t i = 0; i < w ; i++)
		{
			if(i & 7)
			{
				byte <<= 1;
			}
			else
			{
				byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
			}
			if(byte & 0x80)
			{
				OLED_DrawPixel(x+i, y,color);
			}
		}
	}
}

/* --------------------------------I2C---------------------------------------- */
/* =========================================================================== */

/* OLED_I2C_WriteMultiple:
 * 		- gaat meerdere bytes achter elkaar sturen over I2C naar de Slave
 * parameters
 * 		- address 	-> het address naar waar we data willen sturen over I2C
 * 		- reg 		-> 0x00 : commando		0x40 : data
 * 		- *data 	-> pointer naar data array
 * 		- count 	-> het aantal bytes te versturen (address niet meegerekend)
 */
void OLED_I2C_WriteMultiple(uint8_t address, uint8_t reg, uint8_t* data,uint16_t count)
{
	uint8_t pdata[256];
	pdata[0] = reg;
	uint8_t i;
	for(i = 0; i < count; i++)
	{
		pdata[i+1] = data[i];
	}
	HAL_I2C_Master_Transmit(&hi2c1, address, pdata, count+1, 10);
}

/* OLED_I2C_WriteTo
 * 		- gaat een commando of data sturen naar het scherm over I2C
 * 		- Voor het sturen van 1 nuttige byte
 * par:
 * 		-address -> het adress van de slave op de I2C (in ons geval het scherm)
 * 		- reg 	-> 0x00 => commando , 0x40 => data gaan sturen
 * 		- data 	-> de byte die we gaan sturen naar de slave
 */
void OLED_I2C_WriteTo(uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t pdata[2];
	pdata[0] = reg;
	pdata[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, address, pdata, 2, 10);
}
