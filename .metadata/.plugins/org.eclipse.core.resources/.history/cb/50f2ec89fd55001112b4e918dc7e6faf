/**
 ******************************************************************************
 * @file 	stm32g4_ws2812.c
 * @author 	Samuel Poiraud 	&& Luc H�rault
 * @date 	May 3, 2016		&& 2024 --> portage sur G431
 * @brief 	Module pour controler le ws2812s
 ******************************************************************************
 *
 *@verbatim
 *	les envois de pixels sont sous-trait�s � la fonction assembleur WS2812S_send_pixel
 *      	https://github.com/Daedaluz/stm32-ws2812/tree/master/src
 *  Sur la matrice de 64 WS2812, les leds sont chain�es ligne apr�s ligne.
 *@endverbatim
 *
 */
 
 
#include "config.h"
#if USE_WS2812

#include "stm32g4xx_hal.h"
#include "stm32g4_ws2812.h"
#include "config.h"
#include "stm32g4_utils.h"
#include "stm32g4_gpio.h"
#include "stm32g4_sys.h"
extern void WS2812_send_pixel_asm(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);

#define WS2812_send_pixel(pixel) WS2812_send_pixel_asm(pixel, WS2812_PIN_DATA, (uint32_t *)&WS2812_PORT_DATA->BSRR)


asm(".section .text");
#if !(defined WS2812_PORT_DATA) || !defined(WS2812_PIN_DATA)
	#define WS2812_PORT_DATA	GPIOB
	#define WS2812_PIN_DATA		GPIO_PIN_4
#endif


#define T1H		1
#define T1L		1
#define T0H		0
#define T0L		1
#define RES     200
#define MorseArraySize     20 // Nombre de LED du bandeau (5 caractères en morse)

// Génère un signal de RESET pour le protocole WS2812.
#define OUTPUT(x)	HAL_GPIO_WritePin(WS2812_PORT_DATA, WS2812_PIN_DATA, x)


/**
 * @brief Initialisation de la broche PB4 pour le bandeau LED
 * @return void
 */
void BSP_WS2812_init(void)
{
	BSP_GPIO_pin_config(WS2812_PORT_DATA, WS2812_PIN_DATA, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF);
}

/**
 * @param 	Un pointeur vers un tableau de 20 entiers compris entre 0 et 2
 * @note	Les valeurs numériques du tableau de paramètres sont converties en couleur et ajoutées dans le tableau pixels
 * @note    Le 0 à la LED éteinte, le 1 au rouge et le 2 au vert
 */
void BSP_WS2812_Display_Morse(uint8_t * MorseArray){
	while(1)
	{
		uint32_t pixels[MorseArraySize];
		// Fill the pixels with the right color
		for(uint8_t i=0 ; i<MorseArraySize+1 ; i++)
		{
			if(MorseArray[i] == 0){
				pixels[i] = WS2812_COLOR_BLACK;
			}
			if(MorseArray[i] == 1){
				pixels[i] = WS2812_COLOR_RED;
			}
			if(MorseArray[i] == 2){
				pixels[i] = WS2812_COLOR_GREEN;
			}
		}
		BSP_WS2812_display(pixels, MorseArraySize);

		HAL_Delay(100);
	}
}

/**
 * @brief	Cette fonction envoie 20 pixels vers la matrice de leds.
 * @note	les envois de pixels sont sous-trait�s � la fonction assembleur WS2812S_send_pixel
 * 			Cette fonction est r�dig�e en assembleur pour respecter scrupuleusement les d�lais de production des signaux pour les leds de la matrice.
 * 			Remarque : les interruptions sont d�sactiv�es temporairement pendant l'ex�cution de cette fonction pour �viter qu'elles provoquent des 'pauses' lors de la production des signaux.
 * 			La dur�e d'ex�cution de cette fonction est de l'ordre de 2,5ms. Dur�e pendant laquelle aucune interruption ne peut survenir !!!
 * @param 	pixels est un tableau de 20 cases absolument...
 * @note	attention, le tableau de pixels correspond aux leds dans l'ordre o� elles sont c�bl�es. Sur la matrice 8x8, elles sont reli�es en serpentin ! (et non en recommancant � gauche � chaque nouvelle ligne)...
 */
void BSP_WS2812_display(uint32_t * pixels, uint8_t size)
{
	uint8_t i;
	// Disable IRQ Interrupts
	__disable_irq();
	BSP_WS2812_reset();
	for(i=0;i<size;i++)
		WS2812_send_pixel(pixels[i]);
	// Enable IRQ Interrupts
	__enable_irq();
}

/**
 * @brief  Génère un signal de RESET pour le protocole WS2812.
 * @details Maintient la ligne DATA à l'état bas pendant 100µs pour valider
 */
void BSP_WS2812_reset(void){

	OUTPUT(0);
	Delay_us(100);
}

#endif
