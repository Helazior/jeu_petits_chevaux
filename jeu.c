#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

void affichage_intro_jeu()
{
	printf("___________________________________________________________________________");
	printf("\n|                                                                         |");
	printf("\n|                          Jeu des petits chevaux !                       |\n");
	printf("|_________________________________________________________________________|\n\n");
}

void affichage_jeu(Joueur* joueur, int nb_joueurs, int num_joueur)
{
	int j;
	int i;
	int nb_j;
	int nb_i;
	int a;
	int b;
	Bool emplacement_cheval;
	
	if (nb_joueurs >= 2) {
		if (num_joueur != 1) {
			printf("\n                                  Rouge\n");
		}
		else {
			printf("\n                                |-Rouge-|\n");
		}
	}
	for (j = 1; j <= 15; j++) {
		if (j == 8) {
			if (num_joueur != 0) {
				printf("\n            Jaune     ");
			}
			else{
				printf("\n          |-Jaune-|   ");
			}
		}
		else {
			printf("\n                      ");
		}
		nb_j = 7 - abs(8 - j);
		for (i = 1; i <= 15; i++) {
			nb_i = 7 - abs(8 - i); //numéros au centre du jeu
			emplacement_cheval = NON;
			for (a = 0; a < nb_joueurs; a++) {
				for (b = 0; b < 4; b++) {
					if (i == joueur[a].x[b] && j == joueur[a].y[b]) {
						emplacement_cheval = OUI;
						break;
					}
				}
				if (emplacement_cheval == OUI) {
					break;
				}
			}
			if (emplacement_cheval == OUI) {
				printf("%c%d", joueur[a].couleur, b + 1);
			}
			else if (i == 8 && nb_j && nb_j != 7) {
				printf("%d ", nb_j);
			}
			else if (j == 8 && nb_i && nb_i != 7) {
				printf("%d ", nb_i);
			}
			else if (i == 7 || i == 9 || j == 7 || j == 9 || (j == 8 && nb_i == 0) || (i == 8 && nb_j == 0)) {
				printf("* ");
			}
			else if (!nb_i || !nb_j) {
				printf(". ");
			}
			else if (i == 8 && j == 8) {
				printf("X ");
			}
			else {
				printf("  ");
			}
		}
		if (j == 8 && nb_joueurs >= 3) {
			if (num_joueur != 2) {
				printf("        Vert");
			}
			else {
				printf("      |-Vert-|");
			}
		}
	}
	if (nb_joueurs == 4) {
		if (num_joueur != 3) {
			printf("\n\n                                   Bleu");
		}
		else {
			printf("\n\n                                 |-Bleu-|");
		}
	}
}
//faire dernière case triangulaire

void flush(FILE* f) {
	int c;
	while ((c = fgetc(f)) != '\n' && c != EOF);
}
//vide le buffer pour éviter les bugs avec scanf

Bool est_dans(int x, int* tab, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (x == tab[i]) {
			return OUI;
		}
	}
	return NON;
}

int demande_nb(int* tab_val, int taille_tab)
{
	int nb_joueurs;
	Bool nb_correct;
	do {
		scanf_s("%d", &nb_joueurs);
		nb_correct = est_dans(nb_joueurs, tab_val, taille_tab);
		if (nb_correct && fgetc(stdin) != 10) {
			nb_correct = NON;
		}

		if (nb_correct == NON) {
			printf("\nNombre incorrect, recommencez: ");
			flush(stdin);
		}
	} while (nb_correct == NON);
	return nb_joueurs;
}

