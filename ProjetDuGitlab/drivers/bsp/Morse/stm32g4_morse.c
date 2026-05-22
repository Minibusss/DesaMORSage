/**
 *******************************************************************************
 * @file 	stm32g4_morse.c
 * @author 	Alice GEDOUX
 * @date 	Avr 10, 2026
 * @brief	Fichier principal pour convertir une chaine de 5 caractères en morse
 *******************************************************************************
 */
#ifdef TEST
	#include "../Tests/test_morse.h"
#else
	#include "stm32g4_morse.h"
#endif

#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

//MACROS LOCALES AU FICHIER
//Pour la structure "alphabet"
#define TAILLE_TABLEAU_CODE 4
#define TAILLE_TABLEAU_ALPHABET_MORSE 26

//Pour
#define TAILLE_TABLEAU_CONVERSION 5
#define TAILLE_TABLEAU_ALPHABET 26

#define TAILLE_CHAINE_CONVERTI 29

//STRUCTURES LOCALES AU FICHIER
typedef struct{
	uint8_t lettre;
	uint8_t code[TAILLE_TABLEAU_CODE];
}lettresCode;

//VARIABLES LOCALES AU FICHIER
lettresCode alphabet[TAILLE_TABLEAU_ALPHABET_MORSE] = {
    {'A', {1,2,0,0}},    // .-
    {'B', {2,1,1,1}},    // -...
    {'C', {2,1,2,1}},    // -.-.
    {'D', {2,1,1,0}},    // -..
    {'E', {1,0,0,0}},    // .
    {'F', {1,1,2,1}},    // ..-.
    {'G', {2,2,1,0}},    // --.
    {'H', {1,1,1,1}},    // ....
    {'I', {1,1,0,0}},    // ..
    {'J', {1,2,2,2}},    // .---
    {'K', {2,1,2,0}},    // -.-
    {'L', {1,2,1,1}},    // .-..
    {'M', {2,2,0,0}},    // --
    {'N', {2,1,0,0}},    // -.
    {'O', {2,2,2,0}},    // ---
    {'P', {1,2,2,1}},    // .--.
    {'Q', {2,2,1,2}},    // --.-
    {'R', {1,2,1,0}},    // .-.
    {'S', {1,1,1,0}},    // ...
    {'T', {2,0,0,0}},    // -
    {'U', {1,1,2,0}},    // ..-
    {'V', {1,1,1,2}},    // ...-
    {'W', {1,2,2,0}},    // .--
    {'X', {2,1,1,2}},    // -..-
    {'Y', {2,1,2,2}},    // -.--
    {'Z', {2,2,1,1}}     // --..
};
uint8_t chaineValide[5]="";
uint8_t nombreElements = 0;


#ifdef TEST
    #define PRIVATE
#else
    #define PRIVATE static
	/*
	 * Fonction privée
	 */
	static uint8_t verifierChaineValide(int8_t * chaineAConvertir);
#endif

	/**
	 * @brief  Vérifie que la chaîne est dans le bon format et extrait les caractères alphabétiques.
	 * @details Parcourt la chaîne caractère par caractère, conserve uniquement les caractères
	 *          alphabétiques (max 5), les convertit en majuscules et les stocke dans chaineValide[].
	 *          Met à jour la variable globale nombreElements avec le nombre de caractères valides trouvés.
	 * @param[in]  chaineAConvertir  Pointeur vers la chaîne à analyser (terminée par '\0').
	 * @retval 1  La chaîne contient au moins un caractère alphabétique valide.
	 * @retval 0  La chaîne ne contient aucun caractère alphabétique valide.
	 */
	PRIVATE uint8_t verifierChaineValide(int8_t * chaineAConvertir){
		uint8_t indice = 0;
		nombreElements = 0;
		int i = 0;
		while(chaineAConvertir[i]!='\0')
		{
			/*warning: array subscript has type 'char' [-Wchar-subscripts] if(isalpha(chaineAConvertir[i]) && nombreElements < 5)*/
			if(isalpha(chaineAConvertir[i]) && nombreElements < 5)
			{
				chaineValide[indice] = toupper(chaineAConvertir[i]);
				nombreElements++;
				indice++;
			}
			i++;
		}
		return (nombreElements > 0);
	}

	/**
	 * @brief  Convertit une chaîne de caractères en tableau de codes Morse sur 20 cases.
	 * @details Appelle verifierChaineValide() pour valider et nettoyer la chaîne d'entrée,
	 *          puis recherche chaque lettre dans la table alphabet[] et copie son code Morse
	 *          (4 valeurs) dans le tableau de sortie chaineConverti[].
	 *          Si la chaîne contient moins de 5 lettres, les cases restantes sont remplies de 0 (éteint).
	 *          Encodage Morse : 0 = éteint, 1 = point (.), 2 = tiret (-).
	 * @note   Le tableau retourné est statique — son contenu est écrasé à chaque appel.
	 * @param[in]  chaineAConvertir  Pointeur vers la chaîne à convertir (max 5 lettres, terminée par '\0').
	 * @retval Pointeur vers le tableau chaineConverti[20] contenant les codes Morse.
	 * @retval NULL  Si la chaîne ne contient aucun caractère alphabétique valide.
	 */
	uint8_t* conversionChaineMorse(int8_t * chaineAConvertir){
		uint8_t valide = verifierChaineValide(chaineAConvertir);
		uint8_t indiceChaineConverti = 0;
		static uint8_t chaineConverti[20];
		memset(chaineConverti, 0, sizeof(chaineConverti));
		if(valide == 0)
		{
			return NULL;
		}

		for(uint8_t i = 0; i < nombreElements; i++)
		{
			for(uint8_t j = 0; j < TAILLE_TABLEAU_ALPHABET_MORSE; j++)
			{
				if(chaineValide[i] == alphabet[j].lettre)
				{
					for(uint8_t k = 0; k < 4; k++)
					{
						chaineConverti[indiceChaineConverti]=alphabet[j].code[k];
						indiceChaineConverti+=1;
					}
					break;
				}
			}
		}

		if(nombreElements != 5){
			for (uint8_t l = 0; l<5-nombreElements;l++){
				for (uint8_t m = 0; m<4 ;m++){
					chaineConverti[indiceChaineConverti]=0;
					indiceChaineConverti++;
				}
			}
		}

		nombreElements = 0;
		memset(chaineValide, 0, 5);
		return chaineConverti;
	}
