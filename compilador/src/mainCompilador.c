
#include "compilador.h"



int main(int argc, char** argv) {

	Tsimbolo vec[10];

	Tsimbolo simbolo;

	simbolo.lexema = strdup("A");
	simbolo.token = strdup("identificador");
	vec[0]=simbolo;
	printf("%s",vec[0].lexema);

	//validaciones(argc,argv[1]);
	//procesarScript(argv[1]);





	//free(vg_script);//Hacer mas lindo later
return 1;

}



