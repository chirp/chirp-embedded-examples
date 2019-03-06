/**-----------------------------------------------------------------------------
 *
 *  @file main.c
 *
 *  @brief This file handles the setup of the board and enable various systems
 *  like serial, LCD screen and audio. The file in which the user code is meant
 *  to be is `application.c`.
 *
 *  See README.md for further information and known issues.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#include <stdbool.h>
#include <string.h>

#include "main.h"

#include "stm32746g_discovery.h"
#include "stm32746g_discovery_audio.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

#include "uart.h"

/*
 * Forward declarations of functions located either in application.c or at the
 * end of this file.
 */
void CPU_CACHE_Enable(void);
void SystemClock_Config(void);
void setup(uint32_t sample_rate);
void loop(float *buffer, uint16_t blocksize);

#define VOLUME 100
/*
 * The sample rate is higher than the one naming the config file from Chirp. It
 * doesn't matter as long as it is higher than 16kHz.
 */
#define SAMPLE_RATE 16000
#define SHORT_BUFFER_SIZE 2048

/*
 * The float buffer is a quarter the short buffer because it will represent half
 * of the short buffer (division by 2) in mono samples (another division by 2).
 */
#define FLOAT_BUFFER_SIZE (SHORT_BUFFER_SIZE / 4)

/*
 * Used to keep track of the state of buffers.
 */
typedef enum {
	BUFFER_STATE_EMPTY,
	BUFFER_STATE_HALF,
	BUFFER_STATE_FULL,
} BUFFER_STATE;

/**
 * Allows to know if we are playing or listening.
 */
typedef enum {
	NONE,
	PLAYING,
	LISTENING,
} AUDIO_STATE;

AUDIO_STATE audio_state = NONE;

/*
 * Buffers containing the audio to play or record.
 */
uint16_t short_record_buffer[SHORT_BUFFER_SIZE] = {0};
BUFFER_STATE record_buffer_state = BUFFER_STATE_EMPTY;
uint32_t record_buffer_offset = 0;

uint16_t short_play_buffer[SHORT_BUFFER_SIZE] = {0};
BUFFER_STATE play_buffer_state = BUFFER_STATE_EMPTY;
uint32_t play_buffer_offset = 0;

float float_buffer[FLOAT_BUFFER_SIZE] = {0};

// Used to debounce button and screen.
uint32_t tick_saved = 0;

/*
 * Simple handler printing on the serial line where the error occurred.
 */
void error_handler(const char *function, char *file, int line)
{
	printf("Error handler reached in %s in %s at %d\n", function, file, line);
	while(true);
}

/*
 * This function is called when the user press the user button (blue one). The
 * debouncing prevents from pressing the button more than once per second.
 */
void button_user_event(void)
{
	uint32_t tick_time = HAL_GetTick();

	// Here, 1000 = 1000 ms = 1s
	if (tick_time - tick_saved > 1000)
	{
		if (audio_state == PLAYING)
		{
			audio_state = LISTENING;
			display_message("Listening.", LCD_COLOR_BLACK);
		}
		else if (audio_state == LISTENING)
		{
			audio_state = PLAYING;
			display_message("Playing.", LCD_COLOR_BLACK);
		}

		tick_saved = tick_time;
	}
}

/*
 * Whenever an interruption is received for the touch screen or the user button,
 * this function is reached.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == TS_INT_PIN)
	{
		uint32_t tick_time = HAL_GetTick();

		if (tick_time - tick_saved > 1000)
		{
			on_screen_touch();
			tick_saved = tick_time;
		}
	}
	else if (GPIO_Pin == KEY_BUTTON_PIN)
	{
		button_user_event();
	}
}

/*
 * Set the totality of the screen to the specified color.
 */
void set_screen_color(uint32_t color)
{
	uint32_t heigh = BSP_LCD_GetYSize();
	uint32_t width = BSP_LCD_GetXSize();
	uint32_t save_background_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(color);
	BSP_LCD_FillRect(0, 0, width, heigh);
	BSP_LCD_SetTextColor(save_background_color);
}

/*
 * Display a message. The line to display is automatically incremented and goes
 * back to the beginning of the screen when reaching the end.
 */
void display_message(char *message, uint32_t color)
{
	static uint8_t line_count = 0;

	uint8_t chars_per_line = BSP_LCD_GetXSize() / BSP_LCD_GetFont()->Width;
	size_t message_length = strlen(message);

	uint8_t lines_needed = message_length / chars_per_line + 1;
	for (uint8_t l = 0; l < lines_needed; l ++)
	{
		BSP_LCD_DisplayStringAtLine(line_count, (uint8_t *)(message + l * chars_per_line));
		line_count++;
		if (line_count > BSP_LCD_GetYSize() / BSP_LCD_GetFont()->Height)
			line_count = 0;
	}
}

/*
 * Initialise the LCD screen.
 */
