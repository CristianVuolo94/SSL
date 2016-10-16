
#include "compilador.h"

int main(int argc, char** argv) {


//	validaciones(argc,argv[1]);
//	procesarScript(argv[1]);
//	inicializarArchivoSalida(argv[2]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}*/
	char * prueba = strdup("inicio\n@\nfin");
	/* por alguna razon si en leer hay 2
															   	   	   	   	    argumentos el scanner despues de
															   	   	   	   	    detectar fin lee basura en vez de \0  */
	lawea(prueba);
	parser();



/*	acordarse de liberar lista(tabla) */
	free(prueba);
	free(vg_script); /*Hacer mas lindo later*/
//	fclose(salida);

return 1;

}



