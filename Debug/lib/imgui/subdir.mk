################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/imgui/DroidSans.cpp \
../lib/imgui/imgui.cpp \
../lib/imgui/imguiRenderGL3.cpp 

OBJS += \
./lib/imgui/DroidSans.o \
./lib/imgui/imgui.o \
./lib/imgui/imguiRenderGL3.o 

CPP_DEPS += \
./lib/imgui/DroidSans.d \
./lib/imgui/imgui.d \
./lib/imgui/imguiRenderGL3.d 


# Each subdirectory must supply rules for building sources it contributes
lib/imgui/%.o: ../lib/imgui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


