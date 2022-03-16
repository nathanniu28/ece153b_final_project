#ifndef __STM32L476R_NUCLEO_SSD1306_H
#define __STM32L476R_NUCLEO_SSD1306_H

#include "stm32l476xx.h"
#include "SPI.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define BUFFER_SIZE 1024

#define CS_SET GPIOB->BSRR |= GPIO_BSRR_BS3			// PB3, CS pin
#define CS_RES GPIOB->BSRR |= GPIO_BSRR_BR3			// PB3, CS pin
#define RESET_SET GPIOB->BSRR |= GPIO_BSRR_BS5	// PB5, Rst pin
#define RESET_RES GPIOB->BSRR |= GPIO_BSRR_BR5	// PB5, Rst pin
#define DATA GPIOB->BSRR |= GPIO_BSRR_BS4				// PB4, DC pin
#define COMMAND GPIOB->BSRR |= GPIO_BSRR_BR4		// PB4, DC pin

void SPI_Write_Byte(SPI_TypeDef* SPIx, uint8_t write_data);
void SPI_Read_Byte(SPI_TypeDef* SPIx, uint8_t *read_data);
void ssd1306_Init(void);
void EXTI_Init(void);

#endif 
