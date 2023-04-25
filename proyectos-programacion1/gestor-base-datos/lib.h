#include <stdio.h>

#ifndef __LIB_H__
#define __LIB_H__

/*****************************************************************
 *                          DEFINICIONES
*****************************************************************/
// Útiles
#define YES 1
#define NO 0
#define ERROR -1

// Comandos
#define EXIT_CMD "exit"
#define HELP_CMD "help"
#define DB_CMD "db"
#define TAB_CMD "table"
#define NEW_CMD "new"
#define USE_CMD "use"
#define INSERT_CMD "insert"
#define SELECT_CMD "select"
#define ORDER_CMD "order"
#define REMOVE_CMD "remove"
#define ASC_CMD "asc"
#define DES_CMD "des"
#define LIST_CMD "list"
#define DELETE_CMD "delete"

// Nombre de los tipos de datos
#define TYPE_NUM "num"
#define TYPE_DATE "date"
#define TYPE_TEXT "text"
#define TYPE_EMAIL "email"

/*****************************************************************
 *                           ESTRUCTURAS
*****************************************************************/

typedef enum { NUM, DATE, TEXT, EMAIL }TYPE;

typedef struct TABLE{
    char name[150];
    int fieldNumber;
    char **fields;
    TYPE *types;
}TABLE;

typedef struct DB{
    char name[300];
    int tableNumber;
    TABLE** tables;

}DB;

/*****************************************************************
 *                          ENUMERACIONES
*****************************************************************/
typedef enum ERROR_LIST {
    ERR_UNDEF, // Error sin definir
    ERR_COMMAND_INVALID, // Comando inválido
    ERR_ARGUMENTS_NOTENOUGH, // No hay argumentos suficientes
    ERR_ARGUMENTS_INVALID,  // Numero invalido de argumentos
    ERR_ARGUMENTS_TOOMANY, // Demasiados argumentos
    ERR_ARGUMENTS_EMPTY,   // No hay argumentos
    ERR_DIR_ALREADYEXISTS,   // Ya existe este directorio
    ERR_DIR_DOESNOTEXIST,   // NO existe este directorio
    ERR_FILE_ALREADYEXISTS,   // Ya existe este archivo
    ERR_FILE_DOESNOTEXIST,   // NO existe este archivo
    ERR_FILE_CANNOTREMOVE, // No se ha podido borrar un archivo
    ERR_TAB_UNASIGNED,  // No hay ninguna tabla asignada
    ERR_DB_CANNOTREMOVE, // No se ha podido borrar una base de datos
    ERR_DB_UNASIGNED,   // No hay una base de datos asignada
    ERR_DB_HASFILES,     // La base de datos tiene archivos dentro y no se puede borrar/modificar
    ERR_FIELD_REPEATED,  // Se ha repetido un campo
    ERR_FIELD_NUMBER_INVALID, // Numero de campos incorrecto
    ERR_FIELD_DOESNOTEXIST,  // El campo indicado no existe
    ERR_TYPE_INVALID,    // Tipo de dato inválido
    ERR_STR_NOTATEXT,   // La cadena introducida no es un texto
    ERR_STR_NOTANUMBER,   // La cadena introducida no es un numero
    ERR_STR_NOTADATE,   // La cadena introducida no es una fecha
    ERR_STR_NOTANEMAIL, // La cadena introducida no es un email
    ERR_OPERATOR_INVALID, // El operador indicado no es valido
    ERR_VALUE_INVALID,  // El valor introducido no es valido


}ERRORID;

typedef enum KEY_LIST{
    KEY_EXIT, // Clave para salir
    KEY_HELP, // Clave para el menu de ayuda
    KEY_NEW_DB, // Clave para una nueva base de datos
    KEY_USE_DB,  // Clave para usar una base de datos
    KEY_NEW_TAB,  // Clave para una nueva tabla
    KEY_USE_TAB, // Clave para usar una tabla
    KEY_INSERT, // Clave para insertar
    KEY_SELECT,  // Clave para mostrar los campos, los tipos y el contenido de una tabla.
    KEY_SELECT_FIELDVALUE, // Clave para seleccionar con un campo x valor
    KEY_SELECT_FIELDVALUE_SORT, // Clave para seleccionar con un campo x valor ordenado
    KEY_SELECT_SORT,   // Clave para seleccionar u
    KEY_REMOVE_DB,  // Clave para borrar una base de datos
    KEY_REMOVE_TAB,  // Clave para borrar una tabla
    KEY_LIST,   // Clave para listar todas las bases de datos disponibles
    KEY_DELETE,
}KEY;

