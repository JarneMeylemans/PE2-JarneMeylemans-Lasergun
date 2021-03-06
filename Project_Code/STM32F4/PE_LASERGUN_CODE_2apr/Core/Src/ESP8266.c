

#include "ESP8266.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;
extern uint8_t RX_Buffer[100];

uint8_t databuffer[10];

extern uint8_t g_uartHasReceived;
extern char g_uartBuffer[600];
extern uint8_t FullBuffer_flag;
extern uint8_t g_uartByteCounter;
uint8_t TX_data[100];





ESP_ErrorHandler ESP8266_Init(char *SSID, char *Password)
{
	uint8_t InitWithoutError = 1;
	//FullBuffer_flag = 0; // geen Fullbuffermode aanzetten
	printf("\r\n===========================================\r\n"
	    	   "  Start With ESP8266 communication-----AT  \r\n"
	    	   "===========================================\r\n\r\n");

	/* AT */
	printf("Sending: AT ---> ");
	ESP_Send("AT\r\n");
	if(ESP_WaitForString("OK\r\n")){
		printf("OK\r\n");
	}
	else{
		InitWithoutError = 0;
		printf("ERROR\r\n");
	}

	/* AT + RST */
	/*printf("Sending: AT+RST ---> ");
	ESP_Send("AT+RST\r\n");
	if(ESP_WaitForString("OK\r\n")){
		printf("OK\r\n");
	}
	else {
		InitWithoutError = 0;
		printf("ERROR\r\n");
	}*/

	/* STATIC IP ADDRESS */
		/*printf("give ESP an Static ip address ---> ");
		ESP_Send("AT+CIPSTA=\"192.168.10.2\"");
		if(ESP_WaitForString("OK\r\n"))
		{
			printf("Done\r\n");
		}
		else
		{
			InitWithoutError =0;
			printf("ERROR\r\n");
		}*/

	/* AT + CWMODE = 1 */
	printf("Sending: AT+CWMODE=1 ---> ");
	ESP_Send("AT+CWMODE=1\r\n");
	if(ESP_WaitForString("OK\r\n"))
	{
		printf("OK\r\n");
	}
	else
	{
		InitWithoutError = 0;
		printf("ERROR\r\n");
	}

	/* AT + CWLAP */

	FullBuffer_flag = 1; //full buffer mode

	printf("Sending: AT+CWLAP ---> \r\n");
	ESP_Send("AT+CWLAP\r\n");
	printf("\r\n======================\r\n"
			   "Netwerken in de buurt:\r\n"
			   "======================\r\n\r\n");
	HAL_Delay(3000);
	printf(g_uartBuffer);
	printf("___________________________________________\r\n\r\n");
	g_uartByteCounter = 0;
	FullBuffer_flag = 0; //normal buffer mode



	/* AT+CWJAP = "SSID","Password" */
	FullBuffer_flag = 0;
	g_uartByteCounter = 0;
	printf("Sending: AT+CWJAP=SSID,Passwd \r\n");
	sprintf(TX_data,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,Password);
	ESP_Send(TX_data);

	HAL_Delay(500);
	FullBuffer_flag = 0; //normal buffer mode
	printf("disconnecting ---> ");
	if(ESP_WaitForString("WIFI DISCONNECT\r\n"))
	{
		printf("WIFI disconnected done\r\n");
	}
	else
	{
		InitWithoutError = 0;
		printf("ERROR\r\n");
	}

	printf("connecting ---> ");
	if(ESP_WaitForString("WIFI CONNECTED\r\n"))
	{
		printf("WIFI connected done\r\n");
	}
	else
	{
		InitWithoutError = 0;
		printf("ERROR\r\n");
		return ESP_LARGE_ERROR; //geen connectie kunnen maken
	}

	printf("asking for IP ---> ");
	if(ESP_WaitForString("WIFI GOT IP\r\n"))
	{
		printf("IP received\r\n");
	}
	else
	{
		InitWithoutError = 0;
		printf("ERROR\r\n");
	}
	HAL_Delay(3000);
	/* IP gaan weergeven op uart */
	FullBuffer_flag = 1;
	g_uartByteCounter = 0;
	printf("Ontvangen Ip = ");
	ESP_Send("AT+CIFSR\r\n");
	HAL_Delay(3000);
	printf(g_uartBuffer);
	printf("================\r\n\r\n");

	g_uartByteCounter = 0;
	FullBuffer_flag = 0;

	HAL_Delay(1000);
	/* AT + RST */
	printf("resetting the ESP8266... ---> ");
	ESP_Send("AT+RST\r\n");
	if(ESP_WaitForString("OK\r\n"))
	{
		printf("OK\r\n");
	}
	else
	{
		printf("ERROR\r\n");
	}
	HAL_Delay(5000);

	if(InitWithoutError != 0)
	{
		return ESP_OK; 	//geen error voorgedaan
	}

	return ESP_SMALL_ERROR; 	//kleine errors hebben zich voorgedaan

}

