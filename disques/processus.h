#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define max_nb_proc_fils         10
#define max_nb_instructions      6
#define max_processus            10


struct instruction{

	int id_processus_cible;           // on aura besoin de ça si c'est bloquant_processus	
	int bloquant_processus;        // si c'est instruction bloquante qui attend la fin d'un processus
	int bloquant_ressource;			// attente d'une ressource
	
	int valide;        // 0 n'est pas valide 1 valide
	int bloquante; 		// 0 n'est pas bloquante   1 bloquante
	
	// partie disque
	int lecture; 		// 0..ce n'est pas une operation de lecture  1...asynchrone 2..operation lecture synchrone  
	int ecriture;	   //  0 ce n'est pas une operation d'ecriture 1...asynchrone 2... operation ecriture synchrone  
	
	char memoire[16]; // contient la zone mémoire qui est lu
};

struct processus{
	
	struct instruction *programme;
	int num_processus;
	int num_processus_pere;
	int tab_numero_fils[ max_nb_proc_fils ];
	int pt_pile;         // la première instruction à exécuter ????
	int etat;            // 0 en execution....1 en attente.... 2 pret à s'executer ......  3 zombie	
	
	
	// c'est moi qui l'ai a ajouté
	int nb_fils;
	int nb_instruction;
	
	struct processus *next;
	// enregistrer le contexe 
	int registre;
};


struct liste_processus{

	struct processus *premier;
};



int tab_processus[max_processus] ;
struct liste_processus *liste_processus;			 

/******************************************************************************/

int initialisation();
int creation_processus( int num_pere );
void affiche_liste_processus( struct liste_processus *listeProc_pret_execution, int type_liste );
int affecte_fils( int num_pere, int num_fils);
int ajoute_en_fin( struct liste_processus *listeProc , struct processus *proc_a_ajouter );
int supprimer_proc( struct liste_processus *listeProc , int num_processus );
int supprimer_proc_adopte_fils( struct liste_processus *listeProc , int num_processus );
void affiche_file_interruption( );

int affiche_programme( int num_processus );
int gestion_instruction( struct liste_processus *listeProc, struct processus *processus_en_cours );
int gestion_instruction_2( struct liste_processus *listeProc, int num_processus );
int mettre_a_jour_registre( int num_processus , int etat_registre);
void affiche_programmes( int nb_processus);

