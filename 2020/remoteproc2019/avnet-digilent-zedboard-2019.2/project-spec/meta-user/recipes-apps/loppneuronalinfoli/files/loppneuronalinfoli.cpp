#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <memory>
#include <array>
#include <vector>
#include <algorithm>
#include <thread>
#include "stepSimulate.hpp"
#include "statistics.hpp"
#include <stdio.h>
#include <stdlib.h>

float total_sum=0;
float t_average=0;
float t_value[100];
int _N_size_;
int _simSteps_;
const int size_sim = 100;

auto makeGapJunctionIP(uint nsize){return GapJunctionIP{nsize};}

int main(int argc, char *argv[]){
  FILE *fptr;
  fptr = fopen("results.log","w");

  if(fptr == NULL){
      printf("Error!");   
      exit(1);             
  }

  fprintf(fptr,"Iniciando archivo de datos para iteracion de 100\n");

  for (int i=1; i<10; i++){
              total_sum = 0;

	  for (int j=0; j<100; j++){

	      Simulation sim(argc,argv);
	      std::ofstream outFile;
	      socketComm comm(sim._server,std::stoi(sim._server));
	      open_outputFile(outFile,sim._N_Size);
              sim._N_Size = size_sim*i;
              _N_size_ = sim._N_Size;
	      auto times = std::vector<int>(sim._simSteps);
	      auto gjip = makeGapJunctionIP(sim._N_Size);
              gjip.copyCondMatrix(sim._Connectivity_Matrix);
		  for(int step=0;step<sim._simSteps;step++){
		    sim.generateStimulus(step);
   	 	    //mespinoz   std::cout<<"Step: "<< step<<"\n";
		    auto begin = std::chrono::system_clock::now();
		    sim.runStep(gjip);
		    auto end = std::chrono::system_clock::now();
		    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
		    times[step]=elapsed.count();
		    appendToFileStim(outFile,step,sim._iAppArray[0]);
		    appendToFileAxon(outFile,sim._cellOut);
		    //sendtoSocket(comm,sim.cellOut);
		    //outFile<<std::endl;
		  }
	     close_outputFile(outFile);
             auto avgElapsed = average(times);
             auto standardDeviation = standard_deviation(times);
              //std::cout<<std::dec << "Time: "<< avgElapsed<<"\n";
             //std::cout<<std::dec << "Standard Deviation: "<< standardDeviation<<"\n";
              total_sum = total_sum + avgElapsed;
              t_value[j] = avgElapsed;

              }

           t_average = total_sum/100;
           float estandar_Desv = 0;
           for(int k=0; k<100;k++){
                estandar_Desv = estandar_Desv + ((t_value[k] - t_average)*(t_value[k] - t_average));
           }
  
	  estandar_Desv = sqrt(estandar_Desv/100);
	  std::cout<<std::dec << "Red neuronal 2: "<< _N_size_ << "\n";
	  std::cout<<std::dec << "Total: "<< total_sum<<"\n";
	  std::cout<<std::dec << "Average: "<< t_average<<"\n";
	  std::cout<<std::dec << "Desv Estandar: "<< estandar_Desv<<"\n";
          fprintf(fptr,"Tamano de red: %d\n",_N_size_);
	  fprintf(fptr,"Total: %f\n", total_sum);
          fprintf(fptr,"Average: %f\n", t_average);
          fprintf(fptr,"Desv Estandar: %f\n", estandar_Desv);

    } 
  fprintf(fptr,"Fin de archivo\n");
 
  fclose(fptr);
  return 0;
}

