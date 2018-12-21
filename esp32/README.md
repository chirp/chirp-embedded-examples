# ESP32 + SPH0645 + UDA1334

This example shows how you can send and receive data on an ESP32 board, using the SPH0645 MEMS microphone and the UDA1334 audio output.

The code will continuously listen out for the chirps, and print to the terminal when anything is detected. It will also send a random chirp to the audio output when the BOOT switch is pressed on the board.

## Audio Input

The MEMS microphone may require some calibration to centre the audio about zero. To determine this offset, you should print the audio data using `Serial.printf` and open the Serial Plotter functionality in the Arduino IDE.

    Serial.printf("%.6f\n", buffer[i]);
