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
all: planificador test

dependents:
	-cd /home/utnso/TPOperativos/commons/Debug && $(MAKE) clean all
	-cd /home/utnso/TPOperativos/delEquipo/Debug && $(MAKE) clean all

INVOKER_SHARED := -L"/home/utnso/TPOperativos/delEquipo/Debug" \
-L"/home/utnso/TPOperativos/commons/Debug" 

# Tool invocations
planificador: $(OBJS) ./src/mainPlanificador.o $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "planificador" $(OBJS) ./src/mainPlanificador.o  $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
test: $(OBJS) ./src/testPlanificador.o  $(USER_OBJS) /home/utnso/TPOperativos/commons/Debug/libcommons.so /home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc $(INVOKER_SHARED) -o "testPlanificador" $(OBJS) ./src/testPlanificador.o $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) ./src/mainPlanificador.o ./src/testPlanificador.o $(C_DEPS)$(EXECUTABLES) testPlanificador planificador
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/TPOperativos/commons/Debug/libcommons.so:
/home/utnso/TPOperativos/delEquipo/Debug/libdelEquipo.so:

-include ../makefile.targets
