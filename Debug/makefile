################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/sound/subdir.mk
-include src/opencv/subdir.mk
-include src/fouch/subdir.mk
-include src/subdir.mk
-include obj/Debug/stb/subdir.mk
-include obj/Debug/imgui/subdir.mk
-include obj/Debug/glfw/subdir.mk
-include obj/Debug/glew/subdir.mk
-include obj/Debug/Looper/subdir.mk
-include lib/stb/subdir.mk
-include lib/imgui/subdir.mk
-include lib/glm/detail/subdir.mk
-include lib/glfw/lib/x11/subdir.mk
-include lib/glfw/lib/win32/subdir.mk
-include lib/glfw/lib/cocoa/subdir.mk
-include lib/glfw/lib/carbon/subdir.mk
-include lib/glfw/lib/subdir.mk
-include lib/glew/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Looper

# Tool invocations
Looper: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "Looper" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) Looper
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
