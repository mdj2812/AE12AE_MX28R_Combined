################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/RTT/SEGGER_RTT.c \
../Sources/RTT/SEGGER_RTT_printf.c 

OBJS += \
./Sources/RTT/SEGGER_RTT.o \
./Sources/RTT/SEGGER_RTT_printf.o 

C_DEPS += \
./Sources/RTT/SEGGER_RTT.d \
./Sources/RTT/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/RTT/%.o: ../Sources/RTT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/System" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/PDD" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Static_Code/IO_Map" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Generated_Code" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/include" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/config" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/kernel" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m/core/M4" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/MQXLITE/psp/cortex_m/compiler/cwgcc" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485/Servo" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RS485/CurrentMeter" -I"C:/Users/MA/workspace.kds/AE12AE_MX28R_Combined/Sources/RTT" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


