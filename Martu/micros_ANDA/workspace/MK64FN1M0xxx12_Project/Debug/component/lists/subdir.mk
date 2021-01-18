################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/generic_list.c 

OBJS += \
./component/lists/generic_list.o 

C_DEPS += \
./component/lists/generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/board" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/source" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/drivers" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/device" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/CMSIS" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/component/uart" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/utilities" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/component/serial_manager" -I"/Users/martinamaspero/Desktop/labo-de-micros-tpf/Martu/micros_ANDA/workspace/MK64FN1M0xxx12_Project/component/lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


