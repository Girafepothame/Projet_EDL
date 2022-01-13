#include <assert.h>
#include "lecture_elf.c"





Elf32_Ehdr header2;
Elf32_Shdr section2;
section_n *sct2;
symbole_n *sym2;


void unswap_header(int count,int taille){
    for (int i=0; i<EI_NIDENT; i++){
        header2.e_ident[i] = header.e_ident[i];
    }
    header2.e_type = bswap_16(header.e_type);
    header2.e_machine = bswap_16(header.e_machine);
    header2.e_version =  bswap_32(header.e_version);
    header2.e_entry = bswap_32(header.e_entry);
    header2.e_phoff = bswap_32(header.e_phoff);
    header2.e_shoff = bswap_32(header.e_shoff - taille);
    header2.e_flags = bswap_32(header.e_flags);
    header2.e_ehsize = bswap_16(header.e_ehsize);
    header2.e_phentsize = bswap_16(header.e_phentsize);
    header2.e_phnum = bswap_16(header.e_phnum);
    header2.e_shentsize = bswap_16(header.e_shentsize);
    header2.e_shnum = bswap_16(header.e_shnum - count);
    header2.e_shstrndx = bswap_16(header.e_shstrndx - count);

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

void init_table( int * t,int taille){
    for (int i=0;i<taille;i++){
        t[i]=0;}
} 

void renumSection(FILE *fe, FILE *fs){
    int count = 0;
    int i =0;
    int max =0;
    int taille=0;
    int indice=0;
    int *t=malloc(sizeof(int)*header.e_shnum);
    sct2=malloc(sizeof(section_n) * header.e_shnum);
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
        i++;
    }

    int shnum = header.e_shnum;
  
    unswap_header(count,taille);
    fwrite(&header2, sizeof(Elf32_Ehdr),1 , fs);
    
    
    unsigned char *tab ;
    //recupere et affiche section header
    fseek(fs,bswap_32(header2.e_shoff),SEEK_SET);
    for (int k =0; k < shnum;k++){
        if (t[k] != 1 ){
            unswap_sect(k);
            if (!strcmp(sct[k].nom,".shstrtab") && sct[k].nom != NULL){
                section2.sh_offset =bswap_32(sct[k].sect.sh_offset - taille);

            }
            else if (!strcmp(sct[k].nom,".symtab") && sct[k].nom != NULL){
                section2.sh_link =bswap_32(sct[k].sect.sh_link - count);
            }
            sct2[indice].sect.sh_name = sct[k].sect.sh_name;
            sct2[indice].sect.sh_type = sct[k].sect.sh_type; 
            sct2[indice].sect.sh_flags = sct[k].sect.sh_flags;
            sct2[indice].sect.sh_addr = sct[k].sect.sh_addr;
            sct2[indice].sect.sh_offset = sct[k].sect.sh_offset;
            sct2[indice].sect.sh_size = sct[k].sect.sh_size;
            sct2[indice].sect.sh_link = sct[k].sect.sh_link;
            sct2[indice].sect.sh_info = sct[k].sect.sh_info;
            sct2[indice].sect.sh_addralign = sct[k].sect.sh_addralign;
            sct2[indice].sect.sh_entsize = sct[k].sect.sh_entsize;
            sct2[indice].nom = sct[k].nom;
            indice++;
            fwrite(&section2,sizeof(Elf32_Shdr),1,fs);
        } 
    }
    unswap_section();
    //recuper et affiche les sections
    fseek(fs,header.e_ehsize,SEEK_SET);
    for(int j=0; j<shnum; j++){
        tab= malloc(sct[j].sect.sh_size);
       if (sct[j].sect.sh_type != 9) {

            fseek(fe,sct[j].sect.sh_offset, SEEK_SET);
         
            
            fread(tab,1,sct[j].sect.sh_size,fe);
            if (!strcmp(sct[j].nom,".shstrtab")){
                sct[j].sect.sh_offset =section.sh_offset - taille;
            }
            fseek(fs,sct[j].sect.sh_offset,SEEK_SET);
            fwrite(tab,sct[j].sect.sh_size,1,fs);

            

        free(tab);
        }
    }
        
}



void recupSymbole(FILE *fe, FILE *fs){
    int i=0;
    int indice;
    int size=0;
    while(strcmp(sct2[i].nom,".symtab") && i<header.e_shnum){
            i++;
    }
    sym2=malloc(sizeof(symbole_n)*sct2[i].sect.sh_size);
    for (int j =0 ; j <sct2[i].sect.sh_size/16 ;j++){
        indice=0;
        while ( strcmp(sct[sym[j].S.st_shndx].nom,sct2[indice].nom) ) {
            indice++;                          
        }

        sym2[j].S.st_name = bswap_32(sym[j].S.st_name);
        sym2[j].S.st_value = bswap_32(sym[j].S.st_value);
        sym2[j].S.st_size = bswap_32(sym[j].S.st_size);
        sym2[j].S.st_info =sym[j].S.st_info;
        sym2[j].S.st_other=sym[j].S.st_other;
        sym2[j].S.st_shndx = bswap_16(indice);
        if (!strcmp(sct2[indice].nom , ".text")) {
            sym2[j].S.st_value = bswap_32(sym[j].S.st_value + 32);}
        if (!strcmp(sct2[indice].nom ,".data")){
            sym2[j].S.st_value = bswap_32(sym[j].S.st_value + 10240);
        }
        if (!strcmp(sct2[indice].nom ,"bss")){
            sym2[j].S.st_value = bswap_32(sym[j].S.st_value + 0);
        }
        fseek(fs,sct2[i].sect.sh_offset+j*sizeof(Elf32_Sym),SEEK_SET);
        size=size+sym[j].S.st_size;
        fwrite(&sym2[j].S,sizeof(Elf32_Sym),1,fs);
    }
    
	free(sym);
}



int main(int argc, char const *argv[]) {
    
    FILE *fe = fopen(argv[1],"r");
    FILE *fs = fopen(argv[2],"w+");
    assert(fe != NULL);
    
    sct = malloc(sizeof(section_n) * header.e_shnum); 
    lectureHead(fe);
    fclose(fe);
    
    fe = fopen(argv[1],"r");
    
    lectureSection(fe);

    
    fe = fopen(argv[1],"r");
    renumSection(fe,fs);
    lectureSymbol(fe);
    recupSymbole(fe,fs);
    
    return 0;
}