void LCD_Init(void)
{
	if (BSP_LCD_Init() != LCD_OK)
		error_handler(__func__, __FILE__, __LINE__);

	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_DisplayOn();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
}

/*
 * Audio callbacks reached by the audio processing.
 */

void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
	record_buffer_offset = 0;
	record_buffer_state = BUFFER_STATE_HALF;
}

void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
	record_buffer_offset = SHORT_BUFFER_SIZE / 2;
	record_buffer_state = BUFFER_STATE_FULL;
}

void BSP_AUDIO_IN_Error_CallBack(void)
{
	printf("Audio IN error callback reached.\n");
}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
	play_buffer_offset = 0;
	play_buffer_state = BUFFER_STATE_HALF;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	play_buffer_offset = SHORT_BUFFER_SIZE / 2;
	play_buffer_state = BUFFER_STATE_FULL;
}

void BSP_AUDIO_OUT_Error_CallBack(void)
{
	printf("Audio OUT error callback reached.\n");
}

/*
 * Initialise the audio and set the audio state as listening.
 */
bool init_audio(void)
{
	if (BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_DIGITAL_MICROPHONE_2,OUTPUT_DEVICE_HEADPHONE,
			SAMPLE_RATE, DEFAULT_AUDIO_IN_BIT_RESOLUTION,DEFAULT_AUDIO_IN_CHANNEL_NBR))
	{
		printf("Audio IN/OUT initialisation failed.\n");
		return false;
	}

	if (BSP_AUDIO_IN_Record(&short_record_buffer[0], SHORT_BUFFER_SIZE) != AUDIO_OK)
	{
		printf("Recording failed.\n");
		return false;
	}

	BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);

	if (BSP_AUDIO_OUT_Play(short_play_buffer, SHORT_BUFFER_SIZE * sizeof(uint16_t)) != AUDIO_OK)
	{
		printf("Playing failed\n");
	}

	audio_state = LISTENING;

	return true;
}


float uint16_to_float(uint16_t sample)
{
	int16_t val = (int16_t) sample;
	float f = (float) val / 32768.0f;
	if ( f > 1 ) f = 1;
	if ( f < -1 ) f = -1;
	return f;
}

uint16_t float_to_uint16(float sample)
{
	// Move or float sample between 1 and 3.
	float f = sample + 2.0f;

	// Convert the previous value to an unsigned short between 32767 and 98302.
	uint16_t val = f * 32767.0f;

	return val;
}

/*
 * This function is called in the main while loop.
 * Whenever some samples are ready to be played or analysed, a float buffer
 * is sent to the application `loop` function.
 */
void process_audio(void)
{
	if (audio_state == LISTENING)
	{
		if (record_buffer_state != BUFFER_STATE_EMPTY)
		{
			// Convert half of the stereo short buffer into a float one
			for (size_t i = 0; i < FLOAT_BUFFER_SIZE; i++)
			{
				float_buffer[i] = uint16_to_float(short_record_buffer[record_buffer_offset + i * 2]);
			}

			loop(&float_buffer[0], FLOAT_BUFFER_SIZE);

			record_buffer_state = BUFFER_STATE_EMPTY;
		}
	}
	else if (audio_state == PLAYING)
	{
		if (play_buffer_state != BUFFER_STATE_EMPTY)
		{
			loop(&float_buffer[0], FLOAT_BUFFER_SIZE);

			// Convert a mono float buffer into half of a stereo short one.
			for (size_t i = 0; i < FLOAT_BUFFER_SIZE; i++)
			{
				short_play_buffer[play_buffer_offset + i * 2] = float_to_uint16(float_buffer[i]);
				short_play_buffer[play_buffer_offset + i * 2 + 1] = float_to_uint16(float_buffer[i]);
			}

			play_buffer_state = BUFFER_STATE_EMPTY;
		}
	}
}

/*
 * Main of the program. It initialises the board peripherals and loop
 * indefinitely to process any incoming or outgoing audio.
 */
int main(void)
{
	CPU_CACHE_Enable();
	HAL_Init();
	SystemClock_Config();

	UART_Init();

	// Allow to offer a pretty rendering between every starts.
	printf("\n\n");

	LCD_Init();

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) != TS_OK)
		printf("Touch screen initialisation failed.\n");

	BSP_TS_ITConfig();

	printf("Board initialised.\n");

	setup(SAMPLE_RATE);

	if (!init_audio())
	{
		printf("Audio initialisation failed.\n");
		error_handler(__func__, __FILE__, __LINE__);
	}

	printf("Audio initialised.\n");

	while (true)
	{
		process_audio();
	}

	return 0;
}

void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

void SystemClock_Config(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
		error_handler(__func__, __FILE__, __LINE__);

	/* activate the OverDrive */
	ret = HAL_PWREx_ActivateOverDrive();
	if(ret != HAL_OK)
		error_handler(__func__, __FILE__, __LINE__);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	if(ret != HAL_OK)
		error_handler(__func__, __FILE__, __LINE__);
}
