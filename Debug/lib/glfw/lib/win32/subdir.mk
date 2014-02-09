################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/glfw/lib/win32/win32_dllmain.c \
../lib/glfw/lib/win32/win32_enable.c \
../lib/glfw/lib/win32/win32_fullscreen.c \
../lib/glfw/lib/win32/win32_glext.c \
../lib/glfw/lib/win32/win32_init.c \
../lib/glfw/lib/win32/win32_joystick.c \
../lib/glfw/lib/win32/win32_thread.c \
../lib/glfw/lib/win32/win32_time.c \
../lib/glfw/lib/win32/win32_window.c 

OBJS += \
./lib/glfw/lib/win32/win32_dllmain.o \
./lib/glfw/lib/win32/win32_enable.o \
./lib/glfw/lib/win32/win32_fullscreen.o \
./lib/glfw/lib/win32/win32_glext.o \
./lib/glfw/lib/win32/win32_init.o \
./lib/glfw/lib/win32/win32_joystick.o \
./lib/glfw/lib/win32/win32_thread.o \
./lib/glfw/lib/win32/win32_time.o \
./lib/glfw/lib/win32/win32_window.o 

C_DEPS += \
./lib/glfw/lib/win32/win32_dllmain.d \
./lib/glfw/lib/win32/win32_enable.d \
./lib/glfw/lib/win32/win32_fullscreen.d \
./lib/glfw/lib/win32/win32_glext.d \
./lib/glfw/lib/win32/win32_init.d \
./lib/glfw/lib/win32/win32_joystick.d \
./lib/glfw/lib/win32/win32_thread.d \
./lib/glfw/lib/win32/win32_time.d \
./lib/glfw/lib/win32/win32_window.d 


# Each subdirectory must supply rules for building sources it contributes
lib/glfw/lib/win32/%.o: ../lib/glfw/lib/win32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


