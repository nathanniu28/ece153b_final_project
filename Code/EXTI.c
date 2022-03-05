/*
 * ECE 153B - Winter 2022
 *
 * Name(s): Nathan Niu
 * Section: Tuesday, 7 pm
 * Lab: 2A
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize User Button
	// --------------------------------------------------------
	// Set mode of pin PC13 to Input
  GPIOC->MODER &= ~(3UL<<26);		// Clear bits 26 and 27 for Pin 13
	// Set PC13 to No Pull-Up, No Pull-Down
	GPIOC->PUPDR &= ~(3UL<<26); 	// Clear bits 26 and 27 for Pin 13
	// --------------------------------------------------------
	
	// Configure SYSCFG EXTI
	// --------------------------------------------------------
	// Enable the system confugration controller
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Configure the SYSCFG external interupt configuration register
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;			// Clear bits for pin 13
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;		// Set bit at pin 13 port C
	// --------------------------------------------------------
	
	// Configure EXTI Trigger
	// --------------------------------------------------------
	// Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;		// Clear bit for rising edge trigger at line 13
	// Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;			// Set bit for falling edge trigger at line 13
	// --------------------------------------------------------
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;				// Set bit that corresponds to input line 13 in the register
	
	// Configure and Enable in NVIC
	// --------------------------------------------------------
	NVIC_EnableIRQ(EXTI15_10_IRQn);					// Enable and mask bits for lines 10 to 15
	NVIC_SetPriority(EXTI15_10_IRQn, 0);		// Set interrupt priority for lines 10 to 15
	// --------------------------------------------------------
}

// Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;				// Set corresponding bit of EXTI pending register
	// Define behavior that occurs when interrupt occurs
	Green_LED_Toggle();
}