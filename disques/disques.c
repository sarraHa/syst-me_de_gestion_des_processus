

int initialisation( ){


	liste_processus = malloc( sizeof( struct liste_processus));
	file_interruption		 = malloc( sizeof( struct file_interruption));	

	// j'initialise init	
	struct processus *premier_processus = malloc( sizeof( struct processus ));

	premier_processus->num_processus	= 	0;
	premier_processus->num_processus_pere	= 	-1;
	premier_processus->pt_pile	= 	0;
	premier_processus->etat	= 	-2; // en attente
	
	int i;	
	int nb_instruction = rand()%max_nb_instructions;
	
	premier_processus->nb_instruction =  nb_instruction;
	premier_processus->programme	= 	malloc( nb_instruction*sizeof( struct instruction) );
	
	
	for( i = 0 ; i < max_nb_proc_fils ; i++){
	
		premier_processus->tab_numero_fils[i] = -1;
	}
	
	premier_processus->nb_fils = 0;
	
	
	liste_processus->premier = NULL;
	premier_processus->next = liste_processus->premier;
	liste_processus->premier = premier_processus;
	
	/// initialisation d'autres processus
	
	tab_processus[0] = 0; // init
	for( i = 1; i < max_processus ; i++){
	
		tab_processus[i] = -1;
	}

	initialisation_machine();
	initialisation_disque();
	return 1;
}


int genere_interruption_2( int id_origine, int id_cible, int type_interruption ){

	struct interruption *nv_interruption = malloc( sizeof( struct interruption  ));
	nv_interruption->id_origine          = id_origine;
	nv_interruption->id_cible            = id_cible;
	nv_interruption->type_interruption   = type_interruption;
	
	// je rajoute à la fin

	struct interruption *parcourInterruption = file_interruption->premier;
	
	if( parcourInterruption == NULL ){
	
		nv_interruption->next  = NULL;
		file_interruption->premier = nv_interruption;

		return 1;
	}
    while( parcourInterruption->next != NULL ){
    
        parcourInterruption = parcourInterruption->next;
    }
	// 	nv_interruption->type_interruption     =  rand()%4;			    
	nv_interruption->next = NULL;
	parcourInterruption->next = nv_interruption;
	return 1;	
	
}


// cette fonction genere une interruption au hasard
int genere_interruption(){

	// je genere une interruption au hasard	
	struct interruption *nv_interruption = malloc( sizeof( struct interruption  ));
	nv_interruption->type_interruption     =  rand()%nb_type_interruption;
	
	// trouver un id_origine je tire un processus au hasard je verifie bien qu'il existe 
	int i = -1;
	int id_origine_a_trouver;
	
	while( i == -1 ){
	
		id_origine_a_trouver = rand()%max_processus;
		i = tab_processus[ id_origine_a_trouver ];
	}
	//quand on est la cela veut dire qu'on a trouvé un processus qui exite
	nv_interruption->id_origine = id_origine_a_trouver;
	// je trouve une cible
	i = -1;
	int id_cible_a_trouver;
	
	while( ( i == -1 ) || ( id_cible_a_trouver == id_origine_a_trouver) ){
	
		id_cible_a_trouver = rand()%max_processus;
		i = tab_processus[ id_cible_a_trouver ];
	}
	nv_interruption->id_cible = id_cible_a_trouver;

	// si le type d'interruption c'est 2 	
	// alors la cible faut-il qu'elle soit son fils ???
	// j'ajoute à la fin
	
	ajoute_interruption( nv_interruption );
	

}


// j'ajoute l'interruption à la fin

void ajoute_interruption(struct interruption *nv_interruption ){

	struct interruption *parcourInterruption = file_interruption->premier;
	
	if( parcourInterruption == NULL ){
	
		nv_interruption->next  = NULL;
		file_interruption->premier = nv_interruption;

		return ;	
	}
    while( parcourInterruption->next != NULL ){
    
        parcourInterruption = parcourInterruption->next;
    }
	// 	nv_interruption->type_interruption     =  rand()%4;
				    
	nv_interruption->next = NULL;
	parcourInterruption->next = nv_interruption;

}

void supprime_premiere_interruption(){

	struct interruption *tmp = file_interruption->premier;
	file_interruption->premier = file_interruption->premier->next;
	free( tmp );

}

