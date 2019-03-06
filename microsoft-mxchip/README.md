# Microsoft MXChip IoT DevKit example

This example shows a simple demonstration of the Chirp SDK. The example will start in listening mode,
but can be changed to playing mode and vice versa by pressing the A button (on the left).

Each time the SDK starts receiving some data, the LED will turn blue.
If the data has been successfully decoded then the LED will turn green and the
hexadecimal representation of the data will be displayed on the screen.

If the decode fails, the LED will turn red.

In playing mode, each press on the B button (on the right) will start sending a
random payload of random length, and turn the LED yellow. Once the payload
has been sent, the LED will turn cyan and the hexadecimal representation of the
data will be displayed on screen.

The audio data is sent via the 3.5mm jack output.

## Requirements

- Follow the [official IoT DevKit tutorial](https://microsoft.github.io/azure-iot-developer-kit/docs/get-started/) to get set up. This guide will help you install all the toolchains needed. Follow the steps up until the `ST-Link configuration` section.

- Sign up to Chirp at the [Chirp developer hub](https://developers.chirp.io).
- Install ChirpSDK through the Arduino library manager. This can also be accessed via Visual Studio.

## Setup

Copy/paste your Chirp app key, secret and config string for the `16kHz` protocol into the `credentials.h` file.
Currently only the `16kHz` protocol will work on embedded devices.

## Building

To build the code and flash the board, simply open the `microsoft-mxchip.ino` file into Visual Studio or the Arduino IDE.
Then hit the `Arduino: Upload` button to upload to the board.

## Known issues and limitations

 * For some reason, at 16kHz, switching to recording mode often fails after having previously played a chirp. The quick and easy solution is to restart the program by presing the reset button.
 * At present, the only sample rate supported by the SDK on this board is 16kHz.
