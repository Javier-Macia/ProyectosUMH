/*
*
* LIBRARÍA HECHA POR JAVIER MACIÁ SEMPERE
* Librería con todas las funciones necesarias para hacer la pseudo-base de datos
*
*/


/*****************************************************************
 *                           LIBRERÍAS
*****************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <errno.h>

#include "lib.h"


/*****************************************************************
 *                      FUNCIONES ESTÉTICAS
*****************************************************************/
void welcomeMenu(){
    printf("-----------------------------------\n");
    printf(" \tPSEUDO BASE DE DATOS\n");
    printf("-----------------------------------\n");
    printf(" > Javier Macia Sempere\n");
    printf(" > javier.macia05@goumh.umh.es\n");
    printf("-----------------------------------\n");
    printf(" - Ejercicio 1: HECHO\n");
    printf(" - Ejercicio 2: HECHO\n");
    printf(" - Ejercicio 3: HECHO\n");
    printf(" - Ejercicio 4: HECHO\n");

}

void helpMenu(){
    printf("---------------------------------------------------------------------------------------\n");
    printf(" \t\t\t\tCOMANDOS DEL PROGRAMA\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf(" > help \n\t | Muestra esta pantalla. \n\n");
    printf(" > exit \n\t | Sale del programa. \n\n");
    printf(" > new db <nombre_base_de_datos> \n\t | Crea una nueva base de datos, con su respectiva carpeta con el mismo nombre. \n\n");
    printf(" > use db <nombre_base_de_datos> \n\t | Cambia la base de datos activa, de este modo, todas las cosas que se hagan,\n\t | se haran desde esa base de datos. \n\n");
    printf(" > new table <nombre_tabla> <campo> <tipo> [<campo> <tipo> ...] \n\t | Crea una nueva tabla en la base de datos actual. Creara un nuevo fichero \n\t | vacio, y se modifica el fichero metadatos. Los campos tienen que ser nombres\n\t | validos y no se pueden repetir. Los tipos pueden ser NUM, DATE o TEXT. \n\n");
    printf(" > use table <nombre_tabla> \n\t | Cambia la tabla activa. Se le indica al programa que a partir de ahora, \n\t | todas las operaciones que afecten a tablas, se realizaran sobre la tabla\n\t | indicada (mientras no se cambie). \n\n");
    printf(" > insert <valor> [<valor> ...] \n\t | Se inserta en la tabla activa el registro formado por los valores \n\t | indicados. El numero de valores debe coincidir con los campos de la tabla\n\t | activa. Los valores deben ser del mismo tipo que los campos\n\t | correspondientes de la tabla activa. En el caso de valores de tipo texto,\n\t | si la cadena tiene mas de una palabra, debera ponerse entre comillas\n\t | dobles. Si se dan todas las condiciones, se anyadira el registro \n\t | correspondiente al final del fichero de la tabla activa y a la lista\n\t | de registros o filas (ROWS).\n\n");
    printf(" > select [campo x valor] [ord] \n\t | Si se introduce solamente 'select', se mostraran los campos \n\t | de la tabla y sus filas. Si se indica un campo, un operador y un valor \n\t | se mostraran los que cumplen la condicion, y 'ord' los ordenara \n\t | de manera ascendente (indicando asc) o descendente (indicando des) \n\n");
    printf(" > list \n\t | Muestra todas las bases de datos disponibles \n\n");
    printf(" > delete [campo x valor] \n\t | Borrara las filas que cumplan la condicion especificada. \n\t | 'Campo' y 'Valor' son del mismo tipo y x sera '<', '>' o '='  \n\n");
    
    printf("---------------------------------------------------------------------------------------\n");
}

void prError( int errorID , char* mainStatement , char* optionalStatement ){
    const char *helpMessage = "Escribe 'help' para mas informacion.";
    switch (errorID){
    case ERR_UNDEF: //Error genérico
        printf("ERROR: Ha ocurrido un error indeterminado\n");
        break;
    case ERR_COMMAND_INVALID: // El comando no es válido
        printf("ERROR: El comando '%s' no es un comando valido. %s \n",mainStatement, helpMessage);
        break;
    case ERR_ARGUMENTS_NOTENOUGH: // No hay argumentos suficientes
        printf("ERROR: El comando '%s' necesita mas argumentos. %s\n",mainStatement, helpMessage);
        break;
    case ERR_ARGUMENTS_INVALID: // Numero invalido de argumentos
        printf("ERROR: Ha introducido un numero invalido de argumentos en el comando '%s'. %s \n",mainStatement, helpMessage);
        break;
    case ERR_ARGUMENTS_TOOMANY: // Demasiados argumentos
        printf("ERROR: El comando '%s' tiene demasiados argumentos. %s\n",mainStatement, helpMessage);
        break;
    case ERR_ARGUMENTS_EMPTY:   //No se introducen argumentos al comando
        printf("ERROR: No se ha introducido ningun comando. Por favor, introduce uno.\n");
        break;
    case ERR_DIR_ALREADYEXISTS: //Ya existe un directorio con el mismo nombre
        printf("ERROR: Ya existe un directorio con el mismo nombre. Por favor, indica otro.\n");
        break;
    case ERR_DIR_DOESNOTEXIST:
        printf("ERROR: El directorio indicado no existe. Por favor, indica otro.\n");
        break;
    case ERR_FILE_ALREADYEXISTS: //Ya existe un directorio con el mismo nombre
        printf("ERROR: Ya existe un archivo con el mismo nombre. Por favor, indica otro.\n");
        break;
    case ERR_FILE_DOESNOTEXIST:
        printf("ERROR: El archivo indicado no existe. Por favor, indica otro.\n");
        break;
    case ERR_FILE_CANNOTREMOVE:
        printf("ERROR: No se ha podido borrar el archivo '%s'.\n",mainStatement);
        break;
    case ERR_TAB_UNASIGNED:
        printf("ERROR: No se ha asignado ninguna tabla a la que efectuar este comando. \n");
        break;
    case ERR_DB_CANNOTREMOVE:
        printf("ERROR: No se ha podido borrar la base de datos '%s'.\n",mainStatement);
        break;
    case ERR_DB_UNASIGNED:
        printf("ERROR: No se ha asignado ninguna base de datos a la tabla '%s'\n",mainStatement);
        break;
    case ERR_DB_HASFILES:
        printf("ERROR: La base de datos '%s' contiene archivos y por eso no se ha borrado. \n", mainStatement);
        break;
    case ERR_FIELD_REPEATED:
        printf("ERROR: Has repetido el nombre del campo '%s' \n",mainStatement);
        break;
    case ERR_FIELD_NUMBER_INVALID:
        printf("ERROR: Has introducido un numero de campos incorrecto. \n");
        break;
    case ERR_FIELD_DOESNOTEXIST:
        printf("ERROR: El campo introducido no existe.\n ");
        break;
    case ERR_TYPE_INVALID:
        printf("ERROR: Has indicado un tipo de campo invalido. \n");
        break;
    case ERR_STR_NOTATEXT:
        printf("ERROR: '%s' no es un texto valido.\n",mainStatement);
        break;
    case ERR_STR_NOTANUMBER:
        printf("ERROR: '%s' no es un numero valido.\n",mainStatement);
        break;
    case ERR_STR_NOTADATE:
        printf("ERROR: '%s' no es una fecha valida.\n",mainStatement);
        break;
    case ERR_OPERATOR_INVALID:
        printf("ERROR: Has introducido un operador invalido.\n",mainStatement);
        break;
    case ERR_VALUE_INVALID:
        printf("ERROR: Has introducido un valor invalido.\n",mainStatement);
        break;
    default: // Error inesperado.
        printf("ERROR: Ha habido un error inesperado. \n");
        break;
    }
}

void showPrompt(char *dbName , char *tableName ){
    if (dbName == NULL ){
        printf("\n[*]:> ");
    }else if (tableName == NULL){
        printf("\n[%s]:> ",dbName);
    }else{
        printf("\n[%s/%s]:> ", dbName, tableName);
    }
}

void showFiles(char* directory){
    DIR* d;
    struct dirent *dir;
    d = opendir(directory);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".") == 0){
                //No se imprime
            }else if(strcmp(dir->d_name,"..") == 0){
                //No se imprime
            }
            else if (strcmp(dir->d_name,"meta.db") == 0){
                //No se imprime
            }
            else{
                printf("   - %s\n", dir->d_name);
            }
        }
    }
    closedir(d);
}

