#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
	// 1) Modificar esta sección
	const std::string optString = "N:t:l:L:h";
	
	const std::string opciones = "-N tam_problema -t threads -n num_buscado [-h]";

	const std::string descripcion  = "Descripción:\n"
		                             "\t-N   Tamaño del arreglo\n"
									 "\t-t   numero de threads\n"
									 "\t-l   limite inferior\n"
									 "\t-L   limite superior\n"
									 "\t-h   ayuda de uso\n";
	
	typedef struct args_t{
		uint64_t tamanoarregloeglo;
		uint32_t Nhilos;
		uint32_t inf;
		uint32_t sup;
	} args_t;
	
	// 2) Modificar constructor
	// 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
	// 4) Recuerde que para compilar nuevamente, y por tratarse
	//    de un archivo header, debe hacer primero un make clean
	
	args_t  parametros;
	
	int argc;
	char **argv;

	
public:
	checkArgs(int _argc , char **_argv);
	~checkArgs();
	args_t getArgs();
	
private:
	void printUsage();
	
	
};

checkArgs::checkArgs(int _argc , char **_argv){
	parametros.tamanoarregloeglo= 0 ;
	parametros.Nhilos= 0 ;
	parametros.inf= 0 ;
	parametros.sup= 0 ;


	argc = _argc;
	argv = _argv;
	
}

checkArgs::~checkArgs(){
	
}

checkArgs::args_t checkArgs::getArgs(){
	int opcion;
	
	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 'N':
					parametros.tamanoarregloeglo = atoi(optarg);
					break;
			case 't':
					parametros.Nhilos = atoi(optarg);
					break;
			case 'l':
					parametros.inf = atoi(optarg);
					break;
			case 'L':
					parametros.sup = atoi(optarg);
					break;
			case 'h':
			default:
					printUsage();
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.tamanoarregloeglo == 0 ||
		 parametros.Nhilos  == 0 ){
		printUsage();
		exit(EXIT_FAILURE);
	}
	
	return(parametros);
}

void checkArgs::printUsage(){
	std::cout << "Uso: " <<
		argv[0] << " " << opciones << " " << descripcion << std::endl;
}



#endif
