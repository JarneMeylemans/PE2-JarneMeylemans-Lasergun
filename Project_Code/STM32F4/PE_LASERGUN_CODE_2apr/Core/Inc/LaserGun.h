#include "main.h"
#include "stm32f4xx_hal.h"
#include "OLED.h"
#include "Tones.h"
//#include "Images.h"

uint8_t StartUp_Functions(void);
void Setup_WirelessConnection(void);
void Setup_WelcomeScreen (void);
uint8_t Menu_Functions(void);
void DisplaySelectedMenu( uint8_t SelectedMenu );
uint8_t Game_Functions(void);
void ShootCode(uint8_t code);
uint8_t Play_Functions(void);
void DisplayPlayMenu(uint8_t kogels, uint8_t health, uint8_t battery);
uint8_t TimeOut_Functions(void);
uint8_t GameOver_Functions(void);
uint8_t ADC_BatteryStatus(void);
uint8_t Settings_Functions(void);
void DisplaySettings(uint8_t Selected, uint8_t dem, uint16_t code );
