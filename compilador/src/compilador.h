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
#include <commons/string.h>
#include <commons/collections/list.h>
#include <regex.h>

typedef enum {
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

typedef struct{
	TOKEN token;
	char* lexema;
}t_simbolo;

typedef struct{
	char* nombre;
	TOKEN clase;
	char* valor;
}REG_EXPRESION;

/*             VARIABLES GLOBALES								*/
char* vg_script;
int vg_size_script;
extern int vg_script_desp;

FILE * salida;

t_list* tablaDeSimbolos;

void validaciones(int argc, char * argv);
void procesarScript(char * argv);
void inicializarArchivoSalida(char*ruta);
int columna(char c);
void limpiarBuffer();
void inicializarTablaSimbolos();
TOKEN scanner();
void retornoScanner(int valor);
void parser(void);
void pruebas(char * prueba);
void liberarRecursos();
#endif /* COMPILADOR_H_ */
