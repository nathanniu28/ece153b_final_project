#include "I2C.h"

extern void Error_Handler(void);

// Inter-integrated Circuit Interface (I2C)
// up to 100 Kbit/s in the standard mode, 
// up to 400 Kbit/s in the fast mode, and 
// up to 3.4 Mbit/s in the high-speed mode.

// Recommended external pull-up resistance is 
// 4.7 kOmh for low speed, 
// 3.0 kOmh for the standard mode, and 
// 1.0 kOmh for the fast mode
	
//===============================================================================
//                        I2C GPIO Initialization
//===============================================================================
void I2C_GPIO_Init(void) {
	// Configure PB6 and PB7
	// --------------------------------------------------------
	// Enable GPIO Clock for Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// Configure PB6 to be used as alternative function I2C1_SCL
	//	** Set PB6 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE6;				// Clear bits 12 and 13 for Pin 6
	GPIOB->MODER |= GPIO_MODER_MODE6_1;				// Set bit 13 for Pin 6
	//	** Select I2C1_SCL as alternative function
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;				// Clear bits for AFRL6
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;			// Set alternative function number to AF4
	// Configure PB7 to be used as alternative function I2C1_SDA
	//	** Set PB7 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE7;				// Clear bits 14 and 15 for Pin 7
	GPIOB->MODER |= GPIO_MODER_MODE7_1;				// Set bit 15 for Pin 7
	//	** Select I2C1_SDA as alternative function
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7;				// Clear bits for AFRL7
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;			// Set alternative function number to AF4
	// Both GPIO pins should have an open-drain output type
	GPIOB->OTYPER |= GPIO_OTYPER_OT6;					// PB6
	GPIOB->OTYPER |= GPIO_OTYPER_OT7;					// PB7
	// Both GPIO pins should operate at very high speeds
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;		// PB6
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;		// PB7
	// Configure both GPIO pins to use pull-up resistors for I/O
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;				// PB6
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;				// PB6
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7;				// PB7
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;				// PB7
	// --------------------------------------------------------
}
	
#define I2C_TIMINGR_PRESC_POS	28
#define I2C_TIMINGR_SCLDEL_POS	20
#define I2C_TIMINGR_SDADEL_POS	16
#define I2C_TIMINGR_SCLH_POS	8
#define I2C_TIMINGR_SCLL_POS	0

