#include <assert.h>
#include "lecture_elf.c"

/*
index sect; 
string ll = nom_section
fread ( , taille_section);
sect ++



*/

Elf32_Ehdr header2;
Elf32_Shdr section2;
Elf32_Sym symbole2;


void unswap_header(){
    for (int i=0; i<EI_NIDENT; i++){
        header2.e_ident[i] = header.e_ident[i];
    }
    header2.e_type = bswap_16(header.e_type);
    header2.e_machine = bswap_16(header.e_machine);
    header2.e_version =  bswap_32(header.e_version);
    header2.e_entry = bswap_32(header.e_entry);
    header2.e_phoff = bswap_32(header.e_phoff);
    header2.e_shoff = bswap_32(header.e_shoff);
    header2.e_flags = bswap_32(header.e_flags);
    header2.e_ehsize = bswap_16(header.e_ehsize);
    header2.e_phentsize = bswap_16(header.e_phentsize);
    header2.e_phnum = bswap_16(header.e_phnum);
    header2.e_shentsize = bswap_16(header.e_shentsize);
    header2.e_shnum = bswap_16(header.e_shnum);
    header2.e_shstrndx = bswap_16(header.e_shstrndx);

}
void unswap_sect(int j){    
    section2.sh_name = bswap_32(sct[j].sect.sh_name);
    section2.sh_type = bswap_32(sct[j].sect.sh_type);
    section2.sh_flags = bswap_32(sct[j].sect.sh_flags);
    section2.sh_addr = bswap_32(sct[j].sect.sh_addr);
    section2.sh_offset = bswap_32(sct[j].sect.sh_offset);
    section2.sh_size = bswap_32(sct[j].sect.sh_size);
    section2.sh_link = bswap_32(sct[j].sect.sh_link);
    section2.sh_info = bswap_32(sct[j].sect.sh_info);
    section2.sh_addralign = bswap_32(sct[j].sect.sh_addralign);
    section2.sh_entsize = bswap_32(sct[j].sect.sh_entsize);
}

void unswap_section (){
    section2.sh_name = bswap_32(section.sh_name);
    section2.sh_type = bswap_32(section.sh_type);
    section2.sh_flags = bswap_32(section.sh_flags);
    section2.sh_addr = bswap_32(section.sh_addr);
    section2.sh_offset = bswap_32(section.sh_offset);
    section2.sh_size = bswap_32(section.sh_size);
    section2.sh_link = bswap_32(section.sh_link);
    section2.sh_info = bswap_32(section.sh_info);
    section2.sh_addralign = bswap_32(section.sh_addralign);
    section2.sh_entsize = bswap_32(section.sh_entsize); 
}

void unswap_symbole (){
    symbole2.st_name = bswap_32(sym[j].S.st_name);
    symbole2.st_value = bswap_32(sym[j].S.st_value);
    symbole2.st_size = bswap_32(sym[j].S.st_size);
    symbole2.st_shndx = bswap_16(sym[j].S.st_shndx);
}

void init_table( int * t,int taille){
    for (int i=0;i<taille;i++){
        t[i]=0;}
} 

void renumSection(FILE *fe, FILE *fs){
    int count = 0;
    int i =0;
    int max =0;
    int taille=0;
    int *t=malloc(sizeof(int)*header.e_shnum);
    init_table(t,header.e_shnum);
    while (i<header.e_shnum){
        if (sct[i].sect.sh_type == 9) {
            count ++;
            t[i]=1;
            taille=taille+sct[i].sect.sh_size;
        }
        if(sct[i].sect.sh_size > max) {
            max=sct[i].sect.sh_size;
        }
        // printf("\n %d kk \n",t[i]);
        i++;
    }

    int shnum = header.e_shnum;
    header.e_shnum = header.e_shnum - count;
    header.e_shstrndx = header.e_shstrndx - count;
    header.e_shoff = header.e_shoff - taille;
    unswap_header();
    fwrite(&header2, sizeof(Elf32_Ehdr),1 , fs);
    
    
    unsigned char *tab ;
    fseek(fs,header.e_shoff,SEEK_SET);
    for (int k =0; k < shnum;k++){
        if (t[k] != 1 ){
            unswap_sect(k);
            if (!strcmp(sct[k].nom,".shstrtab") && sct[k].nom != NULL){
                section2.sh_offset =bswap_32(sct[k].sect.sh_offset - taille);

            }
            else if (!strcmp(sct[k].nom,".symtab") && sct[k].nom != NULL){
                section2.sh_link =bswap_32(sct[k].sect.sh_link - count);
            }
            printf("\n %d : %d \n" , k,section.sh_link);
            fwrite(&section2,sizeof(Elf32_Shdr),1,fs);
        } 
    }

    unswap_section();
    fseek(fs,header.e_ehsize,SEEK_SET);
    for(int j=0; j<shnum; j++){
        tab= malloc(sct[j].sect.sh_size);
       if (sct[j].sect.sh_type != 9) {

            fseek(fe,sct[j].sect.sh_offset, SEEK_SET);
            //printf("%x",sct[4].sect.sh_offset);
            
            fread(tab,1,sct[j].sect.sh_size,fe);
            if (!strcmp(sct[j].nom,".shstrtab")){
                sct[j].sect.sh_offset =section.sh_offset - taille;
            }
            fseek(fs,sct[j].sect.sh_offset,SEEK_SET);
            fwrite(tab,sct[j].sect.sh_size,1,fs);

            printf("\nSection %s: ",sct[j].nom);
            if (!strcmp(sct[j].nom,".strtab")){
                printf("\n TAILLE : %d\n",sct[j].sect.sh_offset);
            }
                
            for (i=0;i<sct[j].sect.sh_size;i++){
                printf(" %02x ",tab[i]);
            }
        free(tab);
        }
    }

    
    unswap_symbole();
        
}



int main(int argc, char const *argv[]) {
    
    FILE *fe = fopen(argv[1],"r");
    //printf("%s", argv[1]);
    FILE *fs = fopen(argv[2],"w+");
    assert(fe != NULL);
    
    sct = malloc(sizeof(section_n) * header.e_shnum);
    
    lectureHead(fe);
    fclose(fe);
    
    fe = fopen(argv[1],"r");
    
    lectureSection(fe);
    printf("Bonjour");
    
    printf("Bonsoir ouiiffsqdfq");
    
    fe = fopen(argv[1],"r");
    renumSection(fe,fs);
    
    return 0;
}