int coord_x(int p) {
	return 1 + (p == 2 || p == 54 || p == 57) + 2 * (p == 3 || p == 53 || p == 58) + 3 * (p == 4 || p == 52 || p == 59) + 4 * (p == 5 || p == 51 || p == 60) + 5 * (p == 6 || p == 50 || p == 61) + 6 * ((p >= 7 && p <= 49) || p == 62) + (p >= 14 && p <= 42) + (p >= 15 && p <= 41) + (p >= 22 && p <= 34) + (p >= 23 && p <= 33) + (p >= 24 && p <= 32) + (p >= 25 && p <= 31) + (p >= 26 && p <= 30) + (p >= 27 && p <= 29) + 7* (p == 63);
}
int coord_y(int p) {
	return 1 + (p == 12 || p == 16) + 2 * (p == 11 || p == 17) + 3 * (p == 10 || p == 18) + 4 * (p == 9 || p == 19) + 5 * (p == 8 || p == 20) + 6 * ((p >= 1 && p <= 7) || (p >= 21 && p <= 27)) + 7 * (p == 63 || p == 28 || (p >= 56 && p <= 62)) + 8 * (p >= 29 && p <= 55) + (p >= 36 && p <= 48) + (p >= 37 && p <= 47) + (p >= 38 && p <= 46) + (p >= 39 && p <= 45) + (p >= 40 && p <= 44) + (p >= 41 && p <= 43);
}
void coordonnees_cheval(Joueur* joueur, int num_joueur, int num_cheval)
{
	if (joueur->couleur == 'J') {
		joueur->x[num_cheval] = coord_x(joueur->pos[num_cheval]);
		joueur->y[num_cheval] = coord_y(joueur->pos[num_cheval]);
	}
	else if (joueur->couleur == 'R') {
		joueur->x[num_cheval] = 16 - coord_y(joueur->pos[num_cheval]);
		joueur->y[num_cheval] = coord_x(joueur->pos[num_cheval]);
	}
	else if (joueur->couleur == 'V') {
		joueur->x[num_cheval] = 16-coord_x(joueur->pos[num_cheval]);
		joueur->y[num_cheval] = 16-coord_y(joueur->pos[num_cheval]);
	}
	else {
		joueur->x[num_cheval] = coord_y(joueur->pos[num_cheval]);
		joueur->y[num_cheval] = 16 - coord_x(joueur->pos[num_cheval]);
	}
}
void initialiserJoueur(Joueur* joueur, int nb_joueurs, int num_joueur)
{
	joueur->couleur = "JRVB"[num_joueur];
	
	int tab[2] = { 0,1 };
	printf("Joueur %d PNJ ? {0 pour joueur, 1 pour PNJ}: ", num_joueur + 1);
	joueur->pnj = demande_nb(tab, 2); //NON == 0 et OUI == 1
	int j;

	for (j = 0; j < 4; j++) {
		joueur->pos[j] = 0;
		joueur->x[j] = 4 * (4 - abs(3 - 2 * num_joueur)) + j - 2;
		joueur->y[j] = 4 * (2 * (num_joueur >= 2) + 1);
	}
}


int chevaux_dispo(Joueur joueur, int* tab_temp_chvaux_dispo, int nb_alea_1_6)
{
	int nb;
	nb = 0;
	int i;
	int j;
	int pos;
	int pos2;
	Bool collision;
	for (i = 0; i < 4; i++) {
		pos = joueur.pos[i];
		collision = NON;
		for (j = 0; j < 4; j++) {
			pos2 = joueur.pos[j];
			if (i != j && (pos + nb_alea_1_6 <= 56 && (( pos + nb_alea_1_6 == pos2 && pos != 0) || (pos2 == 1 && pos == 0) && nb_alea_1_6 == 6) || (pos + nb_alea_1_6 > 56 && pos < 56 && 112 - nb_alea_1_6 - pos == pos2) || (pos >= 56 && nb_alea_1_6 == pos - 55 && pos + 1 == pos2))) {
				collision = OUI;
				printf("\nCheval %d bloque le cheval %d", j + 1, i + 1);
			}
		}
		if ((pos < 56 && (pos != 0 || nb_alea_1_6 == 6) || (pos >= 56 && nb_alea_1_6 == pos - 55) || (pos == 62 && nb_alea_1_6 == 6)) && collision == NON) {	//Si pos < 56 et (cheval dehors ou 6) et pas de collision
			tab_temp_chvaux_dispo[nb] = i + 1;
			nb++;
		}
	}
	return nb;
}

void initialiser_cheval(Joueur* joueur, int num_cheval, int num_joueur)
{
	joueur->pos[num_cheval] = 0;
	joueur->x[num_cheval] = 4 * (4 - abs(3 - 2 * num_joueur)) + num_cheval - 2;
	joueur->y[num_cheval] = 4 * (2 * (num_joueur >= 2) + 1);
}

