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
#include "WS2812/stm32g4_ws2812.h"
#include <stdio.h>

/**
  * @brief  Point d'entrée de votre application
  */
int main(void)
{
    // 1. Initialisation de la couche matérielle basse
    HAL_Init();

    // 2. Configuration de l'horloge à sa vitesse maximale (TRÈS IMPORTANT)
    SystemClock_Config();

    // 3. Initialisation de la broche PB4 pour le bandeau
    BSP_WS2812_init();

    while (1)
    {
    	BSP_WS2812_demo();
    }
}
