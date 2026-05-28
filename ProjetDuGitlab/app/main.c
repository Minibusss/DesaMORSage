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
#include "stm32g4_matrix_keyboard.h"//Importation de la librairie pour gérer le clavier matriciel
#include "../drivers/bsp/MCP23S17/stm32g4_mcp23s17.h"//Importation de la librairie pour initialiser le GPIO Expander
#include "../drivers/bsp/tft_ili9341/stm32g4_ili9341.h" //Importation de la librairie pour gérer l'affichage de l'écran
#include "../drivers/bsp/tft_ili9341/stm32g4_xpt2046.h" //Importation de la librairie pour gérer la partie tactile de l'écran
#include "../drivers/bsp/WS2812/stm32g4_ws2812.h" //Importation de la librairie pour gérer le bandeau LED
#include "../drivers/bsp/HC-05/stm32g4_hc05.h" //Importation de la librairie pour gérer le module Bluetooth

/*Importation des libraries créées pour le projet*/
#include "../Morse/stm32g4_morse.h"
#include "../ClavierMatriciel/stm32g4_clavier_matriciel.h"
#include "../EcranTFT_SPI/stm32g4_ecran_tft_spi.h"

#define BLINK_DELAY		100	//ms

/*MACROS LOCALES AU FICHIER */
#define TAILLE_MDP 21//20 + caractère de fin de chaine
#define TAILLE_CHAINE_TRADUITE 5
#define ATTENDRE_RELACHEMENT() do { HAL_Delay(50); while(PAGE_lireBouton() != BOUTON_AUCUN); } while(0) //Anti-rebond

/*STRUCTURE LOCALE AU FICHIER*/
typedef enum{
	ETAT_ACCUEIL,
	ETAT_LECTURE_MDP,
	ETAT_ENVOI_CHAINE,
	ETAT_AFFICHAGE_MORSE,
}ETAT;

