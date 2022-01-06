#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include "lecture_elf.h"


Elf32_Ehdr header;
Elf32_Shdr section;
section_n *sct;


int IsElf(unsigned char *str) {
    if(str[0] == 0x7f && str[1] == 'E' && str[2] == 'L' && str[3] == 'F')
    	return 1;
    else
    	return 0;
}


void lectureHead(FILE *f){

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

	fread(&header, 1, sizeof(Elf32_Ehdr), f);

	if (IsElf(header.e_ident) != 1) {
		printf("ERREUR ! Fichier non ELF\n");
		exit(1);
	}
}

void lectureSection(FILE *f){

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

	fseek(f, header.e_shoff + header.e_shentsize * header.e_shstrndx, SEEK_SET);

	fread(&section, 1, sizeof(section), f);

	char* sect_nom = malloc(section.sh_size);

	fseek(f, section.sh_offset, SEEK_SET);

	fread(sect_nom, 1, section.sh_size, f);

	for (int i=0; i<header.e_shnum; i++) {

		sct[i].nom = "";

		fseek(f, header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
		fread(&sct[i].sect, 1, sizeof(section), f);

		if (sct[i].sect.sh_name) {
			sct[i].nom = sect_nom + sct[i].sect.sh_name;
		}

		// printf("\n%d\n", i);

	}
} 


void print_header() {

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
	printf("Clé des fanions :\n  W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes), I (info),\n  L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe),\n  T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),\n  y (purecode), p (processor specific)\n");
}

char *afficheContenuNumero(int valeur){
	
	if(valeur >= 12){
		return "";
	}
	printf("[%d] %s ", valeur,sct[valeur].nom);
	switch(bswap_32(sct[valeur].sect.sh_type)){
			case 0:
				printf("NULL");
				break;
			case 1:
				printf("PROGBITS ");
				break;
			case 2:
				printf("SYMTAB");
				break;
			case 3:
				printf("STRTAB");
				break;
			case 4:
				printf("RELA");
				break;
			case 5:
				printf("HASH");
				break;
			case 6:
				printf("DYNAMIC");
				break;
			case 7:
				printf("NOTE");
				break;
			case 8:
				printf("NOBITS");
				break;
			case 9:
				printf("REL");
				break;
			case 10:
				printf("SHLIB");
				break;
			case 11:
				printf("DYNSYM");
				break;
			case 0x70000003:
				printf("ARM_ATTRIBUTES");
				break;
			case 0x70000000:
				printf("LOPROC");
				break;
			case 0x7fffffff:
				printf("HIPROC");
				break;
			case 0x80000000:
				printf("LOUSER");
				break;
			case 0xffffffff:
				printf("HIUSER");
				break;
			default:
				printf("ERROR");
		}
		printf("%x ", bswap_32(sct[valeur].sect.sh_type));
		printf("%08x %06x %06x %02x ",bswap_32(sct[valeur].sect.sh_addr),bswap_32(sct[valeur].sect.sh_offset),bswap_32(sct[valeur].sect.sh_size),bswap_32(sct[valeur].sect.sh_entsize));
	if(bswap_32(sct[valeur].sect.sh_flags) & 1<<0){
			printf("W");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<1){
			printf("A");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<2){
			printf("X");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<4){
			printf("M");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<5){
			printf("S");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<6){
			printf("I");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<7){
			printf("L");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<8){
			printf("o");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<9){
			printf("G");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<10){
			printf("T");
		}
		if(bswap_32(sct[valeur].sect.sh_flags) & 1<<31){
			printf("E");
		}
	printf(" %d ",bswap_32(sct[valeur].sect.sh_info));
	printf(" %d  ",bswap_32(sct[valeur].sect.sh_link));
	printf(" %d",bswap_32(sct[valeur].sect.sh_addralign));
}

char *afficheContenuString(char *valeur){
	printf("%s\n", valeur);
	int i;
	int index;
	for( i = 0; i < 12; i++){
		if(strcmp(valeur, sct[i].nom) == 0){
			index = i;
		}
	}
	afficheContenuNumero(index);
	
}


int main(int argc , char **argv)
{
    FILE *f;
    f = fopen(argv[1],"r");
    lectureHead(f);
    print_header();
    fclose(f);
	
    sct = malloc(sizeof(section_n) * header.e_shnum);
	
    f = fopen(argv[1],"r");
    lectureSection(f);
    print_section();
    fclose(f);
    return 0;
}
