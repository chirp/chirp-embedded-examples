# Chirp Embedded Examples

A selection of examples using Chirp with embedded devices

## Setup

For all of the example apps you will need to

- Sign up at [developers.chirp.io](https://developers.chirp.io) for an app key, secret and config, and copy and paste into the `credentials.h` file. Each example may require a specific configuration, see the examples README for further details.

----

### ESP32

This example shows how you can send and receive data on an ESP32 board, using the SPH0645 MEMS microphone and the UDA1334 audio output.

The code will continuously listen out for the chirps, and print to the terminal when anything is detected. It will also send a random chirp to the audio output when the BOOT switch is pressed on the board.
