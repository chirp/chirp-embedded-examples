/*------------------------------------------------------------------------------
 *
 *  render.cpp
 *
 *  This exemple sends "Hello World !" when it starts and decodes it at the same
 *  time.
 *
 *  For full information on usage and licensing, see https://chirp.io/
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#include <Bela.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "credentials.h"
#include "chirp_connect.h"

/*
 * Mirrors the block size set in the Bela config.
 */
static uint8_t block_size = 0;

/*
 * Size of the circular buffer.
 * Must be a multiple of the block size.
 */
static uint32_t buffer_size = 0;

/*
 * Global buffer used to store incoming audio data before
 * it is being processed in a lower priority task.
 */
static float *input_buffer = NULL;

/*
 * Global buffer used to store outgoing audio data before
 * it is being processed in a lower priority task.
 */
static float *output_buffer = NULL;

/*
 * Input channel used. Chirp only support audio mono data.
 */
int input_channel = 0;

/*
 * Global instance of the SDK.
 */
static chirp_connect_t *connect = NULL;

/*
 * Auxiliary task to process the incoming audio.
 */
static AuxiliaryTask chirp_auxiliary_input_task = NULL;

/*
 * Auxiliary task to process the outgoing audio.
 */
static AuxiliaryTask chirp_auxiliary_output_task = NULL;

void chirp_process_input_audio(void* data)
{
    static int read_pointer = 0;
    chirp_connect_t *connect = (chirp_connect_t *) data;
    if (chirp_connect_process_input(connect, &input_buffer[read_pointer], block_size) != CHIRP_CONNECT_OK)
    {
        rt_printf("Process decoding error\n");
    }
    read_pointer = (read_pointer + block_size) % buffer_size;
}

void chirp_process_output_audio(void* data)
{
    static int write_pointer = 0;
    chirp_connect_t *connect = (chirp_connect_t *) data;
    if (chirp_connect_process_output(connect, &output_buffer[write_pointer], block_size) != CHIRP_CONNECT_OK)
    {
        rt_printf("Process encoding error\n");
    }
    write_pointer = (write_pointer + block_size) % buffer_size;
}

void on_received_callback(void *ptr, uint8_t *payload, size_t length, uint8_t channel)
{
    if (payload)
    {
        char *msg = (char *) calloc(length + 1, sizeof(char));
        memcpy(msg, payload, sizeof(char) * length);
        rt_printf("Data received : %s\n", msg);
        free(msg);
    }
    else
    {
        rt_printf("Decoding failed\n");
    }

}

void on_state_changed_callback(void *ptr, chirp_connect_state_t old_state, chirp_connect_state_t new_state)
{
    rt_printf("State changed\n");
}

void on_receiving_callback(void *ptr, uint8_t *payload, size_t length, uint8_t channel)
{
    rt_printf("Receiving data\n");
}

void on_sent_callback(void *ptr, uint8_t *payload, size_t length, uint8_t channel)
{
    rt_printf("Data sent\n");
}

void on_sending_callback(void *ptr, uint8_t *payload, size_t length, uint8_t channel)
{
    rt_printf("Sending data\n");
}

bool setup(BelaContext *context, void *userData)
{
    /*
     * Initialise global variables and buffers.
     */
    input_channel = 0;
    block_size = context->audioFrames;
    buffer_size = block_size * 8;
    input_buffer = (float*) calloc(buffer_size, sizeof(float));
    output_buffer = (float*) calloc(buffer_size, sizeof(float));


    connect = new_chirp_connect(CHIRP_APP_KEY, CHIRP_APP_SECRET);
    if (connect == NULL)
    {
        rt_printf("Connect is NULL\n");
    }

    chirp_connect_error_code_t ret = chirp_connect_set_config(connect, CHIRP_APP_CONFIG);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Set config error\n");
    }

    chirp_connect_callback_set_t callbacks = {0};
    callbacks.on_state_changed = on_state_changed_callback;
    callbacks.on_sending = on_sending_callback;
    callbacks.on_sent = on_sent_callback;
    callbacks.on_receiving = on_receiving_callback;
    callbacks.on_received = on_received_callback;
    ret = chirp_connect_set_callbacks(connect, callbacks);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Callback error\n");
    }

    ret = chirp_connect_set_callback_ptr(connect, connect);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Callbackptr error\n");
    }

    /*
     * Tell the SDK it can decode what it sends.
     */
    ret = chirp_connect_set_auto_mute(connect, false);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Auto mute error\n");
    }

    ret = chirp_connect_set_volume(connect, 0.9);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Set volume error\n");
    }

    ret = chirp_connect_set_input_sample_rate(connect, context->audioSampleRate);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Set input sample rate error\n");
    }

    ret = chirp_connect_set_output_sample_rate(connect, context->audioSampleRate);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Set output sample rate error\n");
    }

    ret = chirp_connect_start(connect);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Start connect error\n");
    }

    /*
     * Start the auxiliary tasks
     */
    if ((chirp_auxiliary_input_task = Bela_createAuxiliaryTask (&chirp_process_input_audio, 80, "chirp-auxiliary-input-task", connect)) == NULL) return false;

    if ((chirp_auxiliary_output_task = Bela_createAuxiliaryTask (&chirp_process_output_audio, 80, "chirp-auxiliary-output-task", connect)) == NULL) return false;

    char *info = chirp_connect_get_info(connect);
    rt_printf("%s - Version : %s \n", info, chirp_connect_get_version());
    chirp_connect_free(info);

    std::string payload = "Hello World !";
    size_t payload_length = strlen(payload.c_str());

    ret = chirp_connect_send(connect, (uint8_t *) payload.c_str(), payload_length);
    if(ret != CHIRP_CONNECT_OK)
    {
        rt_printf("Send error\n");
    }

    return true;
}

void render(BelaContext *context, void *userData)
{
    /*
     * To keep track of where to write the samples in the input buffer.
     */
    static int write_pointer = 0;

    /*
     * To keep track of where to read the samples in the output buffer.
     */
    static int read_pointer = 0;

    if (Bela_scheduleAuxiliaryTask(chirp_auxiliary_output_task) != 0)
    {
        rt_printf("Task scheduling for output failed\n");
    }

    for(unsigned int n = 0; n < context->audioFrames; n++)
    {
        input_buffer[write_pointer + n] = context->audioIn[n * context->audioInChannels + input_channel];

        for(unsigned int channel = 0; channel < context->audioOutChannels; channel++)
        {
            context->audioOut[n * context->audioOutChannels + channel] = output_buffer[read_pointer + n];
        }
    }

    read_pointer = (read_pointer + context->audioFrames) % buffer_size;
    write_pointer = (write_pointer + context->audioFrames) % buffer_size;

    if (Bela_scheduleAuxiliaryTask(chirp_auxiliary_input_task) != 0)
    {
        rt_printf("Task scheduling for input failed\n");
    }
}

void cleanup(BelaContext *context, void *userData)
{
    del_chirp_connect(&connect);
    free(input_buffer);
    free(output_buffer);
}
