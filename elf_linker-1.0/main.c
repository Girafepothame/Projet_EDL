#include "lecture_elf.c"

int main(int argc , char **argv)
{
    char *entree = NULL; // Nom du fichier en entrée.
	int aff_all = 0; 	 // Tout afficher
    int aff_hd = 0;      // Est-ce qu'on affiche le header du fichier ? (-h)
    int aff_sc = 0;      // Est-ce qu'on affiche les headers des sections ? (-S)
	int aff_sb = 0;      // Est-ce qu'on affiche la table des symboles ? (-s)
	int aff_rel = 0;     // Est-ce qu'on affiche la table de réalocation ? (-r)
    for (int i=1; i<argc; i++) {
        if (!strcmp(argv[i], "-h")) {
            aff_hd = 1;
        } else {
            if (!strcmp(argv[i], "-S")) {
                aff_sc = 1;
            } else {
				if (!strcmp(argv[i], "-s")) {
					aff_sb = 1;
				}else if (!strcmp(argv[i], "-r")) {
					aff_rel = 1;
				}else if (entree==NULL) {
                    // Le premier argument qui n'est pas une option est le nom du fichier d'entrée.
                    entree = argv[i];
                }
            }
        }
    }
	if(argc == 2) {
		aff_all = 1;
	}
    if (entree==NULL) {
        printf("Aucun nom de fichier donné en argument.\n");
        return 2;
    }

    FILE *f = fopen(entree,"r");
    lectureHead(f);
    
    
    sct = malloc(sizeof(section_n) * header.e_shnum);
    
    lectureSection(f);

    lectureSymbol(f);

	lectureReloc(f);

    if (aff_hd){
        print_header();
	}
	
    if (aff_sc){
        print_section();
	}
	
    if (aff_sb) {
        print_symbole();
	}

	if (aff_rel) {
    	printReloc();
	}

	if (aff_all) {
        print_header();
        print_section();
        print_symbole();
	}

	if (argv[2] != NULL && isNumber(argv[2])) { // à voir pour automatiser si argv[2] est int ou char*
		int num = atoi(argv[2]);
		//char *num = argv[2];
		afficheContenuNumero(num, f);
	}

    fclose(f);

    free(sct);

    return 0;
}