uint8_t ESP_SendCommand_DataToRecv(char *Command[], char *DataToRecv[])
{
	ESP_Send(Command);
	if((ESP_WaitForString(DataToRecv)) == 1 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void ESP_EmptyBuffer(uint8_t Buffer[])
{
	memset(Buffer, '\0', sizeof Buffer);
}

uint8_t ESP_IsDataAvailable(void)
{
	for(uint8_t i = 0; i < 100 ; i++)
	{
		if(RX_Buffer[i] != '\0')
		{
			return 1; 	//data available
		}
	}
	return 0;
}


uint8_t ESP_WaitForString(char *string[])
{
	uint32_t uCounterToWait = 0;
	while(g_uartHasReceived == 0 || uCounterToWait == 10000 )
	{
		uCounterToWait++;
		/* Wait For received data OR until time (counter) has expired */
	}

	if(strcmp(g_uartBuffer,string) == 0 && uCounterToWait != 10000 )
	{
		// String and buffer was the same!
		g_uartHasReceived = 0;
		return 1;
	}

	// String was not the same OR time was expired!
	g_uartHasReceived = 0;
	return 0;
}

void ESP_Send(char * data)
{
	HAL_UART_Transmit(&huart1, (uint8_t*) data, strlen(data), 100);
	HAL_Delay(10);
}

ESP_ErrorHandler ESP_SendDataToIP( char * ip, char * data)
{
	uint16_t DataLength;
	HAL_Delay(1000);
	/* AT + CIPMUX	= 1 */
	printf("enable MULTIPLE connections ---> ");
	ESP_Send("AT+CIPMUX=1\r\n");
	if(ESP_WaitForString("OK\r\n"))
	{
		printf("OK\r\n");
	}
	else
	{
		printf("ERROR\r\n");
		//return ESP_LARGE_ERROR;
	}

	HAL_Delay(2000);
	/* AT + CIPSTART = "TCP",ip,port */		//connectie opzetten
	FullBuffer_flag= 0;
	printf("Sending +CIPSTART ---> ");
	sprintf(TX_data,"AT+CIPSTART=\"TCP\",\"%s\",80\r\n",ip);
	//sprintf(TX_data,"AT+CIPSTART=\"UDP\",\"%s\",80\r\n",ip);
	ESP_Send(TX_data);
	/*if(ESP_WaitForString("OK\r\n"))
	{
		printf("connected\r\n");
	}
	else
	{
		printf("ERROR\r\n");
		return ESP_LARGE_ERROR;
	}*/
	HAL_Delay(3000);

	FullBuffer_flag=1;
	g_uartByteCounter = 0;
	while(1)
	{
		printf("string: '%s'\r\n\r\n",g_uartBuffer);
		printf("=================\r\n\r\n");
		if(HAL_GPIO_ReadPin(TRIGGER_GPIO_Port,TRIGGER_Pin) == 0)
		{
			printf("Send CIPSTART\r\n");
			sprintf(TX_data,"AT+CIPSTART=\"UDP\",\"%s\",80\r\n",ip);
			ESP_Send(TX_data);
		}
		HAL_Delay(1000);
	}

	/* AT + CIPSEND = <lengte> */
	FullBuffer_flag = 1;
	g_uartByteCounter = 0;

	printf("Sending +CIPSEND ---> ");
	sprintf(TX_data,"AT+CIPSEND=%d\r\n",strlen(data));
	ESP_Send(TX_data);
	/*if(ESP_WaitForString(">"))
	{
		printf("OK\r\n");
	}
	else
	{
		printf("ERROR\r\n");
		return ESP_LARGE_ERROR;
	}*/

	HAL_Delay(3000);

	printf(g_uartBuffer);

	FullBuffer_flag = 1;
	g_uartByteCounter = 0;

	/* Sending the data */
	printf("Sending data ---> ");
	ESP_Send(data);
	/*if(ESP_WaitForString("SEND OK\r\n"))
	{
		printf("SEND OK\r\n");
	}
	else
	{
		printf("ERROR\r\n");
		return ESP_LARGE_ERROR;
	}*/
	printf(g_uartBuffer);

	FullBuffer_flag = 0;
	g_uartByteCounter = 0;
	/* AT + CIPCLOSE */
	FullBuffer_flag = 1;
	g_uartByteCounter =0;
	printf("Connection closing ---> ");
	ESP_Send("AT+CIPCLOSE\r\n");
	if(ESP_WaitForString("CLOSED\r\n"))
	{
		printf("CLOSED\r\n");
	}
	else
	{
		printf("ERROR\r\n");
		return ESP_LARGE_ERROR;
	}

	FullBuffer_flag = 0;
	g_uartByteCounter = 0;
	return ESP_OK;


}
