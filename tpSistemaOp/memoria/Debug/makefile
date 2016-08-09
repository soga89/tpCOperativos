-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: memoria test

dependents:
	-cd /home/utnso/TPOperativos/commons/Debug && $(MAKE) clean all
	-cd /home/utnso/TPOperativos/delEquipo/Debug && $(MAKE) clean all

INVOKER_SHARED := -L"/home/utnso/TPOperativos/delEquipo/Debug" \
-L"/home/utnso/TPOperativos/commons/Debug"

# Tool invocations
memoria: $(OBJS) ./src/mainMemoria.o $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "memoria" $(OBJS) ./src/mainMemoria.o $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
test: $(OBJS) ./src/testMemoria.o $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "testMemoria" $(OBJS) ./src/testMemoria.o $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '


# Other Targets
clean:
	-$(RM) $(OBJS) ./src/mainMemoria.o ./src/testMemoria.o $(C_DEPS)$(EXECUTABLES) testMemoria memoria
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/TPOperativos/commons/Debug/libcommons.so:
/home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so:

-include ../makefile.targets
