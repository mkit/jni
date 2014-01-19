################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../simulation.cpp 

OBJS += \
./simulation.o 

CPP_DEPS += \
./simulation.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DMAKEDLL -I"C:\Program Files (x86)\Java\jdk7\include" -I"C:\Program Files (x86)\Java\jdk7\include\win32" -I"D:\Programs\OMNeT++\include" -I"D:\Programs\OMNeT++\src\common" -I"D:\Programs\OMNeT++\include\platdep" -I"D:\Programs\OMNeT++\src\envir" -I"D:\Programs\OMNeT++\src\sim" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


