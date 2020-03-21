/*reste à faire:
-sauvegarde (tester conditions extrèmes, comme fichier ouvert, effacer fichier, écraser etc...)
-pdf.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"


int main(int argc, char* argv[]) {
	srand(( unsigned int ) time(NULL));	//initialisation du temps pour l'utilisation de rand.
	int nb_alea_1_6;
	int nb_joueurs = 2;
	int num_joueur = 0;
	Bool faire_pause = NON;

	affichage_intro_jeu();
	Bool nouvelle_partie = OUI;
	ouvrir_partie(&nouvelle_partie, &num_joueur, &nb_joueurs);
	if (nouvelle_partie) {
		printf("\nNombres de joueurs ? (2-4): ");
		int tab_val[3] = { 2,3,4 };
		nb_joueurs = demande_nb(tab_val, 3); //demande le nb de joueurs
		faire_pause = NON;
	}

	
	/*------------------------------Structure joueur---------------------------------------*/
	int i;
	if (nb_joueurs < 2 || nb_joueurs > 4) {
		exit(1);//sinon le compilateur met un avertissement, mais pas nessessaire.
	}

	Joueur* joueur = (Joueur*)malloc(nb_joueurs * sizeof(Joueur));

	if (!joueur) { //si l'allocation a échoué
		printf("Erreur allocation joueur");
		exit(1);
	}

	if (nouvelle_partie) {
		for (i = 0; i < nb_joueurs; i++) {
			initialiserJoueur(&joueur[i], nb_joueurs, i);
		}
	}
	else {
		ouvrir_partie2(joueur, nb_joueurs);
	}

	/*--------------------------------Boucle principale-------------------------------------*/
	int pos;
	int tab_temp_chevaux_dispo[4] = { 0 }; //{0,0,0,0}
	int nb_chevaux_dispo;
	int num_cheval = 0;
	Bool continuer_jeu;
	continuer_jeu = OUI;
	while (continuer_jeu == OUI) {
		if (faire_pause == OUI && getchar() == '0') {
			continuer_jeu = NON;
		}
		else {
			printf("_________________________________JOUEUR ");
			affiche_nom_couleur(num_joueur);
			printf("__________________________________\n");
			
			faire_pause = OUI;

			nb_alea_1_6 = (rand() % 6) + 1; //nombre du dé aléatoire
			//scanf_s("%d", &nb_alea_1_6);

			printf("\nJoueur ");
			affiche_nom_couleur(num_joueur);
			printf(" a fait : %d", nb_alea_1_6);

			nb_chevaux_dispo = chevaux_dispo(joueur[num_joueur], tab_temp_chevaux_dispo, nb_alea_1_6);
			//mettre collision

			if (nb_chevaux_dispo) { //s'il peut jouer

				// tableau des chevaux hors de l'écurie
				int* tab_chevaux_dispo;
				tab_chevaux_dispo = (int*)calloc(nb_chevaux_dispo + 1, sizeof(int)); // +1 pour mettre le 0 qui correcpond à quitter.
				//tab_chevaux_dispo pas très utile mais montre que je sais faire un calloc

				if (!tab_chevaux_dispo) {
					printf("Erreur d'allocation de tab_chevaux_dispo.");
					exit(1);
				}

				for (i = 0; i < nb_chevaux_dispo; i++) {
					tab_chevaux_dispo[i + 1] = tab_temp_chevaux_dispo[i];
				}


				affichage_jeu(joueur, nb_joueurs, num_joueur);
				printf("\n(Rentrer 0 pour quitter).\nQuel cheval voulez-vous jouer ? {");
				for (i = 1; i < nb_chevaux_dispo; i++) {
					printf("%d,", tab_chevaux_dispo[i]);
				}
				printf("%d}: ", tab_chevaux_dispo[nb_chevaux_dispo]);
				if (joueur[num_joueur].pnj == OUI) {
					num_cheval = pnj_choix(joueur, num_joueur, nb_joueurs, nb_alea_1_6, tab_chevaux_dispo, nb_chevaux_dispo) - 1;
					printf("Le PNJ a choisi: %d\n", num_cheval + 1);
				}
				else {
					num_cheval = demande_nb(tab_chevaux_dispo, nb_chevaux_dispo + 1) - 1; //enlever les collisions alliées.
					faire_pause = NON;
				}
				pos = joueur[num_joueur].pos[num_cheval];
				if (nb_alea_1_6 == 6 && pos == 0) {
					joueur[num_joueur].pos[num_cheval] = 1;
				}
				else if (pos < 56 && pos + nb_alea_1_6 > 56) { //arriver devant l'escalier
					joueur[num_joueur].pos[num_cheval] = 112 - nb_alea_1_6 - pos;
				}
				else if (pos > 56) { //escalier
					joueur[num_joueur].pos[num_cheval] ++;
				}
				else {
					joueur[num_joueur].pos[num_cheval] += nb_alea_1_6;
				}

				coordonnees_cheval(&joueur[num_joueur], num_joueur, num_cheval);
				collision_adverse(joueur, num_joueur, nb_joueurs, joueur[num_joueur].x[num_cheval], joueur[num_joueur].y[num_cheval]);

				if (num_cheval == -1) {
					continuer_jeu = NON;
					num_cheval = 0;
				}
				if (num_cheval >= 4 || num_cheval < 0) {
					exit(1); //inutile en théorie mais enlève un warning
				}
				if (joueur[num_joueur].pos[num_cheval] == 63) {
					printf("\nJoueur ");
					affiche_nom_couleur(num_joueur);
					printf(" gagne !\n");
					continuer_jeu = NON;
				}

				free(tab_chevaux_dispo);
			}
			else {
				printf("\nVous ne pouvez pas jouer.\n");
				printf("(Tous les chevaux sont bloques).\n");
			}


			if (nb_alea_1_6 != 6) {
				num_joueur = (num_joueur + 1) % nb_joueurs;
				printf("_______________________________________________________________________________");
				if (faire_pause == NON) {
					printf("\n");
				}
			}
			else if (continuer_jeu == OUI) {
				printf("Un 6 permet de rejouer.\n");
			}
		}
	}


	affichage_jeu(joueur, nb_joueurs, num_joueur);
	printf("%d", num_joueur);
	printf("%d", num_cheval);
	if (joueur[num_joueur].pos[num_cheval] != 63) {
		printf("\nSauvegarder la partie ? {non : 0, oui : 1} : ");
		int tab_val[2] = { 0,1 };
		if (demande_nb(tab_val, 2) == 1) {
			sauvegarder_partie(joueur, num_joueur, nb_joueurs);
			faire_pause = NON;
		}
		else {
			printf("Partie non sauvegarde.");
		}
	}
	printf("\nRentrer quelque chose pour quitter le programme. ");
	scanf_s("%d", &i);
	free(joueur);

	return 0;
}
