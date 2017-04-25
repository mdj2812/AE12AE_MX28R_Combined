################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/ALIM_12V_RLY.c \
../Generated_Code/Cpu.c \
../Generated_Code/MQX1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/Pins1.c \
../Generated_Code/RS485_DE.c \
../Generated_Code/RS485_RE.c \
../Generated_Code/RS485_UART.c \
../Generated_Code/SystemTimer1.c 

OBJS += \
./Generated_Code/ALIM_12V_RLY.o \
./Generated_Code/Cpu.o \
./Generated_Code/MQX1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/Pins1.o \
./Generated_Code/RS485_DE.o \
./Generated_Code/RS485_RE.o \
./Generated_Code/RS485_UART.o \
./Generated_Code/SystemTimer1.o 

C_DEPS += \
./Generated_Code/ALIM_12V_RLY.d \
./Generated_Code/Cpu.d \
./Generated_Code/MQX1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/Pins1.d \
./Generated_Code/RS485_DE.d \
./Generated_Code/RS485_RE.d \
./Generated_Code/RS485_UART.d \
./Generated_Code/SystemTimer1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/System" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/PDD" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/IO_Map" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Generated_Code" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/include" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/config" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/kernel" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m/core/M4" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m/compiler/cwgcc" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485/Servo" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485/CurrentMeter" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RTT" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


