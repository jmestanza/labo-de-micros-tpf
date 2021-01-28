################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/drivers/adc/adc.c" \

C_SRCS += \
../Sources/drivers/adc/adc.c \

OBJS += \
./Sources/drivers/adc/adc.o \

C_DEPS += \
./Sources/drivers/adc/adc.d \

OBJS_QUOTED += \
"./Sources/drivers/adc/adc.o" \

C_DEPS_QUOTED += \
"./Sources/drivers/adc/adc.d" \

OBJS_OS_FORMAT += \
./Sources/drivers/adc/adc.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/drivers/adc/adc.o: ../Sources/drivers/adc/adc.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/drivers/adc/adc.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/drivers/adc/adc.o"
	@echo 'Finished building: $<'
	@echo ' '


