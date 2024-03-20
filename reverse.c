//Operaciones de entrada y salida
#include <stdio.h>
//Manejo de la memoria
#include <stdlib.h>
//Tipos de datos booleanos
#include <stdbool.h>
//Tipos de datos de sistema
#include <sys/types.h>
#include <string.h>

/*Funcion que se encargara de invertir el texto, toma un puntero a un archivo de entrada,
un puntero a un archivo de salida y un puntero a un booleano que indica si el archivo de salida
*/
void reverseText(FILE *inputFile, FILE *outputFile, bool *isFile){
    //Es un puntero doble que se usa para almacenar las lineas del archivo
    char **lines = NULL;
    //Puntero con el cual se va a leer cada linea del archivo
    char *buffer = NULL;
    //Contador para el numero de lineas leidas
    size_t line_count = 0;
    //Variable que almacena el resultado de la funcion getline
    size_t read;

    //Lee las lineas del archivo de entrada inputFile usando getline, cada linea se guarda en buffer,
    //Devuelve en read el numero de caracteres leidos, si la funcion llega al final del archivo devuelve -1
    while ((read = getline(&buffer, &read, inputFile)) != -1) {
        //Se realoca la memoria para almacenar la nueva linea
        lines = realloc(lines, (line_count + 1) * sizeof(char *));
        //Se asigna memora para almacenar la nueva linea en lines
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
            if (i > 3) fprintf(outputFile, "\n");
            free(lines[i - 1]);
        //Si isFile es falso se imprime en consola
        } else {
            fprintf(stdout, "%s", lines[i - 1]);
            if (i > 3) fprintf(outputFile, "\n");
        }
        
    }

    // Se libera la memoria alocada para lines y buffer
    free(lines);
    free(buffer);
}

//Funcion principal
int main(int argc, char *argv[]) {
    //Verifica que se haya ingresado el archivo de entrada
    if (argc < 2) {
        //Imprime mensade error
        fprintf(stderr, "Para usar escriba el comando: %s <archivo de entrada> [archivo de salida]\n", argv[0]);
        //Salida no exitosa
        return 1;
    }
    //Abre el archivo de entrada especificado en el primer argumento, en modo de lectura
    FILE *inputFile = fopen(argv[1], "r");
    //
    if (inputFile == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    //Abre el archivo de salida especificado en el segundo argumento
    FILE *outputFile = stdout;
    bool isFile = false;
    //Verifica si se ingreso proporciono un tercer arumento en la linea de comandos
    if (argc >= 3) {
        //Se abre el archivo de salida en modo escritura
        outputFile = fopen(argv[2], "w");
        //Si no se puede abrir el archivo de salida, se imprime un mensaje de error
        if (outputFile == NULL) {
            perror("Error abriendo el archivo de salida");
            fclose(inputFile);
            return 1;
        }
        isFile = true;
    }

    //Se llama al metodo que hace la reversion del texto
    reverseText(inputFile, outputFile, &isFile);

    fclose(inputFile);
    //Si el archivo de salida es un archivo, se cierra
    if (isFile) {
        fclose(outputFile);
    }

    return 0;
}