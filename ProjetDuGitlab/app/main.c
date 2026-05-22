/**
 *******************************************************************************
 * @file    main.c
 * @author  Cirou Nicolas
 * @date    2026
 * @brief   Affichage Morse sur bandeau LED via HC-05
 *******************************************************************************
 */

#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "WS2812/stm32g4_ws2812.h"
#include "HC-05/stm32g4_hc05.h"
#include "Morse/stm32g4_morse.h"
#include <string.h>

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Configurer le HC-05 (1 seul fois)
    // HC05_set_echo_for_AT_mode();

    BSP_UART_init(UART1_ID, 38400);
    BSP_WS2812_init();

    // Buffer de réception UART
    int8_t  buffer[21]; // 21 car CR+LF à la fin (réglage dans appli bluetooth)
    uint8_t index = 0;
    uint8_t c;

    // Pixels LED
    uint32_t pixels[20] = {0};
    //BSP_WS2812_display(pixels, 20);

    // Tableau morse courant
    uint8_t tableau[20] = {1,1,1,1,1,1,1,1,1};

    while (1)
    {
        // ---------------------------------------------------------
        // Réception Bluetooth (HC-05 via UART1)
        // --------------------------------------------------------
    	if (BSP_UART_data_ready(UART1_ID))
    	{
    	    c = BSP_UART_getc(UART1_ID);

    	    if (c == '\n' || c == '\r')  // fin de trame
    	    {
    	        buffer[index] = '\0';
    	        index = 0;

    	        uint8_t* result = conversionChaineMorse(buffer);
    	        if (result != NULL)
    	            memcpy(tableau, result, 20);
    	    }
    	    else if (index < 20)  // stocker le caractère
    	    {
    	        buffer[index++] = c;
    	    }
        }

        // --------------------------------------------------------
        // Affichage LED
        // --------------------------------------------------------
        for (uint8_t i = 0; i < 20; i++)
        {
            if      (tableau[i] == 0) pixels[i] = WS2812_COLOR_BLACK;
            else if (tableau[i] == 1) pixels[i] = WS2812_COLOR_RED;
            else if (tableau[i] == 2) pixels[i] = WS2812_COLOR_GREEN;
        }
        BSP_WS2812_display(pixels, 20);

        HAL_Delay(100);
    }
}