//===============================================================================
//                          I2C Initialization
//===============================================================================
void I2C_Initialization(void){
	uint32_t OwnAddr = 0x52;
	
	// Configure I2C settings
	
	// Set up the clock for I2C in the RCC registers
	// --------------------------------------------------------
	// Enable the clock for I2C1 in the peripheral clock enable register
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
	// Set the system clock as the clock source for I2C1 in the peripherals independent clock configuration register
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_0;
	// Reset I2C1 by setting bits in the peripheral reset register
	// Then, clear the bits so that I2C does not remain in a reset state
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST;
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;
	// --------------------------------------------------------
	
	// Configure the I2C registers for communication with the temperature sensor
	// --------------------------------------------------------
	// Disable I2C
	I2C1->CR1 &= ~I2C_CR1_PE;
	// Disable the analog noise filter, enable the digital noise filter, enable error interrupts, and enable clock stretching
	I2C1->CR1 |= I2C_CR1_ANFOFF;
	I2C1->CR1 |= I2C_CR1_DNF;
	I2C1->CR1 |= I2C_CR1_ERRIE;
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
	// Set the master to operate in 7-bit addressing mode
	I2C1->CR2 &= ~I2C_CR2_ADD10;
	// Enable automatic end mode and NACK generation
	I2C1->CR2 |= I2C_CR2_AUTOEND;
	I2C1->CR2 |= I2C_CR2_NACK;
	// Set the values in the timing register
	//	** PRESC
	//	** SCLDEL
	//	** SDADEL
	//	** SCLH
	//	** SCLL
	//	Note: f_I2CCLK = 80 MHz
	
	// Set PRESC value
	//	Note: I2C clock frequency has min value 10 kHz and max value 100 kHz
	//	Choose f_PRESC = 8 MHz
	//	** PRESC is a 4-bit number
	//	** f_PRESC = f_12CCLK / (PRESC + 1)
	//	** PRESC = 9
	I2C1->TIMINGR &= ~I2C_TIMINGR_PRESC;
	I2C1->TIMINGR |= 9UL<<I2C_TIMINGR_PRESC_POS;
	
	// Set SCLDEL value
	//	** SCLDEL is a 4-bit number
	//	** t_SCLDEL > Min. Data Setup Time = 250 ns
	//	** t_SCLDEL = (SCLDEL + 1)t_PRESC
	//	** (SCLDEL + 1)t_PRESC > 250 ns
	//	** SCLDEL > 1
	//	Choose SCLDEL = 8
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLDEL;
	I2C1->TIMINGR |= 8UL<<I2C_TIMINGR_SCLDEL_POS;
	
	// Set SDADEL value
	//	** SDADEL is a 4-bit number
	//	** t_SDADEL > Min. Data Hold Time = 0 ns
	//	** t_SDADEL = (SDADEL + 1)t_PRESC
	//	** (SDADEL + 1)t_PRESC > 0 ns
	//	** SDADEL > -1
	//	Choose SDADEL = 10
	I2C1->TIMINGR &= ~I2C_TIMINGR_SDADEL;
	I2C1->TIMINGR |= 10UL<<I2C_TIMINGR_SDADEL_POS;
	
	// Set SCLH value
	//	** SCLH is an 8-bit number
	//	** t_SCLH > Min. High Clock Period = 4.0 us
	//	** t_SCLH = (SCLH + 1)t_PRESC
	//	** (SCLH + 1)t_PRESC > 4.0 us
	//	** SCLH > 31
	//	Choose SCLH = 32
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLH;
	I2C1->TIMINGR |= 32UL<<I2C_TIMINGR_SCLH_POS;
	
	// Set SCLL value
	//	** SCLL is an 8-bit number
	//	** t_SCLL > Min. Low Clock Period = 4.7 us
	//	** t_SCLL = (SCLL + 1)t_PRESC
	//	** (SCLL + 1)t_PRESC > 4.7 us
	//	** SCLL > 36.6
	//	Choose SCLL = 37
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLL;
	I2C1->TIMINGR |= 37UL<<I2C_TIMINGR_SCLL_POS;
	// --------------------------------------------------------
	
	// Set your own address in the own address register
	// --------------------------------------------------------
	//	** For MB85RC256V, default address = 1010000
	// Disable own address
	I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
	I2C1->OAR2 &= ~I2C_OAR2_OA2EN;
	// Set own address to 7-bit mode
	I2C1->OAR1 &= ~I2C_OAR1_OA1MODE;
	// Write the own address that you want to use
	I2C1->OAR1 &= ~I2C_OAR1_OA1;
	I2C1->OAR1 |= OwnAddr;// << 1;				// 7-bit address stored in bits 7:1
	// Enable own address
	I2C1->OAR1 |= I2C_OAR1_OA1EN;
	// --------------------------------------------------------
	
	// Enable I2C in the control register
	// --------------------------------------------------------
	I2C1->CR1 |= I2C_CR1_PE;
	// --------------------------------------------------------
}

//===============================================================================
//                           I2C Start
// Master generates START condition:
//    -- Slave address: 7 bits
//    -- Automatically generate a STOP condition after all bytes have been transmitted 
// Direction = 0: Master requests a write transfer
// Direction = 1: Master requests a read transfer
//=============================================================================== 
int8_t I2C_Start(I2C_TypeDef * I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction) {	
	
	// Direction = 0: Master requests a write transfer
	// Direction = 1: Master requests a read transfer
	
	uint32_t tmpreg = 0;
	
	// This bit is set by software, and cleared by hardware after the Start followed by the address
	// sequence is sent, by an arbitration loss, by a timeout error detection, or when PE = 0.
	tmpreg = I2Cx->CR2;
	
	tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
	
	if (Direction == READ_FROM_SLAVE)
		tmpreg |= I2C_CR2_RD_WRN;  // Read from Slave
	else
		tmpreg &= ~I2C_CR2_RD_WRN; // Write to Slave
		
	tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES));
	
	tmpreg |= I2C_CR2_START;
	
	I2Cx->CR2 = tmpreg; 
	
   	return 0;  // Success
}

//===============================================================================
//                           I2C Stop
//=============================================================================== 
void I2C_Stop(I2C_TypeDef * I2Cx){
	// Master: Generate STOP bit after the current byte has been transferred 
	I2Cx->CR2 |= I2C_CR2_STOP;								
	// Wait until STOPF flag is reset
	while( (I2Cx->ISR & I2C_ISR_STOPF) == 0 ); 
}