// je peux faire une autre fonction
// qui prend un ptr proc
int gestion_interruption( int num_processus_courant ){
	


	struct interruption *inter_a_traiter = file_interruption->premier;
	
	if(inter_a_traiter == NULL ){
	
		return 1;
	}
		printf("\n*******************gestion des interruptions*******************\n");
	
	if( inter_a_traiter == NULL ){
		// je regarde s'il n'y a pas d'interruption
		printf("pas d'interruption disponible\n");
		return 0;
	}
	
	// interruption concerne le disque
	if( inter_a_traiter->type_interruption == 4){
		
		//FIN_OPERATION_DISQUE
		printf("interruption FIN_OPERATION_DISQUE\n ");
		// je change l'etat de disque il est mnt disponible
		changer_etat_disque( 1 );
		supprime_premiere_interruption();	
		return 1;
	}
	
		//FIN_OPERATION_ASYNCHRONE,    5
	if( inter_a_traiter->type_interruption == 5){
		
		//FIN_OPERATION_ASYNCHRONE
		printf("interruption FIN_OPERATION_ASYNCHRONE\n ");
		// je change l'etat du disque il est maintenant disponible
		changer_etat_disque( 1 );
		// on doit informer le processus origine que l'operation a fini
		// je mets le processus à pret à s'executer 
		supprime_premiere_interruption();	
		return 1;
	}
	
	
	// je ne sais pas si je prends directement le premier de la file ou je le donne à la fonction
	// je regarde si cette interruption concerne le processus courant

	if( inter_a_traiter->id_origine == num_processus_courant ){
		
		printf("Cette interruption concerne le processus courant\n");
		// oui c'est le processus courant
		// je regarde de quel type d'interruption s'agit-il
		
		//FIN_PROCESSUS
		if( inter_a_traiter->type_interruption == 0){
		
			// je supprime ce processus
			
			// mais comment savoir dans quelle liste il est ? 
			// en attente ,,, pret ... ?
			// c'est dans le tableau qu'il y a le dernier etat
			printf("Interruption de type : fin de processus\n");
			supprimer_proc_adopte_fils(  liste_processus , inter_a_traiter->id_origine  );		
			supprime_premiere_interruption();
	
			return 1;
			
		}//FIN_PROCESSUS
		else if( inter_a_traiter->type_interruption == 1 ){    
		
		/// cette partie est à enlever
		// *************************FIN_APPEL_BLOQUNT *******************************
		
			// ce processus a fini son appel bloquant
			// il peut retourner dans la liste des pret a s'executer
			// ici je sais que mon processus est dans la liste des processus en attente
			
			// il faut qu'il passe à l'instruction suivante
			printf("Interruption de type : fin de l'appel bloquant ( reveil du processus num : %d)\n", inter_a_traiter->id_origine );
			
			rendre_pret_a_executer( inter_a_traiter->id_origine );
			// il n'a pas trouvé le processus	
			
			supprime_premiere_interruption();
			printf("erreur : il n'a pas trouvé le processus	\n");
			return -1;	
		}//FIN_APPEL_BLOQUNT
		else if( inter_a_traiter->type_interruption == 2 ){ 
		
			//*************** ATTENTE_FIN_PROCESSUS_FILS***************
			
			printf("Interruption de type : attente fin processus\n");
			
			// si id_cible == 0 ou -1  alors je considere que ce n'est pas une instruction valide
			if( inter_a_traiter->id_origine == inter_a_traiter->id_cible || inter_a_traiter->id_cible == -1 || inter_a_traiter->id_cible == 0){
			
				genere_interruption_2( inter_a_traiter->id_origine, -2 , 0 );	
				supprime_premiere_interruption();
				return 2;			
			
			}

			// on reveille le processus cible (fils) et on le place dans la liste des processus pret a s'executer	
			

			genere_interruption_2( inter_a_traiter->id_cible, -2 , 1 );
			// on change l'etat du processus d'origine à pret à s'excuter et on incremente le pt_pile 
			rendre_pret_a_executer( inter_a_traiter->id_origine );
			supprime_premiere_interruption();	
			return 1;
	
		}//ATTENTE_FIN_PROCESSUS_FILS
		else if( inter_a_traiter->type_interruption == 3){ 
		
			printf("Interruption de type : ATTENTE D'UNE RESSOURCE\n");
			
			int registre = rand()%2;
			if( registre == 1 ){
				
				// je le rends pret à s'executer
				// car sa ressource est prete
				
				rendre_pret_a_executer( inter_a_traiter->id_origine );
				supprime_premiere_interruption();
				
			}
			// je le laisse attendre pour que sa ressource soit prete
			// donc je genere une interruption de type attente ressource 
			
			genere_interruption_2( inter_a_traiter->id_origine, -2 , 3 );
			supprime_premiere_interruption();
			return 2;
			

		}// ATTENTE_UNE_RESSOURCE
	}else{
	
			printf("cette interruption ne concerne pas le processus courant\n");

			if( inter_a_traiter->type_interruption == 2 ){ 
				
				//*************** ATTENTE_FIN_PROCESSUS_FILS***************
				
				printf("Interruption de type : attente fin de processus\n");
				// si id_cible == 0 ou -1  alors je considere que ce n'est pas une instruction valide
				if( inter_a_traiter->id_cible <= 0 ){
				
					genere_interruption_2( inter_a_traiter->id_origine, -2 , 1 );	
					supprime_premiere_interruption();
					return 2;			
				
				}
				// j'ai pas regardé si c'est son fils ou pas 
				// on reville le processus cible et on le place dans la liste des processus pret à s'executer				


				genere_interruption_2( inter_a_traiter->id_cible, -2 , 1 );
				// on change l'etat du processus origine à pret à s'excuter et on incremente le pt_pile 
				rendre_pret_a_executer( inter_a_traiter->id_origine );
				supprime_premiere_interruption();	
				return 1;
				
			}//ATTENTE_FIN_PROCESSUS_FILS
			else if( inter_a_traiter->type_interruption == 3){ 
			
				//***************ATTENTE_UNE_RESSOURCE******************
					
				
			printf("Interruption de type : ATTENTE D'UNE RESSOURCE\n");
			
			int registre = rand()%2;
			if( registre == 1 ){
				
				// je le rends pret à s'executer
				// car sa ressource est prete
				
				mettre_a_jour_registre( inter_a_traiter->id_origine , 1 );
				rendre_pret_a_executer( inter_a_traiter->id_origine );
				supprime_premiere_interruption();
				
			}
			// je le laisse attendre pour que sa ressource soit prete
			// donc je genere une interruption de type attente ressource 
			
			mettre_a_jour_registre( inter_a_traiter->id_origine , 0);
			genere_interruption_2( inter_a_traiter->id_origine, -2 , 3 );
			supprime_premiere_interruption();
			return 2;
				/*
				on place le processus concerné dans la liste des prêts à                                                			s’exécuter et on s'informe sur le fait que l’instruction courante n’est plus bloquante.
				*/
				// creer une interruption pour reveiller id_cible
				// registre ressource pret
				// je déclenche une interruption de type  FIN_APPEL_BLOQUNT,      	//1
				
				// je mets le registre à jour 

			}// ATTENTE_UNE_RESSOURCE
		
			else if( inter_a_traiter->type_interruption == 1){
			
				printf("Interruption de type : FIN APPEL BLOQUANT\n");
			// interruption : fin de l'appel bloquant
			
				rendre_pret_a_executer( inter_a_traiter->id_origine );
				supprime_premiere_interruption();	
				return 1;
			
			}else if( inter_a_traiter->type_interruption == 0 ){
			
				printf("Interruption de type : FIN PROCESSUS\n");
			
				// je supprime ce processus
				
				// mais comment savoir dans quelle liste il est ? 
				// en attente ,,, pret ... ?
				// c'est dans le tableau qu'il y a le dernier etat

				supprimer_proc_adopte_fils(  liste_processus , inter_a_traiter->id_origine  );		
				supprime_premiere_interruption();
		
				return 1;
			
			}
	
	}
	
	printf("l'interruption n'est pas encore executée %d\n", inter_a_traiter->type_interruption);
	return -1;

}

