################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../delEquipo/archivoConfig.c \
../delEquipo/mapeoAMemoria.c \
../delEquipo/protocolo.c \
../delEquipo/redireccionIO.c \
../delEquipo/sockets.c 

OBJS += \
./delEquipo/archivoConfig.o \
./delEquipo/mapeoAMemoria.o \
./delEquipo/protocolo.o \
./delEquipo/redireccionIO.o \
./delEquipo/sockets.o 

C_DEPS += \
./delEquipo/archivoConfig.d \
./delEquipo/mapeoAMemoria.d \
./delEquipo/protocolo.d \
./delEquipo/redireccionIO.d \
./delEquipo/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
delEquipo/%.o: ../delEquipo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/TPOperativos/commons" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


