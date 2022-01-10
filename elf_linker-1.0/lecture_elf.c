#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include "lecture_elf.h"


Elf32_Ehdr header;
Elf32_Shdr section;
section_n *sct;
symbole_n *sym;

int IsElf(unsigned char *str) {
    if(str[0] == 0x7f && str[1] == 'E' && str[2] == 'L' && str[3] == 'F')
    	return 1;
    else
    	return 0;
}


void lectureHead(FILE *f){

	fread(&header, 1, sizeof(Elf32_Ehdr), f);
	header.e_type = bswap_16(header.e_type);
	header.e_machine = bswap_16(header.e_machine);
	header.e_version =  bswap_32(header.e_version);
	header.e_entry = bswap_32(header.e_entry);
	header.e_phoff = bswap_32(header.e_phoff);
	header.e_shoff = bswap_32(header.e_shoff);
	header.e_flags = bswap_32(header.e_flags);
	header.e_ehsize = bswap_16(header.e_ehsize);
	header.e_phentsize = bswap_16(header.e_phentsize);
	header.e_phnum = bswap_16(header.e_phnum);
	header.e_shentsize = bswap_16(header.e_shentsize);
	header.e_shnum = bswap_16(header.e_shnum);
	header.e_shstrndx = bswap_16(header.e_shstrndx);
	if (IsElf(header.e_ident) != 1) {
		printf("ERREUR ! Fichier non ELF\n");
		exit(1);
	}
}

