#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"

void ESP8266_Init(char *SSID, char *Password);
uint8_t ESP_SendCommand_DataToRecv(char *Command[], char *DataToRecv[]);
void ESP_EmptyBuffer(uint8_t Buffer[]);
uint8_t ESP_IsDataAvailable(void);
uint8_t ESP_WaitForString(char *string[]);
void ESP_Send(char * data);
