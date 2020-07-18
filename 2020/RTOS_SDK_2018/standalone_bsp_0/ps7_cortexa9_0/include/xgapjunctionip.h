// ==============================================================
// File generated on Sat Jan 11 14:24:41 CST 2020
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XGAPJUNCTIONIP_H
#define XGAPJUNCTIONIP_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xgapjunctionip_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XGapjunctionip_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XGapjunctionip;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XGapjunctionip_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XGapjunctionip_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XGapjunctionip_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XGapjunctionip_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XGapjunctionip_Initialize(XGapjunctionip *InstancePtr, u16 DeviceId);
XGapjunctionip_Config* XGapjunctionip_LookupConfig(u16 DeviceId);
int XGapjunctionip_CfgInitialize(XGapjunctionip *InstancePtr, XGapjunctionip_Config *ConfigPtr);
#else
int XGapjunctionip_Initialize(XGapjunctionip *InstancePtr, const char* InstanceName);
int XGapjunctionip_Release(XGapjunctionip *InstancePtr);
#endif

void XGapjunctionip_Start(XGapjunctionip *InstancePtr);
u32 XGapjunctionip_IsDone(XGapjunctionip *InstancePtr);
u32 XGapjunctionip_IsIdle(XGapjunctionip *InstancePtr);
u32 XGapjunctionip_IsReady(XGapjunctionip *InstancePtr);
void XGapjunctionip_EnableAutoRestart(XGapjunctionip *InstancePtr);
void XGapjunctionip_DisableAutoRestart(XGapjunctionip *InstancePtr);

void XGapjunctionip_Set_size(XGapjunctionip *InstancePtr, u32 Data);
u32 XGapjunctionip_Get_size(XGapjunctionip *InstancePtr);
void XGapjunctionip_Set_FirstRow(XGapjunctionip *InstancePtr, u32 Data);
u32 XGapjunctionip_Get_FirstRow(XGapjunctionip *InstancePtr);
void XGapjunctionip_Set_LastRow(XGapjunctionip *InstancePtr, u32 Data);
u32 XGapjunctionip_Get_LastRow(XGapjunctionip *InstancePtr);

void XGapjunctionip_InterruptGlobalEnable(XGapjunctionip *InstancePtr);
void XGapjunctionip_InterruptGlobalDisable(XGapjunctionip *InstancePtr);
void XGapjunctionip_InterruptEnable(XGapjunctionip *InstancePtr, u32 Mask);
void XGapjunctionip_InterruptDisable(XGapjunctionip *InstancePtr, u32 Mask);
void XGapjunctionip_InterruptClear(XGapjunctionip *InstancePtr, u32 Mask);
u32 XGapjunctionip_InterruptGetEnabled(XGapjunctionip *InstancePtr);
u32 XGapjunctionip_InterruptGetStatus(XGapjunctionip *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
