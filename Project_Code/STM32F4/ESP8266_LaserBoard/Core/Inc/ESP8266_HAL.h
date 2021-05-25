/*
 * ESP8266_HAL.h
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */

#ifndef INC_ESP8266_HAL_H_
#define INC_ESP8266_HAL_H_

void Uart_Transmit_PC(char * string);

void Uart_Transmit_ESP(char * string);

void ESP_Init (char *SSID, char *PASSWD);

void Server_Start (void);


#endif /* INC_ESP8266_HAL_H_ */
