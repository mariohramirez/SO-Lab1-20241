//Operaciones de entrada y salida
#include <stdio.h>
//Manejo de la memoria
#include <stdlib.h>
//Tipos de datos booleanos
#include <stdbool.h>
//Tipos de datos de sistema
#include <sys/types.h>

/*Funcion que se encargara de invertir el texto, toma un puntero a un archivo de entrada,
un puntero a un archivo de salida y un puntero a un booleano que indica si el archivo de salida
*/
void reverseText(FILE *inputFile, FILE *outputFile, bool *isFile){
    //Se define el valor maximo de una linea
    const MAX_LENGTH = 256;
    //Es un puntero doble que se usa para almacenar las lineas del archivo
    char **lines = NULL;
    //Puntero con el cual se va a leer cada linea del archivo
    char *buffer = NULL;
    //Contador para el numero de lineas leidas
    size_t line_count = 0;
    //Variable que almacena el resultado de la funcion getline
    ssize_t read;

    //Lee las lineas del archivo de entrada inputFile usando getline, cada linea se guarda en buffer
    while ((read = getline(&buffer, &read, inputFile)) != -1) {
        //Se realoca la memoria para almacenar la nueva linea
        lines = realloc(lines, (line_count + 1) * sizeof(char *));
        lines[line_count] = malloc(read);
        //Se copia la linea leida en el buffer a lines
        strcpy(lines[line_count], buffer);
        //Se incrementa el contador de lineas
        line_count++;
    }

    // Se imprimen las lineas en orden inverso
    for (size_t i = line_count; i > 0; i--) {
        //Si isFile es verdadero, se imprime en el archivo de salida
        if(isFile){
            fprintf(outputFile, "%s", lines[i - 1]);
            free(lines[i - 1]);
        //Si isFile es falso se imprime en consola
        } else {
            fprintf(stdout, "%s", lines[i - 1]);
        }
        
    }

    // Se libera la memoria alocada para lines y buffer
    free(lines);
    free(buffer);
}

int main(int argc, char *argv[]) {
    printf("Hola mundo\n");
    return 0;
}