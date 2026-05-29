/**
 *******************************************************************************
 * @file 	stm32g4_clavier_matriciel.c
 * @author 	Alice GEDOUX
 * @date 	Avr 27, 2026
 * @brief	Fichier principal pour ajouter une nouvelle entrée du clavier
 * matriciel dans le tableau contenant le MDP
 *******************************************************************************
 */

#include "stm32g4_clavier_matriciel.h"

/**
 * @brief La fonction permet de modifier, par les pointeurs, les élements du tableau de taille 20 vers lequel pointeurSaisie pointe
 * @param uint8_t * pointeurSaisie : le pointeur vers un tableau dont on cherche à modifier les éléments
 * @param uint8_t * indiceSaisie : le pointeur qui nous permet de nous situer dans le tableau
 * @param uint8_t press_key_event : le caractère qui a été detecté par le clavier matriciel
 * @retval Rien
 */
void ajoutSaisieMotDePasse(uint8_t * pointeurSaisie, uint8_t * indiceSaisie, uint8_t press_key_event){
	if (*indiceSaisie < 20) {
	    pointeurSaisie[*indiceSaisie] = press_key_event;
	    (*indiceSaisie)++;
	}
}
