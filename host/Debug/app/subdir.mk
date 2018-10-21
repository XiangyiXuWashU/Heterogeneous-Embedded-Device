################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app.c \
../app/controler.c \
../app/phone_command.c \
../app/sample_receiver.c \
../app/thread.c 

OBJS += \
./app/app.o \
./app/controler.o \
./app/phone_command.o \
./app/sample_receiver.o \
./app/thread.o 

C_DEPS += \
./app/app.d \
./app/controler.d \
./app/phone_command.d \
./app/sample_receiver.d \
./app/thread.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-arago-linux-gnueabi-gcc -I/home/tl/shareSAMBA/LIB/ipc_1_25_03_15/packages -I/home/tl/shareSAMBA/LIB/bios_6_35_04_50/packages -I/home/tl/shareSAMBA/LIB/syslink_2_21_01_05/packages -I/home/tl/shareSAMBA/MINILOC/OMAPL138/shared -O3 -g3 -Wall -ffloat-store  -fPIC -Wunused -Wall -g -c -fmessage-length=0 -c -MD -MF $@.dep -march=armv5t -D_REENTRANT -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


