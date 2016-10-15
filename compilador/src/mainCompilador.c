
#include "compilador.h"

int main(int argc, char** argv) {


//	validaciones(argc,argv[1]);
//	procesarScript(argv[1]);
	inicializarTablaSimbolos();

	/*while(vg_script[vg_script_desp] != '\0'){
		retornoScanner(scanner());
	}*/
	char * prueba = "inicio\nleer(a);\na:=2+3;\nb:=a-1;\nescribir(b);\nfin"; /* por alguna razon si en leer hay 2
															   	   	   	   	    argumentos el scanner despues de
															   	   	   	   	    detectar fin lee basura en vez de \0  */
	lawea(prueba);
	parser();



/*	acordarse de liberar lista(tabla) */
	free(vg_script); /*Hacer mas lindo later*/

return 1;

}