void showDBs(){
    DIR* d;
    struct dirent *dir;
    int dbNum;
    dbNum = getDBsNumber();

    d = opendir(".");
    if (d) {
        if (dbNum == 0){
            printf("No hay bases de datos.\n");
        }else{
            printf("Hay %d bases de datos.\n", dbNum);
        }
        
        
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".") == 0){
                //No se imprime
            }
            else if (strcmp(dir->d_name,"..") == 0){
                //No se imprime
            }
            else if (strcmp(dir->d_name,".vscode") == 0){
                //No se imprime
            }
            else{
                if (dir->d_type ==  DT_DIR){
                    printf("   - %s\n", dir->d_name);
                }
            }
        }
    }
    closedir(d);
}

void showTableFields(char** tableArr){
    int i = 2;
    int j = 3;
    printf("--------------------\n");
    printf("Nombre de la tabla: %s \n", tableArr[0]);
    printf("Numero de campos: %s\n", tableArr[1]);
    printf("--------------------\n");
    while (tableArr[j] != NULL){
        printf(" %s: %s\n", tableArr[i], tableArr[j]);
        i+=2;
        j+=2;
    }
}

void showTableContent(char* path){
    int i = 0;
    int lineCount = 1;
    FILE* f;
    char* textArr[3000];
    cleanArray(textArr);
    f = fopen(path, "r");
    saveTextInArray(textArr,f);
    rewind(f);
    fclose(f);

    printf("--------------------\n");
    printf("Contenido de la tabla: \n");
    printf("--------------------\n");
    while (textArr[i]){
        printf("%d, %s",lineCount, textArr[i]);
        i++;
        lineCount++;
    }
}

void showSortedTable(char** tableArr, char* mode){
    char aux[256];
    char* line[256];
    int limit;
    int i = 0;

    limit = getArraySize(tableArr);
    int sortList[limit];    

    printf("--------------------\n");
    printf("Contenido de la tabla: \n");
    printf("--------------------\n");

    if (tableArr[0]){
        strcpy(aux,tableArr[0]);
        tokenize(aux, line, " ,\n");
        strcpy(aux,line[1]);
        if(isADate(aux)){
            sortDates(tableArr,limit,sortList,1);
        }
        else if(validNumber(aux)){
            sortNumbers(tableArr,limit,sortList,1);
        }
        else {
            sortWords(tableArr,limit,sortList,1);
        }
    }

    toLowercase(mode);
    if (strcmp(mode,ASC_CMD) == 0){
        while (tableArr[i]){
            printf("%s",tableArr[sortList[i]]);
            i++;
        }
    }else if(mode, DES_CMD){
        i = limit-1;
        while (tableArr[i]){
            printf("%s",tableArr[sortList[i]]);
            i--;
        }
    }
}

void showArrayContent(char** arr){
    int i = 0;
    while (arr[i]){
        printf("%s", arr[i]);
        i++;
    }
}

/*****************************************************************
 *                      FUNCIONES AUXILIARES
*****************************************************************/

int getArraySize(char **wordsArray){
    int i = 0;
    while (wordsArray[i]!=NULL){
        i++;
    }
    return i; 
}

bool isPair(int number){
    return (number%2) == 0;
}

