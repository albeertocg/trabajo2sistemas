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
	cargarDatos();

    while (1) {
        mostrarPrompt();

        char comando[50];
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando, "\n")] = 0;

        procesarComando(comando);
    }
	
	return 0;
}

void cargarDatos(){
	FILE *particion = fopen(PARTICION_FILE, "rb");

    if (particion == NULL) {
        perror("Error al abrir el archivo particion.bin");
        exit(EXIT_FAILURE);
    }
	
	// Leer superbloque
    fseek(particion, 0, SEEK_SET);
    fread(&superbloque, sizeof(EXT_SIMPLE_SUPERBLOCK), 1, particion);

    // Leer bytemaps
    fread(&bytemaps, sizeof(EXT_BYTE_MAPS), 1, particion);

    // Leer inodos
    fseek(particion, 3 * SIZE_BLOQUE, SEEK_SET);
    fread(&inodos, sizeof(EXT_SIMPLE_INODE), MAX_INODOS, particion);

    // Leer directorio
    fread(&directorio, SIZE_ENTRADA_DIR, MAX_BLOQUES_PARTICION, particion);
	
	    fclose(particion);
}

void mostrarPrompt(){
		printf(">> ");
}

void procesarComando(char *comando){
	    char *token = strtok(comando, " ");

}

void listarDirectorio(){
	
}

void mostrarContenidoFichero(char *Fichero){
	
}