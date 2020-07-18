#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <vector>

#ifndef INFOLI
#define INFOLI

void run_GapJunctionIP(
		int N_Size,
		std::vector<mod_prec> &neighVdend,
    GapJunctionIP &gjip)
{
	gjip.copyVdend(neighVdend);
	gjip.run();
}



void ComputeNetwork(
int N_Size,
std::vector<cellState> &local_state,
std::vector<mod_prec> &iAppin,
std::vector<mod_prec> &neighVdend,
std::vector<mod_prec> &cellOut,
GapJunctionIP &gjip)
{

	int  neuron;

	cellState new_state[N_Size];

  run_GapJunctionIP(N_Size,neighVdend,gjip);

	for(neuron=0;neuron<N_Size;neuron++){
		new_state[neuron].dend = CompDend(local_state[neuron].dend, local_state[neuron].soma.V_soma, iAppin[neuron], gjip.IcBuff(neuron) , N_Size );
		new_state[neuron].axon = CompAxon(local_state[neuron].axon, local_state[neuron].soma.V_soma);
		new_state[neuron].soma = CompSoma(local_state[neuron].soma, local_state[neuron].dend.V_dend, local_state[neuron].axon.V_axon);
	}

	for(neuron=0;neuron<N_Size;neuron++){
		local_state[neuron]=new_state[neuron];
		cellOut[neuron] = local_state[neuron].axon.V_axon;
	}

}

Dend CompDend(
		Dend const &prevDend,
		mod_prec prevSoma,
		mod_prec iAppIn,
		mod_prec I_GJ,
		int N_Size)
{

	struct Dend d_output;
    struct channelParams chPrms;
    struct dendCurrVoltPrms chComps, newchComps;

    //H current calculation
    chPrms.prevComp1 = prevDend.Hcurrent_q;
    d_output.Hcurrent_q = DendHCurr(chPrms, prevDend.V_dend);

    //Ca current calculation
    chPrms.prevComp1 = prevDend.Calcium_r;
    d_output.Calcium_r  = DendCaCurr(chPrms, prevDend.V_dend);

    //K plus current calculation
    chPrms.prevComp1 = prevDend.Potassium_s;
    chPrms.prevComp2 = prevDend.Ca2Plus;
    d_output.Potassium_s = DendKCurr(chPrms );

    //K plus current calculation
    chPrms.prevComp1 = prevDend.Ca2Plus;
    chPrms.prevComp2 =prevDend.I_CaH;
    d_output.Ca2Plus  = DendCal(chPrms);

    //Neighbors Ic accumulation
    chComps.iC = I_GJ;

    //Compartment output calculation
    chComps.iApp = iAppIn;
    chComps.vDend = prevDend.V_dend;
    chComps.vSoma = prevSoma;
    chComps.q = d_output.Hcurrent_q;
    chComps.r = d_output.Calcium_r;
    chComps.s = d_output.Potassium_s;
    newchComps = DendCurrVolt(chComps);
    d_output.I_CaH = newchComps.newI_CaH;
    d_output.V_dend = newchComps.newVDend;

    return d_output;
}

mod_prec DendHCurr(
		struct channelParams const &chPrms,
		mod_prec prevV_dend)
{
    mod_prec q_inf, inv_tau_q, dq_dt, q_local;

    //opt for division by constant
    mod_prec const div_four = 0.25;

    //Get inputs
    mod_prec const prevHcurrent_q = chPrms.prevComp1;

    // Update dendritic H current component
    q_inf = 1 /(1 + expf((prevV_dend + 80) * div_four));
    //tau_q = 1 /(expf(-0.086 * prevV_dend - 14.6) + expf(0.070 * prevV_dend - 1.87));
		inv_tau_q = (expf(-0.086 * prevV_dend - 14.6) + expf(0.070 * prevV_dend - 1.87));
    dq_dt = (q_inf - prevHcurrent_q) * inv_tau_q;
    q_local = DELTA * dq_dt + prevHcurrent_q;
    //Put result
    return q_local;
}

mod_prec DendCaCurr(
		struct channelParams const &chPrms,
		mod_prec prevV_dend)
{

    mod_prec alpha_r, beta_r, r_local;

    //opt for division by constant
    mod_prec const div_five = 0.2;
		mod_prec const div_thirteen = -1/13.9;
		mod_prec const delta_five = DELTA*0.2;

    //Get inputs
    mod_prec const prevCalcium_r = chPrms.prevComp1;
		alpha_r = 1.7 / (1 + expf( (prevV_dend - 5) *div_thirteen));
		beta_r = 0.02 * (prevV_dend + 8.5) / (expf((prevV_dend + 8.5) * div_five) - 1);
    r_local =  (delta_five*alpha_r)+(1-delta_five*(alpha_r+beta_r))*prevCalcium_r;
    //Put result
    return r_local;
}

