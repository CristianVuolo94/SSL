
#include "compilador.h"


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

int vg_estado = 0;
char buffer[33];
int i = 0;

int automata(char s){

	vg_estado = tabla[vg_estado][columna(s)];

	if(vg_estado == 1 || vg_estado == 3){
		buffer[i] = s;
		i++;
	}
	if(vg_estado == 2){
		if(esPalabraReservada(buffer)) hacerAlgo(buffer);
		if(esIdentificadorCorreto(buffer)) agregarATS(buffer);
		else printf("IDENTIFICADOR INCORRECTO: %s \n", &buffer);
		limpiarBuffer();
		buffer[i] = s;
	}
	if(vg_estado == 14 || vg_estado == 99) return 0;
	else return 1;
}

int columna(char c){
	if(isdigit(atoi(c))) return 1;
	if(isalpha(c)) return 0;
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
	case FDT: //nuestro FDT seria '\0' ??
		return 10;
		break;
	case ' ':
		return 11;
		break;
	}
	return -1;
}

void limpiarBuffer(void){
	int j;
	for(j=0; 32; j++){
		buffer[j] = ' ';
	}
}
