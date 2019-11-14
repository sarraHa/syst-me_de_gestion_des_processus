
#define max_nb_plateaux 	   		10
#define max_nb_secteur		   		 5
#define max_taille_secteur     		10
#define max_tmp_passage_secteur		 2
#define max_tmp_passage_plateau		 2
#define max_tmp_transfert			 5


struct disque{

	int type_disque; // type caractère... 0 de type bloc... 1
	int etat ; // en action ou pas    0 en action ( pas disponible ) 1 disponible
	int nb_plateaux;
	int nb_secteurs_par_plateaux;
	int taille_secteur;
	//le temps pour passer d’un secteur à un autre(dans un plateau)
	int tmp_passage_secteur;
	//le temps pour passer d’un plateau à un autre (on bouge d’un niveau, mais on reste sur le secteur parallèle),
	int tmp_passage_plateau;
	//le temps de transfert.
	int tmp_transfert;
	int type_operations; // 0 lire 1 ecrire
	

};


struct disque *disque;

void initialisation_disque();
void affiche_disque();
void changer_etat_disque( int etat );
int ret_etat_disque();
