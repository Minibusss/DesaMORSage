/**
 *******************************************************************************
 * @file 	stm32g4_morse.h
 * @author 	Alice GEDOUX
 * @date 	Avr 10, 2026
 * @brief	Module pour convertir une chaine de caractères en morse.
 *******************************************************************************
 */

#ifndef STM32G4_MORSE_H_
#define STM32G4_MORSE_H_

#include <stdint.h>

/**
 * Fonction publique
 */
uint8_t* conversionChaineMorse(int8_t * chaineAConvertir);

#endif /* STM32G4_MORSE_H_ */
