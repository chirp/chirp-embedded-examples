CC				=	arm-none-eabi-gcc
OBJCOPY		=	arm-none-eabi-objcopy
GDB				=	arm-none-eabi-gdb

PROJ_NAME	=	chirp-stm32f746g-discovery-demo

###################################################

C_SRCS	=	src/main.c \
			src/application.c \
			src/uart.c \
			src/system_stm32f7xx.c \
			src/syscalls.c \
			src/stm32f7xx_hal_msp.c \
			src/stm32f7xx_it.c \

CFLAGS	=	-Iinclude \
			-Ichirp \
			-IMiddlewares/pdm-to-pcm/Inc

C_SRCS	+=	Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cryp.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dac.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dfsdm.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hash.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2s.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sram.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_usart.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_wwdg.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.c \
			Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c

CFLAGS	+=	-IDrivers/CMSIS/Include \
			-IDrivers/STM32F7xx_HAL_Driver/Inc

C_SRCS	+=	Drivers/BSP/Components/otm8009a/otm8009a.c \
			Drivers/BSP/Components/wm8994/wm8994.c \
			Drivers/BSP/Components/ft5336/ft5336.c

CFLAGS	+=	-IDrivers/BSP/Components/otm8009a \
			-IDrivers/BSP/Components/ft6x06

C_SRCS	+=	Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c \
			Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.c \
			Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c \
			Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c \
			Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c

CFLAGS	+=	-IDrivers/BSP/STM32746G-Discovery

C_SRCS	+=	Utilities/Fonts/font8.c \
			Utilities/Fonts/font12.c \
			Utilities/Fonts/font16.c \
			Utilities/Fonts/font20.c \
			Utilities/Fonts/font24.c

CFLAGS	+=	-IUtilities/Fonts

C_OBJS	=	$(C_SRCS:.c=.o)

ASM_SRCS	=	src/startup_stm32f746xx.S

ASM_OBJS	=	$(ASM_SRCS:.S=.o)

###################################################

FLOAT_ABI	=	hard

CFLAGS	+=	-DSTM32F746xx \
			-DUSE_HAL_DRIVER \
			-Og \
			-g3 \
			-Wall \
			-mthumb \
			-mcpu=cortex-m4 \
			-mfloat-abi=$(FLOAT_ABI) \
			-mfpu=fpv4-sp-d16 \
			-fmessage-length=0 \
			-fsigned-char \
			-ffunction-sections \
			-fdata-sections \
			-Wl,-Map,"$(PROJ_NAME).map"

LDFLAGS	=	-Tldscripts/STM32F746NGHx_FLASH.ld \
			-Xlinker --gc-sections \
			--specs=nano.specs \
			-u _printf_float \
			-Lchirp \
			-LMiddlewares/pdm-to-pcm/Lib

LIBS	=	-lchirp-connect_none-armv7m-cm7-$(FLOAT_ABI) \
				-lPDMFilter_CM7_GCC

all:	proj

proj:	$(PROJ_NAME).elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROJ_NAME).elf: $(C_OBJS) $(ASM_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(C_OBJS) $(ASM_OBJS) $(LIBS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

flash: proj
	st-flash --format ihex write $(PROJ_NAME).hex

debug: flash
	st-util & $(GDB) -silent -ex 'target extended-remote localhost:4242' $(PROJ_NAME).elf

clean:
	rm -f $(C_OBJS) $(ASM_OBJS)

cleanall: clean
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
