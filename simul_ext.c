#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARTICION_FILE "particion.bin"
#define NUM_INODOS_RESERVED 2
#define NUM_PRIMER_BLOQUE_DATOS 4
#define SIZE_ENTRADA_DIR sizeof(EXT_ENTRADA_DIR)

#define MAX_BLOQUES_PARTICION 100
#define MAX_INODOS 24
#define MAX_NUMS_BLOQUE_INODO 7
#define LEN_NFICH 32
#define SIZE_BLOQUE 512

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

EXT_SIMPLE_SUPERBLOCK superbloque;
EXT_BYTE_MAPS bytemaps;
EXT_SIMPLE_INODE inodos[MAX_INODOS];
EXT_ENTRADA_DIR directorio[MAX_BLOQUES_PARTICION];

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
        printf("Error al abrir el archivo particion.bin");
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

void procesarComando(char *comando) {
    char *token = strtok(comando, " ");

    if (token != NULL) {
        if (strcmp(token, "listar") == 0) {
            listarDirectorio();
        } else if (strcmp(token, "mostrar") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                mostrarContenidoFichero(token);
            } else {
                printf("Error: El comando mostrar requiere un nombre de archivo.\n");
            }
        } else if (strcmp(token, "info") == 0) {
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
				printf(" %d", bytemaps.bmap_inodos[i]);
			}
			printf("\n");

			printf("bloques [0-25]:");
			for (int i = 0; i < 26; i++) {
				printf(" %d", bytemaps.bmap_bloques[i]);
			}
			printf("\n");
		} else if (strcmp(token, "dir") == 0) {
            // Comando dir
            listarDirectorio();
        } else if (strcmp(token, "rename") == 0) {
            char *nombreActual = strtok(NULL, " ");
            char *nuevoNombre = strtok(NULL, "");  // Captura toda la línea después de "rename nombreActual "
            if (nombreActual != NULL && nuevoNombre != NULL) {
                renameFichero(nombreActual, nuevoNombre);
            } else {
                printf("Error: El comando rename requiere un nombre de archivo actual y un nuevo nombre.\n");
            }
        } else if (strcmp(token, "imprimir") == 0) {
            // Comando imprimir
            token = strtok(NULL, " ");
            if (token != NULL) {
                // Implementa la lógica para imprimir el fichero como texto
            } else {
                printf("Error: El comando imprimir requiere un nombre de archivo.\n");
            }
        } else if (strcmp(token, "remove") == 0) {
            // Comando remove
            token = strtok(NULL, " ");
            if (token != NULL) {
                // Implementa la lógica para eliminar el fichero
            } else {
                printf("Error: El comando remove requiere un nombre de archivo.\n");
            }
        } else if (strcmp(token, "copy") == 0) {
            // Comando copy
            char *ficheroOrigen = strtok(NULL, " ");
            char *ficheroDestino = strtok(NULL, " ");
            if (ficheroOrigen != NULL && ficheroDestino != NULL) {
                // Implementa la lógica para copiar el fichero
            } else {
                printf("Error: El comando copy requiere un nombre de archivo origen y un nombre de archivo destino.\n");
            }
        } else if (strcmp(token, "salir") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            printf("Comando no reconocido: %s\n", comando);
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
	int i, encontrado = 0;

    for (i = 0; i < MAX_BLOQUES_PARTICION; i++) {
        if (strcmp(directorio[i].dir_nfich, Fichero) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        printf("Contenido de %s:\n", Fichero);

        for (int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++) {
            if (inodos[i].i_nbloque[j] != 0xFFFF) {
                // Aquí puedes leer y mostrar el contenido del bloque de datos
                // utilizando la posición almacenada en i_nbloque[j]
                // Puedes usar fseek para ir al bloque correspondiente y leerlo

                FILE *particion = fopen(PARTICION_FILE, "rb");
                if (particion == NULL) {
                    perror("Error al abrir el archivo particion.bin");
                    exit(EXIT_FAILURE);
                }

                int offset = inodos[i].i_nbloque[j] * SIZE_BLOQUE;
                fseek(particion, offset, SEEK_SET);

                char buffer[SIZE_BLOQUE];
                fread(buffer, SIZE_BLOQUE, 1, particion);

                printf("%s", buffer);

                fclose(particion);
            }
        }
        printf("\n");
    } else {
        printf("El archivo %s no se encuentra en el directorio.\n", Fichero);
    }
}

void renameFichero(char *nombreActual, char *nuevoNombre) {
    int i, encontrado = 0;

    // Buscar el inodo del fichero actual
    for (i = 0; i < MAX_BLOQUES_PARTICION; i++) {
        if (strcmp(directorio[i].dir_nfich, nombreActual) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        // Verificar que el nuevo nombre no exista
        for (int j = 0; j < MAX_BLOQUES_PARTICION; j++) {
            if (strcmp(directorio[j].dir_nfich, nuevoNombre) == 0) {
                printf("Error: El nombre nuevo ya existe. Cambio de nombre cancelado.\n");
                return;
            }
        }

        // Cambiar el nombre del fichero
        strcpy(directorio[i].dir_nfich, nuevoNombre);
        printf("Nombre cambiado de %s a %s.\n", nombreActual, nuevoNombre);
    } else {
        printf("Error: El archivo %s no se encuentra en el directorio.\n", nombreActual);
    }
}