bool isOdd(int number){
    return !isPair(number);
}

char** cleanArray(char** array){
    int i = 0;
    while (array[i]){
        array[i] = NULL;
        i++;
    }
    return array;
}

// Código original de "itoa". Lo he añadido a mi trabajo ya que no se me estaba reconociendo la librería.
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

char** getTableContent(char** array, char* path){
    FILE *f;

    f = fopen(path,"r");
    saveTextInArray(array,f);
    fclose(f);

    return array;
}

/**
 * @brief Recoge los campos y los tipos del meta de una tabla determinada.
 * 
 * @param array Matriz donde se guardarán los campos
 * @param dbName Nombre de la base de datos donde se buscarán los campos de la tabla
 * @param tableName Nombre de la tabla que se buscará en el meta
 * @return char** 
 */
char** getTableFields(char** array, char* dbName, char* tableName){
    FILE *f;
    int i = 0;
    int j = 0;
    int fieldCount = 0;
    char path[200];
    char* line[256];
    char* textArr[3000];
    char* fields[400];
    bool found = false;

    strcpy(path,dbName);
    strcat(path,"\x5cmeta.db");


    f = fopen(path,"r");
    rewind(f);
    cleanArray(textArr);  // Limpiamos el array original
    saveTextInArray(textArr,f); // Nos guardamos el texto
    fclose(f);

    while (textArr[i] ){ // Lee el texto por líneas
        tokenize(textArr[i],line,": \n");
        j = 0;
        while (line[j]){    // Busca la tabla en esa línea
            if (strcmp(line[j], tableName) == 0){ 
                found = true; // Hemos encontrado la tabla
            }
            if (found){
                if (strcmp(line[j],"--------------------") == 0){ 
                    return array;
                    
                }else{ //Nos guardamos los campos y los tipos
                    array[fieldCount] = (char*)malloc(sizeof(char) * (strlen(line[j]) ) );
                    array[fieldCount] = line[j];
                    fieldCount++;
                }
            }
            j++;
        }
        i++;
    }
    return array;
}

bool areValidTypes(char** commandArray,char** fieldTypeArray, int position){
    int i = position;
    int j = 3;  // j = 0 es el nombre de la tabla, j = 1 n de campos, j = 2  nombre del campo
    char word[100];
    char type[50];
    while (commandArray[i]){
        strcpy(word,commandArray[i]);
        strcpy(type,fieldTypeArray[j]);
        toLowercase(type);
        if (strcmp(type,TYPE_NUM) == 0){
            if (!validNumber(word)){
                prError(ERR_STR_NOTANUMBER,commandArray[i],NULL);
                return false;
            }
        }
        
        else if (strcmp(type,TYPE_TEXT) == 0){
            if (!isAText(word)){
                prError(ERR_STR_NOTATEXT,commandArray[i],NULL);
                return false;
            }
        }
        
        else if (strcmp(type,TYPE_DATE) == 0){
            if (!isADate(word)){
                prError(ERR_STR_NOTADATE,commandArray[i],NULL);
                return false;
            }
        }

        else if (strcmp(type,TYPE_EMAIL) == 0){
            if (!isAnEmail(word)){
                prError(ERR_STR_NOTANEMAIL,commandArray[i],NULL);
                return false;
            }
        }

        else{
            prError(ERR_UNDEF,NULL,NULL);
            return false;
        }
        j+=2;
        i++;
    }

    return true;
}

/**
 * @brief Función que intercambia dos numeros
 * 
 * @param x 
 * @param y 
 */
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void swapFloats(float *x, float *y){
    float temp = *x;
    *x = *y;
    *y = temp;
}

void swapStrs(char** x, char** y){
    char* temp = *x;
    *x = *y;
    *y = temp;
}
/**
 * @brief Función que convierte una fecha en un número equivalente a ella.
 * 
 * @param date 
 * @return int 
 */
int dateToEquivalentInt(const char* date){
    char *p, str[15] ;
    long int num = 0 ;
    strcpy ( str, date ) ;
    int d = 0;
    int m = 0;
    int y = 0;
    p = strtok ( str, "/" ) ;
    if ( p != NULL ){
        d = num = atoi(p);
    }
    p = strtok(NULL, "/");
    if ( p != NULL ){
        m = atoi(p) ;
    }
    num += m * 32 ;
    p = strtok(NULL, "/");
    if ( p != NULL ){
        y = atol(p) ;
    }
    num += y * 512L ;

    return num ;
}

int getDBsNumber(){
    int i = 0;
    DIR* d;
    struct dirent *dir;
    d = opendir(".");
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name,".") == 0){
            //No se imprime
        }
        else if (strcmp(dir->d_name,"..") == 0){
            //No se imprime
        }
        else if (strcmp(dir->d_name,".vscode") == 0){
            //No se imprime
        }
        else{
            if (dir->d_type ==  DT_DIR){
                i++;
            }
        }
    }
    closedir(d);
    return i;
}

/*****************************************************************
 *                        ORDENAR ARRAYS 
*****************************************************************/

/**
 * @brief Algoritmo que ordena una lista de numeros usando el algoritmo de bubblesort
 * 
 * @param arr array de numeros a ordenar
 * @param n 
 */
void bubbleSort(int arr[], int n){
    int i, j;
    bool swapped;
    for (i = 0; i < n-1; i++){
        swapped = false;
        for (j = 0; j < n-i-1; j++){
            if (arr[j] > arr[j+1]){
                swap(&arr[j], &arr[j+1]);
                swapped = true;
            }
        }
        if (swapped == false){
            break;
        }
    }
}

/**
 * @brief Algoritmo que ordenará un array de palabras.
 * 
 * @param wordsArr Array de palabras a ordenar.
 * @param limit Por lo general, será el tamaño del array.
 * @param order Array de numeros que especifican el orden.
 * @param start A partir de dónde se comenzará a ordenar.
 */
