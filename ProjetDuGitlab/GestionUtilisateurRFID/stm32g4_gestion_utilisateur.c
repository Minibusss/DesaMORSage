/**
 *******************************************************************************
 * @file 	stm32g4_gestion_utilisateur.c
 * @author 	Alice GEDOUX
 * @date 	Avr 27, 2026
 * @brief	Fichier principal pour gérer les utilisateurs avec le capteur RFID
 * et la saisie des MDP
 *******************************************************************************
 */

#include <stdint.h> //Fournit des types entiers à taille fixe (uint8_t, int16_t, uint32_t…)->Indispensable en embarqué pour garantir la taille exacte des variables, quel que soit le compilateur ou l’architecture.

#include "../ClavierMatriciel/stm32g4_clavier_matriciel.h"
#include "stm32g4_gestion_rfid.h"

typedef struct{
	//ID BADGE
	//NOM
	//PRENOM
	//MDP
	//ETAT BADGE
	//TYPE BADGE
}utilisateurBadge;

typedef enum{
	ADMINISTATEUR,
	UTILISATEUR,
}typeCompte;

typedef enum{
	INACTIF,
	ACTIF,
	SUSPENDU,
}etatBagde;

/**
  * @brief  La fonction permet de vérifier que le badge appartient bien à un utilisateur connu
  * @param  uint8_t * pointeurIDBadge : pointeur vers l'id du badge scanné
  * @retval Renvoie un entier : 1 (vrai : badge reconnu -> utilisateur existant) / 0 (faux : badge non reconnu -> utilisateur non existant)
 */
uint8_t verificationUtilisateurConnu(uint8_t * pointeurIDBadge){
	return 2;
}

/**
  * @brief  La fonction permet de vérifier le MDP saisi est bien rattaché au badge. L'utilisateur disposera de 3 essais avant refus (etatBadge = SUSPENDU) -> obligeant de réactiver le badge
  * @param  uint8_t * pointeurIDBadge : pointeur vers l'id du badge scanné
  * @param uint8_t * pointeurSaisie : pointeur vers le MDP saisi
  * @retval Renvoie un entier : 1 (vrai : MDP correct) / 0 (faux : MDP incorrect)
 */
uint8_t verificationUtilisateurMDPCorrect(uint8_t * pointeurIDBadge, uint8_t * pointeurSaisie){
	return 2;
}

/**
  * @brief  La fonction permet d'ajouter le badge à un nouvel utilisateur si celui-ci n'est pas déjà rattaché à un badge
  * @param  utilisateurBadge * pointeurUtilisateurBadge : pointeur vers la structure contenant toutes les infos du nouvel utilisateur
  * @retval Renvoie un entier : 1 (vrai : utilisateur crée) / 0 (faux : utilisateur non crée/ou déjà existant)
 */
uint8_t ajouterUtilisateur(utilisateurBadge * pointeurNouveauUtilisateur ){
	return 2;
}

/**
  * @brief  La fonction permet d'ajouter un badge à un nouvel utilisateur si et seulement si que c'est le bage admin qui autorise
  * @param  utilisateurBadge * pointeurAdmin : pointeur vers la structure contenant toutes les infos de l'admin
  * @param  utilisateurBadge * pointeurNouveauUtilisateurBadge : pointeur vers la structure contenant toutes les infos du nouvel utilisateur
  * @retval Renvoie un entier : 1 (vrai : utilisateur ajouté) / 0 (faux : utilisateur non ajouté)
 */
uint8_t ajouterUtilisateurDansBaseDeDonnees(utilisateurBadge * pointeurAdmin, utilisateurBadge * pointeurNouveauUtilisateurBadge){
	return 2;
}
//- ici il faudra récupéré le tag scanné
//- il faudra stocker le resultat dans un ficher où sont stocké les utilisateurs avec mdp

//- il faudra créer une fonction qui compare le mdp avec celui du tag scanné présent dans la base de donnée (accéssible par l'écran et le rfid seulement)



