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

void renameFichero(char *nombreActual, char *nuevoNombre);

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

	if (token != NULL) {
		if (strcmp(token, "info") == 0) {
			// Comando info
			printf("Información del superbloque:\n");
			printf("bloque %d Bytes\n", superbloque.s_block_size);
			printf("inodos particion = %u\n", superbloque.s_inodes_count);
			printf("inodos libres = %u\n", superbloque.s_free_inodes_count);
			printf("bloques particion = %u\n", superbloque.s_blocks_count);
			printf("bloques libres = %u\n", superbloque.s_free_blocks_count);
			printf("primer bloque de datos = %u\n", superbloque.s_first_data_block);
		} else if (strcmp(token, "bytemaps") == 0) {
			// Comando bytemaps
			printf("inodos:");
			for (int i = 0; i < superbloque.s_inodes_count; i++) {
				printf(" %hhu", bytemaps.bmap_inodos[i]);
			}
			printf("\n");

			printf("bloques [0-25]:");
			for (int i = 0; i < 26; i++) {
				printf(" %hhu", bytemaps.bmap_bloques[i]);
			}
			printf("\n");
		} else if (strcmp(token, "dir") == 0) {
            // Comando dir
            listarDirectorioCompleto();
        } else if (strcmp(token, "rename") == 0) {
			
            char *nombreActual = strtok(NULL, " ");
            char *nuevoNombre = strtok(NULL, "");  // Captura toda la línea después de "rename nombreActual "
           
		   if (nombreActual != NULL && nuevoNombre != NULL) {
                renameFichero(nombreActual, nuevoNombre);
            } else {
                printf("Error: El comando rename requiere un nombre de archivo actual y un nuevo nombre.\n");
            }
        }
}

void listarDirectorio(){
	printf("Contenido detallado del directorio:\n");

    for (int i = 0; i < MAX_BLOQUES_PARTICION; i++) {
        if (directorio[i].dir_inodo != 0xFFFF && directorio[i].dir_nfich[0] != '\0') {
            if (directorio[i].dir_inodo < MAX_INODOS) {  // Verificar límites de inodos
                printf("%-20s\ttamaño:%-15u\tinodo:%-5hu\tbloques:", directorio[i].dir_nfich,
                       inodos[directorio[i].dir_inodo].size_fichero, directorio[i].dir_inodo);

                // Mostrar bloques ocupados por el fichero
                for (int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++) {
                    if (inodos[directorio[i].dir_inodo].i_nbloque[j] != 0xFFFF) {
                        printf(" %hu", inodos[directorio[i].dir_inodo].i_nbloque[j]);
                    }
                }
                printf("\n");
            } 
        }
    }
}

void mostrarContenidoFichero(char *Fichero){
	
}

void renameFichero(char *nombreActual, char *nuevoNombre) {

}