mod_prec DendKCurr(struct channelParams const & chPrms){

//    mod_prec  alpha_s, beta_s, s_inf, tau_s, ds_dt, s_local;
		mod_prec  alpha_s, s_local;

	 	mod_prec const beta_s = 0.015;
	 	mod_prec const beta_s_delta_one = beta_s*DELTA-1;
    //Get inputs
  	mod_prec const prevPotassium_s = chPrms.prevComp1;
    mod_prec const prevCa2Plus = chPrms.prevComp2;

    // Update dendritic Ca-dependent K current component
    alpha_s = min((0.00002*prevCa2Plus), 0.01);
    s_local = DELTA*alpha_s - (alpha_s*DELTA+beta_s_delta_one)*prevPotassium_s;

    //Put result
    return s_local;
}

mod_prec DendCal(struct channelParams const &chPrms){

    mod_prec   Ca2Plus_local;

    //Get inputs
    mod_prec const prevCa2Plus = chPrms.prevComp1;
    mod_prec const prevI_CaH = chPrms.prevComp2;

		mod_prec const one_delta_sevenfive = 1-DELTA*0.075;
		mod_prec const minus_three_delta = -3*DELTA;

    // update Calcium concentration
		Ca2Plus_local = minus_three_delta*prevI_CaH+one_delta_sevenfive*prevCa2Plus;
    //Put result
    return Ca2Plus_local;
}

dendCurrVoltPrms DendCurrVolt(struct dendCurrVoltPrms &chComps){

    //Local variables
    mod_prec I_sd, I_CaH, I_K_Ca, I_ld, I_h, dVd_dt;

    //Get inputs
    mod_prec const I_c = chComps.iC;
    mod_prec const I_app = chComps.iApp;
    mod_prec const prevV_dend = chComps.vDend;
    mod_prec const prevV_soma = chComps.vSoma;
    mod_prec const q = chComps.q;
    mod_prec const r = chComps.r;
    mod_prec const s = chComps.s;

		mod_prec const inv_C_M = 1/C_M;

    // DENDRITIC CURRENTS

    // Soma-dendrite interaction current I_sd
    I_sd   = (G_INT / (1 - P1)) * (prevV_dend - prevV_soma);
    // Inward high-threshold Ca current I_CaH
    I_CaH  =  G_CAH * r * r * (prevV_dend - V_CA);
    // Outward Ca-dependent K current I_K_Ca
    I_K_Ca =  G_K_CA * s * (prevV_dend - V_K);
    // Leakage current I_ld
    I_ld   =  G_LD * (prevV_dend - V_L);
    // Inward anomalous rectifier I_h
    I_h    =  G_H * q * (prevV_dend - V_H);

    dVd_dt = (-(I_CaH   + I_sd  + I_ld + I_K_Ca + I_c + I_h) + I_app) * inv_C_M;

    //Put result (update V_dend)
    chComps.newVDend = DELTA * dVd_dt + prevV_dend;
    chComps.newI_CaH = I_CaH;//This is a state value read in DendCal
    return chComps;
}



