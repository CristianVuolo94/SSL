
#include "compilador.h"


void validaciones(int argc, char * argv){

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
