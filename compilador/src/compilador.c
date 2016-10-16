
#include "compilador.h"
int vg_estado = 0;
char buffer[35];
char otroBuffer[35];
int buffer_desp = 0;
int contador = 1;
int tabla [16][14] = 	   {{1,3,5,6,7,8,9,10,11,14,13,0,14},
							{1,1,2,2,2,2,2,2,2,2,2,2,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{4,3,4,4,4,4,4,4,4,4,4,4,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{14,14,14,14,14,14,14,14,14,12,14,14,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99}};
int vg_script_desp=0;

/* FUNCIONES PAS */
void objetivo(void);
void programa(void);
void listaSentencias(void);
void sentencia(TOKEN t);
void expresion(void);
void listaIdentificadores(void);
void listaExpresiones(void);
void primaria(void);
/* void identificador(void);  */
REG_EXPRESION procesarCte(void);
void generar(char* instruccion, char* unaExpresion, char* otraExpresion, char* extra);
char * gen_infijo(void);


void procesarScript(char* argv){

FILE * script = fopen(argv, "rb+");
	if (script != NULL){

		fseek(script, 0L, SEEK_SET);

		char c = fgetc(script);
		vg_script = malloc(sizeof(char));
		int i=0;

		while(!feof(script)){
			vg_script[i] = c;
			i++;
			vg_script = realloc(vg_script, (i+1) * sizeof(char));
			c = fgetc(script);
		}

		vg_script[i]='\0';
		vg_size_script = i;

		fclose(script);

	} else {
		printf("El Script ingresado es invalido o no tiene permisos para ejecutarlo. \n");
		fclose(script);

	}
}
void lawea(char *prueba){
	int i = 0;
	while(prueba[i] != '\0'){
		vg_script = realloc(vg_script, (i+1) * sizeof(char));
		vg_script[i] = prueba[i];
		i++;
	}
}

int validarNombreScript(char * nombreScript){

	int desplazamiento = 0;

	while(nombreScript[desplazamiento] != '\0'){
		desplazamiento++;
	}
	desplazamiento--;
	if(nombreScript[desplazamiento]== 'm'){
		desplazamiento--;
		if(nombreScript[desplazamiento] == '.'){
			return 1;
		}
	}

	return 0;

}
void inicializarArchivoSalida(char* ruta){
	salida=fopen(ruta,"w");
}


void validaciones(int argc, char * argv){

	if(argc==1){
		printf("Debe ingresar un archivo y el nombre del archivo de salida.\n");
		exit(1);
	}

	if(argc != 3){
		printf("Numero incorrecto de argumentos\n");
		exit(1);
	}

	if(validarNombreScript(argv)==0){
		printf("El archivo debe tener extension .m\n");
		exit(1);
	}

}

void inicializarTablaSimbolos(){

	tablaDeSimbolos = list_create();

	t_simbolo* simbolo=malloc(sizeof(t_simbolo));
	simbolo->lexema=(char*)strdup("leer");
	simbolo->token = LEER;
	list_add(tablaDeSimbolos,simbolo);

	t_simbolo* simbolo2=malloc(sizeof(t_simbolo));
	simbolo2->lexema=(char*)strdup("escribir");
	simbolo2->token = ESCRIBIR;
	list_add(tablaDeSimbolos,simbolo2);

	t_simbolo* simbolo3=malloc(sizeof(t_simbolo));
	simbolo3->lexema=(char*)strdup("inicio");
	simbolo3->token = INICIO;
	list_add(tablaDeSimbolos,simbolo3);

	t_simbolo* simbolo4=malloc(sizeof(t_simbolo));
	simbolo4->lexema=(char*)strdup("fin");
	simbolo4->token = FIN;
	list_add(tablaDeSimbolos,simbolo4);
}

void retornoScanner(int valor){
	switch(valor){
	case INICIO:{
			printf("					inicio\n");
			break;
	}
	case FIN:{
			printf("					fin\n");
			break;
	}
	case LEER:{
			printf("					leer\n");
			break;
	}
	case ESCRIBIR:{
			printf("					escribir\n");
			break;
	}
	case ID:{
			printf("					id\n");
			break;
	}
	case CONSTANTE:{
			printf("					constante\n");
			break;
	}
	case PARENIZQUIERDO:{
			printf("					parentesis izquierdo\n");
			break;
	}
	case PARENDERECHO:{
			printf("					parentesis derecho\n");
			break;
	}
	case PUNTOYCOMA:{
			printf("					punto y coma\n");
			break;
	}
	case COMA:{
			printf("					coma\n");
			break;
	}
	case ASIGNACION:{
			printf("					asignacion\n");
			break;
	}
	case SUMA:{
			printf("					suma\n");
			break;
	}
	case RESTA:{
			printf("					resta\n");
			break;
	}
	case FDT:{
			printf("					fdt\n");
			break;
	}
	}
}

void parser(){
	objetivo();
}

int esPalabraReservada(char *buffer){

	int desplazamiento;
	for(desplazamiento=0;desplazamiento<list_size(tablaDeSimbolos);desplazamiento++){

		t_simbolo* unSimbolo = (t_simbolo*)list_get(tablaDeSimbolos,desplazamiento);
		if(strcmp(unSimbolo->lexema,buffer)==0){
			return 1;
		}
	}

	return 0;

}
int	esIdentificadorCorreto(char *buffer){
	if(strlen(buffer)>32){
		return 0;
	}
	strcpy(otroBuffer, buffer);
	return 1;

}



void agregarIdentificadorATS(char *buffer){

	bool perteneceLista (t_simbolo* simbolo) {return strcmp( simbolo->lexema,buffer) == 0 ; }

	if(!list_any_satisfy(tablaDeSimbolos,(void*)perteneceLista)){
		t_simbolo* simbolo = malloc(sizeof(t_simbolo));
		simbolo->lexema=(char*)strdup(buffer);
		simbolo->token = ID;
		list_add(tablaDeSimbolos,simbolo);
		generar("Declara", buffer, "\0", "Entera");
	}

}

void agregarConstanteATS(char* buffer){

	bool perteneceLista (t_simbolo* simbolo) {return strcmp( simbolo->lexema,buffer) == 0 ; }

	if(!list_any_satisfy(tablaDeSimbolos,(void*)perteneceLista)){
		t_simbolo* simbolo = malloc(sizeof(t_simbolo));
		simbolo->lexema=(char*)strdup(buffer);
		simbolo->token = CONSTANTE;
		list_add(tablaDeSimbolos,simbolo);
	}

/*	int i = 0;
	while(buffer[i] != '\0'){
		otroBuffer[i] = buffer[i];
		i++;
	}
	otroBuffer[i] = '\0'; */
	strcpy(otroBuffer, buffer);
}

void errorSemantico(){
	printf("ERROR SEMANTICO\n");
	printf("En el desplazamiento: %d\n", vg_script_desp);
}
TOKEN scanner(){

	while(1){
		printf("vg_script[%d]: %c\n", vg_script_desp, vg_script[vg_script_desp]);
		vg_estado = tabla[vg_estado][columna(vg_script[vg_script_desp])];
/*		se ingresó un numero o letra */
		if(vg_estado == 1){

			buffer[buffer_desp] = vg_script[vg_script_desp];
			buffer_desp++;

		}

		if(vg_estado == 3){

				buffer[buffer_desp] = vg_script[vg_script_desp];
				buffer_desp++;
		}


/*		se ingresó palabra reservada o identificador */
		if(vg_estado == 2){

			if(esPalabraReservada(buffer)){

				if(strcmp(buffer,"leer")==0){
					limpiarBuffer();
					return LEER;
				}

				if(strcmp(buffer,"escribir")==0){
					limpiarBuffer();
					return ESCRIBIR;
				}

				if(strcmp(buffer,"inicio")==0){
					limpiarBuffer();
					return INICIO;
				}

				if(strcmp(buffer,"fin")==0){
					limpiarBuffer();
					return FIN;
				}
			}


			if(esIdentificadorCorreto(buffer)==1){ /*meter buffer de id para generar instruccion*/
				agregarIdentificadorATS(buffer);
				limpiarBuffer();
				return ID;
			}
			else printf("IDENTIFICADOR INCORRECTO: %s \n", buffer);


		}

/*		se ingresó una constante numerica */
		if(vg_estado == 4){
			agregarConstanteATS(buffer);
			limpiarBuffer();
			return CONSTANTE;

		}

/*		se detectó FDT */
		if(vg_estado == 13){
			return FDT;
		}

/*		error lexico */
		if(vg_estado == 14 || vg_estado == 99){
			errorSemantico();
			limpiarBuffer();
			vg_script_desp++;
			scanner();
		}


		if((vg_estado > 4 && vg_estado < 11) || (vg_estado == 12)){


			if(vg_estado == 5){
				limpiarBuffer();
				vg_script_desp++;
				return SUMA;
			}
			if(vg_estado == 6){
				limpiarBuffer();
				vg_script_desp++;
				return RESTA;
			}
			if(vg_estado == 7){
				limpiarBuffer();
				vg_script_desp++;
				return PARENIZQUIERDO;
			}
			if(vg_estado == 8){
				limpiarBuffer();
				vg_script_desp++;
				return PARENDERECHO;
			}
			if(vg_estado == 9){
				limpiarBuffer();
				vg_script_desp++;
				return COMA;
			}
			if(vg_estado == 10){
				limpiarBuffer();
				vg_script_desp++;
				return PUNTOYCOMA;
			}
			if(vg_estado == 12){
				limpiarBuffer();
				vg_script_desp++;
				return ASIGNACION;
			}
		}
		if(vg_estado == 11){
			buffer[buffer_desp] = vg_script[vg_script_desp];
			buffer_desp ++;
		}
		vg_script_desp++;
	}
}

int columna(char c){

	if(isalpha(c)) return 0;
	if(isdigit(c)) return 1;

	switch(c){
	case '+':
		return 2;
		break;
	case '-':
		return 3;
		break;
	case '(':
		return 4;
		break;
	case ')':
		return 5;
		break;
	case ',':
		return 6;
		break;
	case ';':
		return 7;
		break;
	case ':':
		return 8;
		break;
	case '=':
		return 9;
		break;
	case '\0':
		return 10;
		break;
	case ' ':
		return 11;
		break;
	case '\n':
		return 11;
		break;
	default:
		return 12;
	}

}

void limpiarBuffer(){
	int i;
	for(i=0; i<35; i++){
		buffer[i] = '\0';
	}
	buffer_desp = 0;
	vg_estado = 0;
}

int match(TOKEN uno, TOKEN otro){
	if(uno == otro) return 1;
	else return 0;
}

void errorSintactico(int a){
	printf("ERROR SINTACTICO %d\n", a);
	printf("En el desplazamiento: %d\n", vg_script_desp);
}

/* PROCEDIMIENTO DE ANALISIS SINTACTICO*/
/* <objetivo> -> <programa> FDT #terminar*/
void objetivo(void){
	programa();
	TOKEN t = scanner();
	if(!(match(t, FDT))) errorSintactico(1);
}
/* <programa> -> #comenzar INICIO <listaSentencias> FIN */
void programa(void){
	TOKEN t = scanner();
	if(!(match(t, INICIO))) errorSintactico(2);
	listaSentencias();
	t = scanner();
	if(!(match(t, FIN))) errorSintactico(3);
	else generar("Detiene", "\0", "\0", "\0");
}
/*<listaSentencias> -> <sentencia> {<sentencia>} */
void listaSentencias(void){
	TOKEN t = scanner();
	sentencia(t); /* la primera de la lista de sentencias */

	while (1) {  /* un ciclo indefinido */
		switch (t = scanner()) {

		case ID:
			sentencia(ID);
			break;
		case LEER:
			sentencia(LEER);
			break;
		case ESCRIBIR:
			sentencia(ESCRIBIR);
			break;

		default: /*detecto algo incorrecto o FDT */
		vg_script_desp -=3;
		return;
		} /* fin switch */
	}
}
/*<sentencia> -> ID ASIGNACIÓN <expresión> #asignar PUNTOYCOMA |
LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA |
ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA */
void sentencia(TOKEN t){

	if(!(match(t, ID) || match(t, LEER) || match(t, ESCRIBIR))) errorSintactico(4);

	if(match(t, ID)){
		char *infijo = gen_infijo();
		t = scanner();
		if(!(match(t, ASIGNACION))) errorSintactico(5);
		expresion();
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico(6);
		else generar("Almacena", otroBuffer, infijo, "\0");
	}
	if(match(t, LEER)){
		t = scanner();
		if(!(match(t, PARENIZQUIERDO))) errorSintactico(7);
		listaIdentificadores();
		t = scanner();
		if(!(match(t, PARENDERECHO))) errorSintactico(8);
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico(9);
	}
	if(match(t, ESCRIBIR)){
		t = scanner();
		if(!(match(t, PARENIZQUIERDO))) errorSintactico(10);
		listaExpresiones();
		t = scanner();
		if(!(match(t, PARENDERECHO))) errorSintactico(11);
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico(12);
	}
}
/* <listaIdentificadores> -> ID {COMA ID} -- <ID> #leer_id {COMA <ID> #leer_id} */
void listaIdentificadores(){
	TOKEN t = scanner();
	if(!(match(t, ID))) errorSintactico(13);
	else generar("Leer", otroBuffer, "\0", "\0");

	t = scanner();
	if(match(t, COMA)) listaIdentificadores();
	else vg_script_desp--; /* porque sino lo que hay en t se pierde */

}
/* <listaExpresiones> -> <expresión> #escribir_exp {COMA <expresión> #escribir_exp} */
void listaExpresiones(){
	expresion();
	generar("Escribir", otroBuffer, "\0", "\0");


	TOKEN t = scanner();
	if(match(t, COMA))	listaExpresiones();
	else vg_script_desp--; /* porque sino lo que hay en t se pierde */

}
/* <expresión> -> <primaria> {<operadorAditivo> <primaria> #gen_infijo} */
void expresion(){
	primaria();
	TOKEN t;
	REG_EXPRESION infijo;

	while(1){
		t = scanner();
		if(match(t, SUMA) || match(t, RESTA)){
			/* capaz habria que meter todo esto en una funcion para que quede mas prolijo */

			infijo.nombre = gen_infijo();
			primaria();
			char * temp = malloc(sizeof(char)*10);
			strcpy(temp, "temp&");
			char cont[10];
			sprintf(cont, "%d", contador); /* pasa a char lo que hay en contador que es int */
			strcat(temp, cont); /* asi se genera temp&1, temp&2, etc */
			contador++;
			if(t==SUMA) generar("Suma", infijo.nombre, otroBuffer, temp);
			else generar("Resta", infijo.nombre, otroBuffer, temp);

			memcpy(otroBuffer, temp, 34);
		} else{
			vg_script_desp--;
			return;
		}
	}

}
/* <primaria> -> ID -- <ID> | CONSTANTE #procesar_cte|
PARENIZQUIERDO <expresión> PARENDERECHO */
void primaria(){
	TOKEN t = scanner();

	switch(t) {
		case ID:
/*			identificador();    esta preguntando si el id esta en la lista y siempre va a estar porque se
								agrega en el scanner */
			break;
		case CONSTANTE:
			procesarCte();
			break;
		case PARENIZQUIERDO: {
			expresion();
			t = scanner();
			if(!(match(t, PARENDERECHO))) errorSintactico(15);
			break;
		}
		default:
			errorSintactico(14);
	}
}
/* <operadorAditivo> -> SUMA #procesar_op | RESTA #procesar_op -- esta hecho en expresion*/

/* <ID> -> ID #procesar_id */

/* NO SIRVE PARA NADA -- esta hecho en el scanner cuando agrega a la lista
   void identificador(){
	bool perteneceLista (t_simbolo* simbolo) {return strcmp( simbolo->lexema,buffer) == 0 ; }
	if(!list_any_satisfy(tablaDeSimbolos,(void*)perteneceLista)){
		generar("Declara", otroBuffer, "", "Entera");
	}
} */

REG_EXPRESION procesarCte(){
	REG_EXPRESION cte;

	cte.clase = CONSTANTE;
	cte.valor = malloc(sizeof(char)*10);
	sscanf (otroBuffer, "%s", cte.valor);

	return cte;
}
void depurar(char* salida){
	int desplazamiento=0;
	while(salida[desplazamiento] != '\0'){
		salida[desplazamiento] = '\0';
		desplazamiento++;
	}

}

char* formatoSalida(char* instruccion, char* unaExpresion, char* otraExpresion, char* extra){

	char* salida=malloc(sizeof(char)*(strlen(instruccion)+strlen(unaExpresion)+strlen(otraExpresion)+strlen(extra))+5);
	depurar(salida);

	strcat(salida,instruccion);

	if(strlen(unaExpresion)>0){
		strcat(salida," ");
	}
	strcat(salida,unaExpresion);

	if(strlen(otraExpresion)>0){
			strcat(salida," ");
		}
	strcat(salida,otraExpresion);

	if(strlen(extra)>0){
			strcat(salida," ");
		}
	strcat(salida,extra);

	strcat(salida,"\n");


	return salida;
}

void generar(char* instruccion, char* unaExpresion, char* otraExpresion, char* extra){

	if(strcmp(instruccion, "Declara") == 0){
		printf("Declara %s, %s\n", unaExpresion, extra);
	}
	if(strcmp(instruccion, "Suma") == 0 || strcmp(instruccion, "Resta") == 0){
		printf("%s %s, %s, %s\n", instruccion, unaExpresion, otraExpresion, extra);
	}
	if(strcmp(instruccion, "Leer") == 0 || strcmp(instruccion, "Escribir") == 0){
		printf("%s %s\n", instruccion, unaExpresion);  // leer A \n leer B || leer A, B
	}
	if(strcmp(instruccion, "Almacena") == 0){
		printf("%s %s, %s\n", instruccion, unaExpresion, otraExpresion);
	}
	if(strcmp(instruccion, "Detiene") == 0) printf("Detiene\n");
/*

	char* salidaArchivo =formatoSalida(instruccion,unaExpresion,otraExpresion,extra);
	fwrite(salidaArchivo,strlen(salidaArchivo),1,salida);
	free(salidaArchivo);
	*/

}

char * gen_infijo(){
	/* agarra el ID o CTE que esta en otroBuffer y lo guarda en un registro
	  para despues generar suma o resta -- Al pedo hacer un registro me parece	 */
	char * infijo = strdup(otroBuffer);
	return infijo;
}
