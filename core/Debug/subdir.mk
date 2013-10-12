################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../DbNote.o \
../category.o \
../date.o \
../main.o \
../note.o 

CPP_SRCS += \
../DbNote.cpp \
../category.cpp \
../cmd.cpp \
../date.cpp \
../main.cpp \
../note.cpp 

OBJS += \
./DbNote.o \
./category.o \
./cmd.o \
./date.o \
./main.o \
./note.o 

CPP_DEPS += \
./DbNote.d \
./category.d \
./cmd.d \
./date.d \
./main.d \
./note.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


