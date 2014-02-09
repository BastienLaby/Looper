################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/glfw/lib/x11/x11_enable.c \
../lib/glfw/lib/x11/x11_fullscreen.c \
../lib/glfw/lib/x11/x11_glext.c \
../lib/glfw/lib/x11/x11_init.c \
../lib/glfw/lib/x11/x11_joystick.c \
../lib/glfw/lib/x11/x11_keysym2unicode.c \
../lib/glfw/lib/x11/x11_thread.c \
../lib/glfw/lib/x11/x11_time.c \
../lib/glfw/lib/x11/x11_window.c 

OBJS += \
./lib/glfw/lib/x11/x11_enable.o \
./lib/glfw/lib/x11/x11_fullscreen.o \
./lib/glfw/lib/x11/x11_glext.o \
./lib/glfw/lib/x11/x11_init.o \
./lib/glfw/lib/x11/x11_joystick.o \
./lib/glfw/lib/x11/x11_keysym2unicode.o \
./lib/glfw/lib/x11/x11_thread.o \
./lib/glfw/lib/x11/x11_time.o \
./lib/glfw/lib/x11/x11_window.o 

C_DEPS += \
./lib/glfw/lib/x11/x11_enable.d \
./lib/glfw/lib/x11/x11_fullscreen.d \
./lib/glfw/lib/x11/x11_glext.d \
./lib/glfw/lib/x11/x11_init.d \
./lib/glfw/lib/x11/x11_joystick.d \
./lib/glfw/lib/x11/x11_keysym2unicode.d \
./lib/glfw/lib/x11/x11_thread.d \
./lib/glfw/lib/x11/x11_time.d \
./lib/glfw/lib/x11/x11_window.d 


# Each subdirectory must supply rules for building sources it contributes
lib/glfw/lib/x11/%.o: ../lib/glfw/lib/x11/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


