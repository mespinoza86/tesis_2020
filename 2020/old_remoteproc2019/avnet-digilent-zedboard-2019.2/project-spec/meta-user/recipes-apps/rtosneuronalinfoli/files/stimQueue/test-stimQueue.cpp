
#include <iostream>
#include "stimQueue.hpp"


int main(int argc, char** argv){

        float simStep = 0.01;
        float simTime=0.4;

        stims stimulus;

        stimAC AC;
        stimDC DC;
        stimNoise Noise;

        AC.info.range = {0,2};
        DC.info.range = {3,4};
        Noise.info.range = {4,5};

        AC.info.end = 10;
        DC.info.end = 15;
        Noise.info.end = 20;


        stimulus.AC.push_back(AC);
        stimulus.DC.push_back(DC);
        stimulus.Noise.push_back(Noise);

        std::vector<q_event> eventQueue;
        std::vector<float> currInj({0,0,0,0,0});

        generateStimulus(simStep, stimulus,eventQueue,currInj);

        //Generate the stimulus pattern_____________________________________________
        int simSteps = (int) (simTime/simStep);
        for(int i=0; i < simSteps; i++) {
                stimPattern(eventQueue,currInj,i);
                for (auto &e : currInj) {
                  std::cout << e <<" ";
                }
                std::cout <<"\n";
        }
        return 0;
}