void collision_adverse(Joueur* joueur, int num_joueur, int nb_joueurs, int x, int y)
{
	int i;
	int j;
	Bool meme_coord;
	meme_coord = NON;
	for (i = 0; i < nb_joueurs; i++) {
		if (i != num_joueur) {
			for (j = 0; j < 4; j++) {
				if (joueur[i].x[j] == x && joueur[i].y[j] == y) {
					meme_coord = OUI;
					break;
				}
			}
		}
		if (meme_coord == OUI) {
			break;
		}
	}
	if (meme_coord == OUI) {
		printf("%c%d s'est fait manger et revient donc a l'ecurie.\n", joueur[i].couleur, j + 1);
		initialiser_cheval(&joueur[i], j, i);
	}
}

int indice_max(Joueur joueur, int* tab, int taille, int pos_max, int nb_alea_1_6) {
	int i;
	int indice_cheval = 0;
	int m;
	if (taille == 0) {
		printf("tableau vide");
	}
	else {
		m = 0;
		for (i = 1; i < taille + 1; i++) {
			if (joueur.pos[tab[i] - 1] > m && joueur.pos[tab[i] - 1] + nb_alea_1_6 <= pos_max) {
				m = joueur.pos[tab[i] - 1];
				indice_cheval = tab[i] - 1;
			}
		}
	}
	return indice_cheval;
}

int peut_manger(Joueur* joueur, int num_joueur, int nb_joueurs, int nb_aleat_1_6, int* tab_chevaux_dispo, int nb_chevaux_dispo) {
	int i, j, k;
	//changer j en num_autre_joueurs
	//est_ce qu'on peut manger les autres chevaux ?:
	for (i = 1; i < nb_chevaux_dispo + 1; i++) { //chevaux dispo du joueur
		for (j = 0; j < nb_joueurs; j++) { //on parcourt les joueurs
			if (j != num_joueur) {
				for (k = 0; k < 4; k++) { //chevaux des joueurs
					if (joueur[j].pos[k]) {//si le cheval adverse est sorti
						int a = (j - num_joueur + 4) % 4;
						if (joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] == 0 && 1 == (joueur[j].pos[k] + 14 * a) % 56 || (joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] + nb_aleat_1_6 <= 56 && joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] >= 1 && joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] + nb_aleat_1_6 == (joueur[j].pos[k] + 14 * a) % 56) || (joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] + nb_aleat_1_6 > 56 && 112 - joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] - nb_aleat_1_6 == (joueur[j].pos[k] + 14 * a) % 56)) {
							return tab_chevaux_dispo[i];
						}
					}
				}
			}
		}
	}
	return 0;
}

int indice_min(Joueur joueur) {
	int i;
	int m;
	m = joueur.pos[0];
	for (i = 1; i < 4; i++) {
		if (joueur.pos[i] < m && joueur.pos[i] > 0) {
			m = joueur.pos[i];
		}
	}
	for (i = 0; i < 4; i++) {
		if (joueur.pos[i] == 0) {
			return m;
		}
	}
	return 0;
}

int pnj_choix(Joueur* joueur, int num_joueur, int nb_joueurs, int nb_aleat_1_6, int* tab_chevaux_dispo, int nb_chevaux_dispo) {
	int i;
	int nb_choisi = 0;
	int first_cheval = indice_max(joueur[num_joueur], tab_chevaux_dispo, nb_chevaux_dispo, 99, nb_aleat_1_6);
	int first_cheval2 = indice_max(joueur[num_joueur], tab_chevaux_dispo, nb_chevaux_dispo, 56, nb_aleat_1_6);
	int chev_collision = peut_manger(joueur, num_joueur, nb_joueurs, nb_aleat_1_6, tab_chevaux_dispo, nb_chevaux_dispo);
	int pos_last_chev = indice_min(joueur[num_joueur]);

	//priorité 1, si le premier cheval est dans l'escalier ou qu'il arrive pile en dessous on l'avance:
	if (joueur[num_joueur].pos[first_cheval] >= 56 || joueur[num_joueur].pos[first_cheval] + nb_aleat_1_6 == 56) {
		printf("(avancer escalier) ");
		nb_choisi = first_cheval + 1;
	}
	else if (chev_collision) { //2: s'il peut manger un autre cheval
		printf("(manger) ");
		nb_choisi = chev_collision;
	}
	else if (nb_aleat_1_6 == 6 && (pos_last_chev >= 18 || joueur[num_joueur].pos[first_cheval2] == 0 )) { //3: s'il a fait 6 et que tous ces chevaux sont bien avancés ou qu'aucun n'est sorti, on en sort un nouveau.
		printf("(sortir cheval) ");
		for (i = 1; i < nb_chevaux_dispo + 1; i++) {
			if (joueur[num_joueur].pos[tab_chevaux_dispo[i] - 1] == 0) {
				nb_choisi = tab_chevaux_dispo[i];
				break;
			}
		}
	}
	else if (joueur[num_joueur].pos[first_cheval2]) { // 4: avancer le premier qui n'atteind pas l'escalier avec le dé.
		printf("(avancer le premier) ");
		nb_choisi = first_cheval2 + 1;
	}
	else { // normalement inutile
		nb_choisi = (rand() % nb_chevaux_dispo + 1);
	}
	return nb_choisi;
}

