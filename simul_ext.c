#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int s_inodes_count;
    unsigned int s_blocks_count;
    unsigned int s_free_blocks_count;
    unsigned int s_free_inodes_count;
    unsigned int s_first_data_block;
    unsigned int s_block_size;
    unsigned char s_relleno[SIZE_BLOQUE - 6 * sizeof(unsigned int)];
} EXT_SIMPLE_SUPERBLOCK;

typedef struct {
    unsigned char bmap_bloques[MAX_BLOQUES_PARTICION];
    unsigned char bmap_inodos[MAX_INODOS];
    unsigned char bmap_relleno[SIZE_BLOQUE - (MAX_BLOQUES_PARTICION + MAX_INODOS) * sizeof(char)];
} EXT_BYTE_MAPS;

typedef struct {
    unsigned int size_fichero;
    unsigned short int i_nbloque[MAX_NUMS_BLOQUE_INODO];
} EXT_SIMPLE_INODE;

typedef struct {
    char dir_nfich[LEN_NFICH];
    unsigned short int dir_inodo;
} EXT_ENTRADA_DIR;

void cargarDatos();
void mostrarPrompt();
void procesarComando(char *comando);
void listarDirectorio();
void mostrarContenidoFichero(char *Fichero);

int main() {
	
	
	return 0;
}

void cargarDatos(){
	
}

void mostrarPrompt(){
	
}

void procesarComando(char *comando){
	
}

void listarDirectorio(){
	
}

void mostrarContenidoFichero(char *Fichero){
	
}