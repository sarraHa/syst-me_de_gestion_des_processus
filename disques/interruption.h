

#define nb_type_interruption           6


enum type_interruption {

	FIN_PROCESSUS,          	//0
	FIN_APPEL_BLOQUNT,      	//1
	ATTENTE_FIN_PROCESSUS_FILS, //2
	ATTENTE_UNE_RESSOURCE,		//3
	
	FIN_OPERATION_DISQUE ,       //4
	FIN_OPERATION_ASYNCHRONE,    //5 
	
};



struct interruption{

	//ressource terminée
	int type_interruption ;    	// un entier de 0 à 
	int id_origine;
	int id_cible; // faire un traitement avec la cible en le reveillant par exemple

	int activer;	// 1 activer............0 desactiver
	
	struct interruption *next;

};


struct file_interruption{

	struct interruption *premier;
};

struct file_interruption *file_interruption;



/**************************************************************/

void ajoute_interruption(struct interruption *nv_interruption );
int gestion_interruption( int num_processus_courant );
void supprime_premiere_interruption();
int genere_interruption_2( int id_origine, int id_cible, int type_interruption );
int genere_interruption();



