################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../model/DropTailPriorityQueue.cc \
../model/PriorityCompare.cc \
../model/PriorityEtherFrame_m.cc \
../model/PriorityFrameEncap.cc \
../model/PriorityMACRelayUnitNP.cc \
../model/PriorityMACRelayUnitNPAccessPoint.cc \
../model/PriorityTrafficGenerator.cc 

OBJS += \
./model/DropTailPriorityQueue.o \
./model/PriorityCompare.o \
./model/PriorityEtherFrame_m.o \
./model/PriorityFrameEncap.o \
./model/PriorityMACRelayUnitNP.o \
./model/PriorityMACRelayUnitNPAccessPoint.o \
./model/PriorityTrafficGenerator.o 

CC_DEPS += \
./model/DropTailPriorityQueue.d \
./model/PriorityCompare.d \
./model/PriorityEtherFrame_m.d \
./model/PriorityFrameEncap.d \
./model/PriorityMACRelayUnitNP.d \
./model/PriorityMACRelayUnitNPAccessPoint.d \
./model/PriorityTrafficGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
model/%.o: ../model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"D:\Programs\OMNeT++\include" -I"D:\Workspaces\OMNeT++\Workspace\inet\src\linklayer\ethernet\switch" -I"D:\Workspaces\OMNeT++\Workspace\inet\src\linklayer\ethernet" -I"D:\Workspaces\OMNeT++\Workspace\inet\src\linklayer\queue" -I"D:\Workspaces\OMNeT++\Workspace\inet\src\linklayer\contract" -I"D:\Workspaces\OMNeT++\Workspace\inet\src\base" -I"D:\Workspaces\Eclipse\JDEECo\CrossIntegration" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


