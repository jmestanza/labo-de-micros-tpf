################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 7
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Sources/drivers/pit/subdir.mk
-include Sources/drivers/dma/subdir.mk
-include Sources/drivers/adc/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
dma_pit_adc_k64.elf \

EXECUTABLES_QUOTED += \
"dma_pit_adc_k64.elf" \

EXECUTABLES_OS_FORMAT += \
dma_pit_adc_k64.elf \


# All Target
all: warning-messages explicit-dependencies dma_pit_adc_k64.elf secondary-outputs

# Tool invocations
dma_pit_adc_k64.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MK64FN1M0_ram.ld
	@echo 'Building target: $@'
	@echo 'Executing target #8 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) @"dma_pit_adc_k64.args" -o"dma_pit_adc_k64.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) "./*/*/*.o" "./*/*/*/*.o" "./*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*/*/*.d" "./*/*.d"  $(ProjDirPath)/RAM/dma_pit_adc_k64.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MK64FN1M0_ram.ld 

warning-messages: 

