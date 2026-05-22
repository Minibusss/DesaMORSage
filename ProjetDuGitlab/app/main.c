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
	ETAT_SCAN_RFID,
	ETAT_LECTURE_MDP,
	ETAT_CONNECTION_B,
	ETAT_ENVOI_CHAINE,
	ETAT_MENU,
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
	//uint8_t chaineTraduite[TAILLE_CHAINE_TRADUITE]="\0";
	//uint8_t *pointeurChaineTraduite = chaineTraduite;

	while (1)
	{
	    switch(etatProcessus)
	    {
	        case ETAT_INIT:
	            //PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
	            //EetatProcessus = ETAT_MENU;
	            break;

	        case ETAT_MENU:
	        	/*
	            // On attend que l'utilisateur appuie sur "Scanner Badge"
	            // Le tactile est géré ici directement
	            {
	                int16_t x, y;
	                if (XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	                {
	                    // Bouton "Scanner Badge" : (40,200) -> (200,240)
	                    if (x > 40 && x < 200 && y > 200 && y < 240)
	                    {
	                        PAGE_changerPage(PAGE_SCAN_RFID, NULL, tentativesRestantes);
	                        etatProcessus = ETAT_SCAN_RFID;
	                    }
	                }
	            }*/
	            break;

	        case ETAT_SCAN_RFID:
	        	/*
	            // Lecture du badge RFID
	            // TODO : appel à ta fonction de lecture RFID
	            // Exemple : if (RFID_lireBadge(nomUtilisateur)) { ... }
	            {
	                // Simulé pour l'exemple, remplace par ta vraie lecture RFID
	                bool badgeLu = false; // <- remplacer par RFID_lireBadge(nomUtilisateur)

	                if (badgeLu)
	                {
	                    tentativesRestantes = 3;
	                    memset(motDePasse, 0, TAILLE_MDP);
	                    indiceMotDePasse = 0;
	                    PAGE_changerPage(PAGE_MDP, nomUtilisateur, tentativesRestantes);
	                    etatProcessus = ETAT_LECTURE_MDP;
	                }
	            }*/
	            break;

	        case ETAT_LECTURE_MDP:
	            // Lecture du clavier matriciel
	            BSP_MATRIX_KEYBOARD_process_main(pointeurSaisie, indiceSaisie);
	            if(indiceSaisie == 5){
	            	etatProcessus = ETAT_CONNECTION_B;
	            }

	            // Vérification bouton "Valider" sur l'écran tactile
	            /*
	            {
	                int16_t x, y;
	                if (XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	                {
	                    // Bouton "Valider" : (60,200) -> (180,240)
	                    if (x > 60 && x < 180 && y > 200 && y < 240)
	                    {
	                        // TODO : remplacer par ta vraie vérification MDP
	                        bool mdpCorrect = false; // <- verifierMDP(motDePasse)

	                        if (mdpCorrect)
	                        {
	                            PAGE_changerPage(PAGE_ENVOI_B, nomUtilisateur, tentativesRestantes);
	                            etatProcessus = ETAT_CONNECTION_B;
	                        }
	                        else
	                        {
	                            tentativesRestantes--;

	                            if (tentativesRestantes == 0)
	                            {
	                                // Blocage -> retour accueil
	                                tentativesRestantes = 3;
	                                PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
	                                etatProcessus = ETAT_MENU;
	                            }
	                            else
	                            {
	                                // Réaffiche la page MDP avec les tentatives restantes
	                                memset(motDePasse, 0, TAILLE_MDP);
	                                indiceMotDePasse = 0;
	                                PAGE_changerPage(PAGE_MDP, nomUtilisateur, tentativesRestantes);
	                                // On reste dans ETAT_LECTURE_MDP
	                            }
	                        }
	                    }
	                }
	            }*/
	            break;

	        case ETAT_CONNECTION_B:
	        	/*
	            // On attend la réception d'une chaine via Bluetooth
	            {
	                // TODO : remplacer par ta vraie fonction de réception BT
	                // Exemple : if (BT_chaineRecue(chaineBluetooth)) { ... }
	                bool chaineRecue = false; // <- BT_chaineRecue(chaineBluetooth)

	                if (chaineRecue)
	                {
	                    // Conversion en morse
	                    chaineTraduite = conversionChaineMorse(chaineBluetooth);

	                    if (chaineTraduite != NULL)
	                    {
	                        PAGE_changerPage(PAGE_AFFICHAGE_CHAINE, nomUtilisateur, tentativesRestantes);
	                        etatProcessus = ETAT_ENVOI_CHAINE;
	                    }
	                }

	                // Bouton "Quitter session"
	                int16_t x, y;
	                if (XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	                {
	                    if (x > 40 && x < 200 && y > 260 && y < 300)
	                    {
	                        // Libération mémoire si chaine allouée dynamiquement
	                        if (chaineTraduite != NULL)
	                        {
	                            free(chaineTraduite);
	                            chaineTraduite = NULL;
	                        }
	                        PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
	                        etatProcessus = ETAT_MENU;
	                    }
	                }
	            }
	            break;

	        case ETAT_ENVOI_CHAINE:
	            // Affichage de la chaine traduite
	            {
	                PAGE_afficherChaineTraduite(chaineTraduite);  // à créer dans tes pages

	                // Libération mémoire après affichage
	                if (chaineTraduite != NULL)
	                {
	                    free(chaineTraduite);
	                    chaineTraduite = NULL;
	                }

	                // Bouton "Quitter session" ou retour automatique
	                int16_t x, y;
	                if (XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	                {
	                    if (x > 40 && x < 200 && y > 260 && y < 300)
	                    {
	                        PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
	                        etatProcessus = ETAT_MENU;
	                    }
	                }
	            }
	            break;
			*/
	        default:
	            break;
	    }
	 }
}
