
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <memory>
#include <functional>
#include <chrono>
#include <vector>
#include <random>
#include <cassert>
#include "statistics.hpp"
#include "GapJunctionDriver.hpp"
#include "ip-test.h"
#include "xtime_l.h"

 XTime HW_compute_acc = 0;
 XTime HW_init_compute = 0;
 XTime SW_compute_acc = 0;
 XTime SW_init_compute = 0;
auto makeGapJunctionIP(uint nsize){return GapJunctionIP{nsize};}
using vec = std::vector<float>;

/*
auto runTest = [&](const vec &V,const vec &C,vec &outCurrents, std::vector<int> &times){
  auto nsize=V.size();
  std::cout << "Opening GapJunctionIP Driver devs...\n";
  XTime_GetTime(&HW_init_compute);
  auto gjip = makeGapJunctionIP(nsize);
  std::cout << "Ready\n";
  gjip.copyCondMatrix(C);
  for(auto &t : times){
  	//mespinoz auto begin = std::chrono::system_clock::now();
    gjip.copyVdend(V);
    gjip.run();
    uint idx = 0;
    for(auto &curr : outCurrents){curr = gjip.IcBuff(idx);idx++;}

    //mespinoz auto end = std::chrono::system_clock::now();
    //mespinoz auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    //mespinoz t=elapsed.count();
  }
  XTime_GetTime(&HW_compute_acc);
  HW_compute_acc = HW_compute_acc - HW_init_compute;
  float print_fp=((float)HW_compute_acc)/CLOCKS_PER_SEC;
  printf("HW Time exec: %f\n",print_fp);


};
*/

const auto iterationsQuantity = uint(4);
auto times = std::vector<int>(iterationsQuantity);
int n_size=100;
auto V_data = vec(n_size);
auto C_data = vec(n_size*n_size);
auto outCurrents = vec(n_size);
auto gjip = makeGapJunctionIP(n_size);

int main(int argc, char** argv){
  std::cout << "Init GPIP test\n";

  const std::vector<uint> sizes = 
  {
	  100,
  };

    std::cout << "\tNsize: " << n_size <<"\n";
    fillWithRandomData(V_data,C_data);
    /////////////////////////////////////////////////////
    // runTest(V_data,C_data,outCu rrents,times);.
    std::cout << "Opening GapJunctionIP Driver devs...\n";
    XTime_GetTime(&HW_init_compute);

    std::cout << "Ready\n";
    gjip.copyCondMatrix(C_data);

    //for(auto &t : times){
    gjip.copyVdend(V_data);
    gjip.run();
    uint idx = 0;
    for(auto &curr : outCurrents){curr = gjip.IcBuff(idx);idx++;}
    //}

    XTime_GetTime(&HW_compute_acc);
    HW_compute_acc = HW_compute_acc - HW_init_compute;
    float print_fp=((float)HW_compute_acc)/CLOCKS_PER_SEC;
    printf("HW Time exec: %f\n",print_fp);
    /////////////////////////////
    auto avgElapsed = average(times);
    auto standardDeviation = standard_deviation(times);
    std::cout<<std::dec << "Time: "<< avgElapsed<<"\n";
    std::cout<<std::dec << "Standard Deviation: "<< standardDeviation<<"\n";

    for(auto &nsize : sizes ){
    	auto Isw = std::vector<float>(nsize);
    	XTime_GetTime(&SW_init_compute);
    	SimulateSW(C_data,V_data,Isw);
    	XTime_GetTime(&SW_compute_acc);
    	SW_compute_acc = SW_compute_acc - SW_init_compute;
    	float print_swfp=((float)SW_compute_acc)/CLOCKS_PER_SEC;
    	printf("SW Time exec: %f\n",print_swfp);
    	//compareResults(Isw,outCurrents);
    	printf("Done compare result\n");
  }
  return 0;
}
