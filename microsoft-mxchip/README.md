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

You will need a Chirp developer account to use the SDK, which you can sign up for at the [Chirp developer hub](https://developers.chirp.io). After signing up, you'll have a Chirp application with a key and secret, and should be able to download the config file (.conf) for 16kHz audio sample rates (`16kHz` protocol in your Chirp account). The downloaded file should be named `16kHz.conf`.

You also need to have a working development environment for your MXChip. You can follow the [official IoT DevKit tutorial](https://microsoft.github.io/azure-iot-developer-kit/docs/get-started/). This will help you install all the toolchains needed. Follow the steps up until the `ST-Link configuration` section.

## Adding the Chirp SDK to the Arduino build process

Unfortunately, the Arduino toolchain doesn't offer any quick and easy way to add custom archive libraries to Arduino projects. The file needs to be added manually by following these steps:

* Create a directory named `Chirp` in the following directory:
**MacOS :** `~/Library/Arduino15/packages/AZ3166/hardware/stm32f4/1.4.1/libraries`
**Windows :** `%LOCALAPPDATA%\Arduino15\packages\AZ3166\hardware\stm32f4\1.4.1\libraries`

* Copy all the header files from the `include` directory at the root of the folder (`chirp_connect_callbacks.h`, `chirp_connect_errors.h`, `chirp_connect_states.h`, `chirp_connect.h`, `chirp_sdk_defines.h`) into the new `Chirp` directory.

* Copy the library file (`libchirp-connect_none-armv7m-cm4-softfp.a`) in the `lib` directory located at the root of folder into the directory:
**MacOS :** `~/Library/Arduino15/packages/AZ3166/hardware/stm32f4/1.4.1/system/sdk/lib`
**Windows :** `%LOCALAPPDATA%\Arduino15\packages\AZ3166\hardware\stm32f4\1.4.1\system\sdk\lib`

* Copy the [platform.local.txt](platform.local.txt) file into the directory:
**MacOS :** `~/Library/Arduino15/packages/AZ3166/hardware/stm32f4/1.4.1`
**Windows :** `%LOCALAPPDATA%\Arduino15\packages\AZ3166\hardware\stm32f4\1.4.1`

* Load the Arduino example sketch `microsoft-mxchip.ino` into Visual Studio Code, and set up any remaining bits such as your output build folder, etc...

At this point, your project should be ready to compile. Try it out with the following command:
**MacOS :** COMMAND-SHIFT-P
**Windows :** CTRL-SHIFT-P

Then select `Arduino: Verify`.

## Add your Chirp credentials

Now that the project compiles, the example code needs to be completed with your Chirp credentials.

Open `credentials.h` and copy and paste your app key, secret and config string for the `16kHz` protocol from the [Chirp developer hub](https://developers.chirp.io).

```C
#define APP_KEY     "YOUR_APP_KEY"
#define APP_SECRET  "YOUR_APP_SECRET"
#define APP_CONFIG  "YOUR_16KHZ_CONFIG"
```

At this point the code should compile and run. Plug your MXChip to the computer, execute the same command as when you tested the compilation in the previous step but select `Arduino: Upload`. This will compile the code and upload it to the board.

## Known issues and limitations

 * For some reason, at 16kHz, switching to recording mode often fails after having previously played a chirp. The quick and easy solution is to restart the program by presing the reset button.
 * At present, the only sample rate supported by the SDK on this board is 16kHz.
