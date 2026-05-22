/*
 * stm32g4_ecran_tft_spi.h
 *
 *  Created on: 09 may 2026
 *      Author: agedo
 */

#ifndef STM32G4_ECRAN_TFT_SPI_H_
#define STM32G4_ECRAN_TFT_SPI_H_

/* ---- Types ---- */
typedef enum {
    PAGE_ACCUEIL,
    PAGE_MENU,
    PAGE_SCAN_RFID,
    PAGE_MDP,
    PAGE_ENVOI_B,
    PAGE_AFFICHAGE_CHAINE,
} Page_e;

void PAGE_afficherAccueil(void);
void PAGE_afficherScanRFID(void);
void PAGE_afficherMDP(char *nomUtilisateur, uint8_t tentativesRestantes);
void PAGE_afficherEnvoiB(void);
void PAGE_changerPage(Page_e nouvellePage, char *nomUtilisateur, uint8_t tentatives);
void PAGE_handleTouch(void);
uint8_t lectureBoutonValiderMDP(void);

#endif
