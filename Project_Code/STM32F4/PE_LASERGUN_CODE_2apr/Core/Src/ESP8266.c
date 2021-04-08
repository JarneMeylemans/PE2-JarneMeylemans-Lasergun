

#include "ESP8266.h"

extern UART_HandleTypeDef huart1;
extern uint8_t RX_Buffer[100];
extern uint8_t RX_data[100];
uint8_t databuffer[10];

void ESP8266_Init(char *SSID, char *Password)
{
	//HAL_Delay(1000);

	/* AT -> Checken of ESP8266 goed werkt */
	printf("AT ==> ");
	if(ESP_SendCommand_DataToRecv("AT\r\n","AT\r\r\n\r\nOK\r\n"))
	{
		printf("OK\r\n");
	}
	else
	{
		sprintf(RX_data,"Fail! = '%s' \r\n",RX_Buffer);
		printf(RX_data);
	}


	printf("AT ==> ");
		if(ESP_SendCommand_DataToRecv("AT\r\n","AT\r\r\n\r\nOK\r\n"))
		{
			printf("OK\r\n");
		}
		else
		{
			sprintf(RX_data,"Fail! = '%s' \r\n",RX_Buffer);
			printf(RX_data);
		}

	/* AT+CWMODE=1 -> Als station mode gaan gebruiken */
	/*printf("AT+CWMODE=1 ==> ");
	if(ESP_SendCommand_DataToRecv("AT+CWMODE=1","AT+CWMODE=1\r\r\n\r\nOK\r\n"))
	{
		printf("OK\r\n");
	}
	else
	{
		printf("Fail!\r\n");
	}*/

	//printf(RX_Buffer);


	//ESP_Send("AT+RST\r\n");		// resetten van de ESP8266
	//HAL_UART_Receive_IT(&huart1, RX_Buffer,4);
	/* AT */
	/*HAL_Delay(1000);
	ESP_Send("AT\r\n");
	HAL_Delay(20);
	sprintf(RX_data,"output = %s \r\n",RX_Buffer);
	printf(RX_data);
	/* AT + CWMODE = 2 */
	/*ESP_Send("AT+CWMODE=1\r\n");
	HAL_Delay(10);
	sprintf(RX_data,"output = %s \r\n",RX_Buffer);
	printf(RX_data);*/

	/* AT */
	//ESP_Send("AT+CWJAP=\"LaserBoard\",\"12345678\"\r\n");
	//HAL_Delay(1000);

}

uint8_t ESP_SendCommand_DataToRecv(char *Command[], char *DataToRecv[])
{
	//ESP_EmptyBuffer(RX_Buffer);
	HAL_Delay(100);
	HAL_UART_Receive_IT(&huart1,RX_Buffer,sizeof(DataToRecv));
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
	while(ESP_IsDataAvailable() != 1)
	{
		//wachten tot we iets hebben ontvangen
	}

	sprintf(RX_data,"Buf='%s' \r\n Str='%s'\r\n",RX_Buffer,string);
	printf(RX_data);

	if(strcmp(RX_Buffer,string) == 0)
	{
		return 1;
	}
	return 0;
}

void ESP_Send(char * data)
{
	HAL_UART_Transmit(&huart1, (uint8_t*) data, strlen(data), 100);
	HAL_Delay(10);
}
