#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clock for Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Initialize Green LED
	// --------------------------------------------------------
	// Set the mode of PA5 to Output
	GPIOA->MODER &= ~(3UL<<10);		// Clear bits 10 and 11 for Pin 5
	GPIOA->MODER |= 1UL<<10;			// Set bit 10, set Pin 5 as output
	// Set the output type of PA5 as Push-Pull
	GPIOA->OTYPER &= ~(1UL<<5);		// Clear bit 5
	// Set PA5 to No Pull-Up, No Pull-Down	
	GPIOA->PUPDR &= ~(3UL<<10); 	// Clear bits 10 and 11 for Pin 5
	// --------------------------------------------------------
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~(1UL<<5);		// Clear bit 5
}

void Green_LED_On(void) {
	GPIOA->ODR |= 1UL<<5;				// Set bit 5
}

void Green_LED_Toggle(void) {
	GPIOA->ODR ^= 1UL<<5;				// Flip bit 5
}
