/*Library voor
 * Jarne Meylemans
 * Aanmaak Datum: 24/3/2021
 *
 * Updates:
 * -------------------------------------------------------------
 *
 */

/* Include Files */
#include "stm32f1xx_hal.h"
//#include "Images.h"
#include "stdlib.h"
#include "string.h"


void WelcomeScreen(void);

void DisplayMenu( uint8_t Selected );

void ShowGamePlayMenu(void);

uint8_t IsEncoderTurnedPos( void );

uint8_t IsEncoderTurnedNeg( void );
