/**
 *******************************************************************************
 * @file 	stm32g4_ecran_tft_spi.h
 * @author 	Alice GEDOUX
 * @date 	Mai 09, 2026
 * @brief	Module qui permet de gérer les pages de l'écran.
 *******************************************************************************
 */
#ifndef STM32G4_ECRAN_TFT_SPI_H_
#define STM32G4_ECRAN_TFT_SPI_H_

#include <stdint.h>

typedef enum {
    PAGE_ACCUEIL,
    PAGE_MDP,
    PAGE_ENVOI_BT,
} Page_e;

typedef enum {
    BOUTON_AUCUN,
    BOUTON_SAISIR_MDP,
    BOUTON_VALIDER,
    BOUTON_ACTIVER_BT,
    BOUTON_NOUVELLE_RECEPTION,
    BOUTON_ACCUEIL,
} BOUTON_e;

void PAGE_changerPage(Page_e nouvellePage, uint8_t nbCaractereSaisis, int8_t *chaineTraduite);
void PAGE_afficherMDP(uint8_t nbCaractereSaisis);
BOUTON_e PAGE_lireBouton(void);

#endif
