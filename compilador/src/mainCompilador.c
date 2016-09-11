
#include "compilador.h"



int main(int argc, char** argv) {



	validaciones(argc,argv[1]);
	procesarScript(argv[1]);

	inicializarTablaSimbolos();
	int valor;
	valor = automata();
	puts("hola\n");
	printf("%d\n",valor);
	puts("chau\n");

	valor = automata();
	puts("hola2\n");
	printf("%d\n",valor);
	puts("chau2\n");

	//acordarse de liberar lista(tabla)
	free(vg_script);//Hacer mas lindo later
return 1;

}



