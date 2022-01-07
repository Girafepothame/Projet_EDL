#ifndef __LECTURE_ELF_H__
#define __LECTURE_ELF_H__

#define EI_NIDENT 16

#include <stdint.h>


typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf32_Ehdr;


typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Shdr;


typedef struct {
    Elf32_Shdr sect;
    char *nom;
} section_n;


typedef struct {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
} Elf32_Sym;

typedef struct {
    Elf32_Sym S;
    char *nom;
    int taille;
} symbole_n;

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
} Elf32_Rel;
typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    uint32_t r_addend;
} Elf32_Rela;

#endif
