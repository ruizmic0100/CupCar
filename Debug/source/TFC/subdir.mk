################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/TFC/TFC.c \
../source/TFC/TFC_ADC.c \
../source/TFC/TFC_ARM_SysTick.c \
../source/TFC/TFC_BoardSupport.c \
../source/TFC/TFC_CrystalClock.c \
../source/TFC/TFC_LineScanCamera.c \
../source/TFC/TFC_Motor.c \
../source/TFC/TFC_Queue.c \
../source/TFC/TFC_Servo.c \
../source/TFC/TFC_Terminal.c \
../source/TFC/TFC_UART.c \
../source/TFC/TFC_arm_cm0.c 

C_DEPS += \
./source/TFC/TFC.d \
./source/TFC/TFC_ADC.d \
./source/TFC/TFC_ARM_SysTick.d \
./source/TFC/TFC_BoardSupport.d \
./source/TFC/TFC_CrystalClock.d \
./source/TFC/TFC_LineScanCamera.d \
./source/TFC/TFC_Motor.d \
./source/TFC/TFC_Queue.d \
./source/TFC/TFC_Servo.d \
./source/TFC/TFC_Terminal.d \
./source/TFC/TFC_UART.d \
./source/TFC/TFC_arm_cm0.d 

OBJS += \
./source/TFC/TFC.o \
./source/TFC/TFC_ADC.o \
./source/TFC/TFC_ARM_SysTick.o \
./source/TFC/TFC_BoardSupport.o \
./source/TFC/TFC_CrystalClock.o \
./source/TFC/TFC_LineScanCamera.o \
./source/TFC/TFC_Motor.o \
./source/TFC/TFC_Queue.o \
./source/TFC/TFC_Servo.o \
./source/TFC/TFC_Terminal.o \
./source/TFC/TFC_UART.o \
./source/TFC/TFC_arm_cm0.o 


# Each subdirectory must supply rules for building sources it contributes
source/TFC/%.o: ../source/TFC/%.c source/TFC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\board" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\source" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\startup" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\drivers" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\CMSIS" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-TFC

clean-source-2f-TFC:
	-$(RM) ./source/TFC/TFC.d ./source/TFC/TFC.o ./source/TFC/TFC_ADC.d ./source/TFC/TFC_ADC.o ./source/TFC/TFC_ARM_SysTick.d ./source/TFC/TFC_ARM_SysTick.o ./source/TFC/TFC_BoardSupport.d ./source/TFC/TFC_BoardSupport.o ./source/TFC/TFC_CrystalClock.d ./source/TFC/TFC_CrystalClock.o ./source/TFC/TFC_LineScanCamera.d ./source/TFC/TFC_LineScanCamera.o ./source/TFC/TFC_Motor.d ./source/TFC/TFC_Motor.o ./source/TFC/TFC_Queue.d ./source/TFC/TFC_Queue.o ./source/TFC/TFC_Servo.d ./source/TFC/TFC_Servo.o ./source/TFC/TFC_Terminal.d ./source/TFC/TFC_Terminal.o ./source/TFC/TFC_UART.d ./source/TFC/TFC_UART.o ./source/TFC/TFC_arm_cm0.d ./source/TFC/TFC_arm_cm0.o

.PHONY: clean-source-2f-TFC

