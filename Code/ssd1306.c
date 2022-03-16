#include "ssd1306.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI_Write_Byte(SPI_TypeDef* SPIx, uint8_t write_data) {
	// TODO: perform SPI transfer
	
	// Wait for the Transmit Buffer Empty flag to become set
	while((SPIx->SR & SPI_SR_TXE) == 0);
	
	// Write data to the SPIx->DR register to begin transmission
	*((volatile uint8_t*)&SPIx->DR) = write_data;
	//SPIx->DR = write_data;
	
	// Wait for the Busy flag to become unset for the transmission to complete
	while((SPIx->SR & SPI_SR_BSY) != 0);
}

void SPI_Read_Byte(SPI_TypeDef* SPIx, uint8_t *read_data){
	// Wait for the Receive Not Empty flag to set for the data to be received
	while((SPIx->SR & SPI_SR_RXNE) == 0);
	
	// Read received data from the SPIx->DR register
	//*read_data = SPIx->DR & 0xFF;
	//read_data = (volatile uint8_t*)(SPIx->DR & 0xFF);
	*read_data = *((volatile uint8_t*)&SPIx->DR);
}

void ssd1306_Init(void){
	// Select SSD1306 as peripheral
	CS_RES;
	
	// Power ON sequence
	RESET_RES;
	for(int i = 0; i < 1000; i++);
	RESET_SET;
	for(int i = 0; i < 1000; i++);
	RESET_RES;
	
	/*
	Default settings after chip is reset:
	1. Display is OFF
	2. 128 x 64 Display Mode
	3. Normal segment and display data column and row address mapping
			(SEG0 mapped to address 00h and COM0 mapped to address 00h)
	4. Shift register data clear in serial interface
	5. Display start line is set at display RAM address 0
	6. Column address counter is set at 0
	7. Normal scan direction of the COM outputs
	8. Contrast control register is set at 7FH
	9. Normal display mode (equivalent to A4H command)
	*/
	
	// Send command to turn to display on
	COMMAND;
	/*
	SPI_Write_Byte(SPI2, 0XAE);	// display off
	SPI_Write_Byte(SPI2, 0XD5);	// set memory addressing mode
	SPI_Write_Byte(SPI2, 0X80);	// 00, horizontal addressing mode; 01, vertical
	SPI_Write_Byte(SPI2, 0XA8);	// set page start address for page addressing
	SPI_Write_Byte(SPI2, 0X3F);	// set COM output scan direction
	SPI_Write_Byte(SPI2, 0XD3);	// set low column address
	SPI_Write_Byte(SPI2, 0X00);	// set high column address
	SPI_Write_Byte(SPI2, 0X40);	// set start line address
	SPI_Write_Byte(SPI2, 0X8D);	// set contrast control register
	SPI_Write_Byte(SPI2, 0X14);
	SPI_Write_Byte(SPI2, 0X20);	// set segment re-map 0 to 127
	SPI_Write_Byte(SPI2, 0X00);	// set normal display
	SPI_Write_Byte(SPI2, 0XA1);	// set multiplex ratio (1 to 64)
	SPI_Write_Byte(SPI2, 0XC8);
	SPI_Write_Byte(SPI2, 0XDA);	// 0xa4, output follows RAM
	SPI_Write_Byte(SPI2, 0X12);	// set display offset
	SPI_Write_Byte(SPI2, 0X81);	// not offset
	SPI_Write_Byte(SPI2, 0X8F);	// set display clock divide ratio / oscillator frequency
	SPI_Write_Byte(SPI2, 0XD9);	// set divide ratio
	SPI_Write_Byte(SPI2, 0XF1);	// set pre=charge period
	SPI_Write_Byte(SPI2, 0XDB);
	SPI_Write_Byte(SPI2, 0X40);	// set com pins hardware configuration
	SPI_Write_Byte(SPI2, 0XA4);
	SPI_Write_Byte(SPI2, 0XA6);
	*/
	SPI_Write_Byte(SPI2, 0xAE);
	SPI_Write_Byte(SPI2, 0xA4);
	SPI_Write_Byte(SPI2, 0xAF);
	DATA;
	
	// Fill display
	for(int i = 0; i < BUFFER_SIZE; i++){
		SPI_Write_Byte(SPI2, 0xF0);
	}
}