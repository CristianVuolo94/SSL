
#include "compilador.h"
int vg_estado = 0;
int script_desp=0;
char buffer[35];
int buffer_desp = 0;
int tabla [15][13] = 	   {{1,3,5,6,7,8,9,10,11,14,13,0},
							{1,1,2,2,2,2,2,2,2,2,2,2},
							{99,99,99,99,99,99,99,99,99,99,99,99},
							{4,3,4,4,4,4,4,4,4,4,4,4},
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
	simbolo->lexema=strdup("leer");
	simbolo->token=strdup("Palabra Reservada");
	list_add(tablaDeSimbolos,simbolo);

	t_simbolo* simbolo2=malloc(sizeof(t_simbolo));
	simbolo2->lexema=strdup("escribir");
	simbolo2->token=strdup("Palabra Reservada");
	list_add(tablaDeSimbolos,simbolo2);

	t_simbolo* simbolo3=malloc(sizeof(t_simbolo));
	simbolo3->lexema=strdup("inicio");
	simbolo3->token=strdup("Palabra Reservada");
	list_add(tablaDeSimbolos,simbolo3);

	t_simbolo* simbolo4=malloc(sizeof(t_simbolo));
	simbolo4->lexema=strdup("fin");
	simbolo4->token=strdup("Palabra Reservada");
	list_add(tablaDeSimbolos,simbolo4);
}

int esPalabraReservada(char *buffer){

	int desplazamiento;

	for(desplazamiento=0;list_size(tablaDeSimbolos);desplazamiento++){

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
	t_simbolo* simbolo = malloc(sizeof(t_simbolo));
	strcpy(simbolo->lexema,buffer);
	simbolo->token=strdup("Identificador");
	list_add(tablaDeSimbolos,simbolo);
}
void agregarConstanteATS(char* buffer){
	t_simbolo* simbolo = malloc(sizeof(t_simbolo));
	strcpy(simbolo->lexema,buffer);
	simbolo->token=strdup("Constante");
	list_add(tablaDeSimbolos,simbolo);
}


TOKEN automata(){

	while(1){
		printf("ESTADOA=%d\n",vg_estado);
		printf("LETRA%c\n",vg_script[script_desp]);
		vg_estado = tabla[vg_estado][columna(vg_script[script_desp])];
		printf("ESTADOB=%d\n",vg_estado);
		//se ingresó un numero o letra
		if(vg_estado == 1){

			buffer[buffer_desp] = vg_script[script_desp];
			buffer_desp++;

		}

		if(vg_estado == 3){

				buffer[buffer_desp] = vg_script[script_desp];
				buffer_desp++;
		}


		//se ingresó palabra reservada o identificador
		if(vg_estado == 2){
			if(esPalabraReservada(buffer)){

				if(strcmp(buffer,"leer")==0){
					script_desp--;
					limpiarBuffer();
					return LEER;
				}

				if(strcmp(buffer,"escribir")==0){
					script_desp--;
					limpiarBuffer();
					return ESCRIBIR;
				}

				if(strcmp(buffer,"inicio")==0){

					script_desp--;

					limpiarBuffer();

					return INICIO;
				}

				if(strcmp(buffer,"fin")==0){
					//script_desp--;
					printf("BARRAENE %c ashpa\n",vg_script[script_desp]);
					limpiarBuffer();
					return FIN;
				}
			}


			if(esIdentificadorCorreto(buffer)==1){
				script_desp--;
				agregarIdentificadorATS(buffer);
				limpiarBuffer();
				return ID;
			}
			else printf("IDENTIFICADOR INCORRECTO: %s \n", buffer);


		}

		//se ingresó una constante numerica
		if(vg_estado == 4){
			script_desp--;
			agregarConstanteATS(buffer);
			limpiarBuffer();
			return CONSTANTE;

		}

		//se detectó FDT
		if(vg_estado == 13){
			return FDT;
		}

		//error lexico
		if(vg_estado == 14 || vg_estado == 99) return -1;
//		else return 1;// para que?

		if((vg_estado > 4 && vg_estado < 11) || (vg_estado == 12)){


			if(vg_estado == 5){
				limpiarBuffer();
				return SUMA;
			}
			if(vg_estado == 6){
				limpiarBuffer();
				return RESTA;
			}
			if(vg_estado == 7){
				limpiarBuffer();
				return PARENIZQUIERDO;
			}
			if(vg_estado == 8){
				limpiarBuffer();
				return PARENDERECHO;
			}
			if(vg_estado == 9){
				limpiarBuffer();
				return COMA;
			}
			if(vg_estado == 10){
				limpiarBuffer();
				return PUNTOYCOMA;
			}
			if(vg_estado == 12){
				limpiarBuffer();
				return ASIGNACION;
			}
		}
		if(vg_estado == 11){
			buffer[buffer_desp] = vg_script[script_desp];
			buffer_desp ++;
		}
		script_desp++;
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
