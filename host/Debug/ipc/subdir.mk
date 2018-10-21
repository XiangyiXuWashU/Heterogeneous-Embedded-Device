################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ipc/message_heap.c \
../ipc/message_queue.c \
../ipc/ring_buffer_host.c 

OBJS += \
./ipc/message_heap.o \
./ipc/message_queue.o \
./ipc/ring_buffer_host.o 

C_DEPS += \
./ipc/message_heap.d \
./ipc/message_queue.d \
./ipc/ring_buffer_host.d 


# Each subdirectory must supply rules for building sources it contributes
ipc/%.o: ../ipc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-arago-linux-gnueabi-gcc -I/home/tl/shareSAMBA/LIB/ipc_1_25_03_15/packages -I/home/tl/shareSAMBA/LIB/bios_6_35_04_50/packages -I/home/tl/shareSAMBA/LIB/syslink_2_21_01_05/packages -I/home/tl/shareSAMBA/MINILOC/OMAPL138/shared -O3 -g3 -Wall -ffloat-store  -fPIC -Wunused -Wall -g -c -fmessage-length=0 -c -MD -MF $@.dep -march=armv5t -D_REENTRANT -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


