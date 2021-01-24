################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/drivers/dma/dma.c" \

C_SRCS += \
../Sources/drivers/dma/dma.c \

OBJS += \
./Sources/drivers/dma/dma.o \

C_DEPS += \
./Sources/drivers/dma/dma.d \

OBJS_QUOTED += \
"./Sources/drivers/dma/dma.o" \

C_DEPS_QUOTED += \
"./Sources/drivers/dma/dma.d" \

OBJS_OS_FORMAT += \
./Sources/drivers/dma/dma.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/drivers/dma/dma.o: ../Sources/drivers/dma/dma.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/drivers/dma/dma.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/drivers/dma/dma.o"
	@echo 'Finished building: $<'
	@echo ' '


