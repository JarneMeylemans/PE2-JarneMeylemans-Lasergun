#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"

typedef enum
{
  ESP_OK             = 0x00U,		// no error
  ESP_LARGE_ERROR    = 0x01U,		// an essential error occured
  ESP_MEDIUM_ERROR   = 0x02U,		// a medium error occured
  ESP_SMALL_ERROR 	 = 0x03U		// a small occured

} ESP_ErrorHandler;

ESP_ErrorHandler ESP8266_Init(char *SSID, char *Password);
uint8_t ESP_SendCommand_DataToRecv(char *Command[], char *DataToRecv[]);
void ESP_EmptyBuffer(uint8_t Buffer[]);
uint8_t ESP_IsDataAvailable(void);
uint8_t ESP_WaitForString(char *string[]);
void ESP_Send(char * data);
ESP_ErrorHandler ESP_SendDataToIP(char * ip, char * data);