void affiche_liste_processus( struct liste_processus *listeProc_pret_execution, int type_liste ){

    struct processus *parcourProcessus = NULL;
    parcourProcessus = listeProc_pret_execution->premier;
    
    if( type_liste == 1){
		
		printf("\n\t\t***** Table des processus *****\n\n");    
    }else if(type_liste == 2){

		printf("\n\t\t\t*****Table des processus en attente*****\n\n");    
    }else if( type_liste == 3){

		printf("\n\t\t\t*****Table des processus zombie*****\n\n");    
    
    }
    
	printf("\t num_processus \t num_processus_pere \tpt_pile \tetat\n");	
	    
    while( parcourProcessus != NULL ){
    
	printf("\t\t %d \t\t %d \t\t %d \t\t %d\n", parcourProcessus->num_processus, parcourProcessus->num_processus_pere, parcourProcessus->pt_pile, parcourProcessus->etat );  
	   
        parcourProcessus = parcourProcessus->next;
    }
}


void affiche_file_interruption( ){

	struct interruption *parcourInterruption = file_interruption->premier;
	printf("\n\t\t***** Liste d'interruption ******\n");
	printf("\t type_interruption \t id_origine \t id_cible\n");	
	
    while( parcourInterruption != NULL ){
    
	printf("\t\t %d \t\t %d \t\t %d \t\t\n", parcourInterruption->type_interruption, parcourInterruption->id_origine, parcourInterruption->id_cible );  
	   
        parcourInterruption = parcourInterruption->next;
    }
}

/*********************************************************************************************************/


