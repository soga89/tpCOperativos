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

# All Target
all: cpu test

dependents:
	-cd /home/utnso/TPOperativos/commons/Debug && $(MAKE) all
	-cd /home/utnso/TPOperativos/delEquipo/Debug && $(MAKE) all

INVOKER_SHARED := -L"/home/utnso/TPOperativos/delEquipo/Debug" \
-L"/home/utnso/TPOperativos/commons/Debug"

# Tool invocations
cpu: $(OBJS) ./src/mainCPU.o $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "cpu" $(OBJS) ./src/mainCPU.o $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

test: $(OBJS) ./src/testCPU.o $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "testCPU" $(OBJS) $(USER_OBJS) ./src/testCPU.o $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) ./src/testCPU.o ./src/mainCPU.o $(C_DEPS)$(EXECUTABLES) cpu testCPU
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/TPOperativos/commons/Debug/libcommons.so:
/home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so:

-include ../makefile.targets
