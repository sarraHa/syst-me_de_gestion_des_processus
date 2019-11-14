
#include "processus.h"
#include "interruption.h"
#include "machine.h"
#include "disques.h"
#include "disques.c"


int main(){



	initialisation( );

	printf("disque->nb_plateaux %d\n",disque->nb_plateaux);	
	creation_processus( 0 );
	creation_processus( 0 );
	creation_processus( 1 );
	creation_processus( 2 );
	creation_processus( 3 );
	creation_processus( 1 );
	
	
	
	affiche_liste_processus( liste_processus , 1);
	affiche_disque();
	affiche_programmes( 6 );

	ordonnancement_FIFO();
	affiche_file_interruption();
	affiche_liste_processus( liste_processus , 1);

		
	
	return 0;
}
