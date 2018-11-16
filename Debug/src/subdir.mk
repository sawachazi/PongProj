################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/display-cfal1602.c \
../src/display.c \
../src/joy_stick.c \
../src/main.c \
../src/serial_com.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

S_UPPER_SRCS += \
../src/nano_wait.S 

OBJS += \
./src/display-cfal1602.o \
./src/display.o \
./src/joy_stick.o \
./src/main.o \
./src/nano_wait.o \
./src/serial_com.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

S_UPPER_DEPS += \
./src/nano_wait.d 

C_DEPS += \
./src/display-cfal1602.d \
./src/display.d \
./src/joy_stick.d \
./src/main.d \
./src/serial_com.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/zachwalters/eclipse-workspace/PongProject/Utilities" -I"/Users/zachwalters/eclipse-workspace/PongProject/StdPeriph_Driver/inc" -I"/Users/zachwalters/eclipse-workspace/PongProject/inc" -I"/Users/zachwalters/eclipse-workspace/PongProject/CMSIS/device" -I"/Users/zachwalters/eclipse-workspace/PongProject/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/zachwalters/eclipse-workspace/PongProject/Utilities" -I"/Users/zachwalters/eclipse-workspace/PongProject/StdPeriph_Driver/inc" -I"/Users/zachwalters/eclipse-workspace/PongProject/inc" -I"/Users/zachwalters/eclipse-workspace/PongProject/CMSIS/device" -I"/Users/zachwalters/eclipse-workspace/PongProject/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


