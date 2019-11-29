# STM32F469I-DISCOVERY example

This example shows a simple demonstration of the Chirp C SDK. The example will start in listening mode,
but can be changed to playing mode and vice versa by pressing the user button (blue one).

Each time the SDK starts receiving some data, the screen will turn blue.
If the data has been successfully decoded then the screen will turn green and the
hexadecimal representation of the data will be displayed on the screen.

If the decode fails, the screen will turn red.

In playing mode, each touch on the screen will start sending a
random payload of random length, and turn the screen white. Once the payload
has been sent, the screen will display the hexadecimal representation of the payload.

The audio data is sent via the 3.5mm jack output.

## Requirements

- Sign up to Chirp at the [Chirp developer hub](https://developers.chirp.io).
- [OpenOCD](http://openocd.org) - for macOS users `brew install openocd` does the magic.
- [ST-LINK](https://github.com/texane/stlink) - for macOS users `brew install stlink` or your can build the binaries yourself.
- [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) - for macOS users `brew cask install gcc-arm-embedded`.

## Setup

Copy/paste your Chirp app key, secret and config string for the `16kHz-mono-embedded` protocol into the `credentials.h` file.
Currently only the `16kHz-mono-embedded` protocol will work on embedded devices.

## Building

Once you have entered your Chirp credentials into the `credentials.h` file, you can compile the example with

    make

and flash the board with

    make flash

To clean the project

    make cleanall

## Debugging

If the project doesn't run out of the box, you can debug using a Serial Monitor. The example will print
debugging information to the terminal.

---

# Eclipse

## Requirements

Depending on how your Eclipse setup is, you might need to set up your compiler and OpenOCD paths in Eclipse.

- An Eclipse CDT installation. You can download the latest version of Eclipse [here](https://www.eclipse.org/cdt/downloads.php)

## Building in Eclipse

This example is ready to open in Eclipse.

Just right click on `chirp-sdk-stm32f469i-discovery-demo Debug.launch` -> `Debug As` -> `chirp-sdk-stm32f469i-discovery-demo Debug`.

## Known issues and limitations

 * For some reason, the theoretical conversion form float to unsigned short doesn't work well. To avoid this issue, the float value is moved from being between 0 and 2 to being between 1 and 3. This create some discontinuities in the buffer containing the unsigned short values but the audio output is correct.
