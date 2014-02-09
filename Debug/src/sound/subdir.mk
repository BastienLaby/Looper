################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sound/SoundPlayer.cpp 

OBJS += \
./src/sound/SoundPlayer.o 

CPP_DEPS += \
./src/sound/SoundPlayer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sound/%.o: ../src/sound/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