void sortWords(char *wordsArr[],const int limit, int order[], int start){
    int i,j;
    bool finished = false;
    int aux;

    for (i = 0; i < limit; i++){
        order[i] = i;
    }
    while(!finished){
        finished = true;
        for(i = start; i < limit - 1; i++){
            j = start;
            while(wordsArr[order[i]][j] == wordsArr[order[i+1]][j]){
                j++;
            }
            if(wordsArr[order[i]][j] > wordsArr[order[i+1]][j]){
                aux = order[i];
                order[i] = order[i+1];
                order[i+1] = aux;
                finished = false;
            }
        }
    }
}

/**
 * @brief Algoritmo que ordenará un array de numeros.
 * 
 * @param wordsArr Array de numeros(en string) a ordenar.
 * @param limit Por lo general, será el tamaño del array.
 * @param order Array de numeros que especifican el orden.
 * @param start A partir de dónde se comenzará a ordenar.
 */
void sortNumbers(char *wordsArr[],const int limit, int order[], int start){
    int i,j;
    bool swapped;
    float numArray[getArraySize(wordsArr)+2];
    char line[256];
    char aux[256];
    char* tokenArr[256];
    
    for (i = 0; i < limit; i++){
        order[i] = i;
    }
    i = 0;
    while(wordsArr[i]){
        strcpy(line,wordsArr[i]);   // Nos copiamos la linea
        tokenize(line,tokenArr," ,\n"); // La tokenizamos
        strcpy(aux,tokenArr[1]);    // Nos copiamos el primer token
        numArray[i] = atof(aux);    // Lo añadimos al array de numeros
        i++;
    }
    
    for (i = 0; i < limit-1; i++){
        swapped = false;
        for (j = 0; j < limit-i-1; j++){
            if (numArray[j] > numArray[j+1]){
                swapFloats(&numArray[j], &numArray[j+1]);
                swap(&order[j], &order[j+1]);
                swapped = true;
            }
        }
        if (swapped == false){
            break;
        }
    }
}


void sortDates(char *wordsArr[], int limit, int order[], int start){
    int i = 0;
    int j = 0;
    int d,m,y;  // Dia, mes, año
    int nD,nM,nY;   // Siguiente dia, mes y año
    bool swapped = false;
    char line[256];
    char aux[256];
    char* tokenArr[256];
    float numArray[3000];

    char* date[40];
    char* dateNext[40];
    char dateCopy[30];
    char dateNextCopy[30];

    for (i = 0; i < limit; i++){
        order[i] = i;
    }

    for (i = 0; i < limit; i++){
        strcpy(line,wordsArr[i]);
        tokenize(line,tokenArr," ,\n"); // Tokenizamos en lineas
        strcpy(aux, tokenArr[start]);
        numArray[i] = dateToEquivalentInt(aux);
    }

    for (i = 0; i < limit-1; i++){
        swapped = false;
        for (j = 0; j < limit-i-1; j++){
            if (numArray[j] > numArray[j+1]){
                swapFloats(&numArray[j], &numArray[j+1]);
                swap(&order[j], &order[j+1]);
                swapped = true;
            }
        }
        if (swapped == false){
            break;
        }
    }
}

/*****************************************************************
 *           VALIDACIÓN DE NUMEROS, FECHAS, TEXTO E EMAIL
*****************************************************************/

bool validNumber(const char *str){
    int i = 0;
    bool decimal = false;
    bool negative = false;
    while (str[i]){
        if (str[i]<'0' || str[i]>'9') {
            if(str[i] == '.' && !decimal && i > 0) {   // Detecta si es decimal. Si tiene más de un punto, no es un número válido.
                decimal = true;
            } 
            else if(str[i] == '-' && !negative && i == 0) { //Detecta si es negativo. Si tiene más de un "-", no es un número válido.
                negative = true;
            }else{
              return false;  
            }  
        }
    i++;
    }
    return true;

}

/**
 * @brief Función que determina si, dados un día, mes y año, es una fecha válida. Tiene en cuenta años bisiestos.
 * @param d Día
 * @param m Mes
 * @param a Año
 * 
 * @return true si es una fecha válida
 * */
bool validDate(int d, int m, int a){
    int max; 
    if ( m >= 1 && m <= 12 ){
        switch (m){
            case 2: 
                if (a % 4 == 0 && a % 100 != 0 || a % 400 == 0){
                    max = 29;
                }else{
                    max = 28;
                }
                break;
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                max = 31;
                break;
            case 4: case 6: case 9: case 11:
                max = 30;
                break;
        }
        if ( d >= 1 && d <= max ){
            return true;
        }
    }
    return false;
}

/**
 * @brief Función que dice si una cadena de texto es una fecha o no.
 * @param word Cadena de texto
 * 
 * @return true si es una fecha válida
 * */
bool isADate(const char *word){
    int i = 0;
    int j, k;
    int d,m,a;
    char *dateArr[30];
    char copyWord[90];
    strcpy(copyWord,word);

    if ((word[2] == '/' && word[5] == '/') ){
        //En este punto, consideramos " xx/yy/zzzzzz " como una posible fecha.
        tokenize(copyWord,dateArr,"/");
        for(j=0; j<3; j++){ //Comprobamos que cada uno de los elementos de la fecha sea un número
            for (k = 0; k < strlen(dateArr[j]); k++){
                if(dateArr[j][k]<'0' || dateArr[j][k]>'9'){
                    return false;
                }
            }
        }
        d = atoi(dateArr[0]);
        m = atoi(dateArr[1]);
        a = atoi(dateArr[2]);
        
        if (validDate(d,m,a) == 1){ //Comprobamos que, realmente, sea una fecha.
            return true;
        }
    }
    return false;    
}

