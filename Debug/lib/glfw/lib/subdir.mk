################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/glfw/lib/enable.c \
../lib/glfw/lib/fullscreen.c \
../lib/glfw/lib/glext.c \
../lib/glfw/lib/image.c \
../lib/glfw/lib/init.c \
../lib/glfw/lib/input.c \
../lib/glfw/lib/joystick.c \
../lib/glfw/lib/stream.c \
../lib/glfw/lib/tga.c \
../lib/glfw/lib/thread.c \
../lib/glfw/lib/time.c \
../lib/glfw/lib/window.c 

OBJS += \
./lib/glfw/lib/enable.o \
./lib/glfw/lib/fullscreen.o \
./lib/glfw/lib/glext.o \
./lib/glfw/lib/image.o \
./lib/glfw/lib/init.o \
./lib/glfw/lib/input.o \
./lib/glfw/lib/joystick.o \
./lib/glfw/lib/stream.o \
./lib/glfw/lib/tga.o \
./lib/glfw/lib/thread.o \
./lib/glfw/lib/time.o \
./lib/glfw/lib/window.o 

C_DEPS += \
./lib/glfw/lib/enable.d \
./lib/glfw/lib/fullscreen.d \
./lib/glfw/lib/glext.d \
./lib/glfw/lib/image.d \
./lib/glfw/lib/init.d \
./lib/glfw/lib/input.d \
./lib/glfw/lib/joystick.d \
./lib/glfw/lib/stream.d \
./lib/glfw/lib/tga.d \
./lib/glfw/lib/thread.d \
./lib/glfw/lib/time.d \
./lib/glfw/lib/window.d 


# Each subdirectory must supply rules for building sources it contributes
lib/glfw/lib/%.o: ../lib/glfw/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


