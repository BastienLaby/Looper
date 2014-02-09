################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencv/pattern.cpp \
../src/opencv/patterndetector.cpp 

OBJS += \
./src/opencv/pattern.o \
./src/opencv/patterndetector.o 

CPP_DEPS += \
./src/opencv/pattern.d \
./src/opencv/patterndetector.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencv/%.o: ../src/opencv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