int creation_processus( int num_pere ){		

		struct processus *nv_processus = malloc( sizeof( struct processus ));
		int i;

		// je crée un nombre d'instruction au hasard
		// je crée son programme
		
		int nb_instruction = rand()%max_nb_instructions;
		nv_processus->nb_instruction = nb_instruction;
		nv_processus->programme = malloc( nb_instruction*sizeof( struct instruction) );

		int valide, bloquante ;
	
		// je crée une table d'instruction au hasard
		for( i = 0; i < nb_instruction; i++){
				
			nv_processus->programme[i].valide = rand()%2;
 			nv_processus->programme[i].bloquante = rand()%2;

 			nv_processus->programme[i].bloquant_processus = rand()%2;
  			nv_processus->programme[i].bloquant_ressource = rand()%2;
  			
  			// ça peut poser un probleme s'il n'y a aucun proc dans la table
	  		int indice = -1;
			// s'il a une instruction bloquant_processus
			// s'il n'est pas bloquant_processus et il est bloquant_ressource
			// je dois trouver un processus cible
			
			if( nv_processus->programme[i].bloquant_processus == 1 ){
			
				indice = rand()%max_processus;
				nv_processus->programme[i].id_processus_cible = indice ;
			}
			// s'il n'est pas bloquant_processus et il est bloquant_ressource
			// je dois trouver un processus cible
					
			if( nv_processus->programme[i].bloquant_processus != 1 ){
				
				if( nv_processus->programme[i].bloquant_ressource == 1 ){
					indice = rand()%max_processus;
					nv_processus->programme[i].id_processus_cible = indice ;
				}
			}
			
			/// partie ajouter pour le disque
			nv_processus->programme[i].lecture = rand()%3;
			if( nv_processus->programme[i].lecture == 0 ){
			
				nv_processus->programme[i].ecriture = rand()%3;	
			
			}else{
			
				nv_processus->programme[i].ecriture = 0 ;
			}						
		}
		
		// je donne un num a mon processus
		int numero_indice_dispo ;
		
		for( i = 0; i < max_processus; i++){
			if( tab_processus[i]  == -1  ){     
				tab_processus[i]    = 1;      // on le prend
				numero_indice_dispo = i;
				break;
			}
		}

		// on regrde si on a pas trouvé
		
		if( i == max_processus ){
		
			printf("Impossible de creer un nouveau processus\n");
			return -1;
		}		
		
		nv_processus->num_processus = numero_indice_dispo;
	
		// je donne un pere a mon processus
		nv_processus->num_processus_pere = num_pere;

		// je donne au pere, le fils que je suis en train de creer
		
		affecte_fils( num_pere, numero_indice_dispo );
		// je donne la première instruction à exécuter 
		nv_processus->pt_pile = 0;
		
		// je donne un etat
		//  le processus dans la creation il est dans quel etat ???
		nv_processus->etat = 2;   // il est pret a s'executer

		nv_processus->registre = -1;		
		// mon processus a 0 fils au depart

		nv_processus->nb_fils = 0;
		tab_processus[numero_indice_dispo] = nv_processus->etat;
		
		ajoute_en_fin( liste_processus , nv_processus );
		free(nv_processus);

		return 1;
		
}


int affecte_fils( int num_pere, int num_fils){

	//printf("num_pere %d num_fils %d\n",num_pere, num_fils);
 	struct processus *parcourProcessus = NULL;
    parcourProcessus = liste_processus->premier;
    
    while( parcourProcessus != NULL ){
    	
    	if( parcourProcessus->num_processus == num_pere ){
    	
    		int indice  = 	parcourProcessus->nb_fils;
    		parcourProcessus->tab_numero_fils[ indice ]= num_fils;
			parcourProcessus->nb_fils++;
			
			return 1;
    	
    	}
    
        parcourProcessus = parcourProcessus->next;
    }

	printf("ce processus pere n'existe pas\n");
	return -1;	
}

/*************************************************************/

int affiche_programme( int num_processus ){

	struct processus *parcourProcessus = liste_processus->premier;
	int i;
		
	while( parcourProcessus != NULL ){
    	
    	if( parcourProcessus->num_processus == num_processus ){
    		
    		int nb_instr = parcourProcessus->nb_instruction;
    		printf("\nAffichage des Instructions du programme du processus num : %d\n\n", parcourProcessus->num_processus);
	
    		for( i = 0; i < nb_instr; i++){
    			
				printf("V/B/BR/BP/id_P_C/lec/ecr\t");
    		}
    		printf("\n");
    		
    		for( i = 0; i < nb_instr; i++){
    			
    			printf("%d %d %d  %d    %d    %d   %d\t\t", parcourProcessus->programme[i].valide, parcourProcessus->programme[i].bloquante, parcourProcessus->programme[i].bloquant_ressource, parcourProcessus->programme[i].bloquant_processus, parcourProcessus->programme[i].id_processus_cible, parcourProcessus->programme[i].lecture, parcourProcessus->programme[i].ecriture   );
    		}
    		
    		printf("\n");
    		return 1;
    	}
    
        parcourProcessus = parcourProcessus->next;
     
    }
	printf("ce processus n'existe pas dans cette liste\n");
	return -1;
}

// je retourne 2 si la fonction a terminé par une instruction en attente
// 1 pour dire que la fonction a executé une instruction valide pas bloquante


