
#include "compilador.h"

int main(int argc, char** argv) {


//	validaciones(argc,argv[1]);
//	procesarScript(argv[1]);
//	inicializarArchivoSalida(argv[2]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}*/
	char * prueba = strdup("inicio\na:=4;\nfin");
	pruebas(prueba);
	puts(vg_script);
	parser();



//	acordarse de liberar lista(tabla)
	free(prueba);
	free(vg_script);
//	fclose(salida);

return 0;

}



