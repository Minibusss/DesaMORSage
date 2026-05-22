/**
 *******************************************************************************
 * @file 	stm32g4_gestion_utilisateur.h
 * @author 	Alice GEDOUX
 * @date 	Avr 27, 2026
 * @brief	Module qui permet de gérer les utilisateur
 *******************************************************************************
 */


#ifndef STM32G4_GESTION_UTILISATEUR_H_
#define STM32G4_GESTION_UTILISATEUR_H_

#include <stdint.h> //Fournit des types entiers à taille fixe (uint8_t, int16_t, uint32_t…)->Indispensable en embarqué pour garantir la taille exacte des variables, quel que soit le compilateur ou l’architecture.

uint8_t verificationUtilisateurConnu(uint8_t * pointeurIDBadge);
uint8_t verificationUtilisateurMDPCorrect(uint8_t * pointeurIDBadge, uint8_t * pointeurSaisie);
uint8_t ajouterUtilisateur(utilisateurBadge * pointeurNouveauUtilisateur);
uint8_t ajouterUtilisateurDansBaseDeDonnees(utilisateurBadge * pointeurAdmin, utilisateurBadge * pointeurNouveauUtilisateurBadge);

#endif /* STM32G4_GESTION_UTILISATEUR_H_ */
