// ==============================================================
// File generated on Sat Jan 11 14:24:41 CST 2020
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xgapjunctionip.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XGapjunctionip_CfgInitialize(XGapjunctionip *InstancePtr, XGapjunctionip_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XGapjunctionip_Start(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL) & 0x80;
    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL, Data | 0x01);
}

u32 XGapjunctionip_IsDone(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XGapjunctionip_IsIdle(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XGapjunctionip_IsReady(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XGapjunctionip_EnableAutoRestart(XGapjunctionip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL, 0x80);
}

void XGapjunctionip_DisableAutoRestart(XGapjunctionip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_AP_CTRL, 0);
}

void XGapjunctionip_Set_size(XGapjunctionip *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_SIZE_DATA, Data);
}

u32 XGapjunctionip_Get_size(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_SIZE_DATA);
    return Data;
}

void XGapjunctionip_Set_FirstRow(XGapjunctionip *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_FIRSTROW_DATA, Data);
}

u32 XGapjunctionip_Get_FirstRow(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_FIRSTROW_DATA);
    return Data;
}

void XGapjunctionip_Set_LastRow(XGapjunctionip *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_LASTROW_DATA, Data);
}

u32 XGapjunctionip_Get_LastRow(XGapjunctionip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_LASTROW_DATA);
    return Data;
}

void XGapjunctionip_InterruptGlobalEnable(XGapjunctionip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_GIE, 1);
}

void XGapjunctionip_InterruptGlobalDisable(XGapjunctionip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_GIE, 0);
}

void XGapjunctionip_InterruptEnable(XGapjunctionip *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_IER);
    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_IER, Register | Mask);
}

void XGapjunctionip_InterruptDisable(XGapjunctionip *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_IER);
    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_IER, Register & (~Mask));
}

void XGapjunctionip_InterruptClear(XGapjunctionip *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGapjunctionip_WriteReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_ISR, Mask);
}

u32 XGapjunctionip_InterruptGetEnabled(XGapjunctionip *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_IER);
}

u32 XGapjunctionip_InterruptGetStatus(XGapjunctionip *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGapjunctionip_ReadReg(InstancePtr->Axilites_BaseAddress, XGAPJUNCTIONIP_AXILITES_ADDR_ISR);
}

