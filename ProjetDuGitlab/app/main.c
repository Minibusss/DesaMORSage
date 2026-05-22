/**
 *******************************************************************************
 * @file 	main.c
 * @author 	Nicolas CIROU et Alice GEDOUX
 * @date 	Mar 09, 2026
 * @brief	Fichier principal du projet "DesaMORSage" sur carte Nucléo STM32G431KB
 *******************************************************************************
 */

#include <stdint.h> //Fournit des types entiers à taille fixe (uint8_t, int16_t, uint32_t…)->Indispensable en embarqué pour garantir la taille exacte des variables, quel que soit le compilateur ou l’architecture.
#include <stdio.h> //Fournit les fonctions d’entrée/sortie standard (printf, scanf…)->En embarqué, on l’utilise surtout pour printf via UART ou pour du debug sur PC.

#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "../drivers/bsp/MCP23S17/stm32g4_mcp23s17.h"//Importation de la librairie pour initialiser le GPIO Expander
#include "stm32g4_matrix_keyboard.h"//Importation de la librairie pour gérer le clavier matriciel
#include "../drivers/bsp/tft_ili9341/stm32g4_ili9341.h"
#include "../drivers/bsp/tft_ili9341/stm32g4_xpt2046.h"
#include "../drivers/bsp/WS2812/stm32g4_ws2812.h"
#include "../drivers/bsp/HC-05/stm32g4_hc05.h"

/* Importation des libraries créées pour le projet*/
#include "../Morse/stm32g4_morse.h"
#include "../ClavierMatriciel/stm32g4_clavier_matriciel.h"
#include "../EcranTFT_SPI/stm32g4_ecran_tft_spi.h"

#define BLINK_DELAY		100	//ms

/* MACROS LOCALES AU FICHIER  */
#define TAILLE_MDP 21//20 + caractère de fin de chaine potentiellement
#define TAILLE_CHAINE_TRADUITE 5

/* STRUCTURE LOCALE AU FICHIER */
typedef enum{
	ETAT_INIT,
	ETAT_LECTURE_MDP,
	ETAT_ENVOI_CHAINE,
	ETAT_AFFICHAGE_MORSE,
}ETAT;

/**
  * @brief  Point d'entrée de l'application
  */
int main(void)
{
	/*
	 * Elle permet d'initialiser toutes les couches basses des drivers (Hardware Abstraction Layer),
	 * condition préalable indispensable à l'exécution des lignes suivantes.
	 */
	HAL_Init();

	/* Initialisation des périphériques utilisés dans le programme */
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID,115200);
	BSP_UART_init(UART1_ID, 38400);

	//Initialisation des LED
	BSP_WS2812_init();

	//Initialisation du GPIO expander
	BSP_MCP23S17_init();

	//Initialisation du clavier matriciel
	BSP_MATRIX_KEYBOARD_init(NULL);

	/* Indique que les printf sont dirigés vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	// Initialisation de l'écran TFT
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Portrait_1);
	ILI9341_Fill(ILI9341_COLOR_BLACK);

	// Initialisation du tactile
	XPT2046_init();

	/* Variables globales à la tache de fond */
	//ETAT_INIT
	//PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);  // affichage initial
	ETAT etatProcessus = ETAT_LECTURE_MDP;

	//ETAT_MENU

	//ETAT_SCAN_RFID
	//char nomUtilisateur[32] = "\0";

	//ETAT_LECTURE_MDP
	uint8_t motDePasse[TAILLE_MDP]="\0";
	uint8_t *pointeurSaisie = motDePasse;
	uint8_t indiceMotDePasse = 0;
	uint8_t *indiceSaisie = &indiceMotDePasse;
	//uint8_t tentativesRestantes = 3;

	//ETAT_CONNECTION_B

	//ETAT_ENVOI_CHAINE


	// Buffer de réception UART
	int8_t  buffer[21]; // 21 car CR+LF à la fin (réglage dans appli bluetooth)
    uint8_t index = 0;
    uint8_t c;

    // Tableau morse courant
   	uint8_t tableau[20] = {1,1,1,1,1,1,1,1,1};

    // Pixels LED
    uint32_t pixels[20] = {0};
    for (uint8_t i = 0; i < 20; i++)
    {
    	if (tableau[i] == 0) pixels[i] = WS2812_COLOR_BLACK;
    }
    BSP_WS2812_display(pixels, 20);



	while (1)
	{
	    switch(etatProcessus)
	    {
	        case ETAT_INIT:
	            //PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
	            //etatProcessus = ETAT_MENU;
	            break;


	        	/*
	            // On attend que l'utilisateur appuie sur "Renseigner un mdp
	            // Le tactile est géré ici directement
	            {
	                int16_t x, y;
	                if (XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	                {
	                    // Bouton "Scanner Badge" : (40,200) -> (200,240)
	                    if (x > 40 && x < 200 && y > 200 && y < 240)
	                    {
	                        PAGE_changerPage(PAGE_SCAN_RFID, NULL, tentativesRestantes);
	                        etatProcessus = ETAT_LECTURE_MDP;
	                    }
	                }
	            }*/


	        case ETAT_LECTURE_MDP:
	            // Lecture du clavier matriciel
	        	//Ici ne change pas d'état tant que le mdp n'est pas bon
	            BSP_MATRIX_KEYBOARD_process_main(pointeurSaisie, indiceSaisie);
	            if(indiceMotDePasse == 5){ //EGAL AU MDP
	            	etatProcessus = ETAT_ENVOI_CHAINE;
	            }
	            break;

	        case ETAT_ENVOI_CHAINE:
	        	if (BSP_UART_data_ready(UART1_ID)){
	        		c = BSP_UART_getc(UART1_ID);

	        		if (c == '\n' || c == '\r'){
	        			buffer[index] = '\0';
	        	    	index = 0;

	        	    	uint8_t* result = conversionChaineMorse(buffer);
	        	    	if (result != NULL){
	        	            memcpy(tableau, result, 20);
	        	    		etatProcessus = ETAT_AFFICHAGE_MORSE;
	        			}
	        		}
	        	   else if (index < 20)  // stocker le caractère
	        	   {
	        		   buffer[index++] = c;
	        	   }

	    		}
	            break;

	        case ETAT_AFFICHAGE_MORSE:
	            // Affichage de la chaine traduite
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
				etatProcessus = ETAT_ENVOI_CHAINE;
				HAL_Delay(100);
	            break;
	        default:
	            break;
	    }
	 }
}
