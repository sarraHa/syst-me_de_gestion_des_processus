
#define max_tmp_cpu              5
#define nb_cycle_max             10


struct machine{

	int compteur_ordinale;
	int registre;
	struct processus *processus_a_traite;
	
};


struct machine *machine;





int ordonnancement_FIFO();
struct processus *premier_proc_pret_executer();
int rendre_pret_a_executer( int num_processus);
void initialisation_machine();
int ordonnancement_moins_CPU();
struct processus *premier_proc_moins_CPU();
