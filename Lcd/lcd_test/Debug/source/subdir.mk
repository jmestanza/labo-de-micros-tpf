################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/GFX_Lib_K64F.c \
../source/ILI9341_K64F.c \
../source/lcd_test.c \
../source/semihost_hardfault.c 

OBJS += \
./source/GFX_Lib_K64F.o \
./source/ILI9341_K64F.o \
./source/lcd_test.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/GFX_Lib_K64F.d \
./source/ILI9341_K64F.d \
./source/lcd_test.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../CMSIS -I../drivers -I../board -I../source -I../ -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


