/**
 *******************************************************************************
 * @file	stm32g4_clavier_matriciel.h
 * @author	Alice GEDOUX
 * @date	Avr 27, 2026
 * @brief	Module pour ajouter une nouvelle entrée du clavier matriciel
 * dans un tableau
 *******************************************************************************
 */

#ifndef STM32G4_CLAVIER_MATRICIEL_H_
#define STM32G4_CLAVIER_MATRICIEL_H_

#include <stdint.h> //Fournit des types entiers à taille fixe (uint8_t, int16_t, uint32_t…)->Indispensable en embarqué pour garantir la taille exacte des variables, quel que soit le compilateur ou l’architecture.

void ajoutSaisieMotDePasse(uint8_t * pointeurSaisie, uint8_t * indiceSaisie,uint8_t press_key_event);

#endif /* STM32G4_CLAVIER_MATRICIEL_H_ */
