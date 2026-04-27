/**
 *******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte Nucléo STM32G431KB
 *******************************************************************************
 */

#include "config.h"
#include "stm32g4_sys.h"

#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "stm32g4_matrix_keyboard.h"//Importation de la librarie pour gérer le clavier
#include <stdio.h>
#include <stdint.h>
#include "../Morse/stm32g4_morse.h"

#define BLINK_DELAY		100	//ms

bool char_received(uart_id_t uart_id)
{
	if( BSP_UART_data_ready(uart_id) )	/* Si un caractère est reçu sur l'UART 2*/
	{
		/* On "utilise" le caractère pour vider le buffer de réception */
		BSP_UART_get_next_byte(uart_id);
		return true;
	}
	else
		return false;
}

/**
  * @brief  Point d'entrée de votre application
  */
int main(void)
{
	/* Cette ligne doit rester la première de votre main !
	 * Elle permet d'initialiser toutes les couches basses des drivers (Hardware Abstraction Layer),
	 * condition préalable indispensable à l'exécution des lignes suivantes.
	 */
	HAL_Init();

	/* Initialisation des périphériques utilisés dans votre programme */
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID,115200);

	/* Initialisation du clavier matriciel */
	const char clavierMatriciel[16]={'1','2','3','A',
			'4','5','6','B',
			'7','8','9','C',
			'*','0','#','D'};
	BSP_MATRIX_KEYBOARD_init(clavierMatriciel);

	/* Indique que les printf sont dirigés vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/* Initialisation du port de la led Verte (carte Nucleo) */
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH,GPIO_NO_AF);
	/*
			if( char_received(UART2_ID) )
			{
				write_LED(true);		 write_LED? Faites un ctrl+clic dessus pour voir...
				HAL_Delay(BLINK_DELAY);	... ça fonctionne aussi avec les macros, les variables. C'est votre nouveau meilleur ami
				write_LED(false);
			}*/
	/* Hello student */
	//printf("Hi <Student>, can you read me?\n");

	/* Tâche de fond, boucle infinie, Infinite loop,... quelque soit son nom vous n'en sortirez jamais */
	uint8_t *chaineTraduite;
	while (1)
	{
		chaineTraduite = conversionChaineMorse("ABC3DE");
		printf("Chaine {");
		for (uint8_t i = 0; i<20; i++){
			printf(" %d ",chaineTraduite[i]);
		}
		printf("}");

	}
}