/***************************/
int gestion_instruction( struct liste_processus *listeProc, struct processus *processus_en_cours ){

	printf("\n*******************Gestion Des Instructions*******************\n");
	int i;
	int indice_instruction = processus_en_cours->pt_pile;
	//printf("indice_instruction %d\n", indice_instruction);
	// je genere une interruption de fin_processus
	
	if( indice_instruction >= processus_en_cours->nb_instruction ){
	
		processus_en_cours->etat = 1;
		genere_interruption_2( processus_en_cours->num_processus, -2 , 0 );
		// je retourne 2 pour dire que cette instruction est bloquante 
		return 2;
	}
	
	printf("gestion de l'instruction num : %d, ", processus_en_cours->pt_pile );
	
	// je regarce si l'instruction est valide 
	int valide = processus_en_cours->programme[ indice_instruction ].valide;
	if( valide == 0){
	

		// cette instruction n'est pas valide alors je dois declencher une interruption fin_processus
		// 0 pour dire c'est l'interruption fin_processus
		
		processus_en_cours->etat = 1;
		genere_interruption_2( processus_en_cours->num_processus, -2 , 0 );
		return 2;
		


	}else if( valide == 1 ){
	

		// je regarde si l'instruction est une instruction de lecture  
		
		if( processus_en_cours->programme[ indice_instruction ].lecture == 1 ){
				
				// c'est une opération de lecture asynchrone
				printf("instruction de lecture asynchrone\n");
				// c'est une instruction de lecture
				// je dois voir l'etat du disque
				int etat_disque = ret_etat_disque();
				if( etat_disque == 1 ){
					printf("le disque est disponible\n");
					
					// le disque est disponible
					// je dois mnt changer l'etat du disque à non disponible
					changer_etat_disque( 0 );
					processus_en_cours->pt_pile++;	
					changer_etat_disque( 1);
					// je retourne 1 pour dire que cette instruction n'est pas bloquante
					return 1;
					
				}else{
				
					printf("le disque n'est pas disponible\n");
					// le disque n'est pas disponible
					// le processus reste pret a s'executer
					// je le mets à la fin de la liste
					
					processus_en_cours->etat = 2;
					return 2;
				}
				
				
				
		}
		if( processus_en_cours->programme[ indice_instruction ].lecture == 2 ){
		
		// c'est une opération de lecture asynchrone
		printf("instruction de lecture synchrone\n");
		// c'est une instruction de lecture
		// je dois voir l'etat du disque
		int etat_disque = ret_etat_disque();
		if( etat_disque == 1 ){
			printf("le disque est disponible\n");
			
			// le disque est disponible
			// je dois mnt changer l'etat du disque à non disponible
			changer_etat_disque( 0 );
			int rn = 15;
			
			while( rn < 50){
			
				printf("kjdcksldfnsldk\n");
				rn = rand();
			
			}
			
			processus_en_cours->pt_pile++;	
			
			changer_etat_disque( 1);
			processus_en_cours->etat = 2;
			// je retourne 1 pour dire que cette instruction n'est pas bloquante
			return 1;
			
		}else{
		
			printf("le disque n'est pas disponible\n");
			// le disque n'est pas disponible
			// le processus reste pret à s'executer
			// je le mets à la fin de la liste
			
			processus_en_cours->etat = 2;
			return 2;
		}
		
		
		
}
		
		
		if( processus_en_cours->programme[ indice_instruction ].ecriture == 1 ){

			// c'est une instruction d'ecriture
			// je dois voir l'etat du disque
			
			printf("instruction d'ecriture asynchrone\n");
			int etat_disque = ret_etat_disque();
			
			if( etat_disque == 1 ){
				printf("le disque est disponible\n");
				// le disque est disponible
				// je dois mnt changer l'etat du disque à non disponible
				changer_etat_disque( 0 );
				processus_en_cours->pt_pile++;	
				changer_etat_disque( 1 );
				// je retourne 1 pour dire que cette instruction n'est pas bloquante
				return 1;
			}else{
			
				printf("le disque n'est pas disponible\n");
				// le disque n'est pas disponible
				// je mets ce processus en attente
				
				processus_en_cours->etat = 2;
				return 2;
			}
	
		}
		
			
		
		if( processus_en_cours->programme[ indice_instruction ].ecriture == 2 ){

			// c'est une instruction d'ecriture
			// je dois voir l'etat du disque
			
			printf("instruction d'ecriture synchrone\n");
			int etat_disque = ret_etat_disque();
			
			if( etat_disque == 1 ){
				printf("le disque est disponible\n");
				// le disque est disponible
				// je dois mnt changer l'etat du disque à non disponible
				changer_etat_disque( 0 );
				int rn = 15;
				
				while( rn < 50 ){
				
					// on simule le bloquage
					// parce que c'est une operation synchrone
					rn = rand();
				}
				
				
				processus_en_cours->pt_pile++;	
				changer_etat_disque( 1 );
				// je retourne 1 pour dire que cette instruction n'est pas bloquante
				return 1;
			}else{
			
				printf("le disque n'est pas disponible\n");
				// le disque n'est pas disponible
				// je mets ce processus en attente
				
				processus_en_cours->etat = 1;
				return 2;
			}
	
		}
		// cette instruction est valide 
		// je regarde maintenant si elle est bloquant_processus ou bloquant_ressource

		int bloquant_processus = processus_en_cours->programme[ indice_instruction ].bloquant_processus ;
		int bloquant_ressource = processus_en_cours->programme[ indice_instruction ].bloquant_ressource ;

		if(  bloquant_processus == 0 && bloquant_ressource == 0){

			// alors cette instruction est valide et pas bloquante
			// on affiche que cette instruction est valide et pas bloquante
			
			printf("instruction est valide \n");
			processus_en_cours->pt_pile++;
			// je retourne 1 pour dire que cette instruction n'est pas bloquante
			return 1;
			
		}
		else if ( bloquant_processus == 1 ){

			printf("instruction bloquante processus \n");
			// je regarde si le processus cible est valide ou pas 
			// cad si le processus cible c'est moi ou il attend un processus -1 ou 0 alors il y a une erreur
			
			if( processus_en_cours->programme[ indice_instruction ].id_processus_cible == processus_en_cours->num_processus || processus_en_cours->programme[ indice_instruction ].id_processus_cible == -1 || processus_en_cours->programme[ indice_instruction ].id_processus_cible == 0 ){
			
				processus_en_cours->pt_pile++;
				// j'arrete ce processus
				processus_en_cours->etat = 1;
				genere_interruption_2( processus_en_cours->num_processus , -1 , 0);

				return 2;
			}
			
		
			// si ce processus attend la fin d'un autre processus
			// je genere une interruption attente_fin_processus_fils ???
			
			// je regarde si c'est son fils ou non si oui alors 
			//je genere une interruption attente_fin_processus_fils
			
			// si c'est pas son fils alors je considere que c'est pas une instruction valide
			
			int nb_fils = processus_en_cours->nb_fils;
			
			for( i = 0 ; i < nb_fils ; i++ ){
			
				// je regarde si ce processus est un processus fils
				int id_processus_cible = processus_en_cours->programme[ indice_instruction ].id_processus_cible ; 
				if( id_processus_cible == processus_en_cours->tab_numero_fils[i]){
				
					// si oui c'est un fils de mon processus alors 
					// je genere une interruption attente_fin_processus_fils
					
					processus_en_cours->pt_pile++;
					processus_en_cours->etat = 1;
					genere_interruption_2( processus_en_cours->num_processus , id_processus_cible , 2 );
					return 2;
				}	
				// si on est la c'est que notre processus ne corespond pas à un autre processus fils
				// donc je l'arrete

				processus_en_cours->pt_pile++;				
				processus_en_cours->etat = 1;
				genere_interruption_2( processus_en_cours->num_processus , -2 , 0 );

				return 2;
				
			}	
		}else if( bloquant_ressource == 1){
				
			printf("instruction bloquante ressource \n");
		
			// cette instruction est bloquante ressource
			// je genere une interruption  de type ATTENTE_UNE_RESSOURCE

			int id_processus_cible = processus_en_cours->programme[ indice_instruction ].id_processus_cible ;	
			processus_en_cours->etat = 1;		
			genere_interruption_2( processus_en_cours->num_processus , id_processus_cible , 3 );
			return 2;
		}	
	}
	return -1;
}




