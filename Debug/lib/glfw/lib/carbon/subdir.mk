################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/glfw/lib/carbon/carbon_enable.c \
../lib/glfw/lib/carbon/carbon_fullscreen.c \
../lib/glfw/lib/carbon/carbon_glext.c \
../lib/glfw/lib/carbon/carbon_init.c \
../lib/glfw/lib/carbon/carbon_joystick.c \
../lib/glfw/lib/carbon/carbon_thread.c \
../lib/glfw/lib/carbon/carbon_time.c \
../lib/glfw/lib/carbon/carbon_window.c 

OBJS += \
./lib/glfw/lib/carbon/carbon_enable.o \
./lib/glfw/lib/carbon/carbon_fullscreen.o \
./lib/glfw/lib/carbon/carbon_glext.o \
./lib/glfw/lib/carbon/carbon_init.o \
./lib/glfw/lib/carbon/carbon_joystick.o \
./lib/glfw/lib/carbon/carbon_thread.o \
./lib/glfw/lib/carbon/carbon_time.o \
./lib/glfw/lib/carbon/carbon_window.o 

C_DEPS += \
./lib/glfw/lib/carbon/carbon_enable.d \
./lib/glfw/lib/carbon/carbon_fullscreen.d \
./lib/glfw/lib/carbon/carbon_glext.d \
./lib/glfw/lib/carbon/carbon_init.d \
./lib/glfw/lib/carbon/carbon_joystick.d \
./lib/glfw/lib/carbon/carbon_thread.d \
./lib/glfw/lib/carbon/carbon_time.d \
./lib/glfw/lib/carbon/carbon_window.d 


# Each subdirectory must supply rules for building sources it contributes
lib/glfw/lib/carbon/%.o: ../lib/glfw/lib/carbon/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


