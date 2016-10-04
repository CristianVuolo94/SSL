
#include "compilador.h"

int main(int argc, char** argv) {

	validaciones(argc,argv[1]);
	procesarScript(argv[1]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}*/

	parser();


/*	acordarse de liberar lista(tabla) */
	free(vg_script); /*Hacer mas lindo later*/

return 1;

}



