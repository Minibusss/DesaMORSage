################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/main.c \
../app/stm32g4_morse.c 

OBJS += \
./app/main.o \
./app/stm32g4_morse.o 

C_DEPS += \
./app/main.d \
./app/stm32g4_morse.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -I"C:/Users/Utilisateur/Documents/FISA/E3a/S6/ElecNum/DesaMORSage/DesaMORSage/ProjetDuGitlab/drivers/bsp/MatrixKeyboard" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/main.cyclo ./app/main.d ./app/main.o ./app/main.su ./app/stm32g4_morse.cyclo ./app/stm32g4_morse.d ./app/stm32g4_morse.o ./app/stm32g4_morse.su

.PHONY: clean-app