int ajoute_en_fin( struct liste_processus *listeProc , struct processus *proc_a_ajouter ){

	
	int i ;
    if( listeProc->premier == NULL ){
    
		struct processus *nv_processus = malloc( sizeof( struct processus ));
		
		nv_processus->num_processus = proc_a_ajouter->num_processus;
		nv_processus->num_processus_pere = proc_a_ajouter->num_processus_pere;
		nv_processus->pt_pile = proc_a_ajouter->pt_pile;
		nv_processus->etat = proc_a_ajouter->etat;
		nv_processus->nb_fils = proc_a_ajouter->nb_fils;
		nv_processus->nb_instruction = proc_a_ajouter->nb_instruction;
	
		nv_processus->programme = malloc(proc_a_ajouter->nb_instruction*sizeof( struct instruction ));
		// je copie tout le programme
		for( i = 0 ; i < proc_a_ajouter->nb_instruction; i++ ){
		
			nv_processus->programme[i].valide = proc_a_ajouter->programme[i].valide;
			nv_processus->programme[i].bloquante = proc_a_ajouter->programme[i].bloquante;
			nv_processus->programme[i].id_processus_cible = proc_a_ajouter->programme[i].id_processus_cible;
			nv_processus->programme[i].bloquant_processus = proc_a_ajouter->programme[i].bloquant_processus;
			nv_processus->programme[i].bloquant_ressource = proc_a_ajouter->programme[i].bloquant_ressource;
			nv_processus->programme[i].lecture = proc_a_ajouter->programme[i].lecture;
			nv_processus->programme[i].ecriture = proc_a_ajouter->programme[i].ecriture;
			
		}
		

		
		// je copie les num des fils
		for( i = 0 ; i < proc_a_ajouter->nb_fils; i++ ){
		
			nv_processus->tab_numero_fils[i] = proc_a_ajouter->tab_numero_fils[i];
		}
			
		nv_processus->next = listeProc->premier   ;
		listeProc->premier    = nv_processus;
    	
    	return 1;
    }
    
    struct processus *parcourProcessus = listeProc->premier;
    
    while( parcourProcessus->next != NULL ){
    
        parcourProcessus = parcourProcessus->next;
    }
    
	struct processus *nv_processus = malloc( sizeof( struct processus ));
	// on fait une copie de tous

	nv_processus->num_processus = proc_a_ajouter->num_processus;
	nv_processus->num_processus_pere = proc_a_ajouter->num_processus_pere;
	nv_processus->pt_pile = proc_a_ajouter->pt_pile;
	nv_processus->etat = proc_a_ajouter->etat;
	nv_processus->nb_fils = proc_a_ajouter->nb_fils;
	nv_processus->nb_instruction = proc_a_ajouter->nb_instruction;

	nv_processus->programme = malloc(proc_a_ajouter->nb_instruction*sizeof( struct instruction ));
		// je copie tout le programme
		for( i = 0 ; i < proc_a_ajouter->nb_instruction; i++ ){
			
			nv_processus->programme[i].valide = proc_a_ajouter->programme[i].valide;
			nv_processus->programme[i].bloquante = proc_a_ajouter->programme[i].bloquante;
			nv_processus->programme[i].id_processus_cible = proc_a_ajouter->programme[i].id_processus_cible;
			nv_processus->programme[i].bloquant_processus = proc_a_ajouter->programme[i].bloquant_processus;
			nv_processus->programme[i].bloquant_ressource = proc_a_ajouter->programme[i].bloquant_ressource;
			nv_processus->programme[i].lecture = proc_a_ajouter->programme[i].lecture;
			nv_processus->programme[i].ecriture = proc_a_ajouter->programme[i].ecriture;
		}
		
	// je copie les num des fils
	for( i = 0 ; i < proc_a_ajouter->nb_fils; i++ ){
	
		nv_processus->tab_numero_fils[i] = proc_a_ajouter->tab_numero_fils[i];
	}
	
	nv_processus->next = NULL;
	parcourProcessus->next = nv_processus;
	
	
	return -1;	
	
}

