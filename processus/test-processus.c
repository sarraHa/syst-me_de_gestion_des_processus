#include "processus.h"
#include "interruption.h"
#include "machine.h"
#include "processus.c"


int main(){

	initialisation( );
	creation_processus_aleatoire( 6 );
	affiche_liste_processus( liste_processus);
	affiche_programmes(6);	
	
	ordonnancement_moins_CPU();
	affiche_file_interruption();
	affiche_liste_processus( liste_processus);




	return 0;
}