/*****************************************************************
*                        FUNCIONES ESTÉTICAS
*****************************************************************/
void welcomeMenu();
void helpMenu();
void prError( int errorID , char* mainStatement , char* optionalStatement);
void showPrompt(char* dbName , char* tableName );
void showFiles(char* directory);
void showDBs();
void showTableFields(char** tableArr);
void showTableContent(char* path);
void showSortedTable(char** tableArr, char* mode);
void showArrayContent(char** auxArr);

/*****************************************************************
 *                      FUNCIONES AUXILIARES
*****************************************************************/
int getArraySize(char** wordsArray);
bool isPair(int number);
bool isOdd(int number);
char** cleanArray(char** commandArray);
char* itoa(int value, char* result, int base);
void replaceWordInLine(char* line,char* oldWord, char* newWord);
char** getTableFields(char** arr, char* dbName, char* tableName);
char** getTableContent(char** arr, char* path);
bool areValidTypes(char** commandArray,char** fieldTypeArray, int position);
void swap(int *x, int *y);
void swapFloats(float *x, float *y);
int dateToEquivalentInt(const char* date);

/*****************************************************************
 *                        ORDENAR ARRAYS 
*****************************************************************/
void bubbleSort(int arr[], int n);
void floatBubbleSort(float arr[], int n);
void sortWords(char *wordsArr[], int limit, int order[], int start);
void sortNumbers(char *wordsArr[], int limit, int order[], int start);
void sortDates(char *wordsArr[], int limit, int order[], int start);
int getDBsNumber();

/*****************************************************************
 *           VALIDACIÓN DE NUMEROS, FECHAS, TEXTO E EMAIL
*****************************************************************/
bool validNumber(const char *str);
bool validDate(int d, int m, int a);
bool isADate(const char* word);
bool isAText(const char* word);
bool isAnEmail(const char* word);
/*****************************************************************
 *                      FUNCIONES DE CARPETAS
*****************************************************************/
int getNumberOfFiles(char* directory);
bool existsDirectory(char* directory);
void createNewDirectory(char* directory);
void deleteDirectory(char* directory);

/*****************************************************************
 *                      FUNCIONES DE FICHEROS
*****************************************************************/
int getFieldPosition(char** textArr, char* wantedField);
void selectTableValues(char** tableArr, int fieldPosition, char* mode, const char* value);
void insertRowInTable(char* path, char** commandArray, int position);
int getNumberOfLinesInFile(FILE* f);
void setMetaFile(char* path);
char** saveTextInArray(char** textArr, FILE *f);
void insertTableInMeta(char* dbName, char** commandArray, int position);
bool existsFile(char *fileName);
void createFile(char* fileName);
void createMetaFile(char* dbName);
bool existsTableFile(char* dbName, char* tabName);
void createTableFile(char* dbName, char* fileName);
void removeTableFile(char* dbName, char* tableName);
void rewrite(char** tableArr, char* path);
void deleteSelectedRow(char** tableArr, int fieldPosition, char* mode, const char* value);

/*****************************************************************
 *                      LECTURA DE DATOS
*****************************************************************/
char* toLowercase(char* word);
char* toUppercase(char* word);
char* getUserInput();

/*****************************************************************
 *                      ANÁLISIS DEL COMANDO
*****************************************************************/
bool validSelectArguments(char** commandArray, char** tableFields);
bool isFieldTypeValid(char** commandArray, const int position);
char** tokenize(char* str, char** arr, const char separator[30]);
int analizeCommand(char** command);

#endif