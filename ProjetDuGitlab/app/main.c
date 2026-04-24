/**
 *******************************************************************************
 * @file 	main.c
 * @author 	jjo et nc
 * @date 	2026
 * @brief	Fichier principal de votre projet sur carte Nucléo STM32G431KB
 *******************************************************************************
 */

#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "WS2812/stm32g4_ws2812.h"
#include <stdio.h>

/**
  * @brief  Point d'entrée de votre application
  */
uint8_t tableau[20] = {1,2,0,0,1,1,2,0,2,0,2,0,1,1,2,0,2,2,2,2};

uint8_t *ptr = tableau;

int main(void)
{
    // Initialisation de la couche matérielle basse (microprocesseur)
    HAL_Init();

    // Configuration de l'horloge à sa vitesse maximale
    SystemClock_Config();

    // Initialisation de la broche PB4 pour le bandeau LED
    BSP_WS2812_init();

    while (1)
    {
    	// Fonction d'affichage du morse
    	BSP_WS2812_Display_Morse(ptr);
    }
}
