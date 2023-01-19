################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.c 

OBJS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.o 

C_DEPS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/%.o Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/%.su: ../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/%.c Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_KEYBOARD/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Class-2f-HID_MOUSE-2f-Src

clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Class-2f-HID_MOUSE-2f-Src:
	-$(RM) ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_MOUSE/Src/usbd_hid_mouse.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Class-2f-HID_MOUSE-2f-Src