bool isAText(const char* word){
    char wordCpy[256];
    strcpy(wordCpy,word);
    if(validNumber(wordCpy)){
        return false;
    }
    if (isADate(wordCpy)){
        return false;
    }
    if (isAnEmail(wordCpy)){
        return false;
    }
    
    
    return true;
}

bool isAlfaNumeric(const char* word){
    int c;
    int i = 0;
    while (word[i]){
        c = (int)word[i];
        if (isalnum(c) == 0){
            return false;
        }
        i++;
    }
    return true;
    
}

bool isAnEmail(const char* word){
    char* emailArr[30];
    char* domain[20];
    char wordCpy[256];

    strcpy(wordCpy,word);
    tokenize(wordCpy,emailArr,"@\0");
    tokenize(emailArr[1], domain," .\0");

    if (getArraySize(emailArr) == 2){   //hola@mundo.com
        if (!isAlfaNumeric(emailArr[0]) || (emailArr[0] == NULL) ){ // hola
            return false;
        }
        if (!isAlfaNumeric(domain[0]) || (domain[0] == NULL) ){ // mundo
            return false;
        }
        if (!isAlfaNumeric(domain[1]) || (domain[1] == NULL) ){ // com
            return false;
        }
        return true;
    }
    if (getArraySize(domain) != 2){
        return false;
    }
    
    return false;
}

/*****************************************************************
 *                      FUNCIONES DE CARPETAS
*****************************************************************/
int getNumberOfFiles(char* directory){
    int i = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    if (d) {
        while ((dir = readdir(d)) != NULL) {    
            i++;
        }
        closedir(d);
    }
    return i-2; // El -2 se debe a que siempre habrán dos ficheros ocultos: el '.' y el '..' (la carpeta actual y la anterior respectivamente)
}

bool existsDirectory(char *directory){
    DIR *dir = opendir(directory);
    if (dir) {
        closedir(dir);
        return true;
    } else if (ENOENT == errno) {
        return false;
    }
    return ERROR;
}

void createNewDirectory(char *directory){
    if ( existsDirectory(directory) ){
        prError(ERR_DIR_ALREADYEXISTS,NULL,NULL);
    }else{
        mkdir(directory);
    }
}

void deleteDirectory(char *directory){
    struct stat st = {0};
    bool confirmation = false;
    char* input = NULL;
    DIR* d;
    struct dirent *dir;
    char aux[200];

    printf("\nATENCION!\n");
    printf("Esta accion eliminara PERMANENTEMENTE el directorio '%s'. Quiere hacer eso? (Si/No): ", directory);
    input = getUserInput();
    toLowercase(input);

    if (strcmp(input,"si") == 0){
        if ( existsDirectory(directory) ){
            if (getNumberOfFiles(directory) != 0){
                printf("Dentro del directorio estan los siguientes archivos:\n");
                showFiles(directory);
                printf("Desea eliminarlos? (Si/No)\n");
                input = getUserInput();
                toLowercase(input);
                if (strcmp(input,"si") == 0){
                    printf("Se eliminara el directorio '%s' con todos sus archivos.\n",directory);
                    d = opendir(directory);
                    if (d) {
                        while ((dir = readdir(d)) != NULL) {
                            if (strcmp(dir->d_name,".") == 0){
                                //Nada
                            }else if(strcmp(dir->d_name,"..") == 0){
                                //Nada
                            }else{
                                strcpy(aux,directory);
                                strcat(aux,"\x5c");
                                strcat(aux,dir->d_name);
                                remove(aux);
                            }
                        }
                    }
                    closedir(d);
                }else if(strcmp(input,"no") == 0){
                    printf("No se eliminara.\n");
                }
            }
            if (rmdir(directory) == -1){
                prError(ERR_DB_CANNOTREMOVE,directory,NULL);
            }
        }else{
            prError(ERR_DIR_DOESNOTEXIST,NULL,NULL);
        }
    }
    else if (strcmp(input,"no") == 0){
        printf("El directorio %s NO se borrara\n",directory);
    }
    else{
        printf("No hemos entendido tu orden. Por precaucion, NO se borrara el fichero '%s'\n",directory);
    }
}

/*****************************************************************
 *                      FUNCIONES DE FICHEROS
*****************************************************************/
int getFieldPosition(char** textArr, char* wantedField){
    int i = 2;
    while (textArr[i]){
        if (strcmp(textArr[i],wantedField) == 0){
            break;
        }
        i+=2;
    }
    return (i-2)/2;
}

void rewrite(char** tableArr, char* path){
    int i = 0;
    FILE* f;
    char* line[256];
    f = fopen(path,"w");
    while (tableArr[i]){
        fputs(tableArr[i],f);
        i++;
    }
    fclose(f);    
}

void deleteSelectedRow(char** tableArr, int fieldPosition, char* mode, const char* value){
    int i = 0;
    int auxCounter = 0;
    int pos = fieldPosition;
    int size = getArraySize(tableArr) + 20;
    char* auxTableArr[size];
    memcpy(auxTableArr,tableArr, (sizeof(char**) * size ));
    cleanArray(tableArr);
    char line[256];
    char* token[256];

    char* auxStr;
    int auxInt;
    long auxDate;

    
    if (strcmp(mode,">") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt > atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate > dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos])>0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos])>0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }

    else if (strcmp(mode,"<") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt < atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate < dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos]) < 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos])<0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }
    
    else if (strcmp(mode,"=") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt != atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate != dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos]) != 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos]) != 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }
}

