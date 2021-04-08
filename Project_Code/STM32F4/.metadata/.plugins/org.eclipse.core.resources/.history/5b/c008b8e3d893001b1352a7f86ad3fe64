/*Library voor het OLED aan te sturen via I2C
 * Jarne Meylemans
 * Aanmaak Datum: 18/3/2021
 *
 * Updates:
 * -------------------------------------------------------------
 *
 */

/* Include Files */
#include "stm32f1xx_hal.h"
#include "fonts.h"
//#include "Images.h"
#include "stdlib.h"
#include "string.h"


/* Defines */
#define OLED_ADDR	0x78	/* Het I2C address om apparaat aan te spreken */
#define OLED_WIDTH 	128		/* Aantal pixels in de breedte */
#define OLED_HEIGHT	64		/* Aantal pixels in de hoogte */

typedef enum {
	OLED_BLACK = 0x00, 	/*Zwarte kleur*/
	OLED_BLUE  = 0x01	/*Blauwe kleur*/
} OLED_COLOR ;

/* Functions */
uint8_t OLED_Init(void);

void OLED_UpdateScreen(void);

void OLED_UpdateLayer(uint8_t layer);

void OLED_Fill(OLED_COLOR color);

void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color);

void OLED_Clear(void);

void OLED_SetXY(uint8_t x, uint8_t y);

char OLED_PrintChar( char ch, FontDef_t* Font, OLED_COLOR color);

char OLED_PrintStr( char* str , FontDef_t* Font, OLED_COLOR color);

void OLED_DrawBitmap(int8_t x, int8_t y, const unsigned char* bitmap, int8_t w, int8_t h, uint8_t color);

/* I2C Functions */
void OLED_I2C_WriteMultiple(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count );

void OLED_I2C_WriteTo(uint8_t address, uint8_t reg, uint8_t data);
