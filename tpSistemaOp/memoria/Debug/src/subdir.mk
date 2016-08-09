# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/funcionesMemoria.c \
../src/mainMemoria.c \
../src/testMemoria.c 

OBJS += ./src/funcionesMemoria.o 

C_DEPS += \
./src/funcionesMemoria.d \
./src/mainMemoria.d \
./src/testMemoria.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/TPOperativos/delEquipo" -I"/home/utnso/TPOperativos/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
