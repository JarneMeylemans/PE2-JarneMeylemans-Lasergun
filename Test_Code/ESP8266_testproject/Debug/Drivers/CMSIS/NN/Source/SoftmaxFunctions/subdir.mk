################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c 

OBJS += \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.o 

C_DEPS += \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Source/SoftmaxFunctions/%.o: ../Drivers/CMSIS/NN/Source/SoftmaxFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32F103xB -I"F:/STM32Projects/ESP8266_testproject/Core/Inc" -I"F:/STM32Projects/ESP8266_testproject/Drivers/STM32F1xx_HAL_Driver/Inc" -I"F:/STM32Projects/ESP8266_testproject/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"F:/STM32Projects/ESP8266_testproject/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"F:/STM32Projects/ESP8266_testproject/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


