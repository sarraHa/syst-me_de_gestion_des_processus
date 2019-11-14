
enum type_interruption {

	FIN_PROCESSUS,          	//0
	FIN_APPEL_BLOQUNT,      	//1
	ATTENTE_FIN_PROCESSUS, //2
	ATTENTE_UNE_RESSOURCE,		//3


};



struct interruption{

	int type_interruption ;    
	int id_origine;
	int id_cible; // faire un traitement avec la cible en le reveillant par un exemple
	struct interruption *next;
};


struct file_interruption{

	struct interruption *premier;
};

struct file_interruption *file_interruption;



/**************************************************************/

int gestion_interruption( int num_processus_courant );
void supprime_premiere_interruption();
int genere_interruption_2( int id_origine, int id_cible, int type_interruption );
int genere_interruption();
void affiche_file_interruption( );