Soma CompSoma(
		Soma const &prevSoma,
		mod_prec prevDend,
		mod_prec prevAxon)
{

	struct Soma s_output;
    struct channelParams chPrms, chPrmsNew;
    struct somaCurrVoltPrms chComps;

    // update somatic components
    // SCHWEIGHOFER:
    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Calcium_k;
    chPrms.prevComp2 = prevSoma.Calcium_l;

    //Ca current calculation
    chPrmsNew = SomaCalcium(chPrms);

    s_output.Calcium_k = chPrmsNew.newComp1;
    s_output.Calcium_l = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Sodium_m;
    chPrms.prevComp2 = prevSoma.Sodium_h;

    //Sodium current calculation
    chPrmsNew = SomaSodium(chPrms);

    s_output.Sodium_m = chPrmsNew.newComp1;
    s_output.Sodium_h = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Potassium_n;
    chPrms.prevComp2 = prevSoma.Potassium_p;

    //Potassium current calculation
    chPrmsNew = SomaPotassium(chPrms);

    s_output.Potassium_n = chPrmsNew.newComp1;
    s_output.Potassium_p = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Potassium_x_s;

    //Potassium X current calculation
    chPrmsNew.newComp1 = SomaPotassiumX(chPrms);

    s_output.Potassium_x_s = chPrmsNew.newComp1;
    //Compartment output calculation
    chComps.g_CaL = prevSoma.g_CaL;
    s_output.g_CaL = prevSoma.g_CaL;
    chComps.vDend = prevDend;
    chComps.vSoma = prevSoma.V_soma;
    chComps.vAxon = prevAxon;
    chComps.k = s_output.Calcium_k;
    chComps.l = s_output.Calcium_l;
    chComps.m = s_output.Sodium_m;
    chComps.h = s_output.Sodium_h;
    chComps.n = s_output.Potassium_n;
    chComps.x_s = s_output.Potassium_x_s;
    chComps.newVSoma = SomaCurrVolt(chComps);

    s_output.g_CaL = prevSoma.g_CaL;
    s_output.V_soma = chComps.newVSoma;

    return s_output;
}

channelParams SomaCalcium(struct channelParams  &chPrms){

    mod_prec k_inf, l_inf, tau_l, dk_dt, dl_dt, k_local, l_local;

    //Get inputs
    mod_prec const prevV_soma = chPrms.v;
    mod_prec const prevCalcium_k = chPrms.prevComp1;
    mod_prec const prevCalcium_l = chPrms.prevComp2;

    //opt for division with constant
    mod_prec const four = 1/4.2;
    mod_prec const eight = 1/8.5; //no change in area by synthesizer on V7
    mod_prec const thirty = 1/30.0;  //no change in area by synthesizer on V7
    mod_prec const seven = 1/7.3; //no change in area by synthesizer on V7

    k_inf = (1 / (1 + expf(-1 * (prevV_soma + 61)  * four)));
    l_inf = (1 / (1 + expf((     prevV_soma + 85.5) * eight)));
    tau_l = ((20 * expf((prevV_soma + 160) * thirty) / (1 + expf((prevV_soma + 84) * seven ))) +35);
//    dk_dt = (k_inf - prevCalcium_k) / tau_k;
	dk_dt = (k_inf - prevCalcium_k);
    dl_dt = (l_inf - prevCalcium_l) / tau_l;
    k_local = DELTA * dk_dt + prevCalcium_k;
    l_local = DELTA * dl_dt + prevCalcium_l;
    //Put result
    chPrms.newComp1= k_local;
    chPrms.newComp2= l_local;

    return chPrms;
}

channelParams SomaSodium(struct channelParams &chPrms){

    mod_prec m_inf, h_inf, tau_h, dh_dt, m_local, h_local;

    //Get inputs
    mod_prec const prevV_soma = chPrms.v;
    //mod_prec prevSodium_m = *chPrms->prevComp1;
    mod_prec const prevSodium_h = chPrms.prevComp2;

    //opt for division by constant
    mod_prec const five_five = 1/5.5; //no change in area by synthesizer on V7
    mod_prec const five_eight = -1/5.8; //no change in area by synthesizer on V7
    mod_prec const thirty_three = 1/33.0;  //no change in area by synthesizer on V7

    // RAT THALAMOCORTICAL SODIUM:

    m_inf   = 1 / (1 + (expf((-30 - prevV_soma)* five_five)));
    h_inf   = 1 / (1 + (expf((-70 - prevV_soma)* five_eight)));
    tau_h   =       3 * expf((-40 - prevV_soma)* thirty_three);
    dh_dt   = (h_inf - prevSodium_h)/tau_h;
    m_local       = m_inf;
    h_local       = prevSodium_h + DELTA * dh_dt;

    //Put result
    chPrms.newComp1 = m_local;
    chPrms.newComp2 = h_local;
    return chPrms;
}

