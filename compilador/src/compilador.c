
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

int automata(char s){

	vg_estado = tabla[vg_estado][columna(s)];

	//se ingresó un numero o letra
	if(vg_estado == 1 || vg_estado == 3){
		buffer[vg_desp] = s;
		vg_desp++;
	}

	//se ingresó palabra reservada o identificador
	if(vg_estado == 2){
		if(esPalabraReservada(buffer)) hacerAlgo(buffer);
		if(esIdentificadorCorreto(buffer)) agregarATS(buffer);
		else printf("IDENTIFICADOR INCORRECTO: %s \n", &buffer);
		limpiarBuffer();
		buffer[vg_desp] = s;
	}

	//se ingresó una constante numerica
	if(vg_estado == 4){
		agregarATS(buffer);
		limpiarBuffer();
		buffer[vg_desp] = s;
	}

	//se detectó FDT
	if(vg_estado == 13){
		return FDT;
	}

	//error lexico
	if(vg_estado == 14 || vg_estado == 99) return -1;
	else return 1;

	if((vg_estado > 4 && vg_estado < 11) || (vg_estado == 12)){ //ESTO ES LO MISMO QUE TODO LO COMENTADO ABAJO
		limpiarBuffer();
		if(vg_estado == 5) return SUMA;
		if(vg_estado == 6) return RESTA;
		if(vg_estado == 7) return PARENIZQUIERDO;
		if(vg_estado == 8) return PARENDERECHO;
		if(vg_estado == 9) return COMA;
		if(vg_estado == 10) return PUNTOYCOMA;
		if(vg_estado == 12) return ASIGNACION;
	}

	if(vg_estado == 11){
			buffer[vg_desp] = s;
			vg_desp ++;
	}

	/*if(vg_estado == 5){
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
	}*/
}

int columna(char c){

	if(isalpha(c)) return 0;
	if(isdigit(atoi(c))) return 1;

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
	}
	return -1;
}

void limpiarBuffer(void){
	int i;
	for(i=0; 32; i++){
		buffer[i] = ' ';
	}
	vg_desp=0;
}