// elle supprime le processus de la liste et adopte ses fils
// si un processus a des fils je les adopte
int supprimer_proc_adopte_fils( struct liste_processus *listeProc , int num_processus ){

	int i;
	struct processus *parcourProcessus = listeProc->premier;
  	struct processus *precedent        = listeProc->premier;
  	struct processus *parcourProcessus_2 = listeProc->premier;
	

	while( parcourProcessus != NULL ){

		if( parcourProcessus->num_processus == num_processus ){
		
			for( i = 0 ; i < parcourProcessus->nb_fils ; i++ ){
		
				// j'ajoute tous ses fils dans la liste des processus zombie
				int num_processus_fils ;
				num_processus_fils = parcourProcessus->tab_numero_fils[i];
				
				// il faut que je le trouve d'abord car ma fonction prend des ptr
				 
				 parcourProcessus_2 = listeProc->premier;
				 
				 while( parcourProcessus_2 != NULL ){
		
					if( parcourProcessus_2->num_processus == num_processus_fils ){
					
						// je mets son fils en etat zombie
						parcourProcessus_2->etat = 3;
						parcourProcessus_2->num_processus_pere = 0; /// sera adopter par init
					}
					
					parcourProcessus_2 = parcourProcessus_2->next;
					
					
				}
			}
			
			// je change le tab du processus
			tab_processus[ num_processus ] = -1;
			
			// je le free
			precedent->next = parcourProcessus->next;
			free( parcourProcessus );
			
			return 1;
		}
	
	 	precedent        = parcourProcessus;
	    parcourProcessus = parcourProcessus->next;
	 
	}// fin while



	printf("ce processus n'existe pas dans cette liste\n");
	return -1;

}

// elle supprime juste le processus de la liste
int supprimer_proc( struct liste_processus *listeProc , int num_processus ){

	struct processus *parcourProcessus = listeProc->premier;
  	struct processus *precedent        = listeProc->premier;
    
  	
	if( listeProc->premier->num_processus == num_processus ){
			
		parcourProcessus = listeProc->premier;
		listeProc->premier = listeProc->premier->next;
		free( parcourProcessus );
		return 1;
	}
  	
  	
    while( parcourProcessus != NULL ){
    	
    	if( parcourProcessus->num_processus == num_processus ){
    		
			printf("precedent->num_processus %d\n", parcourProcessus->next->num_processus);
    		precedent->next = parcourProcessus->next;
    		free( parcourProcessus );
    		return 1;
    	}
    
	 	precedent        = parcourProcessus;
        parcourProcessus = parcourProcessus->next;
     
    }

	// il faut que je regarde s'il a des fils, si oui il faut que de les adopte
	printf("ce processus n'existe pas dans cette liste\n");
	return -1;

}