/**
  * @brief  Point d'entrée de l'application.
  * @param 	Rien.
  * @retval Rien.
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

	//Initialisation des LED
	BSP_WS2812_init();

	//Initialisation du GPIO expander
	BSP_MCP23S17_init();

	//Initialisation du clavier matriciel
	BSP_MATRIX_KEYBOARD_init(NULL);

	/* Indique que les printf sont dirigés vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation de l'écran TFT
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Portrait_1);
	ILI9341_Fill(ILI9341_COLOR_BLACK);

	//Initialisation du tactile
	XPT2046_init();

	/*Variables globales à la tache de fond */
	//ETAT_ACCUEIL
	ETAT etatProcessus = ETAT_ACCUEIL;

	//ETAT_LECTURE_MDP
	uint8_t motDePasse[TAILLE_MDP]="\0";
	uint8_t *pointeurSaisie = motDePasse;
	uint8_t indiceMotDePasse = 0;
	uint8_t *indiceSaisie = &indiceMotDePasse;

	//ETAT_ENVOI_CHAINE
	uint8_t* result ={};
	bool receptionActive = false;//Flag permettant de savoir s'il faut recevoir ou non une chaine de caractères
	//Buffer de réception UART du module Bluetooth
	int8_t  buffer[21]; // 21 car CR+LF à la fin (réglage dans appli bluetooth)
    uint8_t index = 0;
    uint8_t c;

    // Tableau morse courant
   	uint8_t chaineTraduite[20] = {0};

    //Initialisation du bandeau LED
    uint32_t pixels[20] = {0};
    for (uint8_t i = 0; i < 20; i++)
    {
    	if (chaineTraduite[i] == 0) pixels[i] = WS2812_COLOR_BLACK;
    }
    BSP_WS2812_display(pixels, 20);

	while (1)
	{
		switch(etatProcessus)
		{
		    case ETAT_ACCUEIL:
		        PAGE_changerPage(PAGE_ACCUEIL, 0, NULL);//Affichage de la page d'accueil
		        while (PAGE_lireBouton() != BOUTON_SAISIR_MDP);//Attendre tant que le bouton tactile n'a pas été préssé
		        ATTENDRE_RELACHEMENT();
		        etatProcessus = ETAT_LECTURE_MDP;//Changement d'état
		        break;

		    case ETAT_LECTURE_MDP:
		        PAGE_changerPage(PAGE_MDP, 0, NULL);//Affichage de la page mot de passe
		        indiceMotDePasse = 0;
		        pointeurSaisie = motDePasse;
		        memset(motDePasse, 0, TAILLE_MDP);//Remise à "0"/vide du tableau contenant le mot de passe
		        uint8_t dernierIndice = 0xFF;//L'indice de fin du tableau contenant le mot de passe

		        //Boucle infinie
		        while (1) {
		        	//Récupération des sorties du clavier matriciel et ajout de
		        	//la saisie dans le tableau mot de passe
		            BSP_MATRIX_KEYBOARD_process_main(pointeurSaisie, indiceSaisie);

		            if (indiceMotDePasse != dernierIndice) {
		                PAGE_afficherMDP(indiceMotDePasse);//Affichage de la page mot de passe
		                dernierIndice = indiceMotDePasse;
		            }

		            if (PAGE_lireBouton() == BOUTON_VALIDER) {
		            	ATTENDRE_RELACHEMENT();
		                if (motDePasse[0] == '1' && motDePasse[1] == 'B' &&
		                    motDePasse[2] == 'C' && motDePasse[3] == '0' &&
		                    motDePasse[4] == '7') { //Mot de passe : 1BC07
		                    PAGE_changerPage(PAGE_ENVOI_BT, 0, NULL);//Affichage de la page Bluetooth
		                    receptionActive = false;//Désactive la reception
		                    etatProcessus = ETAT_ENVOI_CHAINE;//Changement d'état
		                    break;
		                }
		                indiceMotDePasse = 0;
		                dernierIndice = 0xFF;
		                memset(motDePasse, 0, TAILLE_MDP);//Remise à "0"/vide du tableau contenant le mot de passe
		                PAGE_afficherMDP(0);
		            }
		        }
		        break;

		    case ETAT_ENVOI_CHAINE:
		        BSP_UART_init(UART1_ID, 38400);	//Activation Bluetooth à l'entrée de l'état -> l'UART peut recevoir
		        PAGE_changerPage(PAGE_ENVOI_BT, 0, NULL);//Affichage de la page Bluetooth

		        //Boucle infinie
		        while (1) {
		            if (PAGE_lireBouton() == BOUTON_ACTIVER_BT) {
		                ATTENDRE_RELACHEMENT();
		                receptionActive = true;//Active la reception
		                etatProcessus = ETAT_AFFICHAGE_MORSE;//Changement d'état
		                break;
		            }
		            else if (PAGE_lireBouton() == BOUTON_ACCUEIL) {
		                ATTENDRE_RELACHEMENT();
		                receptionActive = false;
		                BSP_UART_deinit(UART1_ID);//Désactivation Bluetooth car retour à l'accueil
		                PAGE_changerPage(PAGE_ACCUEIL, 0, NULL);//Affichage de la page d'accueil
		                etatProcessus = ETAT_ACCUEIL;//Changement d'état
		                uint32_t pixels[20] = {0};
		                for (uint8_t i = 0; i < 20; i++) {
		                    if (chaineTraduite[i] == 0) pixels[i] = WS2812_COLOR_BLACK;
		                }
		                BSP_WS2812_display(pixels, 20);//Remise à 0 des LED du bandeau
		                break;
		            }
		        }

		        while (receptionActive)
		        	if(BSP_UART_data_ready(UART1_ID)) {
						c = BSP_UART_getc(UART1_ID);//Récupère ce qui vient d'être envoyé
						if (c == '\n' || c == '\r') {//Permet de détécter si l'envoi est fini
							buffer[index] = '\0';
							index = 0;
							result = conversionChaineMorse(buffer);//Stocke la chaine traduite
							if (result != NULL) {
								memcpy(chaineTraduite, result, 20);
								etatProcessus = ETAT_AFFICHAGE_MORSE;
								BSP_UART_deinit(UART1_ID);//Désactivation BT = réception terminée
								goto sortir_envoi;//Permet de sortir de la boucle while
							}
						} else if (index < 20) {//Ajoute le caractère dans le buffer
							buffer[index++] = c;
						}
		        	}
		        }
		        sortir_envoi:
		        break;

		    case ETAT_AFFICHAGE_MORSE:
		    	PAGE_changerPage(PAGE_ENVOI_BT, 0, NULL);//Affichage de la page Bluetooth

		        for (uint8_t i = 0; i < 20; i++) {
		            if      (chaineTraduite[i] == 0) pixels[i] = WS2812_COLOR_BLACK;
		            else if (chaineTraduite[i] == 1) pixels[i] = WS2812_COLOR_RED;
		            else if (chaineTraduite[i] == 2) pixels[i] = WS2812_COLOR_GREEN;
		        }
		        BSP_WS2812_display(pixels, 20);//Bandeau LED affichage
		        etatProcessus = ETAT_ENVOI_CHAINE;//Changement d'état
		        break;

		    default:
		        break;

	    }
	}
}
