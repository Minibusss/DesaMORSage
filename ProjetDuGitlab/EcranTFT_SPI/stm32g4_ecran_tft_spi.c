/*
 * stm32g4_ecran_tft_spi.c
 *
 *  Created on: 09 may 2026
 *      Author: agedo
 */

#include "stm32g4_ecran_tft_spi.h"
#include "stm32g4_ili9341.h"
#include "stm32g4_xpt2046.h"

static Page_e pageCourante = PAGE_ACCUEIL;

/* ============================================================
 *  PAGE ACCUEIL
 *  Nom du projet, auteurs, bouton "Scanner Badge"
 * ============================================================ */
void PAGE_afficherAccueil(void) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);

    // Titre
    ILI9341_Puts(30, 40,  "DesaMORSage",   &Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_TRANSPARENT);
    // Auteurs
    ILI9341_Puts(20, 90,  "CIROU Nicolas",     &Font_11x18, ILI9341_COLOR_WHITE,  ILI9341_TRANSPARENT);
    ILI9341_Puts(20, 115, "GEDOUX Alice",     &Font_11x18, ILI9341_COLOR_WHITE,  ILI9341_TRANSPARENT);

    // Bouton "Scanner Badge"
    ILI9341_DrawFilledRectangle(40, 200, 200, 240, ILI9341_COLOR_BLUE);
    ILI9341_Puts(55, 212, "Scanner Badge", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
}

/* ============================================================
 *  PAGE SCAN RFID
 *  Scan en cours...
 * ============================================================ */
void PAGE_afficherScanRFID(void) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);

    ILI9341_Puts(30, 100, "Scan en cours...", &Font_11x18, ILI9341_COLOR_CYAN, ILI9341_TRANSPARENT);
    ILI9341_DrawCircle(120, 200, 30, ILI9341_COLOR_CYAN);  // visuel décoratif
}

/* ============================================================
 *  PAGE MOT DE PASSE
 *  Nom utilisateur, champ mdp, bouton Valider, tentatives restantes
 * ============================================================ */
void PAGE_afficherMDP(char *nomUtilisateur, uint8_t tentativesRestantes) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);

    // Nom utilisateur
    ILI9341_Puts(10, 20, "Utilisateur :", &Font_7x10, ILI9341_COLOR_WHITE,  ILI9341_TRANSPARENT);
    ILI9341_Puts(10, 35, nomUtilisateur,  &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_TRANSPARENT);

    // Champ MDP (rectangle vide = zone de saisie visuelle)
    ILI9341_Puts(10, 80, "Mot de passe :", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
    ILI9341_DrawRectangle(10, 95, 230, 120, ILI9341_COLOR_GRAY);

    // Tentatives restantes (affiché seulement si < 3)
    if (tentativesRestantes < 3) {
        char msg[40];
        sprintf(msg, "Il reste %d tentative(s) !", tentativesRestantes);
        ILI9341_Puts(10, 150, msg, &Font_7x10, ILI9341_COLOR_RED, ILI9341_TRANSPARENT);
    }

    // Bouton Valider
    ILI9341_DrawFilledRectangle(60, 200, 180, 240, ILI9341_COLOR_GREEN);
    ILI9341_Puts(80, 212, "Valider", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN);
}

/* ============================================================
 *  PAGE ENVOI BLUETOOTH
 *  Message de connexion BT + bouton Quitter
 * ============================================================ */
void PAGE_afficherEnvoiB(void) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);

    ILI9341_Puts(10, 50,  "Connectez votre",    &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
    ILI9341_Puts(10, 75,  "telephone via BT",   &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
    ILI9341_Puts(10, 110, "pour envoyer.",      &Font_11x18, ILI9341_COLOR_CYAN,  ILI9341_TRANSPARENT);

    // Bouton Quitter session
    ILI9341_DrawFilledRectangle(40, 260, 200, 300, ILI9341_COLOR_RED);
    ILI9341_Puts(60, 272, "Quitter session", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);
}

/* ============================================================
 *  GESTIONNAIRE DE PAGES
 *  Appelle la bonne page et gère les touches
 * ============================================================ */
void PAGE_changerPage(Page_e nouvellePage, char *nomUtilisateur, uint8_t tentatives) {
    pageCourante = nouvellePage;
    switch (pageCourante) {
        case PAGE_ACCUEIL:      PAGE_afficherAccueil();                        break;
        case PAGE_SCAN_RFID:    PAGE_afficherScanRFID();                       break;
        case PAGE_MDP:          PAGE_afficherMDP(nomUtilisateur, tentatives);  break;
        case PAGE_ENVOI_B:      PAGE_afficherEnvoiB();                         break;
        default: break;
    }
}

/* ============================================================
 *  GESTION DU TACTILE  (à appeler dans ta boucle principale)
 * ============================================================ */
void PAGE_handleTouch(void) {
    int16_t x, y;

    if (!XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
        return;

    switch (pageCourante) {

        case PAGE_ACCUEIL:
            // Bouton "Scanner Badge" : rectangle (40,200) -> (200,240)
            if (x > 40 && x < 200 && y > 200 && y < 240)
                PAGE_changerPage(PAGE_SCAN_RFID, NULL, 3);
            break;

        case PAGE_MDP:
            // Bouton "Valider" : rectangle (60,200) -> (180,240)
            if (x > 60 && x < 180 && y > 200 && y < 240) {
                // --> ici tu vérifies le MDP
                // Si bon :
                PAGE_changerPage(PAGE_ENVOI_B, NULL, 3);
                // Si mauvais : PAGE_changerPage(PAGE_MDP, nomUtilisateur, tentatives - 1);
            }
            break;

        case PAGE_ENVOI_B:
            // Bouton "Quitter" : rectangle (40,260) -> (200,300)
            if (x > 40 && x < 200 && y > 260 && y < 300)
                PAGE_changerPage(PAGE_ACCUEIL, NULL, 3);
            break;

        default: break;
    }
}
