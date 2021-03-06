
#include "players.h"
#include <stdio.h>

/* deelnemerslijst */

PLAYER deelnemerslijst[10] =
{
	//	{  ID ,  Name 			}
		{ 10 , "RED" 			},
		{ 20 , "GREEN"			},
		{ 30 , "BLUE"			},
		{ 40 , "YELLOW" 		}, 		/* vergeet hier geen ',' te zetten!!!*/
	 	{ 50 , "ORANGE"			},
		{ 60 , "PURPLE"			},
		{ 70 , "PINK"		    },
		{ 80 , "BLACK"			},
		{ 90 , "WHITE"			}
		//{ 		, 				},

};

/* Functies */


/*
 * LookForKillerID:
 *
 * -> vergelijkt de ontvangen IR data met de ID's van de deelnemerslijst
 * is ID aanwezig in de deelnemerslijst dan '1' returnen
 * anders wordt een '0' terug gestuurd
 */
uint8_t LookForKillerID( uint8_t DATA )
{
	if(DATA >= 0xF0)
	{
		//Foutieve data ontvangen ( alles boven 0xF0 is fout!)
		printf("Fake Data!\r\n");
		return 0;
	}

	for(uint8_t i = 0; i < sizeof(deelnemerslijst) ; i++ )
	{
		if(DATA == deelnemerslijst[i].ID )
		{
			// speler gevonden (staat in lijst)
			return 1;
		}
		else
		{
			// geen ID gevonden
		}
	}

	// ID niet gevonden ( geen bestaande speler gevonden )
	return 0;
}


/*
 * NameOfKillerInString:
 *
 * -> Wanneer DATA gelijk is aan een deelnemerslijst player
 * zet de functie de naam van de speler in de string die meegegeven wordt met de functie
 */
uint8_t NameOfKillerInString ( char * string[] , uint8_t DATA )
{
	for (uint8_t i = 0; i < sizeof(deelnemerslijst) ; i++ )
	{
		if (DATA == deelnemerslijst[i].ID )
		{
			//Geselecteerde speler is gevonden
			strcpy(string,deelnemerslijst[i].Name);
			return 1;
		}
	}

	//default name invullen
	strcpy(string,"Unknown");
	return 0;
}



uint8_t CheckIR_Byte(uint8_t data)
{
	for(uint8_t i = 1; i <= 9; i++)
	{
		if(data == 10 * i )
		{
			return 1; 	//data is een correcte data
		}
	}
	return  0; // geen correcte data
}