//===============================================================================
//                           Wait for the bus is ready
//=============================================================================== 
void I2C_WaitLineIdle(I2C_TypeDef * I2Cx){
	// Wait until I2C bus is ready
	while( (I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY );	// If busy, wait
}

//===============================================================================
//                           I2C Send Data
//=============================================================================== 
int8_t I2C_SendData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i;
	
	if (Size <= 0 || pData == NULL) return -1;
	
	I2C_WaitLineIdle(I2Cx);
	
	if (I2C_Start(I2Cx, DeviceAddress, Size, WRITE_TO_SLAVE) < 0 ) return -1;

	// Send Data
	// Write the first data in DR register
	// while((I2Cx->ISR & I2C_ISR_TXE) == 0);
	// I2Cx->TXDR = pData[0] & I2C_TXDR_TXDATA;  

	for (i = 0; i < Size; i++) {
		// TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
		// data to be sent is written in the I2C_TXDR register.
		// while( (I2Cx->ISR & I2C_ISR_TXE) == 0 ); 

		// TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
		// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
		// sent is written in the I2C_TXDR register.
		// The TXIS flag is not set when a NACK is received.
		while((I2Cx->ISR & I2C_ISR_TXIS) == 0 );
		I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
	}
	
	// Wait until TC flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0);
	
	if( (I2Cx->ISR & I2C_ISR_NACKF) != 0 ) return -1;

	I2C_Stop(I2Cx);
	return 0;
}


//===============================================================================
//                           I2C Receive Data
//=============================================================================== 
int8_t I2C_ReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i;
	
	if(Size <= 0 || pData == NULL) return -1;

	I2C_WaitLineIdle(I2Cx);

	I2C_Start(I2Cx, DeviceAddress, Size, READ_FROM_SLAVE); // 0 = sending data to the slave, 1 = receiving data from the slave
						  	
	for (i = 0; i < Size; i++) {
		// Wait until RXNE flag is set 	
		while( (I2Cx->ISR & I2C_ISR_RXNE) == 0 );
		pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	
	// Wait until TCR flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0);
	
	I2C_Stop(I2Cx);
	
	return 0;
}

int8_t I2C_SendReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pDataRegHigh, uint8_t *pDataRegLow, uint8_t *pDataReceive, uint8_t Size){
		int i;

	if (Size <= 0 || pDataRegHigh == NULL || pDataRegLow == NULL || pDataReceive == NULL) return -1;
	
	I2C_WaitLineIdle(I2Cx);
	
	Size = 2;
	if (I2C_Start(I2Cx, DeviceAddress, Size, WRITE_TO_SLAVE) < 0 ) return -1;

	// Send Data
	// Write the first data in DR register
	// while((I2Cx->ISR & I2C_ISR_TXE) == 0);
	// I2Cx->TXDR = pData[0] & I2C_TXDR_TXDATA;  
	Size = 1;
	for (i = 0; i < Size; i++) {
		// TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
		// data to be sent is written in the I2C_TXDR register.
		// while( (I2Cx->ISR & I2C_ISR_TXE) == 0 ); 

		// TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
		// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
		// sent is written in the I2C_TXDR register.
		// The TXIS flag is not set when a NACK is received.
		while((I2Cx->ISR & I2C_ISR_TXIS) == 0 );
		I2Cx->TXDR = pDataRegHigh[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
	}
	
	for (i = 0; i < Size; i++) {
		// TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
		// data to be sent is written in the I2C_TXDR register.
		// while( (I2Cx->ISR & I2C_ISR_TXE) == 0 ); 

		// TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
		// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
		// sent is written in the I2C_TXDR register.
		// The TXIS flag is not set when a NACK is received.
		while((I2Cx->ISR & I2C_ISR_TXIS) == 0 );
		I2Cx->TXDR = pDataRegLow[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
	}
	
	// Wait until TC flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0);
	
	if( (I2Cx->ISR & I2C_ISR_NACKF) != 0 ) return -1;
	

	I2C_Start(I2Cx, DeviceAddress, Size, READ_FROM_SLAVE); // 0 = sending data to the slave, 1 = receiving data from the slave
						  	
	for (i = 0; i < Size; i++) {
		// Wait until RXNE flag is set 	
		while( (I2Cx->ISR & I2C_ISR_RXNE) == 0 );
		pDataReceive[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	
	
	// Wait until TCR flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0);
	
	I2C_Stop(I2Cx);
	return 0;
}