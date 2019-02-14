#include "stm32f7xx_it.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_audio.h"

/* SAI handler declared in "stm32746g_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_out_sai;

/* SAI handler declared in "stm32746g_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_in_sai;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	printf("HardFault handler reached.\n");
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void EXTI15_10_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_IT(TS_INT_PIN) != RESET)
	{
		HAL_GPIO_EXTI_IRQHandler(TS_INT_PIN);
	}
	else if (__HAL_GPIO_EXTI_GET_IT(KEY_BUTTON_PIN) != RESET)
	{
		HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
	}
}

void AUDIO_IN_SAIx_DMAx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
}

void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}
