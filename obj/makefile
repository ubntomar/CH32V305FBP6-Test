################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
FLASH_Program.hex \

SECONDARY_LIST += \
FLASH_Program.lst \

SECONDARY_SIZE += \
FLASH_Program.siz \


# All Target
all: FLASH_Program.elf secondary-outputs

# Tool invocations
FLASH_Program.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "D:\Descargas\CH32V307EVT\EVT\EXAM\SRC\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"FLASH_Program.map" --specs=nano.specs --specs=nosys.specs -o "FLASH_Program.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
FLASH_Program.hex: FLASH_Program.elf
	@	riscv-none-embed-objcopy -O ihex "FLASH_Program.elf"  "FLASH_Program.hex"
	@	@
FLASH_Program.lst: FLASH_Program.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "FLASH_Program.elf" > "FLASH_Program.lst"
	@	@
FLASH_Program.siz: FLASH_Program.elf
	@	riscv-none-embed-size --format=berkeley "FLASH_Program.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) FLASH_Program.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
