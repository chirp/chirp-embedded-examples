#include "stm32f4xx_it.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_audio.h"

/* SAI handler declared in "stm32469i_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_out_sai;

/* I2S handler declared in "stm32469i_discovery_audio.c" file */
extern I2S_HandleTypeDef haudio_in_i2s;

void NMI_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
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

void AUDIO_SAIx_DMAx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

void AUDIO_I2Sx_DMAx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(haudio_in_i2s.hdmarx);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(TS_INT_PIN);
}
