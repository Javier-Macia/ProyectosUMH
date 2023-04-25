/*
*
* PROGRAMA HECHO POR JAVIER MACIÁ SEMPERE
* Este programa hace una pseudo-base de datos.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lib.h"

int main (int argc, char* argv[]){
/*****************************************************************
*                      DECLARACION DE VARIABLES
*****************************************************************/
    // Constantes
    const char defaultSeparator[30] = " \"\n\t:;,.!¡?¿'()"; 

    // Útiles
    int key;
    int auxInt;
    int i = 0;
    bool exitProgram = false;

    // Línea de comandos y array de comandos

    char* prompt;
    char* aux;
    char* commandArray[500];
    char* auxArr[3000];
    char* dbName = (char*)malloc(sizeof (char*) * 100);
    char* tabName = (char*)malloc(sizeof (char*) * 100);
    char path[200];

    TABLE tab;
    DB db;
    
    dbName = NULL;
    tabName = NULL;

/*****************************************************************
*                        PROGRAMA PRINCIPAL
*****************************************************************/
    welcomeMenu();

    while (exitProgram == false){
        showPrompt(dbName,tabName);
        prompt = getUserInput();
        tokenize(prompt,commandArray, " ");
        key = analizeCommand(commandArray);
        
        switch (key){
        case KEY_EXIT:
            exitProgram = true;
            break;
        
        case KEY_HELP: // No se pide en la práctica, pero la he añadido por comodidad
            helpMenu();
            break;

        case KEY_NEW_DB:
            if (!existsDirectory(commandArray[2])){
                printf("Creando la base de datos '%s' ...\n",commandArray[2]);
                createNewDirectory(commandArray[2]);
                createMetaFile(commandArray[2]);
                printf("Base de datos creada.\n");
            }else{
                prError(ERR_DIR_ALREADYEXISTS,NULL,NULL);
            }
            break;

        case KEY_USE_DB:
            if (existsDirectory(commandArray[2])){
                strcpy(path,commandArray[2]);
                strcat(path,"\x5c");
                dbName = commandArray[2];
                tabName = NULL;
                auxInt = getNumberOfFiles(path)-1;
                if (auxInt == 0){
                    printf("No hay tablas en la base de datos '%s'\n",dbName);
                }else{
                    printf("'%s' tiene las siguientes tablas:\n", dbName);
                    showFiles(path);
                }
                
                
            }else{
                prError(ERR_DIR_DOESNOTEXIST,NULL,NULL);
            }
            break;
        
        case KEY_NEW_TAB:
            if (dbName == NULL){
                prError(ERR_DB_UNASIGNED,commandArray[2],NULL);
            }else{
                if (!existsTableFile(dbName,commandArray[2])){
                    if ((isFieldTypeValid(commandArray , 3))){
                        printf("Creando la tabla '%s' ...\n",commandArray[2]);
                        createTableFile(dbName, commandArray[2]);
                        insertTableInMeta(dbName,commandArray, 3);
                        printf("Tabla creada. \n",commandArray[2]);
                    }
                }else{
                    prError(ERR_FILE_ALREADYEXISTS,NULL,NULL);
                }
            }
            break;
        
        case KEY_USE_TAB:
            if (dbName == NULL){
                prError(ERR_DB_UNASIGNED,commandArray[2],NULL);
            }
            
            else if (existsTableFile(dbName, commandArray[2])){
                tabName = commandArray[2];
                strcpy(path,"");
                strcpy(path,dbName);
                strcat(path,"\x5c");
                strcat(path,commandArray[2]);
                strcat(path,".tab");
            }else{
                prError(ERR_FILE_DOESNOTEXIST,NULL,NULL);
            }
            break;

        case KEY_INSERT:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr, dbName, tabName);
                auxInt = ( (getArraySize(auxArr) - 2) / 2);
                if ( auxInt == (getArraySize(commandArray) - 1) ){
                    if (areValidTypes(commandArray,auxArr, 1)){
                        insertRowInTable(path, commandArray, 1);
                        printf("Se ha insertado la nueva fila.\n");
                    }else{
                        prError(ERR_TYPE_INVALID,"","");
                    }
                }else{
                    prError(ERR_FIELD_NUMBER_INVALID,NULL,NULL);
                }
                
            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            break;

        case KEY_SELECT:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr,dbName,tabName);
                showTableFields(auxArr); 
                showTableContent(path);       
            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            break;
        

        case KEY_SELECT_FIELDVALUE:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr,dbName,tabName);
                showTableFields(auxArr); 
                if(validSelectArguments(commandArray, auxArr)){
                    auxInt = getFieldPosition(auxArr, commandArray[1]);
                    cleanArray(auxArr);
                    getTableContent(auxArr,path);
                    selectTableValues(auxArr, auxInt, commandArray[2], commandArray[3]);
                    printf("--------------------\n");
                    printf("Contenido de la tabla: \n");
                    printf("--------------------\n");
                    showArrayContent(auxArr);
                }

            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            break;
        
        case KEY_SELECT_FIELDVALUE_SORT:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr,dbName,tabName);
                showTableFields(auxArr); 
                if(validSelectArguments(commandArray, auxArr)){
                    auxInt = getFieldPosition(auxArr, commandArray[1]);
                    cleanArray(auxArr);
                    getTableContent(auxArr,path);
                    selectTableValues(auxArr, auxInt, commandArray[2], commandArray[3]);
                    showSortedTable(auxArr, commandArray[4]);
                }
                
            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            break;

        case KEY_SELECT_SORT:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr,dbName,tabName);
                showTableFields(auxArr); 
                cleanArray(auxArr);
                getTableContent(auxArr, path);
                showSortedTable(auxArr, commandArray[1]);
            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            break;

        case KEY_LIST:
            showDBs(".");
            break;

        case KEY_DELETE:
            if (tabName !=NULL){
                cleanArray(auxArr);
                getTableFields(auxArr,dbName,tabName);
                if(validSelectArguments(commandArray, auxArr)){
                    printf("Has introducido bien los campos\n");
                    auxInt = getFieldPosition(auxArr, commandArray[1]);
                    cleanArray(auxArr);
                    getTableContent(auxArr,path);
                    deleteSelectedRow(auxArr, auxInt, commandArray[2], commandArray[3]);
                    rewrite(auxArr,path);
                }
            }else{
                prError(ERR_TAB_UNASIGNED,NULL,NULL);
            }
            
            break;

        case KEY_REMOVE_DB:     // No se pide en la práctica, pero se ha añadido por comodidad
            deleteDirectory(commandArray[2]);
            if (dbName != NULL){
                if (strcmp(commandArray[2], dbName) == 0){
                    dbName = NULL;
                    tabName = NULL;
                }
            }
            
            break;
        
        case KEY_REMOVE_TAB:    // No se pide en la práctica, pero se ha añadido por comodidad
            if(existsTableFile(dbName, commandArray[2])){
                removeTableFile(dbName,commandArray[2]);
                if (tabName != NULL){
                    if (strcmp(commandArray[2], tabName) == 0){
                        tabName = NULL;
                    }
                }
                
            }else{
                prError(ERR_FILE_CANNOTREMOVE,path,NULL);
            }
            break; 
        
        default:
            break;
        }
        cleanArray(commandArray);
    }
    

    printf("\nFIN...\n");
    
    free(dbName);
    free(tabName);
    return 0;
}