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

auto makeGapJunctionIP(uint nsize){return GapJunctionIP{nsize};}

int main(int argc, char *argv[]){

  Simulation sim(argc,argv);
  std::ofstream outFile;

  socketComm comm(sim._server,std::stoi(sim._server));

    open_outputFile(outFile,sim._N_Size);
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
  std::cout<<std::dec << "Time: "<< avgElapsed<<"\n";
  std::cout<<std::dec << "Standard Deviation: "<< standardDeviation<<"\n";
  return 0;
}