channelParams SomaPotassium(struct channelParams &chPrms){

    mod_prec n_inf, p_inf, tau_n, dn_dt, dp_dt, n_local, p_local;

    //Get inputs
    mod_prec const prevV_soma = chPrms.v;
    mod_prec const prevPotassium_n = chPrms.prevComp1;
    mod_prec const prevPotassium_p = chPrms.prevComp2;

    //opt for division by constant
    mod_prec const ten = 0.1; //precision error if 1/10 is used
    mod_prec const twelve = 1/12.0; //no change in area by synthesizer on V7
    mod_prec const nine_hundred = 1/900.0;  //no change in area by synthesizer on V7

    // NEOCORTICAL
    n_inf = 1 / (1 + expf( ( -3 - prevV_soma) * ten));
    p_inf = 1 / (1 + expf( (51 + prevV_soma) * twelve));
    tau_n =   5 + (  47 * expf( (50 + prevV_soma) * nine_hundred));
    dn_dt = (n_inf - prevPotassium_n) / tau_n;
	dp_dt = (p_inf - prevPotassium_p) / tau_n;
    n_local = DELTA * dn_dt + prevPotassium_n;
    p_local = DELTA * dp_dt + prevPotassium_p;

    //Put result
    chPrms.newComp1 = n_local;
    chPrms.newComp2 = p_local;
    return chPrms;
}

mod_prec SomaPotassiumX(struct channelParams &chPrms){

    mod_prec alpha_x_s, beta_x_s, x_s_local;

    //Get inputs
    mod_prec const prevV_soma = chPrms.v;
    mod_prec const prevPotassium_x_s = chPrms.prevComp1;

    //opt for division by constant
     mod_prec const ten = 0.1; //no change in area by synthesizer on V7

    // Voltage-dependent (fast) potassium
    alpha_x_s = 0.13 * (prevV_soma + 25) / (1 - expf(-(prevV_soma + 25) * ten));
    beta_x_s  = 1.69 * expf(-0.0125 * (prevV_soma + 35));
		x_s_local = DELTA*alpha_x_s + (1-(alpha_x_s+beta_x_s)*DELTA)*prevPotassium_x_s;

    //Put result
    chPrms.newComp1 = x_s_local;
    return chPrms.newComp1;
}

mod_prec SomaCurrVolt(struct somaCurrVoltPrms &chComps){

    //Local variables
    mod_prec I_ds, I_CaL, I_Na_s, I_ls, I_Kdr_s, I_K_s, I_as, dVs_dt;

    //Get inputs
    mod_prec const g_CaL = chComps.g_CaL;
    mod_prec const prevV_dend = chComps.vDend;
    mod_prec const prevV_soma = chComps.vSoma;
    mod_prec const prevV_axon = chComps.vAxon;
    mod_prec const k = chComps.k;
    mod_prec const l = chComps.l;
    mod_prec const m = chComps.m;
    mod_prec const h = chComps.h;
    mod_prec const n = chComps.n;
    mod_prec const x_s = chComps.x_s;

    // SOMATIC CURRENTS

		mod_prec const inv_CM = 1/C_M;

    // Dendrite-soma interaction current I_ds
    I_ds  = (G_INT / P1) * (prevV_soma - prevV_dend);
    // Inward low-threshold Ca current I_CaL
    I_CaL = g_CaL * k * k * k * l * (prevV_soma - V_CA); //k^3
    // Inward Na current I_Na_s
    I_Na_s  = G_NA_S * m * m * m * h * (prevV_soma - V_NA);
    // Leakage current I_ls
    I_ls  = G_LS * (prevV_soma - V_L);
    // Outward delayed potassium current I_Kdr
    I_Kdr_s = G_KDR_S * n * n * n * n * (prevV_soma - V_K); // SCHWEIGHOFER
    // I_K_s
    I_K_s   = G_K_S * x_s*x_s*x_s*x_s * (prevV_soma - V_K);
    // Axon-soma interaction current I_as
    I_as    = (G_INT / (1 - P2)) * (prevV_soma - prevV_axon);

    dVs_dt = (-(I_CaL   + I_ds  + I_as + I_Na_s + I_ls   + I_Kdr_s + I_K_s))*inv_CM;
    chComps.newVSoma = DELTA * dVs_dt + prevV_soma;


    return chComps.newVSoma;
}

