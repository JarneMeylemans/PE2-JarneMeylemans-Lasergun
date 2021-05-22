#include "main.h"
#include "stm32f4xx_hal.h"

/* typedef deelnemer aanmaken */
typedef struct
{
	uint8_t ID;			// elke speler heeft een ID ( deze wordt verstuurd over de UART IR )
	char Name[20]; 		// Naam van de speler ( vaste naam bijgehouden in de lijst )

} PLAYER ;




/* FUNCTIES */

uint8_t LookForKillerID( uint8_t DATA );
uint8_t NameOfKillerInString( char * string[] , uint8_t DATA );
