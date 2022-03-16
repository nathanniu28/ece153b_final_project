/*
 * ECE 153B - Winter 2022
 *
 * Name(s): Nathan Niu
 * Section: Tuesday, 7 pm
 * Lab: Final Project
 */

#include "EXTI.h"
#include "UART.h"
#include "Invaders.h"

uint32_t count = 10;

void EXTI_Init(void) {
	// Initialize User Button
	// --------------------------------------------------------
	// Enable GPIO Clock for Port C 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
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

void EXTI_Button_Init(void) {
	
	// Enable GPIO Clock for Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Enable GPIO Clock for Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	/*
	// Initialize User Button for PA5
	// --------------------------------------------------------
	// Set mode of pin PA5 to Input
  GPIOA->MODER &= ~GPIO_MODER_MODE5;
	// Set PA5 to No Pull-Up, No Pull-Down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	// --------------------------------------------------------
	*/
	
	// Initialize User Button for PA6
	// --------------------------------------------------------
	// Set mode of pin PA6 to Input
  GPIOA->MODER &= ~GPIO_MODER_MODE6;
	// Set PA6 to No Pull-Up, No Pull-Down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;
	// --------------------------------------------------------
	
	// Initialize User Button for PA7
	// --------------------------------------------------------
	// Set mode of pin PA7 to Input
  GPIOA->MODER &= ~GPIO_MODER_MODE7;
	// Set PA7 to No Pull-Up, No Pull-Down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;
	// --------------------------------------------------------
	
	// Initialize User Button for PB9
	// --------------------------------------------------------
	// Set mode of pin PB9 to Input
  GPIOB->MODER &= ~GPIO_MODER_MODE9;
	// Set PB9 to No Pull-Up, No Pull-Down
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD9;
	// --------------------------------------------------------
	
	// Configure SYSCFG EXTI
	// --------------------------------------------------------
	// Enable the system confugration controller
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Configure the SYSCFG external interupt configuration register
	/*
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;			// Clear bits for pin 5
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;		// Set bit at pin 5 port A
	*/
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6;			// Clear bits for pin 6
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;		// Set bit at pin 6 port A
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI7;			// Clear bits for pin 7
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;		// Set bit at pin 7 port A
	SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI9;			// Clear bits for pin 9
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB;		// Set bit at pin 5 port B
	// --------------------------------------------------------
	
	// Configure EXTI Trigger
	// --------------------------------------------------------
	/*
	// Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT5;		// Clear bit for rising edge trigger at line 5
	// Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;			// Set bit for falling edge trigger at line 5
	*/
	// Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT6;		// Clear bit for rising edge trigger at line 6
	// Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT6;			// Set bit for falling edge trigger at line 6
	// Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT7;		// Clear bit for rising edge trigger at line 7
	// Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT7;			// Set bit for falling edge trigger at line 7
	// Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT9;		// Clear bit for rising edge trigger at line 9
	// Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT9;			// Set bit for falling edge trigger at line 9
	// --------------------------------------------------------
	
	// Enable EXTI
	//EXTI->IMR1 |= EXTI_IMR1_IM5;				// Set bit that corresponds to input line 5 in the register
	EXTI->IMR1 |= EXTI_IMR1_IM6;				// Set bit that corresponds to input line 6 in the register
	EXTI->IMR1 |= EXTI_IMR1_IM7;				// Set bit that corresponds to input line 7 in the register
	EXTI->IMR1 |= EXTI_IMR1_IM9;				// Set bit that corresponds to input line 9 in the register
	
	// Configure and Enable in NVIC
	// --------------------------------------------------------
	NVIC_EnableIRQ(EXTI9_5_IRQn);					// Enable and mask bits for lines 5 to 9
	NVIC_SetPriority(EXTI9_5_IRQn, 0);		// Set interrupt priority for lines 5 to 9
	// --------------------------------------------------------
}

// Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;				// Set corresponding bit of EXTI pending register
	// Define behavior that occurs when interrupt occurs
	count++;
	printf("%d", count);
	printf("\n");
}

void EXTI9_5_IRQHandler(void) {
	// Check which interrupt occured
	/*
	if(EXTI->PR1 & EXTI_PR1_PIF5){
		EXTI->PR1 |= EXTI_PR1_PIF5;
		printf("Right\n");
	}
	*/
	if(EXTI->PR1 & EXTI_PR1_PIF9){
		EXTI->PR1 |= EXTI_PR1_PIF9;
		//printf("Right\n");
		position++;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF7){
		EXTI->PR1 |= EXTI_PR1_PIF7;
		//printf("Shoot\n");
		projectiles[HEIGHT-3][position] = true;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF6){
		EXTI->PR1 |= EXTI_PR1_PIF6;
		//printf("Left\n");
		position--;
	}

	//count = count+2;
	/*
	printf("%x", EXTI->PR1);
	count = EXTI->PR1;
	if(EXTI->PR1 & EXTI_PR1_PIF5){
		EXTI->PR1 |= EXTI_PR1_PIF5;
	}
	else if(EXTI->PR1 & EXTI_PR1_PIF6){
	EXTI->PR1 |= EXTI_PR1_PIF6;
	}
	else if(EXTI->PR1 & EXTI_PR1_PIF7){
		EXTI->PR1 |= EXTI_PR1_PIF7;
	}
	
	count++;
	*/
	/*
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;				// Set corresponding bit of EXTI pending register
	// Define behavior that occurs when interrupt occurs
	count++;
	*/
}
