
#include "compilador.h"
int vg_estado = 0;
char buffer[35];
int buffer_desp = 0;
int tabla [16][14] = 	   {{1,3,5,6,7,8,9,10,11,14,13,0,99},
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
void sentencia(void);
void expresion(void);
void listaIdentificadores(void);
void listaExpresiones(void);
void primaria(void);


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
	return 1;

}



void agregarIdentificadorATS(char *buffer){

	bool perteneceLista (t_simbolo* simbolo) {return strcmp( simbolo->lexema,buffer) == 0 ; }

	if(!list_any_satisfy(tablaDeSimbolos,(void*)perteneceLista)){
		t_simbolo* simbolo = malloc(sizeof(t_simbolo));
		simbolo->lexema=(char*)strdup(buffer);
		simbolo->token = ID;
		list_add(tablaDeSimbolos,simbolo);
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
}


TOKEN scanner(){

	while(1){

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


			if(esIdentificadorCorreto(buffer)==1){
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
		if(vg_estado == 14 || vg_estado == 99) return -1;


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
	}

	return -1;
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
	else return -1;
}

void errorSintactico(){
	printf("ERROR SINTACTICO\n");
	printf("En el desplazamiento: %d\n", buffer_desp);
}

/* PROCEDIMIENTO DE ANALISIS SINTACTICO*/
/* <objetivo> -> <programa> FDT */
void objetivo(void){
	programa();
	TOKEN t = scanner();
	if(!(match(t, FDT))) errorSintactico();
}
/* <programa> -> INICIO <listaSentencias> FIN */
void programa(void){
	TOKEN t = scanner();
	if(!(match(t, INICIO))) errorSintactico();
	listaSentencias();
	if(!(match(t, FIN))) errorSintactico();
}
/*<listaSentencias> -> <sentencia> {<sentencia>} */
void listaSentencias(void){
	sentencia(); /* la primera de la lista de sentencias */
	TOKEN t;

	while (1) {  /* un ciclo indefinido */
		switch (t = scanner()) {

		case ID: case LEER: case ESCRIBIR: /* detectó token correcto */
			sentencia(); /* procesa la secuencia opcional */
			break;

		default: /*detecto algo incorrecto o FDT */
		buffer_desp--;
		return;
		} /* fin switch */
	}
}
/*<sentencia> -> ID ASIGNACIÓN <expresión> PUNTOYCOMA |
LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA |
ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA */
void sentencia(void){
	TOKEN t = scanner();
	if(!(match(t, ID) || match(t, LEER) || match(t, ESCRIBIR))) errorSintactico();

	if(match(t, ID)){
		t = scanner();
		if(!(match(t, ASIGNACION))) errorSintactico();
		expresion();
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico();
	}
	if(match(t, LEER)){
		t = scanner();
		if(!(match(t, PARENIZQUIERDO))) errorSintactico();
		listaIdentificadores();
		t = scanner();
		if(!(match(t, PARENDERECHO))) errorSintactico();
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico();
	}
	if(match(t, ESCRIBIR)){
		t = scanner();
		if(!(match(t, PARENIZQUIERDO))) errorSintactico();
		listaExpresiones();
		t = scanner();
		if(!(match(t, PARENDERECHO))) errorSintactico();
		t = scanner();
		if(!(match(t, PUNTOYCOMA))) errorSintactico();
	}
}
/* <listaIdentificadores> -> ID {COMA ID} */
void listaIdentificadores(void){
	TOKEN t = scanner();
	if(!(match(t, ID))) errorSintactico();

	t = scanner();
	if(match(t, COMA)) listaIdentificadores();
	else buffer_desp--; /* porque sino lo que hay en t se pierde */

}
/* <listaExpresiones> -> <expresión> {COMA <expresión>} */
void listaExpresiones(void){
	expresion();

	TOKEN t = scanner();
	if(match(t, COMA))	listaExpresiones();
	else buffer_desp--; /* porque sino lo que hay en t se pierde */

}
/* <expresión> -> <primaria> {<operadorAditivo> <primaria>} */
void expresion(void){
	primaria();
	TOKEN t = scanner();
	if(match(t, SUMA) || match(t, RESTA)) expresion();
	else buffer_desp--; /* porque sino lo que hay en t se pierde */

}
/* <primaria> -> ID | CONSTANTE |
PARENIZQUIERDO <expresión> PARENDERECHO */
void primaria(void){
	TOKEN t = scanner();
	if (!(t == ID || t == CONSTANTE || t == PARENIZQUIERDO)) errorSintactico();

	else if(t == PARENIZQUIERDO){
			expresion();
			t = scanner();
			if(!(match(t, PARENDERECHO))) errorSintactico();
		 }
}
/* <operadorAditivo> -> uno de SUMA RESTA -- esta hecho en expresion*/


