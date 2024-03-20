// Operaciones de entrada y salida
#include <stdio.h>
// Manejo de la memoria
#include <stdlib.h>
// Tipos de datos booleanos
#include <stdbool.h>
// Tipos de datos de sistema
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/*Funcion que se encargara de invertir el texto, toma un puntero a un archivo de entrada,
un puntero a un archivo de salida y un puntero a un booleano que indica si el archivo de salida
*/
void reverseText(FILE *inputFile, FILE *outputFile, bool *isFile)
{
    // Es un puntero doble que se usa para almacenar las lineas del archivo
    char **lines = NULL;
    // Puntero con el cual se va a leer cada linea del archivo
    char *buffer = NULL;
    // Contador para el numero de lineas leidas
    size_t line_count = 0;
    // Variable que almacena el resultado de la funcion getline
    size_t read;

    // Lee las lineas del archivo de entrada inputFile usando getline, cada linea se guarda en buffer,
    // Devuelve en read el numero de caracteres leidos, si la funcion llega al final del archivo devuelve -1
    while ((read = getline(&buffer, &read, inputFile)) != -1)
    {
        // Se realoca la memoria para almacenar la nueva linea
        lines = realloc(lines, (line_count + 1) * sizeof(char *));
        // Se asigna memora para almacenar la nueva linea en lines
        lines[line_count] = malloc(read);
        // Se copia la linea leida en el buffer a lines
        strcpy(lines[line_count], buffer);
        // Se incrementa el contador de lineas
        line_count++;
    }

    // Se imprimen las lineas en orden inverso
    for (size_t i = line_count; i > 0; i--)
    {
        // Si isFile es verdadero, se imprime en el archivo de salida
        if (isFile)
        {
            fprintf(outputFile, "%s", lines[i - 1]);
            free(lines[i - 1]);
            // Si isFile es falso, se imprime en consola
        }
        else
        {
            fprintf(stdout, "%s\n", lines[i - 1]);
            free(lines[i - 1]);
        }
    }

    // Se libera la memoria alocada para lines y buffer
    free(lines);
    free(buffer);
}

// Función para verificar si dos archivos son hardlinked
int isHardlinked(const char *inputFile, const char *outputFile)
{
    //Declaracion de estructuras para almacenar la informacion de los archivos
    struct stat inputInfo, outputInfo;
    //La funcion stat obtiene la informacion de los archivos y devuelve -1 si hay fallo para cualquier archivo
    if (stat(inputFile, &inputInfo) != 0 || stat(outputFile, &outputInfo) != 0)
    {
        return -1;
    }
    //Se compara el numero de dispositivo y el numero de inodo de los archivos
    if (inputInfo.st_dev != outputInfo.st_dev || inputInfo.st_ino != outputInfo.st_ino)
    {
        return 0;
    }
        return 1;
}

// Funcion principal
int main(int argc, char *argv[])
{

    // Verifica que se haya ingresado el archivo de entrada y no haya demasiados argumentos
    if (argc < 2 || argc > 3)
    {
        // Imprime mensaje de error
        fprintf(stderr, "usage: reverse <input> <output>\n");
        // Devuelve 1 para indicar un error en los argumentos
        return 1;
    }

    // Verifica que se haya ingresado el archivo de entrada
    if (argc < 2)
    {
        // Imprime mensade error
        fprintf(stderr, "usage: reverse <input> <output>\n");
        // Salida no exitosa
        return 1;
    }
    // Abre el archivo de entrada especificado en el primer argumento, en modo de lectura
    FILE *inputFile = fopen(argv[1], "r");
    // Error abriendo el archvivo
    if (inputFile == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
        return 1;
    }

    // Verifica si se proporcionaron nombres diferentes para los archivos de entrada y salida
    if (argc >= 3 && strcmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return 1;
    }

    // Verificar si los archivos de entrada y salida son hardlinked
    if (argc >= 3 && isHardlinked(argv[1], argv[2])==1)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return 1;
    }

    // Abre el archivo de salida especificado en el segundo argumento
    FILE *outputFile = stdout;
    bool isFile = false;
    // Verifica si se ingreso proporciono un tercer arumento en la linea de comandos
    if (argc >= 3)
    {
        // Se abre el archivo de salida en modo escritura
        outputFile = fopen(argv[2], "w");
        // Si no se puede abrir el archivo de salida, se imprime un mensaje de error
        if (outputFile == NULL)
        {
            fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
            fclose(inputFile);
            return 1;
        }
        isFile = true;
    }

    // Se llama al metodo que hace la reversion del texto
    reverseText(inputFile, outputFile, &isFile);

    fclose(inputFile);
    // Si el archivo de salida es un archivo, se cierra
    if (isFile)
    {
        fclose(outputFile);
    }

    return 0;
}