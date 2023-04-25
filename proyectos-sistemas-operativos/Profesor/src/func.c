//*** NO MODIFICAR LAS FUNCIONES DE ESTE CONTENIDO
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include "../inc/func.h"
//*** Si añades funciones al final y necesitas includes puedes añadirlos aqui

int IxArray(int x, int y){
	return y*NCOL+x;
}

void Sintaxis(){
	printf("------------------------------------\n");
    printf("shared <numHijos>\n");
    printf("  <numHijos> valores válidos: 4 16, \n");
	printf("cualquier otro valor es erroneo.\n");
}

void PrintMatrix(int *matrix){
	int x,y;
	for (y=0;y<NFIL;y++){
		for(x=0;x<NCOL;x++){
			printf("%1d ",matrix[IxArray(x,y)]);
		}
		printf("\n");
	}
	printf("\n");
}

void GenerarNumerosHijo(int *numeros,int numsPorHijo){
	int i;
	for (i=0;i<numsPorHijo;i++)
		numeros[i]=rand()%10;
}

void GenerarNumerosPadre(int *matrix){
	int i;
	for (i=0;i<(NFIL*NCOL);i++)
		matrix[i]=rand()%10;
}

void AsignarAciertos(int* matrix, int *numeros, int nHijo, int numHijos){
	int x,y,xIni,yIni,xEnd,yEnd;
	int numInts=(int)(NCOL*NFIL)/(numHijos);
	int blockSize=(int)sqrt(numInts);
	int hijosPerRow=(int)NCOL/blockSize;
	int acierto,numero,apuesta;

	//printf("Tamaño de bloque %d, HijosPerRow %d \n",blockSize,hijosPerRow);

	xIni=(nHijo*blockSize)%NCOL;
	xEnd=xIni+blockSize-1;
	yIni=(int)(nHijo/hijosPerRow)*blockSize;
	yEnd=yIni+blockSize-1;
	
	int ix=0;
	for (y=yIni; y<=yEnd; y++)
		for(x=xIni; x<=xEnd; x++){
			apuesta=numeros[ix];
			numero=matrix[IxArray(x,y)];
			if (apuesta==numero) 
				matrix[IxArray(x,y)]=1;
			else
				matrix[IxArray(x,y)]=0;
			ix=ix+1;
		}
	
}

void ImprimirAciertos(int *matrix, int nHijo, int numHijos){
	int x,y,xIni,yIni,xEnd,yEnd;
	char nombreFichero[5];
	int numInts=(int)(NCOL*NFIL)/(numHijos);
	int blockSize=(int)sqrt(numInts);
	int hijosPerRow=(int)NCOL/blockSize;
	int fd,i;
	char sAcierto[5]="";

	xIni=(nHijo*blockSize)%NCOL;
	xEnd=xIni+blockSize-1;
	yIni=(int)(nHijo/hijosPerRow)*blockSize;
	yEnd=yIni+blockSize-1;

	sprintf(nombreFichero,"r%02d",nHijo);
	if ((fd = open(nombreFichero, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
		perror("Cannot open output file\n"); 
		exit(1);
	}
	for (y=yIni; y<=yEnd; y++){
		for(x=xIni; x<=xEnd; x++){
			sprintf(sAcierto,"%d",matrix[IxArray(x,y)]);
			write(fd,sAcierto,sizeof(sAcierto));
		}
	}
	
	close(fd);
}

//*** A PARTIR DE AQUI PUEDES PONER LO QUE QUIERAS, SI LO NECESITAS