void lectureSection(FILE *f){
	fseek(f, header.e_shoff + header.e_shentsize * header.e_shstrndx, SEEK_SET);

	fread(&section, 1, sizeof(section), f);

	section.sh_name = bswap_32(section.sh_name);
	section.sh_type = bswap_32(section.sh_type);
	section.sh_flags = bswap_32(section.sh_flags);
	section.sh_addr = bswap_32(section.sh_addr);
	section.sh_offset = bswap_32(section.sh_offset);
	section.sh_size = bswap_32(section.sh_size);
	section.sh_link = bswap_32(section.sh_link);
	section.sh_info = bswap_32(section.sh_info);
	section.sh_addralign = bswap_32(section.sh_addralign);
	section.sh_entsize = bswap_32(section.sh_entsize);

	char* sect_nom = malloc(section.sh_size);

	fseek(f, section.sh_offset, SEEK_SET);

	fread(sect_nom, 1, section.sh_size, f);

	for (int i=0; i<header.e_shnum; i++) {

		sct[i].nom = "";

		fseek(f, header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
		fread(&sct[i].sect, 1, sizeof(section), f);

		sct[i].sect.sh_name = bswap_32(sct[i].sect.sh_name);
		sct[i].sect.sh_type = bswap_32(sct[i].sect.sh_type);
		sct[i].sect.sh_flags = bswap_32(sct[i].sect.sh_flags);
		sct[i].sect.sh_addr = bswap_32(sct[i].sect.sh_addr);
		sct[i].sect.sh_offset = bswap_32(sct[i].sect.sh_offset);
		sct[i].sect.sh_size = bswap_32(sct[i].sect.sh_size);
		sct[i].sect.sh_link = bswap_32(sct[i].sect.sh_link);
		sct[i].sect.sh_info = bswap_32(sct[i].sect.sh_info);
		sct[i].sect.sh_addralign = bswap_32(sct[i].sect.sh_addralign);
		sct[i].sect.sh_entsize = bswap_32(sct[i].sect.sh_entsize);

		if (sct[i].sect.sh_name) {
			sct[i].nom = sect_nom + sct[i].sect.sh_name;
		}

		// printf("\n%d\n", i);

	}
}


void lectureSymbol (FILE *f){
	int i=0;
 	while(strcmp(sct[i].nom,".strtab") && i<header.e_shnum){
			i++;
	}

	char *sym_nom = malloc(sct[i].sect.sh_size);
	fseek(f,sct[i].sect.sh_offset,SEEK_SET);
	fread(sym_nom, 1,sct[i].sect.sh_size, f);


	i=0;
	while(strcmp(sct[i].nom,".symtab") && i<header.e_shnum){
			i++;
	}

	fseek(f,sct[i].sect.sh_offset,SEEK_SET);
	sym=malloc(sizeof(symbole_n)*sct[i].sect.sh_size);
	sym->taille=sct[i].sect.sh_size/16;
	for (int j=0;j<sym->taille;j++){
		sym[j].nom = "";

		fread(&sym[j].S,1,sizeof(Elf32_Sym),f);

		sym[j].S.st_name = bswap_32(sym[j].S.st_name);
		sym[j].S.st_value = bswap_32(sym[j].S.st_value);
		sym[j].S.st_size = bswap_32(sym[j].S.st_size);
		sym[j].S.st_shndx = bswap_16(sym[j].S.st_shndx);

		if (sym[j].S.st_name){
        		sym[j].nom = sym_nom + sym[j].S.st_name;}
    }
}

void lectureReloc (FILE *f){
	for (int i=0; i<header.e_shnum; i++) {
		if(sct[i].sect.sh_type == 4) {
			printf("Coucou etape 1 \n");
		}


	}



	// switch(header.e_type) {
	// 	case 1: // cas repositionnable

	// 		break;
	// 	case 2: // cas executable
	// 	case 3: // cas objet partagé

	// 		break;
	// }
}


void printReloc() {
//	 printf("Section de réadressage '%s' a l'adresse de décalage %x contient %d entrées ",);
// 	 printf("  Décalage        Info           Type           Val.-symboles Noms-symb.+ Addenda");

}



void print_header() {

    printf("En-tête ELF: \n");
    printf("    Magic: ");
    for(int i = 0 ; i < 16 ; i++) {
    	printf("%02x ",header.e_ident[i]);
    }
        
    printf("\n    Class: 						");

    switch (header.e_ident[4]) { //Faire des switch case pour chaque print avec les resultats possibles comme fait dans l'ancien code.
    	case 1 : 
    		printf("ELF32");
            break;

       case 2 : 
       		printf("ELF64");
            break;

       default : 
       		printf("TYPE ELF INCONNU");
    }

    printf("\n    Data: 						");

    switch (header.e_ident[5]) { //Faire des switch case pour chaque print avec les resultats possibles comme fait dans l'ancien code.
    	case 1 : 
    		printf("Complément à deux, petit boutisme");
            break;

       case 2 : 
       		printf("Complément à deux, gros boutisme");
            break;

       default : 
       		printf("Format de données inconnu");
    }

    printf("\n    Version: 						");

    switch (header.e_ident[6]) { //Faire des switch case pour chaque print avec les resultats possibles comme fait dans l'ancien code.
       case 1 : 
       		printf("1 (current)");
            break;

       default : 
       		printf("0 (Invalid version)");

   	}

   	printf("\n    OS/ABI: 						");

   	switch (header.e_ident[7]) {
   		case 0:
   			printf("UNIX System V");
   			break;
   		case 1:
   			printf("HP-UX");
   			break;
   		case 2:
   			printf("NetBSD");
   			break;
   		case 3:
   			printf("Linux");
   			break;
   		case 6:
   			printf("Sun Solaris");
   			break;
   		case 7:
   			printf("IBM AIX");
   			break;
   		case 8:
   			printf("SGI Irix");
   			break;
   		case 9:
   			printf("FreeBSD");
   			break;
   		case 10:
   			printf("Compaq TRU64");
   			break;
   		case 11:
   			printf("Novell Modesto");
   			break;
   		case 12:
   			printf("OpenBSD");
   			break;
   		case 64:
   			printf("ARM EABI");
   			break;
   		case 97:
   			printf("ARM");
   			break;
   		case 255:
   			printf("Standalone");
   			break;
   	}

   	printf("\n    Version ABI: 					%x", header.e_ident[8]);

   	printf("\n    Type: 						");
    		
	switch (header.e_type) {
        case 0 :
            printf("Aucun");
            break;
        case 1 :
            printf("Repositionable");
            break;
        case 2 :
           printf("Executable");
           break;
        case 3 :
            printf("Objet partagé");
            break;
        case 4 :
            printf("Fichier Core");
	    	break;
		default:
            printf("Processor-specific");
    }

   	printf("\n    Machine: 						");
	switch (header.e_machine) {
		case 0:
			printf("No machine");
			break;
		case 1:
			printf("AT&T WE 32100");
			break;
		case 2:
			printf("SPARC");
			break;
		case 3:
			printf("Intel Architecture");
			break;
		case 4:
			printf("Motorola 68000");
			break;
		case 5:
			printf("Motorola 88000");
			break;
		case 7:
			printf("Intel 80860");
			break;
		case 8:
			printf("MIPS RS3000 Big-Endian");
			break;
		case 10:
			printf("MIPS RS4000 Big-Endian");
			break;
		case 19:
			printf("Intel i960");
			break;
		case 20:
			printf("Power PC");
			break;
		case 40:
			printf("ARM");
			break;
		case 50:
			printf("Intel IA64");
			break;
		case 62:
			printf("x64");
			break;
		case 243:
			printf("RISC-V");
			break;
		default:
			printf("Reserved for future use");
			break;
    }

    printf("\n    Version Machine: 					");

    switch (header.e_version) {
       case 0 : 
       		printf("0x0 (aucune)");
       		break;
        
        case 1 : 
    		printf("0x1 (current)");
    		break;

       default : 
       		printf("Version Machine non reconnue");
   	}

   	printf("\n    Adresse du point d'entrée: 				");

	if (header.e_entry == 0) {
		printf("no entry point");
	} 
	else {		
        printf("0x%x", header.e_entry);
	}

	printf("\n    Début des en-têtes de programme:			%d (octets dans le fichier)", header.e_phoff);

    printf("\n    Début des en-têtes de section:			%d (octets dans le fichier)", header.e_shoff);

    printf("\n    Flags: 						0x%02x, Version5 EABI, soft-float ABI", header.e_flags);

    printf("\n    Taille de cet en-tete: 				%d (octets)", header.e_ehsize);

    printf("\n    Taille de l'en-tete du programme: 			%d (octets)", header.e_phentsize);

    printf("\n    Nombre d'en-tête du programme: 			%d", header.e_phnum);
    
    printf("\n    Taille des en-têtes de section: 			%d (octets) ", header.e_shentsize);
    
    printf("\n    Nombre d'en-têtes de section: 		   	%d", header.e_shnum);
    
    printf("\n    Table d'indexes des chaînes d'en-tête de section: 	%d \n", header.e_shstrndx);
    
}

void print_section() {


	printf("\nIl y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x:\n", header.e_shnum, header.e_shoff);

	int Lmax = 3; // Longueur du mot "Nom", longueur minimum de la colonne.
	int L;
	for (int i=0; i<header.e_shnum; i++) {
		L = strlen(sct[i].nom);
		if (L>Lmax)
			Lmax = L;
	}
	Lmax++;

	printf("\n[NR] Nom");
	for (int j=3; j<Lmax; j++) {
		printf(" ");
	}
	
	printf("Type           Adr      Décala Taille ES Fan LN Inf Al\n");
	
	for (int i=0; i<header.e_shnum; i++) {


		printf("[%02d] ", i);
		
		printf("%s", sct[i].nom);
		for (int j=strlen(sct[i].nom); j<Lmax; j++) {
			printf(" ");
		}

		switch(sct[i].sect.sh_type){
			case 0:
				printf("NULL");
				L = 4;
				break;
			case 1:
				printf("PROGBITS");
				L = 8;
				break;
			case 2:
				printf("SYMTAB");
				L = 6;
				break;
			case 3:
				printf("STRTAB");
				L = 6;
				break;
			case 4:
				printf("RELA");
				L = 4;
				break;
			case 5:
				printf("HASH");
				L = 4;
				break;
			case 6:
				printf("DYNAMIC");
				L = 7;
				break;
			case 7:
				printf("NOTE");
				L = 4;
				break;
			case 8:
				printf("NOBITS");
				L = 6;
				break;
			case 9:
				printf("REL");
				L = 3;
				break;
			case 10:
				printf("SHLIB");
				L = 5;
				break;
			case 11:
				printf("DYNSYM");
				L = 6;
				break;
			case 0x70000003:
				printf("ARM_ATTRIBUTES");
				L = 14;
				break;
			case 0x70000000:
				printf("LOPROC");
				L = 6;
				break;
			case 0x7fffffff:
				printf("HIPROC");
				L = 6;
				break;
			case 0x80000000:
				printf("LOUSER");
				L = 6;
				break;
			case 0xffffffff:
				printf("HIUSER");
				L = 6;
				break;
			default:
				printf("ERROR");
				L = 5;
		}
		while (L<15) {
			printf(" ");
			L++;
		}
		printf("%08x %06x %06x %02x ", sct[i].sect.sh_addr, sct[i].sect.sh_offset, sct[i].sect.sh_size, sct[i].sect.sh_entsize);
		L = 0;
		if(sct[i].sect.sh_flags & 1<<0){
			printf("W");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<1){
			printf("A");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<2){
			printf("X");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<4){
			printf("M");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<5){
			printf("S");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<6){
			printf("I");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<7){
			printf("L");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<8){
			printf("o");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<9){
			printf("G");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<10){
			printf("T");
			L++;
		}
		if(sct[i].sect.sh_flags & 1<<31){
			printf("E");
			L++;
		}
		while (L<4) {
			printf(" ");
			L++;
		}
		printf("%d  ", sct[i].sect.sh_link);
		if (sct[i].sect.sh_link<10)
			printf(" ");
		printf("%d ", sct[i].sect.sh_info);
		if (sct[i].sect.sh_info<10)
			printf(" ");
		printf("%d", sct[i].sect.sh_addralign);

		printf("\n");
	}
	printf("Clé des fanions :\n  W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes), I (info),\n  L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe),\n  T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),\n  y (purecode), p (processor specific)\n");
}

void afficheContenuNumero(int valeur, FILE *f){
	int i;
	int taille;
	int c;
	if(valeur >= header.e_shnum){
		printf("ERROR pas de section %d\n", valeur);
		exit(1);
	}
	fseek(f, sct[valeur].sect.sh_offset, SEEK_SET);
	unsigned char *tab = malloc(sct[valeur].sect.sh_size);
	taille = 0;
	if(tab != NULL){
		printf("Vidange hexadécimale de la section « %s »  \n", sct[valeur].nom);
		fread(tab, 1, sct[valeur].sect.sh_size, f);
		for(i = 0; i < sct[valeur].sect.sh_size;i++){	
		
			if( i%16==0){
				printf("\n");
				if(bswap_32(sct[valeur].sect.sh_size) == 16){
					printf("0x%08x", sct[valeur].sect.sh_addr);
				}else{
					printf("0x%08x", taille);
					taille +=16;
				}
			}
			if((i%4) == 0){
					printf(" ");
				}
			printf("%02x", tab[i]);
		}
		printf("\n");
		for(i=0; i<sct[valeur].sect.sh_size; i++){
			c = (int) tab[i];
			//printf("\ndeci : %d , hexa : %02x\n",c, tab[i]);
			if( i%16==0){
				printf("\n");
			}
			if(c >= 33 && c <= 126){
				printf("%c", c);
			}
			else{
				printf(".");
			}
		}	
	}
	if(sct[valeur].sect.sh_size == 0){
		printf("La section « %s » n'a pas de données à vidanger", sct[valeur].nom);
	}
}

void afficheContenuString(char *valeur, FILE *f){
	printf("%s\n", valeur);
	int i;
	int index;
	
	for( i = 0; i < header.e_shnum; i++){
		//printf("valeur : %s sct|i].nom : %s \n", valeur, sct[i].nom);
		//printf("valeur : %s sct|i].nom : %s \n", valeur, sct[index].nom);
		if(strcmp(valeur, sct[i].nom) == 0){
			index = i;
		}
	}
	printf("index section : %d", index);
	afficheContenuNumero(index, f);
}



void print_symbole() {
	printf("\nSymbol table '.symtab' contains %d entries:\n",sym->taille);
	printf("Num:    Value  Size Type    Bind    Vis      Ndx Name\n");
	
	for (int j=0;j<sym->taille;j++){
		printf(" %2d : %.8x     %d ",j,sym[j].S.st_value,sym[j].S.st_size);
		switch(ELF32_ST_TYPE(sym[j].S.st_info)){
			case 0:
				printf("NOTYPE  ");
				break;
			case 1:
				printf("OBJECT  ");
				break;
			case 2:
				printf("FUNC   ");
				break;
			case 3:
				printf("SECTION ");
				break;
			case 4:
				printf("FILE    ");
				break;
			case 13:
				printf("LOPROC ");
				break;			
			case 15:
				printf("HIPROC ");
				break;	
		}
		switch(ELF32_ST_BIND(sym[j].S.st_info)){
			case 0:
				printf("LOCAL  ");
				break;
			case 1:
				printf("GLOBAL ");
				break;
			case 2:
				printf("WEAK   ");
				break;
			case 13:
				printf("LOPROC ");
				break;			
			case 15:
				printf("HIPROC ");
				break;	
		}
	printf("DEFAULT ");
		switch(sym[j].S.st_shndx){
			case 0:
				printf("UND  ");
				break;
			case 0xff00:
				printf("LOPROC  ");
				break;
			case 0xff1f:
				printf("HIPROC   ");
				break;
			case 0xfff1:
				printf("ABS ");
				break;			
			case 0xfff2:
				printf("COMMON ");
				break;
			case 0xffff :
				printf("HIRESERVE ");
				break;	
			default :
				printf("%2d ",sym[j].S.st_shndx);
				break;
	
	}
	printf("%s",sym[j].nom);
	printf("\n");
	}
}

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
                    // Le premier ergument qui n'est pas une option est le nom du fichier d'entrée.
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
    fclose(f);
    
    
    sct = malloc(sizeof(section_n) * header.e_shnum);
    
    f = fopen(entree,"r");
    lectureSection(f);
    fclose(f);


	f = fopen(entree,"r");
    lectureSymbol(f);
    fclose(f);

	f = fopen(entree,"r");
	lectureReloc(f);
	fclose(f);

	if (argv[2] != NULL) { // à voir pour automatiser si argv[2] est int ou char*
		f = fopen(entree,"r");
		// int num = atoi(argv[2]);
		char *num = argv[2];
		afficheContenuString(num, f);
    	fclose(f);
	}

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
      // lectureReloc(f);
	}

	if (aff_all) {
        print_header();
        print_section();
        print_symbole();
	}

	// f = fopen(argv[1],"r");
	// lectureReloc(f);
	// fclose(f);

    return 0;
}