void affiche_nom_couleur(int num_joueur) {
	if (num_joueur == 0)
		printf("Jaune");
	if (num_joueur == 1)
		printf("Rouge");
	if (num_joueur == 2)
		printf("Vert");
	if (num_joueur == 3)
		printf("Bleu");
}

void sauvegarder_partie(Joueur* joueur, int num_joueur, int nb_joueurs) {
	FILE* fichier = NULL;
	errno_t err;
	err = fopen_s(&fichier, "sauvergarde.txt", "w+");
	
	if (!err && fichier) {
		fprintf(fichier, "%d%d", num_joueur, nb_joueurs);
		int i, j;
		for (i = 0; i < nb_joueurs; i++) {
			for (j = 0; j < 4; j++) {
				//printf("joueur[i].pos[j] = %d, x = %d, y = %d\n", joueur[i].pos[j], joueur[i].x[j], joueur[i].y[j]);
				if (joueur[i].pos[j] < 10) {
					fprintf(fichier, "0");
				}
				fprintf(fichier, "%d", joueur[i].pos[j]);
				if (joueur[i].x[j] < 10) {
					fprintf(fichier, "0");
				}
				fprintf(fichier, "%d", joueur[i].x[j]);
				if (joueur[i].y[j] < 10) {
					fprintf(fichier, "0");
				}
				fprintf(fichier, "%d", joueur[i].y[j]);
			}
			//printf("pnj: %d\n", joueur[i].pnj);
			fprintf(fichier, "%d", joueur[i].pnj);
		}
		printf("Sauvegarde reussie !");
		fclose(fichier);
	}
}
void ouvrir_partie(Bool* p_nouvelle_partie, int* p_num_joueur, int* p_nb_joueurs) {
	FILE* fichier = NULL;
	errno_t err;
	err = fopen_s(&fichier, "sauvergarde.txt", "r");
	if (!err && fichier) {
		printf("Voulez-vous charger la partie sauvegardee ? {non : 0, oui : 1} : ");
		int tab[2] = { 0,1 };
		if (demande_nb(tab, 2) == 1) {
			printf("\nReprise de la sauvegarde.\n");
			*p_nouvelle_partie = NON;
			*p_num_joueur = (fgetc(fichier) - '0');//récupération de num_joueur
			*p_nb_joueurs = (fgetc(fichier) - '0');
		}
		else {
			printf("\nNouvelle partie.\n");
		}
		fclose(fichier);
	}
	else {
		printf("\nPas de sauvegarde; Nouvelle partie.\n");
	}
}
void ouvrir_partie2(Joueur* joueur, int nb_joueurs) {//structure
	FILE* fichier = NULL;
	errno_t err;
	err = fopen_s(&fichier, "sauvergarde.txt", "r");
	if (!err && fichier) {
		fgetc(fichier); fgetc(fichier);
		int i, j;
		//printf("%d", nb_joueurs);
		for (i = 0; i < nb_joueurs; i++) {
			joueur[i].couleur = "JRVB"[i];
			for (j = 0; j < 4; j++) {
				joueur[i].pos[j] = 10 * (fgetc(fichier) - '0') + (fgetc(fichier) - '0');
				joueur[i].x[j] = 10 * (fgetc(fichier) - '0') + (fgetc(fichier) - '0');
				joueur[i].y[j] = 10 * (fgetc(fichier) - '0') + (fgetc(fichier) - '0');
				//printf("joueur[%d].pos[%d] = %d, x = %d, y = %d\n", i, j, joueur[i].pos[j], joueur[i].x[j], joueur[i].y[j]);
			}
			joueur[i].pnj = (fgetc(fichier) - '0');
			//printf("pnj: %d\n", joueur[i].pnj);
		}
		err = fclose(fichier);
	}
}