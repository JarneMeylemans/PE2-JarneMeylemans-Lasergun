/*
 * ESP8266_Functions.c
 *
 * Created on: 15 feb, 2021
 * Author: Jarne Meylemans
 *
 */

#include "ESP8266_Functions.h"
#include "main.h"
#include "string.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1; 	//Uart naar en van de ESP8266
extern UART_HandleTypeDef huart2; 	//Uart (Debug) naar de PC

#define esp_uart &huart1
#define pc_uart &huart2

/********************************************************************************/

void ESP8266_Init(char *SSID, char *PASSWORD)
{
	char data[80];

	/*klaarzetten van de ESP8266*/
	ESP_Send_Uart("AT+RST\r\n");
	PC_Send_Uart("RESET ESP8266MOD.");

	/*Loading points weergeven voor 5 seconden*/
	for(int i = 0; i < 5; i++)
	{
		PC_Send_Uart(".");
		HAL_Delay(1000);
	}

	/* AT */
	ESP_Send_Uart("AT\r\n");
	//wait for "OK" over UART
	PC_Send_Uart("\r\nAT----->OK\r\n");
	HAL_Delay(500);

	/* AT+CWMODE=1 */
	ESP_Send_Uart("AT+CWMODE=2\r\n");
	//wait for "OK" over UART
	PC_Send_Uart("AT+CWMODE----->2\r\n");
	HAL_Delay(1000);

	ESP_Send_Uart("AT+CWSAP=\"ESP8266 Server\",\"12345678\",3,4");
	HAL_Delay(1000);


	/* AT+CWJAP="SSID","PASSWORD" */
	/*PC_Send_Uart("connecteren aan de Access Point\r\n");
	sprintf(data,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASSWORD);
	ESP_Send_Uart(data);
	//wait for "OK" over UART
	HAL_Delay(1000);
	sprintf(data,"Connected to %s \r\n",SSID);
	PC_Send_Uart(data);

	ESP_Send_Uart("AT+CIPMUX=1\r\n");
	HAL_Delay(1000);
	PC_Send_Uart("CIPMUX--->OK\r\n");

	ESP_Send_Uart("AT+CIPSERVER=1,80\r\n");
	HAL_Delay(1000);
	PC_Send_Uart("CIPSERVER--->OK\r\n");

*/





}

void ESP_Send_Uart(char *data)
{
	/*Versturen van data over uart naar ESP8266*/
	HAL_UART_Transmit(esp_uart,(uint8_t*)data,strlen(data),100);
	HAL_Delay(150); //delay 150ms
}

void PC_Send_Uart(char *data)
{
	/*Debuggen van data over de uart naar de PC*/
	HAL_UART_Transmit(pc_uart,(uint8_t*)data,strlen(data),50);
	HAL_Delay(100); //delay 100ms
}
