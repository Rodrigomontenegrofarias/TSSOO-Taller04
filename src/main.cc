#include <global.h>
#include <checkArgs.hpp>

//Variables tippo//
uint64_t* serial_OMP = nullptr;
uint64_t* paralelo_OMP = nullptr;
//variables sumas
uint64_t sumaParalela_OMP= 0;
uint64_t sumaSerial_OMP = 0;

//sumado 
void suma_OMP(size_t tipo, size_t Nhilos, size_t tamanoarreglo){		
	switch(tipo){
		case 0: 
				#pragma omp parallel for reduction(+:sumaParalela_OMP) num_threads(Nhilos)
				for(size_t i = 0; i < tamanoarreglo; ++i){
					sumaParalela_OMP+= paralelo_OMP[i];
				}				
			break;
		case 1:
				#pragma omp parallel for reduction(+:sumaSerial_OMP) num_threads(Nhilos)
				for(size_t i = 0; i < tamanoarreglo; ++i){
					sumaSerial_OMP += paralelo_OMP[i];
				}
			break;
	}
}

//llenado dependiendo del caso si es 
void llenado_OMP(size_t tipo, size_t Nhilos, size_t tamanoarreglo,size_t inf, size_t sup){

	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<uint32_t> nRandom(inf, sup);
	uint32_t number = nRandom(rng);	
	switch (tipo)
	{
		case 0:		
				#pragma omp parallel for  num_threads(Nhilos)
				for(size_t i = 0; i < tamanoarreglo; ++i){
					paralelo_OMP[i] =number;
				}
		break;
		case 1:		
				#pragma omp parallel for  num_threads(1)
				for(size_t i = 0; i < tamanoarreglo; ++i){
					serial_OMP[i] = number;				
				}
		break;		
	}
}
	//Metodomain el cual obtieme los datos ingresados desdecomando//////////////////////////////////
int main(int argc, char** argv){
	
	uint64_t tamanoArreglo;
	uint64_t Nhilos;
	uint64_t sup;
	uint64_t inf;
	
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	tamanoArreglo = argumentos->getArgs().tamanoarregloeglo;
	Nhilos = argumentos->getArgs().Nhilos;
	sup = argumentos->getArgs().sup;
	inf = argumentos->getArgs().inf;
    
    //Mostrar datos ingrsados/////////////////////////////////////////////////////

    std::cout << "Datos Ingresados" << std::endl;	
	std::cout << "Tamaño del Arreglo: " << tamanoArreglo << std::endl;
	std::cout << "Numero de Nhilos : " << Nhilos << std::endl;
	std::cout << "Numero de limite inferior: " << inf << std::endl;
	std::cout << "Numero de limite superior  : " << sup << std::endl;

	serial_OMP = new uint64_t[tamanoArreglo];
	paralelo_OMP = new uint64_t[tamanoArreglo];;

	//------------------------------------------------------------------------------------//
	//modulo1
	//Proceso en Paralelo/////////////////////////////////////////////////////////////////////////////
	//llenado OMP PARALELO////////////////////////////////////////////////////////////////////////////

	auto start = std::chrono::high_resolution_clock::now();
	llenado_OMP(0,Nhilos,tamanoArreglo,inf,sup);
	auto end     = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeLLENADO_OMP_P = elapsed.count();
	//modulo2
	//SUMA OMP PARALELO////////////////////////////////////////////////////////////////////////////////

	start = std::chrono::high_resolution_clock::now();
	suma_OMP(0,Nhilos,tamanoArreglo);
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSUMA_OMP_P = elapsed.count();
	//FinProceso en Paralelo/////////////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------------------//




	//------------------------------------------------------------------------------------//
	//Proceso en serial////////////////////////////////////////////////////////////////////
	//LlenadoOMPSERIAL/////////////////////////////////////////////////////////////////////

	start = std::chrono::high_resolution_clock::now();
	llenado_OMP(1,1,tamanoArreglo,inf,sup);
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeLLENADO_OMP_S = elapsed.count();


	//SUMAOMPSERIAL//////////////////////////////////////////////////////////////////////////////

	start = std::chrono::high_resolution_clock::now();
	suma_OMP(1,1,tamanoArreglo);
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSUMA_OMP_S = elapsed.count();
	//FINProceso en serial////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------------------//
	//suma total
    float sumaparalelo = (totalTimeSUMA_OMP_P + totalTimeLLENADO_OMP_P);
    float sumaserial = ( totalTimeSUMA_OMP_S + totalTimeSUMA_OMP_S); 
	//DESEMPEÑO
    float speedUp = (sumaparalelo/sumaserial);
    float eficiencia = 1/(1+(sumaparalelo/sumaserial));  


    //Resultados///////////////////////////////////////////////////////////////////////////////////
	
	std::cout << "Resultados de sumas OMP:" << std::endl;	
	std::cout << "Valor total de suma paralelo OMP :"  << sumaParalela_OMP << std::endl;
	std::cout << "Valor total de suma serial OMP :"  << sumaSerial_OMP << std::endl;
	std::cout << "Tiempos de llenado OMP: " << std::endl;
	std::cout << "Tiempo llenado paralelo OMP  :"  << totalTimeLLENADO_OMP_P << "[ms]" << std::endl;
	std::cout << "Tiempo llenado serial OMP  :"  << totalTimeLLENADO_OMP_S << "[ms]" << std::endl;
    std::cout << "Tiempos de sumas OMP: "  << std::endl;
	std::cout << "Tiempo suma paralela OMP  :"  << totalTimeSUMA_OMP_P << "[ms]" << std::endl;
	std::cout << "Tiempo suma serial OMP :"  << totalTimeSUMA_OMP_S << "[ms]" << std::endl;
    std::cout << "Tiempos suma total :"  << std::endl;
	std::cout << "Tiempo total de suma paralela OMP  :"  << sumaparalelo<< "[ms]" << std::endl;
	std::cout << "Tiempo total de suma serial OMP:"  << sumaserial << "[ms]" << std::endl;
	std::cout << "Desempeñpos "  << std::endl;
	std::cout << "rapidez :"  << speedUp << "[ms]" << std::endl;
	std::cout << "eficiencia:"  << eficiencia << "[ms]" << std::endl;


	//Finderesultados///////////////////////////////////////////////////////////////////////////////////
	return(EXIT_SUCCESS);
}

