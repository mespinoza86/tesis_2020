#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON



#include <string>
#include <iostream>
#include "stimQueue/stimQueue.hpp"   //Include for test getSimSpec
#include <fstream>
#include <memory>

namespace parserNetwork{

template<typename T=float>
struct Dend {
      T V_dend;
      T Hcurrent_q;
      T Calcium_r;
      T Potassium_s;
      T I_CaH;
      T Ca2Plus;
};

template<typename T=float>
struct Soma {
      T g_CaL;
      T V_soma;
      T Sodium_m;
      T Sodium_h;
      T Calcium_k;
      T Calcium_l;
      T Potassium_n;
      T Potassium_p;
      T Potassium_x_s;
};

template<typename T=float>
struct Axon {
      T V_axon;
      T Sodium_m_a;
      T Sodium_h_a;
      T Potassium_x_a;
};

template<typename T=float>
struct GapJunction {
      T Conductance;
      T Connectivity;
};

struct NeuralModel {
      virtual void printInit()const=0;
      virtual std::string modelName()const=0;
      virtual ~NeuralModel()=default;
};

struct  eHH : public NeuralModel{
      Axon<> axon;
      Dend<> dendrite;
      Soma<> soma;
      GapJunction<> gj;
      virtual ~eHH()=default;
      void printInit()const{
            std::cout   <<"dendritic: " << "----------------"<<"\n"
                        << "v_dend: " << dendrite.V_dend<<"\n"
                        <<"Calcium_r: " << dendrite.Calcium_r<<"\n"
                        <<"Potassium_s: " << dendrite.Potassium_s<<"\n"
                        <<"Hcurrent_q: " << dendrite.Hcurrent_q<<"\n"
                        <<"Ca2Plus: " << dendrite.Ca2Plus<<"\n"
                        <<"I_CaH: " << dendrite.I_CaH<<"\n"
                        <<"somatic: " << "----------------"<<"\n"
                        <<"g_CaL: " << soma.g_CaL<<"\n"
                        <<"V_soma: " << soma.V_soma<<"\n"
                        <<"Sodium_m: " << soma.Sodium_m<<"\n"
                        <<"Sodium_h: " << soma.Sodium_h<<"\n"
                        <<"Potassium_n: " << soma.Potassium_n<<"\n"
                        <<"Potassium_p: " << soma.Potassium_p<<"\n"
                        <<"Potassium_x_s: " << soma.Potassium_x_s<<"\n"
                        <<"Calcium_k: " << soma.Calcium_k<<"\n"
                        <<"Calcium_l: " << soma.Calcium_l<<"\n"
                        <<"axonal: " << "----------------"<<"\n"
                        <<"V_axon: " << axon.V_axon<<"\n"
                        <<"Sodium_m_a: " << axon.Sodium_m_a<<"\n"
                        <<"Sodium_h_a: " << axon.Sodium_h_a<<"\n"
                        <<"Potassium_x_a: " << axon.Potassium_x_a<<"\n"
                        <<"Connectivity: " << gj.Connectivity<<"\n"
                        <<"Conductance: " << gj.Conductance<<"\n";
      }
      void getDendrite(const rapidjson::Value& Model){
            dendrite.V_dend= atof(Model["dendrite"]["v_dend"].GetString());
            dendrite.Calcium_r= atof(Model["dendrite"]["Calcium_r"].GetString());
            dendrite.Potassium_s= atof(Model["dendrite"]["Potassium_s"].GetString());
            dendrite.Hcurrent_q= atof(Model["dendrite"]["Hcurrent_q"].GetString());
            dendrite.Ca2Plus= atof(Model["dendrite"]["Ca2Plus"].GetString());
            dendrite.I_CaH= atof(Model["dendrite"]["I_CaH"].GetString());
      }
      void getSoma(const rapidjson::Value& Model){
            soma.V_soma= atof(Model["soma"]["v_soma"].GetString());
            soma.g_CaL= atof(Model["soma"]["g_Cal"].GetString());
            soma.Sodium_m= atof(Model["soma"]["Sodium_m"].GetString());
            soma.Sodium_h= atof(Model["soma"]["Sodium_h"].GetString());
            soma.Potassium_n= atof(Model["soma"]["Potassium_n"].GetString());
            soma.Potassium_p= atof(Model["soma"]["Potassium_p"].GetString());
            soma.Potassium_x_s= atof(Model["soma"]["Potassium_x_s"].GetString());
            soma.Calcium_k= atof(Model["soma"]["Calcium_k"].GetString());
            soma.Calcium_l= atof(Model["soma"]["Calcium_l"].GetString());
      }
      void getAxon(const rapidjson::Value& Model){
            axon.V_axon= atof(Model["axon"]["v_axon"].GetString());
            axon.Sodium_m_a= atof(Model["axon"]["Sodium_m_a"].GetString());
            axon.Sodium_h_a= atof(Model["axon"]["Sodium_h_a"].GetString());
            axon.Potassium_x_a= atof(Model["axon"]["Potassium_x_a"].GetString());
      }
      void getGapJunction(const rapidjson::Value& Model){
            gj.Conductance= Model["GapJunction"]["Conductance"].GetDouble();
            gj.Connectivity= std::stof(Model["GapJunction"]["Connectivity"].GetString());
      }
      eHH(const rapidjson::Value& Model){
            getDendrite(Model);
            getSoma(Model);
            getAxon(Model);
            getGapJunction(Model);
      }
      std::string modelName() const {
            return std::string{"eHH"};
      }
};

struct SimSpec {
      int simTime;
      unsigned int NeuralNetwork_id;
      unsigned int population;
      std::unique_ptr<NeuralModel> model;
      std::string region;
      std::string execution;
      unsigned int nodes;
};

void getPopulation(rapidjson::Value& NeuralNetwork, SimSpec &spec){
      for (auto& e : NeuralNetwork.GetArray()) {
            spec.population = e["Population"].GetInt();
      }
}

void getRegion(rapidjson::Value& NeuralNetwork, SimSpec &spec){
      for (auto& e : NeuralNetwork.GetArray()) {
            spec.region = e["Region"].GetString();
      }
}


void getModel(rapidjson::Value& NeuralNetwork, SimSpec &spec){
      rapidjson::Value& Model = NeuralNetwork[0]["Model"];
      //std::cout<<Model["name"].GetString()<<"\n";
      if(Model["name"].GetString()==std::string("eHH")){
            std::unique_ptr<eHH> ehh(new eHH(Model));
            spec.model=std::move(ehh);
      }
}

void getBrainActivityTime(std::string &BrainActivityTime, SimSpec &spec){
      spec.simTime=std::stoi(BrainActivityTime);
}

void getNeuralNetwork(rapidjson::Value& NeuralNetwork, SimSpec &spec){
      getPopulation(NeuralNetwork, spec);
      getRegion(NeuralNetwork, spec);
      getModel(NeuralNetwork, spec);
}

void getNeuralExecution(std::string &NeuralExecution, SimSpec &spec){
      spec.execution=NeuralExecution;
}

void getNumberNodes(int &numberNodes, SimSpec &spec){
      spec.nodes=numberNodes;
}

void getAC(rapidjson::Value& InjectionCurrentPattern,
      std::vector<stimAC> &AC,
      std::string stimulusName){
      if(stimulusName==std::string("AC")) {
            const auto hstep = float(1000/50);
            stimAC stim;
            stim.frequency=std::stof(InjectionCurrentPattern["frequency"].GetString());
            stim.offset=InjectionCurrentPattern["offset"].GetInt();
            stim.amplitude=InjectionCurrentPattern["amplitude"].GetDouble();
            stim.info.begin= (hstep)*std::stod(InjectionCurrentPattern["begin"].GetString());
            stim.info.end= (hstep)*std::stod(InjectionCurrentPattern["end"].GetString());
            std::string range (InjectionCurrentPattern["range"].GetString());
            stim.info.range[0]=std::stoul(range.substr (1,range.find(",")-1));
            stim.info.range[1]=std::stoul(range.substr (range.find(",")+1));
            AC.push_back(stim);
      }
}

void getDC(rapidjson::Value& InjectionCurrentPattern,
      std::vector<stimDC> &DC,
      std::string stimulusName){
      if(stimulusName==std::string("DC")) {
            const auto hstep = float(1000/50);
            stimDC stim;
            stim.amplitude=InjectionCurrentPattern["amplitude"].GetDouble();
            stim.info.begin= (hstep)*std::stod(InjectionCurrentPattern["begin"].GetString());
            stim.info.end= (hstep)*std::stod(InjectionCurrentPattern["end"].GetString());
            std::string range (InjectionCurrentPattern["range"].GetString());
            stim.info.range[0]=std::stoul(range.substr (1,range.find(",")-1));
            stim.info.range[1]=std::stoul(range.substr (range.find(",")+1));
            DC.push_back(stim);
      }
}

void getNoise(rapidjson::Value& InjectionCurrentPattern,
      std::vector<stimNoise> &Noise,
      std::string stimulusName){
      if(stimulusName==std::string("noise")) {
            const auto hstep = float(1000/50);
            stimNoise stim;
            stim.info.begin = (hstep)*std::stod(InjectionCurrentPattern["begin"].GetString());
            stim.info.end = (hstep)*std::stod(InjectionCurrentPattern["end"].GetString());
            stim.mean = InjectionCurrentPattern["mean"].GetInt();
            stim.std = InjectionCurrentPattern["std"].GetInt();
            auto  range = std::string{InjectionCurrentPattern["range"].GetString()};
            stim.info.range[0] = std::stoul(range.substr (1,range.find(",")-1));
            stim.info.range[1] = std::stoul(range.substr (range.find(",")+1));
            Noise.push_back(stim);
      }
}

void getStimulus(rapidjson::Value& InjectionCurrentPattern, stims &stimulus){
      using std::vector;
      auto AC = vector<stimAC>{};
      auto DC = vector<stimDC>{};
      auto Noise = vector<stimNoise>{};
      for (auto &e : InjectionCurrentPattern.GetArray()) {
            auto stimulusName = std::string{e["name"].GetString()};
            //std::cout << stimulusName <<"\n";
            getAC( e,AC, stimulusName);
            getDC( e,DC, stimulusName);
            getNoise( e,Noise, stimulusName);
      }
      stimulus.AC = std::move(AC);
      stimulus.DC = std::move(DC);
      stimulus.Noise = std::move(Noise);
}

int parseFile(const std::string& SimSpecFile,rapidjson::Document &doc){
      auto ifs = std::ifstream{ SimSpecFile };
      if ( !ifs.is_open() ){
            std::cerr << "Could not open file for reading!\n";
            return EXIT_FAILURE;
      }
      rapidjson::IStreamWrapper isw { ifs };
      doc.ParseStream( isw );
      rapidjson::StringBuffer buffer {};
      rapidjson::Writer<rapidjson::StringBuffer> writer { buffer };
      doc.Accept( writer );
      if ( doc.HasParseError() ){
            std::cerr << "Error  : " << doc.GetParseError()  << '\n'
                        << "Offset : " << doc.GetErrorOffset() << '\n';
            return EXIT_FAILURE;
      }
      const auto SimSpec = std::string{ buffer.GetString() };
      std::cout<<"\nInput file parsing  succeeded.\n";
      return 0;
}


int getSimSpec(SimSpec &spec,const std::string& SimSpecFile, stims &stimulus ){
      auto doc = rapidjson::Document{};
      parseFile(SimSpecFile,doc);
      auto BrainActivityTime = std::string{doc["BrainActivityTime"].GetString()};
      getBrainActivityTime(BrainActivityTime, spec);
      auto &NeuralNetwork = doc["NeuralNetwork"];
      getNeuralNetwork(NeuralNetwork, spec);
      auto NeuralExecution = std::string{doc["Execution"].GetString()};
      getNeuralExecution(NeuralExecution, spec);
      auto numberNodes = doc["Nodes"].GetInt();
      getNumberNodes(numberNodes, spec);
      auto &InjectionCurrentPattern = doc["InjectionCurrentPattern"];
      getStimulus(InjectionCurrentPattern, stimulus);
      return 0;
}

}