void selectTableValues(char** tableArr, int fieldPosition, char* mode, const char* value){
    int i = 0;
    int auxCounter = 0;
    int pos = fieldPosition;
    int size = getArraySize(tableArr) + 20;
    char* auxTableArr[size];
    memcpy(auxTableArr,tableArr, (sizeof(char**) * size ));
    cleanArray(tableArr);
    char line[256];
    char* token[256];

    char* auxStr;
    int auxInt;
    long auxDate;

    
    if (strcmp(mode,">") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt < atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate < dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos])<0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos])<0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }

    else if (strcmp(mode,"<") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt > atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate > dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos]) > 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos])>0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }
    
    else if (strcmp(mode,"=") == 0){
        while (auxTableArr[i]){
            strcpy(line,auxTableArr[i]);
            tokenize(line,token," ,\n\"");
            if (validNumber(value)){
                auxInt = atoi(value);
                if (auxInt == atoi(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }           
            }
            if (isADate(value)){
                auxDate = dateToEquivalentInt(value);
                if (auxDate == dateToEquivalentInt(token[pos])){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            if (isAText(value)){
                if (strcmp(value,token[pos]) == 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }

            if (isAnEmail(value)){
                if (strcmp(value,token[pos]) == 0){
                    tableArr[auxCounter] = (char *) malloc(sizeof(char) * (strlen(value) + 8));
                    strcpy(tableArr[auxCounter], auxTableArr[i]);
                    auxCounter++;
                }  
            }
            i++;
        }
    }
        
}

void insertRowInTable(char* path, char** commandArray, int position){
    FILE *f;
    int nLines;
    int i = position;
    int max = getArraySize(commandArray)-position;
    char aux[30];

    f = fopen(path,"r");
    nLines = getNumberOfLinesInFile(f) + 1;
    itoa(nLines,aux,10);
    fclose(f);

    f = fopen(path,"a");
    //fputs(aux,f);
    //fputs(", ",f);
    while (commandArray[i]){
        if (isAText(commandArray[i])){
            fputs("\"",f);
            fputs(commandArray[i],f);
            fputs("\"",f);
        }else{
            fputs(commandArray[i],f);
        }
        
        if (i++ < max){
            fputs(", ",f);
        }
    }
    fputs("\n",f);

    fclose(f);

}

int getNumberOfLinesInFile(FILE* f){
    int lineCount = 0;
    char str[256];

    while (fgets(str, sizeof(str), f)) {
        lineCount++;
    }
    rewind(f);

    return lineCount;
}

/**
 * @brief Funcion que inicializa un archivo meta. Le pondrá que hay
 * 0 tablas y una linea.
 * @param path ruta del archivo
*/
void setMetaFile(char* path){
    TABLE tab;
    FILE *f;
    if(existsFile(path)){
        f = fopen(path,"w");
        fprintf(f,"TABLAS: 0\n");
        fprintf(f,"--------------------\n");
        fclose(f);
    }
}

char** saveTextInArray(char** textArr, FILE *f){
    int lineCount = 0;
    char str[256];

    while (fgets(str, sizeof(str), f)) {
        textArr[lineCount] = (char *) malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(textArr[lineCount], str);
        ++lineCount;
    }
    rewind(f);
    
    return textArr;
}

void insertTableInMeta(char* dbName, char** commandArray, int position){
    FILE *f;
    char* textArr[3000];
    char metaDir[strlen(dbName)+10];
    char* tokenizedLine[256];
    int nCommands = ((getArraySize(commandArray))-position) / 2;
    char strNCommands[40];
    int num;
    int i;

    strcpy(metaDir,dbName);
    strcat(metaDir,"\x5cmeta.db");

    f = fopen(metaDir,"r");
    saveTextInArray(textArr,f);
    fclose(f);

    // TABLAS: NUM
    num = getNumberOfFiles(dbName) - 1;
    tokenize(textArr[0],tokenizedLine," ");
    itoa(num, tokenizedLine[1], 10);
    
    f = fopen(metaDir,"w");
    fputs(tokenizedLine[0],f);
    fputs(" ",f);
    fputs(tokenizedLine[1],f);
    fputs("\n",f);
    // TABLAS: NUM

    // Pegamos lo que teníamos antes
    i = 1;
    while (textArr[i]){
        fputs(textArr[i],f);
        i++;
    }
    fclose(f);
    // Pegamos lo que teníamos antes

    // Añadimos la nueva tabla
    f = fopen(metaDir,"a");
    // Nombre tabla y nº de campos
    fputs(commandArray[position-1], f);
    fputs(": ",f);
    itoa(nCommands, tokenizedLine[0], 10);
    fputs(tokenizedLine[0],f);
    fputs("\n",f);
    // Nombre tabla y nº de campos

    i = position;
    while (commandArray[i]){
        if ( (isPair(i) == isPair(position)) || (isOdd(i) == isOdd(position)) ){
            fputs(commandArray[i],f); 
            fputs(": ",f);
        }else{
            toUppercase(commandArray[i]);
            fputs(commandArray[i],f);
            fputs("\n",f);
        }

        i++;        
    }
    fputs("--------------------\n",f);
    fclose(f);
    // Añadimos la nueva tabla
    
    cleanArray(textArr);
    free(textArr);
}

bool existsFile(char *fileName){
    struct stat buffer;
    int exist = stat(fileName,&buffer);
    if(exist == 0)
        return true;
    else  
        return false;
}

bool existsTableFile(char* dbName, char* tabName){
    bool answer = false;
    char* aux = (char*)malloc(sizeof (char) * 100);
    strcpy(aux,dbName);
    char* file = strcat(aux,"\x5c");
    file = strcat(file,tabName);
    file = strcat(file,".tab");

    if(existsFile(file)){
        answer = true;
    }
    
    free(aux);
    return answer;
}

void createFile(char* fileName){
    FILE *f;
    if (!existsFile(fileName)){
        f = fopen(fileName, "w");
        fclose(f);
    }else{
        prError(ERR_FILE_ALREADYEXISTS,NULL,NULL);
    }
}

void createMetaFile(char* dbName){
    char* metaFile = strcat(dbName,"\x5cmeta.db");
    createFile(metaFile);
    setMetaFile(metaFile);
}

void createTableFile(char* dbName, char* tabName){
    char* aux = (char*)malloc(sizeof (char) * 100);
    strcpy(aux,dbName);
    char* dir = strcat(aux,"\x5c");
    dir = strcat(dir,"\x5c");
    char* file = strcat(dir,tabName);
    file = strcat(file,".tab");
    createFile(file);
    free(aux);
}

void removeTableFile(char* dbName, char* tableName){
    char path[200];
    strcpy(path,"");
    strcpy(path,dbName);
    strcat(path,"\x5c");
    strcat(path,tableName);
    strcat(path,".tab");
    remove(path);
}

/*****************************************************************
 *                      LECTURA DE DATOS
*****************************************************************/

char* toLowercase(char* word){
    int start = 0;
    int end = strlen(word)-1;

    while (start<=end){
        if (word[start] >= 'A' && word[start] <= 'Z'){ 
            word[start] = word[start] - 'A' + 'a';
        }
        start++;
    }
    return word;
}

char* toUppercase(char* word){
    int start = 0;
    int end = strlen(word)-1;

    while (start<=end){
        if (word[start] >= 'a' && word[start] <= 'z'){ 
            word[start] = word[start] + 'A' - 'a';
        }
        start++;
    }
    return word;
}

char *getUserInput(){
    int c;
    char * line = (char*)malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;

    if(line == NULL){
        return NULL;
    }

    while (true){
        c = fgetc(stdin);
        if(c == EOF)
            break;
        if(--len == 0) {
            len = lenmax;
            char * linen = (char*)realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n'){
            *line-- = ' ';
            break;
        }
    }
    *line = '\0';

    return linep;
}

/*****************************************************************
 *                      ANÁLISIS DEL COMANDO
*****************************************************************/

bool validSelectArguments(char** commandArray, char** tableFields){
    int i = 2;
    bool flag = false;
    const char * const array[] = { "<", ">", "=" };
    char str[256];
    char value[256];

    while (tableFields[i]){
        if (strcmp(commandArray[1], tableFields[i]) == 0){  // Comprobar que existe el campo. Si existe, se pone un flag.
            strcpy(value,tableFields[i+1]);
            toLowercase(value);
            flag = true;
        }
        i++;
    }
    if (flag == false){
        prError(ERR_FIELD_DOESNOTEXIST,NULL,NULL);
        return false;
    }

    flag = false;
    for (i = 0; i < 3; i++){
        if (strcmp(commandArray[2], array[i]) == 0){    // Comprueba que el segundo comando sea un <, > o un =
            flag = true;
        }
    }
    if (flag == false){
        prError(ERR_OPERATOR_INVALID,NULL,NULL);
        return false;
    }

    flag = false;
    strcpy(str,commandArray[3]);    // Comrpueba que se haya introducido un tipo de dato valido
    if (strcmp(value, TYPE_TEXT) == 0){
        if (isAText(str)){
            flag = true;
        }
    }

    strcpy(str,commandArray[3]);
    if (strcmp(value, TYPE_NUM) == 0){
        if (validNumber(str)){
            flag = true;
        }
    }
    
    strcpy(str,commandArray[3]);
    if (strcmp(value, TYPE_DATE) == 0){
        if (isADate(str)){
            flag = true;
        }
    }

    if (strcmp(value, TYPE_EMAIL) == 0){  
        if (isAnEmail(str)){
            flag = true;
        }
    }

    if (flag == false){
        prError(ERR_VALUE_INVALID,NULL,NULL);
        return false;
    }
    
    return true;
}


/**
 * @brief Función que, dados una serie de comandos y una posición determinada, concretarán si se cumple la propiedad "campo-tipo"
 * 
 * @param commandArray Un array de comandos que se analizarán
 * @param position  Indica la posición en la que debería estar el primer campo
 * @return true Si es los campos no se repiten y los tipos son "DATE", "NUM" o "TEXT"
 */
bool isFieldTypeValid(char** commandArray, const int position){
    int size = 10;
    char **auxArray = (char**)malloc(sizeof(char**)*size);
    char* aux;
    int i = 0;
    int j = 0;

    while (commandArray[i]){
        size++;
        auxArray = (char**)realloc(auxArray,size*sizeof(char**));
        auxArray[i] = (char*)malloc( strlen(commandArray[i]) * sizeof(char));
        i++;
    }
    i = 0;
    while (commandArray[i]){
        strcpy(auxArray[i], commandArray[i]);
        i++;
    }
    
    i = position;
    while (commandArray[i]){
        /* Si el primer campo es impar, se seguirá la lógica de que todas las posiciones impares son campos
        *  Si el primer campo, por su parte, es par, se hará lo contrario.
        */
        if ( (isPair(i) == isPair(position)) || (isOdd(i) == isOdd(position)) ){
            for (j = position; j < i-1; j++){    // Comprueba que no se ha repetido el campo
                if(strcmp(auxArray[j],commandArray[i]) == 0){
                    prError(ERR_FIELD_REPEATED,commandArray[i],NULL);
                    free(auxArray);
                    return false;
                }
            }
        }else{ // Cuando ocupe una posición contraria, será un TIPO
            auxArray[i] = toLowercase(commandArray[i]);
            if (strcmp(auxArray[i], TYPE_NUM ) == 0){
                //Es un numero, todo ok.
            }
            else if (strcmp(auxArray[i], TYPE_DATE ) == 0){
                //Es una fecha, todo ok.
            }
            else if (strcmp(auxArray[i], TYPE_TEXT) == 0){
                //Es un texto, todo ok.
            }
            else if (strcmp(auxArray[i], TYPE_EMAIL) == 0){
                //Es un email, todo ok.
            }
            else{   // No es niguno. No es válido.
                prError(ERR_TYPE_INVALID,NULL,NULL);
                free(auxArray);
                return false;
            }
        }
        i++;
    }
    
    free(auxArray);
    return true;    // Ha pasado todas las pruebas, se han puesto campos y tipos
}

char **tokenize(char *str, char **arr, const char separator[30]){
    char *token;
    int i = 0;
    token = strtok (str,separator);
    while (token != NULL){
        arr[i] = (char*)malloc( strlen(token) * sizeof(char));
        arr[i++] = token;
        token = strtok (NULL,separator);
    }
    arr[i] = NULL;
    return arr;
}

int analizeCommand(char** commandArray){
    int i = 0;
    int arraySize = getArraySize(commandArray);
    
    char* command;

    if (commandArray[0] == NULL){
        prError(ERR_ARGUMENTS_EMPTY,NULL,NULL);
        return ERROR;
    }
    command = commandArray[0];
    toLowercase(command);

    if (strcmp(command, EXIT_CMD) == 0){
        return KEY_EXIT;
    }

    if (strcmp(command, HELP_CMD) == 0){
        return KEY_HELP;
    }

    if (strcmp(command , NEW_CMD) == 0){
        if (commandArray[1] == NULL){
            prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
            return ERROR;
        }
        command = commandArray[1];
        toLowercase(command);

        if (strcmp(command , DB_CMD) == 0){
            if (commandArray[2] == NULL){
                prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
                return ERROR;
            }else{
                if (arraySize > 3){
                    prError(ERR_ARGUMENTS_TOOMANY,command,NULL);
                    return ERROR;
                }else{
                    return KEY_NEW_DB;
                }
            }
        }
        else if (strcmp(command , TAB_CMD) == 0){
            if ( (arraySize >= 5) && ( isOdd(arraySize) == true ) ){
                return KEY_NEW_TAB;
            }else{
                prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
                return ERROR;
            }
        } else {
            prError(ERR_COMMAND_INVALID,command,NULL);
            return ERROR;
        }
    }

    if (strcmp(command , USE_CMD) == 0){
        if (commandArray[1] == NULL){
            prError(ERR_ARGUMENTS_NOTENOUGH,commandArray[0],NULL);
            return ERROR;
        }
        command = commandArray[1];
        toLowercase(command);

        if (strcmp(commandArray[1] , DB_CMD) == 0){
            if (commandArray[2] == NULL){
                prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
                return ERROR;
            }else{
                if (arraySize > 3){
                    prError(ERR_ARGUMENTS_TOOMANY,command,NULL);
                    return ERROR;
                }else{
                    return KEY_USE_DB;
                }
            }
        }

        else if (strcmp(commandArray[1] , TAB_CMD) == 0){
            return KEY_USE_TAB;
        }

        else{
            prError(ERR_COMMAND_INVALID,commandArray[1],NULL);
            return ERROR;
        }
    }

    if (strcmp(command , INSERT_CMD) == 0){
        if ( getArraySize(commandArray) > 1 ){
            return KEY_INSERT;
        }else{
            prError(ERR_ARGUMENTS_INVALID,command,NULL);
            return ERROR;
        } 
    }

    if (strcmp(command , SELECT_CMD) == 0){
        if ( getArraySize(commandArray) == 1 ){
            return KEY_SELECT;
        }
        else if( getArraySize(commandArray) == 2){
            command = commandArray[1];
            toLowercase(command);
            if ((strcmp(command, ASC_CMD) == 0) || (strcmp(command,DES_CMD)) == 0){
                return KEY_SELECT_SORT;
            }else
            {
                prError(ERR_COMMAND_INVALID,command,NULL);
                return ERROR;
            }
            
            
        }
        else if (getArraySize(commandArray) == 4){
            return KEY_SELECT_FIELDVALUE;
        }
        else if (getArraySize(commandArray) == 5){
            command = commandArray[4];
            toLowercase(command);
            if ((strcmp(command, ASC_CMD) == 0) || (strcmp(command,DES_CMD)) == 0){
                return KEY_SELECT_FIELDVALUE_SORT;

            }else{
                prError(ERR_COMMAND_INVALID,command,NULL);
                return ERROR;
            }
        }
        else{
            prError(ERR_ARGUMENTS_INVALID, NULL, NULL);
            return ERROR;
        }
    }

    if (strcmp(command , LIST_CMD) == 0){
        if (arraySize == 1){
            return KEY_LIST;
        }
        else{
            prError(ERR_ARGUMENTS_TOOMANY,command,NULL);
            return ERROR;
        }
    }

    if (strcmp(command , DELETE_CMD) == 0){
        if (arraySize == 4){
            return KEY_DELETE;
        }else{
            prError(ERR_ARGUMENTS_INVALID,command,NULL);
            return ERROR;
        }
        
    }
    
    
    if (strcmp(command, REMOVE_CMD) == 0){
        if (commandArray[1] == NULL){
            prError(ERR_ARGUMENTS_NOTENOUGH,commandArray[0],NULL);
            return ERROR;
        }
        command = commandArray[1];
        toLowercase(command);

        if (strcmp(command , DB_CMD) == 0){
            if (commandArray[2] == NULL){
                prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
                return ERROR;
            }else{
                if (arraySize > 3){
                    prError(ERR_ARGUMENTS_TOOMANY,command,NULL);
                    return ERROR;
                }else{
                    return KEY_REMOVE_DB;
                }
            }
        }
        else if (strcmp(command , TAB_CMD) == 0){
            if (commandArray[2] == NULL){
                prError(ERR_ARGUMENTS_NOTENOUGH,command,NULL);
                return ERROR;
            }else{
                if (arraySize > 3){
                    prError(ERR_ARGUMENTS_TOOMANY,command,NULL);
                    return ERROR;
                }else{
                    return KEY_REMOVE_TAB;
                }
            }
        }
    } 
    
    prError(ERR_COMMAND_INVALID, command, NULL);
    return ERROR;
}