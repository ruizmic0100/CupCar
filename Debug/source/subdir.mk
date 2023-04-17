################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Ported_TFC_A.c \
../source/mtb.c \
../source/pid.c \
../source/semihost_hardfault.c \
../source/tpm.c 

C_DEPS += \
./source/Ported_TFC_A.d \
./source/mtb.d \
./source/pid.d \
./source/semihost_hardfault.d \
./source/tpm.d 

OBJS += \
./source/Ported_TFC_A.o \
./source/mtb.o \
./source/pid.o \
./source/semihost_hardfault.o \
./source/tpm.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\board" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\source" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\startup" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\drivers" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\CMSIS" -I"C:\Users\ruizm\Downloads\TFC_CupCar_Start\Ported_TFC\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Ported_TFC_A.d ./source/Ported_TFC_A.o ./source/mtb.d ./source/mtb.o ./source/pid.d ./source/pid.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/tpm.d ./source/tpm.o

.PHONY: clean-source

