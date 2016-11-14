
#include "compilador.h"

int main(int argc, char** argv) {


	validaciones(argc,argv[1]);
	procesarScript(argv[1]);
	inicializarArchivoSalida(argv[2]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}

	char * prueba = strdup("inicio A :=BB -34+A; fin");
	pruebas(prueba);
	puts(vg_script);*/
	parser();

	liberarRecursos();


return 0;

}



