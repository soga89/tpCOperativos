# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/funcionesCPU.c \
../src/mainCPU.c \
../src/testCPU.c 

OBJS += ./src/funcionesCPU.o 

C_DEPS += \
./src/funcionesCPU.d \
./src/mainCPU.d \
./src/testCPU.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/TPOperativos/delEquipo" -I"/home/utnso/TPOperativos/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


