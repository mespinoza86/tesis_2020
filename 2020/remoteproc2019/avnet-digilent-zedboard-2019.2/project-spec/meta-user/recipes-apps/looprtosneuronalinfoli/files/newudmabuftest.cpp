
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

auto makeGapJunctionIP(uint nsize){return GapJunctionIP{nsize};}

int main(int argc, char** argv){
  std::cout << "Init GPIP test\n";
  using vec = std::vector<float>;
  auto runTest = [&](const vec &V,const vec &C,vec &outCurrents, std::vector<int> &times){
    auto nsize=V.size();
    std::cout << "Opening GapJunctionIP Driver devs...\n";
    auto gjip = makeGapJunctionIP(nsize);
    std::cout << "Ready\n";
    gjip.copyCondMatrix(C);
    std::cout << "Ready this flow 1\n";


    for(auto &t : times){
      std::cout << "Ready this flow 2\n";
      auto begin = std::chrono::system_clock::now();
            std::cout << "Ready this flow 3\n";
      gjip.copyVdend(V);
            std::cout << "Ready this flow 4\n";
      gjip.run();
      std::cout << "Ready this flow 5\n";
      uint idx = 0;
      for(auto &curr : outCurrents){curr = gjip.IcBuff(idx);idx++;}
      std::cout << "Ready this flow 6\n";
      auto end = std::chrono::system_clock::now();
      std::cout << "Ready this flow 7\n";
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
      std::cout << "Ready this flow 8\n";
      t=elapsed.count();
      std::cout << "Ready this flow 9\n";
    }
     std::cout << "Ready this flow 10\n";

  };
  const std::vector<uint> sizes = 
  {
	  4,
	  44,
	  64,
	  1000,
	  2000,
	  3000,
	  4000,
	  5000
  };
  for(auto &nsize : sizes ){
    const auto iterationsQuantity = uint(4);
    auto times = std::vector<int>(iterationsQuantity);
    std::cout << "\tNsize: " << nsize <<"\n";
    auto V_data = vec(nsize);
    auto C_data = vec(nsize*nsize);
    fillWithRandomData(V_data,C_data);
    auto outCurrents = vec(nsize);
    runTest(V_data,C_data,outCurrents,times);
    auto avgElapsed = average(times);
    auto standardDeviation = standard_deviation(times);
    std::cout<<std::dec << "Time: "<< avgElapsed<<"\n";
    std::cout<<std::dec << "Standard Deviation: "<< standardDeviation<<"\n";
    auto Isw = std::vector<float>(nsize);
    auto begin = std::chrono::system_clock::now();
    SimulateSW(C_data,V_data,Isw);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    auto boost = static_cast<float>(elapsed.count())/avgElapsed;
    std::cout << "SW perf: "<< elapsed.count() << "\n"<<"HW boost: "<<boost <<"\n";
    compareResults(Isw,outCurrents);
    printf("Done compare result\n");
  }
  return 0;
}

