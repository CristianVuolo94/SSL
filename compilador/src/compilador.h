/*
 * compilador.h
 *
 *  Created on: 6/9/2016
 *      Author: utnso
 */

#ifndef COMPILADOR_H_
#define COMPILADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

typedef struct{
	char* token;
	char* lexema;

}t_simbolo;

char* vg_script;
int vg_size_script;
int tabla [15][13] = {{1,3,5,6,7,8,9,10,11,14,13,0},
							{1,1,2,2,2,2,2,2,2,2,2,1},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{4,3,4,4,4,4,4,4,4,4,4,3},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{14,14,14,14,14,14,14,14,14,12,14,14},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99}};


void validaciones(int argc, char * argv);
void procesarScript(char * argv);
int columna(char c);

#endif /* COMPILADOR_H_ */
