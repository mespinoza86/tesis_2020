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
#include <linux/rpmsg.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <linux/rpmsg.h>

////////////////////////////////////////////
/// For RTOS
////////////////////////////////////////////
//////////////////////////////////////////


auto makeGapJunctionIP(uint nsize){return GapJunctionIP{nsize};}

int main(int argc, char *argv[]){

///////////////////

  
  int _count=0;
	
  Simulation sim(argc,argv);
  std::ofstream outFile;

  socketComm comm(sim._server,std::stoi(sim._server));

    open_outputFile(outFile,sim._N_Size);
  auto times = std::vector<int>(sim._simSteps);
//  auto begin = std::chrono::system_clock::now();
  auto gjip = makeGapJunctionIP(sim._N_Size);
  gjip.copyCondMatrix(sim._Connectivity_Matrix);
  double time_spent2[sim._simSteps];
  for(int step=0;step<sim._simSteps;step++){
    auto begin = std::chrono::system_clock::now();
    sim.generateStimulus(step);
    //std::cout<<"Step: "<< step<<"\n";
    sim.runStep(gjip);    
    _count++;
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    times[step]=elapsed.count();
    appendToFileStim(outFile,step,sim._iAppArray[0]);
    appendToFileAxon(outFile,sim._cellOut);
    //sendtoSocket(comm,sim.cellOut);
    //outFile<<std::endl;
  }
  //auto end = std::chrono::system_clock::now();
  //auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  //times[0]=elapsed.count();

  close_outputFile(outFile);
  auto avgElapsed = average(times);
  auto standardDeviation = standard_deviation(times);


  std::cout<<std::dec << "Time: "<< avgElapsed<<"\n";
  std::cout<<std::dec << "Standard Deviation: "<< standardDeviation<<"\n";
    
  return 0;
}

