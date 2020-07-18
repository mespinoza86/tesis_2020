
#ifndef INFOLI
#define INFOLI


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

/*** MACROS ***/
//Connection Matrix size in ints.
// Cell properties///////////////////////////////////////////////////////////////////////////////////////////////
#define DELTA 0.05
//Conductance for neighbors' coupling
#define CONDUCTANCE 0.04
// Capacitance
#define C_M 1
// Somatic conductances (mS/cm2)
#define G_NA_S 150      // Na gate conductance (=90 in Schweighofer code, 70 in paper) 120 too little
#define G_KDR_S 9.0    // K delayed rectifier gate conductance (alternative value: 18)
#define G_K_S 5      // Voltage-dependent (fast) potassium
#define G_LS 0.016  // Leak conductance (0.015)
// Dendritic conductances (mS/cm2)
#define G_K_CA 35       // Potassium gate conductance (35)
#define G_CAH 4.5     // High-threshold Ca gate conductance (4.5)
#define G_LD 0.016   // Dendrite leak conductance (0.015)
#define G_H 0.125    // H current gate conductance (1.5) (0.15 in SCHWEIGHOFER 2004)
// Axon hillock conductances (mS/cm2)
#define G_NA_A 240      // Na gate conductance (according to literature: 100 to 200 times as big as somatic conductance)
#define G_NA_R 0      // Na (resurgent) gate conductance
#define G_K_A 20      // K voltage-dependent
#define G_LA 0.016  // Leak conductance
// Cell morphology
#define P1 0.25        // Cell surface ratio soma/dendrite (0.2)
#define P2 0.15      // Cell surface ratio axon(hillock)/soma (0.1)
#define G_INT 0.13       // Cell internal conductance (0.13)
// Reversal potentials
#define V_NA 55       // Na reversal potential (55)
#define V_K -75       // K reversal potential
#define V_CA 120       // Ca reversal potential (120)
#define V_H -43       // H current reversal potential
#define V_L 10       // leak current

//typedef double mod_prec;
typedef float mod_prec;

//Structs for internal calculations and general inputs
typedef struct Dend{
	mod_prec V_dend;
	mod_prec Hcurrent_q;
	mod_prec Calcium_r;
	mod_prec Potassium_s;
	mod_prec I_CaH;
	mod_prec Ca2Plus;
}Dend;

typedef struct Soma{
	mod_prec g_CaL;
	mod_prec V_soma;
	mod_prec Sodium_m;
	mod_prec Sodium_h;
	mod_prec Calcium_k;
	mod_prec Calcium_l;
	mod_prec Potassium_n;
	mod_prec Potassium_p;
	mod_prec Potassium_x_s;
}Soma;

typedef struct Axon{
	mod_prec V_axon;
	mod_prec Sodium_m_a;
	mod_prec Sodium_h_a;
	mod_prec Potassium_x_a;
}Axon;

typedef struct GapJunction {
        float Conductance;
        float Connectivity;
}GapJunction;


typedef struct cellState{
	 Dend dend;
	 Soma soma;
	 Axon axon;
	 GapJunction gj;
}cellState;

typedef struct channelParams{
	mod_prec v;
	mod_prec prevComp1, prevComp2;
	mod_prec newComp1, newComp2;
}channelParams;

typedef struct dendCurrVoltPrms{
	mod_prec iApp;
	mod_prec iC;
	mod_prec vDend;
	mod_prec vSoma;
	mod_prec q, r, s;
	mod_prec newVDend;
	mod_prec newI_CaH;
}dendCurrVoltPrms;

typedef struct somaCurrVoltPrms{
	mod_prec g_CaL;
	mod_prec vSoma;
	mod_prec vDend;
	mod_prec vAxon;
	mod_prec k, l, m, h, n, x_s;
	mod_prec newVSoma;
}somaCurrVoltPrms;

typedef struct axonCurrVoltPrms{
	mod_prec vSoma;
	mod_prec vAxon;
	mod_prec m_a, h_a, x_a;
	mod_prec newVAxon;
}axonCurrVoltPrms;

/*** FUNCTION PROTOTYPES ***/


void ComputeNetwork(int,std::vector<cellState> &, std::vector<mod_prec> &,
	std::vector<mod_prec> &,
	std::vector<mod_prec> &,GapJunctionIP &gjip);


Dend CompDend(struct Dend const &, mod_prec , mod_prec,mod_prec, int );
mod_prec DendHCurr(struct channelParams const &, mod_prec );
mod_prec DendCaCurr(struct channelParams const & , mod_prec );
mod_prec DendKCurr(struct channelParams const & );
mod_prec DendCal(struct channelParams const & );
dendCurrVoltPrms DendCurrVolt(struct dendCurrVoltPrms & );
mod_prec IcNeighbors(mod_prec*, mod_prec, mod_prec*);

Soma CompSoma(struct Soma const &, mod_prec ,mod_prec);
channelParams SomaCalcium(struct channelParams  &);
channelParams SomaSodium(struct channelParams  &);
channelParams SomaPotassium(struct channelParams &);
mod_prec SomaPotassiumX(struct channelParams &);
mod_prec SomaCurrVolt(struct somaCurrVoltPrms &);

Axon CompAxon(struct Axon const &, mod_prec);
channelParams AxonSodium(channelParams &);
mod_prec AxonPotassium(channelParams &);
mod_prec AxonCurrVolt(axonCurrVoltPrms &);


void InitState(cellState&);
int ReadFileLine(char *, int, FILE *, mod_prec *);
inline mod_prec min(mod_prec a, mod_prec b);

#endif

