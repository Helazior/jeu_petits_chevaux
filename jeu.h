#ifndef _JEU_H
#define _JEU_H

typedef enum Bool Bool;
enum Bool
{
	NON,
	OUI
};

typedef struct Joueur Joueur;
struct Joueur {
	char couleur;
	int pos[4];	//case de 0(écrurie) à 63
	int x[4];	//coordonnés
	int y[4];
	Bool pnj;
};

void affichage_intro_jeu();

void flush(FILE* f);

Bool est_dans(int x, int* tab, int n);

int demande_nb(int* tab_val, int taille_tab);

void initialiserJoueur(Joueur* joueur, int nb_joueurs, int equipe);

void affichage_jeu(Joueur* joueur, int nb_joueurs, int num_joueur);

int coord_x(int p);
int coord_y(int p);
void coordonnees_cheval(Joueur* joueur, int num_joueur, int num_cheval);

int chevaux_dispo(Joueur joueur, int* tab_temp_chvaux_dispo, int nb_alea_1_6);

void initialiser_cheval(Joueur* joueur, int num_cheval, int num_joueur);

void collision_adverse(Joueur* joueur, int num_joueur, int nb_joueurs, int x, int y);

//---------------IA-------------
int indice_max(Joueur joueur, int* tab, int taille, int pos_max, int nb_alea_1_6);

int peut_manger(Joueur* joueur, int num_joueur, int nb_joueurs, int nb_aleat_1_6, int* tab_chevaux_dispo, int nb_chevaux_dispo);

int indice_min(Joueur joueur);

int pnj_choix(Joueur* joueur, int num_joueur, int nb_joueurs, int nb_aleat_1_6, int* tab_chevaux_dispo, int nb_chevaux_dispo);
//------------------------------
void affiche_nom_couleur(int num_joueur);

void sauvegarder_partie(Joueur* joueur, int num_joueur, int nb_joueurs);

void ouvrir_partie(Bool* p_nouvelle_partie, int* p_num_joueur, int* p_nb_joueurs);

void ouvrir_partie2(Joueur* joueur, int nb_joueurs);
#endif