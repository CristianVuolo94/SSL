
#include "compilador.h"



int main(int argc, char** argv) {



	validaciones(argc,argv[1]);
	procesarScript(argv[1]);
	inicializarTablaSimbolos();



	//acordarse de liberar lista(tabla)
	free(vg_script);//Hacer mas lindo later
return 1;

}