Axon CompAxon(
		Axon const &prevAxon,
		mod_prec prevSoma)
{

		struct Axon a_output;
    struct channelParams chPrms, chPrmsNew;
    struct axonCurrVoltPrms chComps;

    // update somatic components
    // SCHWEIGHOFER:

    chPrms.v = prevAxon.V_axon;
    chPrms.prevComp1 =prevAxon.Sodium_h_a;

    //Sodium current calculation
    chPrmsNew = AxonSodium(chPrms);

    a_output.Sodium_h_a =  chPrmsNew.newComp1;
    a_output.Sodium_m_a =  chPrmsNew.newComp2;

    chPrms.v = prevAxon.V_axon;
    chPrms.prevComp1 = prevAxon.Potassium_x_a;

    //Potassium current calculation
    chPrmsNew.newComp1 = AxonPotassium(chPrms);

    a_output.Potassium_x_a =  chPrmsNew.newComp1;

    chComps.vSoma = prevSoma;
    chComps.vAxon = prevAxon.V_axon;
    //Compartment output calculation
    chComps.m_a = a_output.Sodium_m_a;
    chComps.h_a = a_output.Sodium_h_a;
    chComps.x_a = a_output.Potassium_x_a;
    chComps.newVAxon = AxonCurrVolt(chComps);

    a_output.V_axon =  chComps.newVAxon;

    return a_output;
}

channelParams AxonSodium(struct channelParams &chPrms){

    mod_prec m_inf_a, h_inf_a, tau_h_a, dh_dt_a, m_a_local, h_a_local;

    //Get inputs
    mod_prec const prevV_axon = chPrms.v;
    mod_prec const prevSodium_h_a = chPrms.prevComp1;

    //opt for division by constant
    mod_prec const five_five = 1/5.5; //no change in area by synthesizer on V7
    mod_prec const five_eight = -1/5.8; //no change in area by synthesizer on V7
    mod_prec const thirty_three = 1/33.0;  //no change in area by synthesizer on V7

    // Update axonal Na components
    // NOTE: current has shortened inactivation to account for high
    // firing frequencies in axon hillock


    m_inf_a   = 1 / (1 + (expf((-30 - prevV_axon) * five_five)));
    h_inf_a   = 1 / (1 + (expf((-60 - prevV_axon) * five_eight)));
    tau_h_a   =     1.5 * expf((-40 - prevV_axon) * thirty_three);
    dh_dt_a   = (h_inf_a - prevSodium_h_a)/tau_h_a;
    m_a_local = m_inf_a;
    h_a_local = prevSodium_h_a + DELTA * dh_dt_a;

    //Put result
    chPrms.newComp1 = h_a_local;
    chPrms.newComp2 = m_a_local;
    return chPrms;
}

mod_prec AxonPotassium(struct channelParams &chPrms){

    mod_prec alpha_x_a, beta_x_a, x_a_local;

    //Get inputs
    mod_prec const prevV_axon = chPrms.v;
    mod_prec const prevPotassium_x_a = chPrms.prevComp1;

    //opt for division by constant
    mod_prec const ten = 0.1; //no change in area by synthesizer on V7

    // D'ANGELO 2001 -- Voltage-dependent potassium
    alpha_x_a = 0.13 * (prevV_axon + 25) / (1 - expf(-(prevV_axon + 25) * ten));
    beta_x_a  = 1.69 * expf(-0.0125 * (prevV_axon + 35));
		x_a_local = DELTA * alpha_x_a + (1-(alpha_x_a+beta_x_a)*DELTA)*prevPotassium_x_a;

    //Put result
    chPrms.newComp1 = x_a_local;
    return chPrms.newComp1;
}

mod_prec AxonCurrVolt(struct axonCurrVoltPrms &chComps){

    //Local variable
    mod_prec I_Na_a, I_la, I_sa, I_K_a, dVa_dt;

    //Get inputs
    mod_prec const prevV_soma = chComps.vSoma;
    mod_prec const prevV_axon = chComps.vAxon;
    mod_prec const m_a = chComps.m_a;
    mod_prec const h_a = chComps.h_a;
    mod_prec const x_a = chComps.x_a;

	mod_prec const inv_CM= 1/C_M;

    // AXONAL CURRENTS
    // Sodium
    I_Na_a  = G_NA_A  * m_a * m_a * m_a * h_a * (prevV_axon - V_NA);
    // Leak

    I_la    = G_LA * (prevV_axon - V_L);
    // Soma-axon interaction current I_sa
    I_sa    = (G_INT / P2) * (prevV_axon - prevV_soma);
    // Potassium (transient)
    I_K_a   = G_K_A * x_a *x_a *x_a *x_a * (prevV_axon - V_K);
    dVa_dt = (-(I_K_a + I_sa + I_la + I_Na_a)) * inv_CM;
    chComps.newVAxon = DELTA * dVa_dt + prevV_axon;

    return chComps.newVAxon;
}

inline mod_prec min(mod_prec a, mod_prec b){
    return (a < b) ? a : b;
}

#endif

