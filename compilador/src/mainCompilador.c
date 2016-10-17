
#include "compilador.h"

int main(int argc, char** argv) {


	validaciones(argc,argv[1]);
	procesarScript(argv[1]);
	inicializarArchivoSalida(argv[2]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}*/
/*	char * prueba = strdup("inicio\n@\na:=82;\nfin");
	pruebas(prueba);*/

	parser();



/*	acordarse de liberar lista(tabla)
	free(prueba);*/
	free(vg_script);
	fclose(salida);

return 0;

}



