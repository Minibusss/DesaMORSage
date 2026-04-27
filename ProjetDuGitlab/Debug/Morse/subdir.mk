################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Morse/stm32g4_morse.c 

OBJS += \
./Morse/stm32g4_morse.o 

C_DEPS += \
./Morse/stm32g4_morse.d 


# Each subdirectory must supply rules for building sources it contributes
Morse/%.o Morse/%.su Morse/%.cyclo: ../Morse/%.c Morse/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -I"C:/Users/Utilisateur/Documents/FISA/E3a/S6/ElecNum/DesaMORSage/DesaMORSage/ProjetDuGitlab/drivers/bsp/MatrixKeyboard" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Morse

clean-Morse:
	-$(RM) ./Morse/stm32g4_morse.cyclo ./Morse/stm32g4_morse.d ./Morse/stm32g4_morse.o ./Morse/stm32g4_morse.su

.PHONY: clean-Morse

