#include "UART.h"

void UART1_Init(void) {
	// Set up the clock for USART1
	// --------------------------------------------------------
	// Enable the USART1 clock in the peripheral clock register
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Select the system clock as the USART1 clock source in the peripherals independent clock configuration register
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;				// Clear bits 0 and 1
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;			// Set bit 0
	// --------------------------------------------------------
}

void UART2_Init(void) {
	// Set up the clock for USART2
	// --------------------------------------------------------
	// Enable the USART2 clock in the peripheral clock register
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	// Select the system clock as the USART2 clock source in the peripherals independent clock configuration register
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;				// Clear bits 2 and 3
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;			// Set bit 2
	// --------------------------------------------------------
}

void UART1_GPIO_Init(void) {
		// Configure PB6 and PB7 to operate as UART trasmitters and receivers
	// --------------------------------------------------------
	// Enable GPIO Clock for Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// Configure PB6 to be used as alternative function USART1_TX
	//	** Set PB6 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE6;				// Clear bits 12 and 13 for Pin 6
	GPIOB->MODER |= GPIO_MODER_MODE6_1;				// Set bit 13 for Pin 6
	//	** Select USART1_TX as alternative function
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6;				// Set bits for AFRL6
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;			// Set alternative function number to AF7
	// Configure PB7 to be used as alternative function USART1_RX
	//	** Set PB7 to alternative function mode
	GPIOB->MODER &= ~GPIO_MODER_MODE7;				// Clear bits 14 and 15 for Pin 7
	GPIOB->MODER |= GPIO_MODER_MODE7_1;				// Set bit 15 for Pin 7
	//	** Select USART1_RX as alternative function
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7;				// Set bits for AFRL7
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;			// Set alternative function number to AF7
	// Both GPIO pins should operate at very high speeds
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;		// PB6
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;		// PB7
	// Both GPIO pins should have a push-pull output type
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;				// PB6
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;				// PB7
	// Configure both GPIO pins to use pull-up resistors for I/O
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;				// PB6
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;				// PB6
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7;				// PB7
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;				// PB7
	// --------------------------------------------------------
}

void UART2_GPIO_Init(void) {
	// Configure PA2 and PA3 to operate as UART trasmitters and receivers
	// --------------------------------------------------------
	// Enable GPIO Clock for Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Configure PA2 to be used as alternative function USART2_TX
	//	** Set PA2 to alternative function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE2;				// Clear bits 4 and 5 for Pin 2
	GPIOA->MODER |= GPIO_MODER_MODE2_1;				// Set bit 5 for Pin 2
	//	** Select USART2_TX as alternative function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2;				// Set bits for AFRL2
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;			// Set alternative function number to AF7
	// Configure PA3 to be used as alternative function USART2_RX
	//	** Set PA3 to alternative function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE3;				// Clear bits 6 and 7 for Pin 3
	GPIOA->MODER |= GPIO_MODER_MODE3_1;				// Set bit 7 for Pin 3
	//	** Select USART2_RX as alternative function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3;				// Set bits for AFRL3
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;			// Set alternative function number to AF7
	// Both GPIO pins should operate at very high speeds
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;		// PA2
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;		// PA3
	// Both GPIO pins should have a push-pull output type
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;				// PA2
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;				// PA3
	// Configure both GPIO pins to use pull-up resistors for I/O
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;				// PA2
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;				// PA2
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3;				// PA3
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;				// PA3
	// --------------------------------------------------------
}

void USART_Init(USART_TypeDef* USARTx) {
	// Configure the settings of USART
	// --------------------------------------------------------
	// Disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	// In the control registers, set word length to 8 bits, oversampling mode to oversample by 16, and number of stop bits to 1
	USARTx->CR1 &= ~USART_CR1_M;							// Set word length
	USARTx->CR1 &= ~USART_CR1_OVER8;					// Set oversampling mode
	USARTx->CR2 &= ~USART_CR2_STOP;						// Set number of stop bits
	// Set the baud rate to 9600
	//	** Note: Baud Rate = f_CLK / USARTDIV
	//  ** f_CLK = 80 MHZ
	//	** Write value of USARTDIV to USARTx_BRR
	USARTx->BRR = 8333;
	// In the control registers, enable both the transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;
	// Enable USART in the control registers
	USARTx->CR1 |= USART_CR1_UE;
	// --------------------------------------------------------
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
