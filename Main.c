switch(etatProcessus){
			case ETAT_INIT:
				//Affichage de la page d'accueil
				etatProcessus = ETAT_MENU;

				//
				break;

			case ETAT_MENU:
				//Affichage de la page 1
				etatProcessus = ETAT_SCAN_RFID;
				break;

			case ETAT_SCAN_RFID:
				//Affichage de la page 2
				/*
				 * Scan du badge RFID
				 * */
				etatProcessus = ETAT_LECTURE_MDP;
				break;

			case ETAT_LECTURE_MDP:
				//Affichage de la page 3
				BSP_MATRIX_KEYBOARD_process_main(pointeurSaisie,indiceSaisie);

				/*
				if(indiceSaisie<19 && etat_bouton_page3 == 0){
					//Poursuite de la lecture du mot de passe
					BSP_MATRIX_KEYBOARD_process_main(mot_de_passe);
					indice_actuel_mot_de_passe +=1;

					//Vérification de l'état du bouton entré
					etat_bouton_page3 = lectureBoutonValiderMDP();
				}
				else {

				}*/
				etatProcessus = ETAT_CONNECTION_B;
				break;

			case ETAT_CONNECTION_B:
				//Affichage de la page 4
				etatProcessus = ETAT_ENVOI_CHAINE;
				break;

			case ETAT_ENVOI_CHAINE:
				//Affichage de la page 4
				etatProcessus = ETAT_MENU;//A voir si on ajoute le fait qu'on peut retourner au menu quand on le souhaite
				break;

			default :
				break;
		}