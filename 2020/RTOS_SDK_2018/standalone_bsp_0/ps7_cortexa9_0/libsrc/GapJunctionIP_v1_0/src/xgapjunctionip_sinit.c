// ==============================================================
// File generated on Sat Jan 11 14:24:41 CST 2020
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xgapjunctionip.h"

extern XGapjunctionip_Config XGapjunctionip_ConfigTable[];

XGapjunctionip_Config *XGapjunctionip_LookupConfig(u16 DeviceId) {
	XGapjunctionip_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XGAPJUNCTIONIP_NUM_INSTANCES; Index++) {
		if (XGapjunctionip_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XGapjunctionip_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XGapjunctionip_Initialize(XGapjunctionip *InstancePtr, u16 DeviceId) {
	XGapjunctionip_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XGapjunctionip_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XGapjunctionip_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

