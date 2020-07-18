#include <vector>
#include <functional>
#include <cmath>
#include <random>
#include <algorithm>

struct eventInfo {
      using ArrRange = std::array<unsigned int,2>;
      unsigned int begin=0;
      unsigned int end=0;
      ArrRange range = ArrRange{0,0};
};
struct stimDC {
      float amplitude=1;
      eventInfo info;
};
struct stimAC {
      float frequency=1;
      int offset=0;
      float amplitude=1;
      eventInfo info;
};
struct stimNoise {
      float mean=5;
      float std=2;
      eventInfo info;
};
struct stims {
      std::vector<stimAC> AC;
      std::vector<stimDC> DC;
      std::vector<stimNoise> Noise;
};
struct q_event {
      using ftype =
            std::function<void(std::vector<float> &,int)>;
      ftype runEvent;
      ftype stopEvent;
      unsigned int begin_step=0;
      unsigned int end_step=0;
      q_event(ftype f,unsigned int begin,unsigned int end) :
      runEvent(f), begin_step(begin), end_step(end){
            stopEvent = [](std::vector<float> &c,int i){
                  auto resetFunc = [](auto &e){e=0;};
                  std::for_each(c.begin(),c.end(),resetFunc);
            };
      }
};

auto makeStimACLambda(const stimAC &stim,const float simStep){
      const auto w = stim.frequency/simStep;
      const auto range{stim.info.range};
      const auto a = stim.amplitude;
      const auto o = stim.offset;
      auto returnFunc =
            [=](std::vector<float> &currInj,const int i){
                  auto calcSine =
                  [&](float &c){
                        c = a*std::sin(w*i+o);
                  };
                  const auto begin = currInj.begin()+range[0];
                  const auto end = currInj.begin()+range[1];
                  std::for_each(begin,end,calcSine);
      };
      return returnFunc;
}

auto makeStimDCLambda(const stimDC &stim,const float simStep){
      const auto range{stim.info.range};
      const auto a = stim.amplitude;
      auto returnFunc =
            [=](std::vector<float> &currInj,const int i){
                  auto calcStep =
                        [&](float &c){
                              c = a;
                        };
                  const auto begin = currInj.begin()+range[0];
                  const auto end = currInj.begin()+range[1];
                  std::for_each(begin,end,calcStep);
            };
      return returnFunc;
}

auto makeStimNoiseLambda(stimNoise &stim,float simStep){
      auto generator = std::default_random_engine{};
      const auto range{stim.info.range};
      const auto mean = stim.mean;
      const auto std = stim.std;
      auto distribution = std::normal_distribution<float>{mean,std};
      auto returnFunc =
      [=](std::vector<float> &currInj,int i) mutable {
            const auto begin = currInj.begin()+range[0];
            const auto end = currInj.begin()+range[1];
            auto calcNormalDist =
                  [&](float &c)mutable{
                        c = distribution(generator);
                  };
            std::for_each(begin,end,calcNormalDist);
      };
      return returnFunc;
}

void stimPattern(
      std::vector<q_event> &eventQueue,
      std::vector<float> &currInj,
      const unsigned int i
){
      static auto buff = std::vector<float>{currInj};
      auto clearBuff =
            [](std::vector<float>&vec){
                  for(auto &elem : vec){elem=0;}
            };
      clearBuff(buff);
      using std::transform;
      for(auto &ev : eventQueue){
            if(ev.begin_step<=i && i<ev.end_step) {
                  ev.runEvent(buff,i);
            }else if(i==ev.end_step) {
                  ev.stopEvent(buff,i);
            }
            transform(
                  buff.begin(),buff.end(),
                  buff.begin(),currInj.begin(),
                  std::plus<float>()
            );
      }
}

void generateStimulus(float simStep,stims stimulus,std::vector<q_event> &eventQueue,std::vector<float> &currInj){
      for(auto &ac : stimulus.AC) {
            auto q1 = q_event{makeStimACLambda(ac,simStep),
                  ac.info.begin, ac.info.end};
            eventQueue.push_back(q1);
      }
      for(auto &dc : stimulus.DC) {
            auto q2 = q_event{makeStimDCLambda(dc,simStep),
                  dc.info.begin, dc.info.end};
            eventQueue.push_back(q2);
      }
      for(auto &noise : stimulus.Noise) {
            auto q3 = q_event{makeStimNoiseLambda(noise,simStep),
                  noise.info.begin, noise.info.end};
            eventQueue.push_back(q3);
      }
}
