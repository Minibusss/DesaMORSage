/**
 *******************************************************************************
 * @file 	test_morse.h
 * @author 	Alice GEDOUX
 * @date 	Avr 27, 2026
 * @brief	Module pour tester le module "stm32g4_morse.c"
 *******************************************************************************
 */

#ifndef TEST_MORSE_H
#define TEST_MORSE_H

#include <stdint.h>
/**
 * Fonctions publiques pour le fichier morse.c
 */
uint8_t* conversionChaineMorse(int8_t * chaineAConvertir);
uint8_t verifierChaineValide(int8_t * chaineAConvertir);

/**
 * Fonctions publiques pour les tests
 */
void test_verifierChaineValide(void);
void test_conversionChaineABCDE(void);
void test_conversionChaineFGHIJ(void);
void test_conversionChaineKLMNO(void);
void test_conversionChainePQRST(void);
void test_conversionChaineUVWXY(void);
void test_conversionChaineZ(void);

#endif /* STM32G4_MORSE_H_ */
