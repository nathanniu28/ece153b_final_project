#include "SPI.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI2_GPIO_Init(void) {
	// TODO: initialize SPI2 GPIO pins
	
	// Enable GPIO Clock for Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Configure PB13
	// --------------------------------------------------------
	// Configure PB13 to be used as alternative function SPI2_SCK
	//	** Set PB13 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE13;
	GPIOB->MODER |= GPIO_MODER_MODE13_1;
	//	** Select SPI2_SCK as alternative function
	GPIOB->AFR[1] &= ~(15UL<<20);
	GPIOB->AFR[1] |= 5UL<<20;
	// Set PB13 to push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT13;
	// Set PB13 to very high speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13;
	// Set PB13 to no pull-up, no pull-down
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD13;
	// --------------------------------------------------------
	
	// Configure PB14
	// --------------------------------------------------------
		// Configure PB14 to be used as alternative function SPI2_MISO
	//	** Set PB14 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE14;
	GPIOB->MODER |= GPIO_MODER_MODE14_1;
	//	** Select SPI2_MISO as alternative function
	GPIOB->AFR[1] &= ~(15UL<<24);
	GPIOB->AFR[1] |= 5UL<<24;
	// Set PB14 to push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT14;
	// Set PB14 to very high speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED14;
	// Set PB14 to no pull-up, no pull-down
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD14;
	// --------------------------------------------------------
	
	// Configure PB15
	// --------------------------------------------------------
		// Configure PB15 to be used as alternative function SPI2_MOSI
	//	** Set PB15 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE15;
	GPIOB->MODER |= GPIO_MODER_MODE15_1;
	//	** Select SPI2_MOSI as alternative function
	GPIOB->AFR[1] &= ~(15UL<<28);
	GPIOB->AFR[1] |= 5UL<<28;
	// Set PB15 to push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT15;
	// Set PB15 to very high speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED15;
	// Set PB15 to no pull-up, no pull-down
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD15;
	// --------------------------------------------------------
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
	
	// Enable the SPI clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
	// Set the RCC SPI reset bit, then clear it to reset the SPI2 peripheral
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST;
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST;	
	// Disable the SPI enable bit
	SPI2->CR1 &= ~SPI_CR1_SPE; 
	// Configure the peripheral for full-duplex communication
	SPI2->CR1 &= ~SPI_CR1_RXONLY;
	// Configure the peripheral for 2-line unidirectional data mode
	SPI2->CR1 &= ~SPI_CR1_BIDIMODE;
	// Disable output in bidirectional mode
	SPI2->CR1 &= ~SPI_CR1_BIDIOE;
	// Configure the frame format as MSB first
	SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
	// Configure the frame format to 8-bit mode
	//SPI2->CR1 &= ~SPI_CR1_CRCL;
	SPI2->CR2 |= SPI_CR2_DS;
	SPI2->CR2 &= ~SPI_CR2_DS_3;
	// Use Motorola SPI mode
	SPI2->CR2 &= ~SPI_CR2_FRF;
	// Configure the clock to low polarity
	SPI2->CR1 &= ~SPI_CR1_CPOL;
	// Configure the clock to first clock transition
	SPI2->CR1 &= ~SPI_CR1_CPHA;
	// Set the baud rate prescaler to 16
	SPI2->CR1 |= SPI_CR1_BR;
	SPI2->CR1 &= ~SPI_CR1_BR_2;
	// Disable hardware CRC calculation
	SPI2->CR1 &= ~SPI_CR1_CRCEN;
	// Set SPI2 to master mode
	SPI2->CR1 |= SPI_CR1_MSTR;
	// Enable software slave select management
	SPI2->CR1 |= SPI_CR1_SSM;
	// Enable NSS pulse generation
	SPI2->CR2 |= SPI_CR2_NSSP;
	// Set the FIFO threshold to 1/4
	SPI2->CR2 |= SPI_CR2_FRXTH;
	// Enable the SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE; 
}
 
void SPI_Transfer_Byte(SPI_TypeDef* SPIx, uint8_t write_data, uint8_t* read_data) {
	// TODO: perform SPI transfer
	
	// Wait for the Transmit Buffer Empty flag to become set
	while((SPIx->SR & SPI_SR_TXE) == 0);
	
	// Write data to the SPIx->DR register to begin transmission
	*((volatile uint8_t*)&SPIx->DR) = write_data;
	//SPIx->DR = write_data;
	
	// Wait for the Busy flag to become unset for the transmission to complete
	while((SPIx->SR & SPI_SR_BSY) != 0);
	
	// Wait for the Receive Not Empty flag to set for the data to be received
	while((SPIx->SR & SPI_SR_RXNE) == 0);
	
	// Read received data from the SPIx->DR register
	//*read_data = SPIx->DR & 0xFF;
	//read_data = (volatile uint8_t*)(SPIx->DR & 0xFF);
	*read_data = *((volatile uint8_t*)&SPIx->DR);

}
