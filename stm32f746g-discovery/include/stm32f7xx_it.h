#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_audio.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void EXTI15_10_IRQHandler(void);
void AUDIO_IN_SAIx_DMAx_IRQHandler(void);
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
