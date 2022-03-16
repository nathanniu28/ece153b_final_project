/*
 * ECE 153B - Winter 2022
 *
 * Name(s): Nathan Niu
 * Section: Tuesday, 7 pm
 * Lab: Final Project
 */

#include "stm32l476xx.h"

#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include "EXTI.h"
#include "Invaders.h"
#include "SysTimer.h"
#include "SPI.h"
#include "ssd1306.h"

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // System Clock = 80 MHz
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();

	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(2);
	
	EXTI_Init();
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Enable GPIO Clock for Port C 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	EXTI_Button_Init();
	SysTick_Init();
	
	//Reset_Scores();
	
	// 20 points are received per alien hit
	while(1) {
		// GAME START

		Game_Start_Menu();
		if(show_leaderboard){
			Display_Scores();
			continue;
		}
		Game_Init();
		Game_Print();
		while(game_status){
			// projectiles move x3 times faster than aliens
			delay(200);
			Game_Update();
			Game_Print();
			delay(200);
			Game_Update_Projectiles();
			Game_Print();
			delay(200);
			Game_Update_Projectiles();
			Game_Print();
		}
		Game_Over();
		delay(2000);
		Game_Input_Initials();
		Game_Show_Initials();
		delay(2000);
		Display_Scores();
	}
}
