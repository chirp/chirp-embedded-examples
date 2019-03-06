/**-----------------------------------------------------------------------------
 *
 *  Simple example of the Chirp Connect Arm SDK on the STM32F469I-DISCOVERY.
 *
 *  @file application.c
 *
 *  @brief After creating a developer account on https://developers.chirp.io,
 *  get your key, secret and config string from your account using the "16kHz"
 *  protocol, and set them in the credentials.h file.
 *
 *  This example will start in listening mode. The listening and playing modes
 *  can alternate by pressing the user button (blue one).
 *
 *  Each time the SDK starts receiving some data, the screen will turn blue.
 *  If the data has been successfully decoded then the screen will turn green
 *  and will display the hexadecimal representation of the data as well as the
 *  length of the message, in bytes. If the decode fails, the screen will turn
 *  red and display "Decode failed.".
 *
 *  In playing mode, each touch on the screen will start sending a
 *  random payload of random length, and turn the screen yellow. Once the
 *  payload has been sent, the screen will be white and the hexadecimal
 *  representation of the data sent as well as the length of the payload,
 *  in bytes, will be displayed. The audio data is sent via the 3.5mm green
 *  jack output.
 *
 *  See README.md for further information and known issues.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/
#include <stdlib.h>
/*
 * Main Chirp Connect header. This header and the ones it depends on must be in
 * the same folder. For this example it is the `chirp` one.
 */
#include "chirp_connect.h"
#include "credentials.h"

/*
 * Main header regrouping any header needed in the project.
 */
#include "main.h"

/*
 * Global pointer to the SDK structure. This is global as this pointer is
 * needed when processing the audio in the loop() function.
 */
chirp_connect_t *connect = NULL;

/*
 * Simple error handler which display an error message on the serial port and
 * loop indefinitely.
 */
void chirp_error_handler(chirp_connect_error_code_t errorCode)
{
	if (errorCode != CHIRP_CONNECT_OK)
	{
		const char *error_string = chirp_connect_error_code_to_string(errorCode);
		printf("Chirp error handler : %s\n", error_string);
		chirp_connect_free((void *) error_string);
		while(true);
	}
}

/*
 * Every time the screen is touched, this function is called.
 */
void on_screen_touch(void)
{
	// A length of 0 means random length
	size_t payload_length = 0;
	uint8_t *payload = chirp_connect_random_payload(connect, &payload_length);

	// At this point, the value of `payload_length` has been updated with the new
	// random length
	chirp_connect_error_code_t error = chirp_connect_send(connect, payload, payload_length);
	chirp_connect_free(payload);
	if (error != CHIRP_CONNECT_OK)
		chirp_error_handler(error);
}

/*
 * Callback reached when the SDK starts sending data.
 */
void on_sending_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
	// This call erase all previously displayed elements on the screen.
	set_screen_color(LCD_COLOR_YELLOW);
}

/*
 * Callback reached when the SDK has sent the data.
 */
void on_sent_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
	char *hexa_string = chirp_connect_as_string(connect, payload, length);
	char str_length[16];
	itoa((int) length, str_length, 10);
	set_screen_color(LCD_COLOR_WHITE);
	display_message(hexa_string, LCD_COLOR_BLACK);
	display_message(str_length, LCD_COLOR_BLACK);
	chirp_connect_free(hexa_string);
}

void on_receiving_callback(void *connect, uint8_t *payload, size_t length, uint8_t channel)
{
	set_screen_color(LCD_COLOR_BLUE);
	printf("Receiving data.\n");
}

void on_received_callback(void *connect, uint8_t *payload, size_t length, uint8_t channel)
{
	if (payload)
	{
		set_screen_color(LCD_COLOR_GREEN);
		char *hexa_string = chirp_connect_as_string(connect, payload, length);
		char str_length[16];
		itoa((int) length, str_length, 10);
		display_message(hexa_string, LCD_COLOR_BLACK);
		display_message(str_length, LCD_COLOR_BLACK);
	}
	else
	{
		set_screen_color(LCD_COLOR_RED);
		display_message("Decoding failed.", LCD_COLOR_BLACK);
	}
}

/*
 * This function is called once when the program starts. Use it to initialise
 * anything you will need later in the processing.
 * From Chirp's side, we are initialising the SDK.
 */
void setup(uint32_t sample_rate)
{
	connect = new_chirp_connect(CHIRP_APP_KEY, CHIRP_APP_SECRET);
	if (connect == NULL)
	{
		printf("Chirp Connect initialisation failed.\n");
		error_handler(__func__, __FILE__, __LINE__);
	}

	chirp_connect_error_code_t err = chirp_connect_set_config(connect, CHIRP_APP_CONFIG);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	err = chirp_connect_set_input_sample_rate(connect, sample_rate);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	err = chirp_connect_set_output_sample_rate(connect, sample_rate);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	chirp_connect_callback_set_t callbacks = {0};
	callbacks.on_sending = on_sending_callback;
	callbacks.on_sent = on_sent_callback;
	callbacks.on_receiving = on_receiving_callback;
	callbacks.on_received = on_received_callback;
	err = chirp_connect_set_callbacks(connect, callbacks);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	err = chirp_connect_set_callback_ptr(connect, connect);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	// It looks the audio output gets saturated when playing with the default
	// software volume of 1 thus we decrease it to get a clean output.
	err = chirp_connect_set_volume(connect, 0.25f);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	err = chirp_connect_start(connect);
	if (err != CHIRP_CONNECT_OK)
		chirp_error_handler(err);

	printf("Chirp Connect initialised.\n");
}

/*
 * Audio processing loop function. When called, this function gives a buffer
 * full of samples recorded by the microphone. To play back, one must fill the
 * buffer before leaving the function.
 */
void loop(float *buffer, uint16_t blocksize)
{
	chirp_connect_error_code_t error;

	error = chirp_connect_process_input(connect, buffer, blocksize);
	if (error != CHIRP_CONNECT_OK)
		chirp_error_handler(error);

	// When not sending data, this function fills the buffer with zeroes so it
	// is fine to leave it even in listening mode.
	error = chirp_connect_process_output(connect, buffer, blocksize);
	if (error != CHIRP_CONNECT_OK)
		chirp_error_handler(error);

}