int ordonnancement_FIFO(){
	
	int tmp_cpu_processus = 0;
	
	int i, ret ;
	
	while( machine->compteur_ordinale !=  nb_cycle_max){
		
		// je prends le premier processus 
		machine->processus_a_traite = premier_proc_pret_executer();
		
		while( machine->processus_a_traite == NULL && file_interruption->premier != NULL ){
			// tq il n'y a pas de processus pret à s'excuter
			// et tq il y a des interruption 
			// je fais la boucle
			// je gere toutes les interruption s'il n'y a pas de processus pret a s'executer
			gestion_interruption(0);
		
		}
		
		machine->processus_a_traite = premier_proc_pret_executer();		
		if( machine->processus_a_traite == NULL && file_interruption->premier == NULL ){
			
			// si j'ai pas de processus pret a s'executer
			// je traite les processus en attente ou toutes les interruptions
			
			//gestion_interruption(0);
			
			// j'ai fini tous les processus
			printf("*******FIN D'EXECUTION DE TOUS LES PROCESSUS********\n");
			printf("le compteur ordinale = %d\n", machine->compteur_ordinale);
			return 0; 
			
		}
			
		printf("\n\t*************************************\n");	
		printf("\t* le processus en execution est : %d *\n", machine->processus_a_traite->num_processus );
		printf("\t*************************************\n");
		
		// je change son etat mnt 
		machine->processus_a_traite->etat = 0;		
		//  je tire au hasard tmp_cpu_processus
		tmp_cpu_processus = rand()%max_tmp_cpu;
	
		if( tmp_cpu_processus == 0){
			machine->processus_a_traite->etat = 2;		
		}
		
		printf("le temps cpu accordé à ce processus est : %d\n", tmp_cpu_processus );
		
		for( i = 0; i < tmp_cpu_processus ; i++){
		

			// j'execute une interruption
			gestion_interruption( machine->processus_a_traite->num_processus );
			
			// j'execute une instruction de ce processus en cours
			ret = gestion_instruction( liste_processus , machine->processus_a_traite );
			if( ret == 2 ){
		
				// cad le processus declenche une interruption
				break;
			}	
			
		}
		
			// j'arrete et je passe au processus suivant	
		machine->compteur_ordinale++;
		
		sleep(2);
	}
	
	printf("le compteur ordinale = %d\n", machine->compteur_ordinale);
	return 0;
}



struct processus *premier_proc_pret_executer(){

	struct processus *premier_processus = liste_processus->premier->next;
	
	while( premier_processus != NULL ){
	
		if( premier_processus->etat == 2){
		
			return premier_processus;
		}
		
		premier_processus = premier_processus->next;
	
	}
	
	return NULL;
}



// cette fonction change l'etat du processus de en attente à pret à s'executer
// et incremente le pt_pile
int rendre_pret_a_executer( int num_processus){

		struct processus *parcourProcessus = liste_processus->premier;		
		while( parcourProcessus != NULL ){

			if( parcourProcessus->num_processus == num_processus ){
			/// cible ou origine iciii
				// j'incremente le pt_pile et je change son etat
				
				//parcourProcessus->pt_pile++;
				parcourProcessus->etat = 2;    // pret a s'executer
				// je change le tab aussi
				tab_processus[num_processus] = 2;
				// je l'ajoute dans la liste des processus en attente
				//ajoute_en_fin( listeProc_pret_execution , parcourProcessus );
				//supprimer_proc( listeProc_en_attente , parcourProcessus->num_processus );
				
				return 1;
			}
			parcourProcessus = parcourProcessus->next;
		}

	return -1;
}




int mettre_a_jour_registre( int num_processus , int etat_registre){

	struct processus *parcourProcessus = liste_processus->premier;		
	while( parcourProcessus != NULL ){

		if( parcourProcessus->num_processus == num_processus ){
		
			parcourProcessus->registre = etat_registre;
			
			supprime_premiere_interruption();
			return 1;
		}

		parcourProcessus = parcourProcessus->next;
	}
}


 
void initialisation_disque(){

	disque = malloc( sizeof( struct disque ));
	
	disque->type_disque = rand()%2;
	disque->etat = 1;
	disque->nb_plateaux = rand()%max_nb_plateaux;
	disque->nb_secteurs_par_plateaux = rand()%max_nb_secteur;
	disque->taille_secteur = rand()%max_taille_secteur;
	disque->tmp_passage_secteur = rand()%max_tmp_passage_secteur;
	disque->tmp_passage_plateau = rand()%max_tmp_passage_plateau; 
	disque->tmp_transfert = rand()%max_tmp_transfert;
} 


 
void initialisation_machine(){

	machine = malloc( sizeof( struct machine));
	file_interruption->premier = NULL;

	machine->compteur_ordinale = 0;
	machine->registre = 0;
	machine->processus_a_traite = NULL;
	
} 


void affiche_disque(){

	printf("\n\t\t*****************************************");
	printf("\n\t\t*\t\t DISQUE\t\t\t*\n");
	printf("\t\t*****************************************\n\n");

		printf("type_disque\tnb_plateaux\tnb_secteurs\ttaille_secteur\ttmp_passage_secteur\ttmp_passage_plateau\ttmp_transfert\n");
printf("    %d\t\t   %d\t\t\t%d\t\t%d\t\t    %d\t\t\t%d\t\t\t%d\n", disque->type_disque, disque->nb_plateaux, disque->nb_secteurs_par_plateaux, disque->taille_secteur, disque->tmp_passage_secteur, disque->tmp_passage_plateau, disque->tmp_transfert);	
}



void changer_etat_disque( int etat ){

	disque->etat = etat ;
}

int ret_etat_disque(){

	disque->etat = rand()%2;
	return disque->etat;
	
}

void affiche_programmes( int nb_processus){

	int i ;
	for( i = 1 ; i <= nb_processus ; i++){
	
		affiche_programme(i);
	}

}

