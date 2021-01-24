################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/drivers/pit/pit.c" \

C_SRCS += \
../Sources/drivers/pit/pit.c \

OBJS += \
./Sources/drivers/pit/pit.o \

C_DEPS += \
./Sources/drivers/pit/pit.d \

OBJS_QUOTED += \
"./Sources/drivers/pit/pit.o" \

C_DEPS_QUOTED += \
"./Sources/drivers/pit/pit.d" \

OBJS_OS_FORMAT += \
./Sources/drivers/pit/pit.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/drivers/pit/pit.o: ../Sources/drivers/pit/pit.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/drivers/pit/pit.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/drivers/pit/pit.o"
	@echo 'Finished building: $<'
	@echo ' '


