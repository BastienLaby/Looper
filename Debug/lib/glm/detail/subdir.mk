################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/glm/detail/dummy.cpp \
../lib/glm/detail/glm.cpp 

OBJS += \
./lib/glm/detail/dummy.o \
./lib/glm/detail/glm.o 

CPP_DEPS += \
./lib/glm/detail/dummy.d \
./lib/glm/detail/glm.d 


# Each subdirectory must supply rules for building sources it contributes
lib/glm/detail/%.o: ../lib/glm/detail/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


