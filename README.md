# Chirp Embedded Examples

A selection of examples using Chirp with embedded devices

## Setup

For all of the example apps you will need to

- Sign up at [developers.chirp.io](https://developers.chirp.io)
- Copy/paste your Chirp app key, secret and config string into the `credentials.h` file

----

## Microsoft MXChip

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

See the [README](microsoft-mxchip/README.md) for further details.

## STM32F469i Discovery

This example shows a simple demonstration of the Chirp SDK. The example will start in listening mode,
but can be changed to playing mode and vice versa by pressing the user button (blue one).

Each time the SDK starts receiving some data, the screen will turn blue.
If the data has been successfully decoded then the screen will turn green and the
hexadecimal representation of the data will be displayed on the screen.

If the decode fails, the screen will turn red.

In playing mode, each touch on the screen will start sending a
random payload of random length, and turn the screen white. Once the payload
has been sent, the screen will display the hexadecimal representation of the payload.

The audio data is sent via the 3.5mm jack output.

See the [README](stm32f469i-discovery/README.md) for further details.

## STM32F746g Discovery

This example shows a simple demonstration of the Chirp SDK. The example will start in listening mode,
but can be changed to playing mode and vice versa by pressing the user button (blue one).

Each time the SDK starts receiving some data, the screen will turn blue.
If the data has been successfully decoded then the screen will turn green and the
hexadecimal representation of the data will be displayed on the screen.

If the decode fails, the screen will turn red.

In playing mode, each touch on the screen will start sending a
random payload of random length, and turn the screen white. Once the payload
has been sent, the screen will display the hexadecimal representation of the payload.

The audio data is sent via the 3.5mm jack output.

See the [README](stm32f746g-discovery/README.md) for further details.
