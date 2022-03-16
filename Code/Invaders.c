#include "Invaders.h"
#include "UART.h"
#include "I2C.h"

char display[HEIGHT][WIDTH];
bool aliens[HEIGHT][WIDTH];
bool projectiles[HEIGHT][WIDTH];
int position;
int timer_count;
int high_score;
int score;
int num_aliens;
int drift;
bool drift_direction;
bool game_status;
char intials[3];
bool show_leaderboard;

void Print_Score(int value){
	// pad score with zeros to make value 4 digits long
	if(score < 1000){
		printf("0");
		if(score < 100){
			printf("0");
			if(score < 10)
				printf("0");
		}
	}
	printf("%d", score);
}

int Get_Score(int index, bool print){
	// Gets stored player intials and their scores
	// Four 8-bit addresses are used: three for the intials, and one for the score

	uint8_t SlaveAddress = 0xA0;
	uint8_t Data_Reg_High = 0x04;
	uint8_t Data_Reg_Low = 0x00 + index*4;
	uint8_t Data_Receive;
	
	for(int j = 0; j < 3; j++){
		if(print){
			I2C_SendReceiveData(I2C1, SlaveAddress, &Data_Reg_High, &Data_Reg_Low, &Data_Receive, 1);
			printf("%c", Data_Receive);
		}
		Data_Reg_Low++;
	}
	I2C_SendReceiveData(I2C1, SlaveAddress, &Data_Reg_High, &Data_Reg_Low, &Data_Receive, 1);
	if(print){
		printf(" ");
		//Print_Score(Data_Receive);
		if(Data_Receive < 1000){
			printf("0");
			if(Data_Receive < 100){
				printf("0");
				if(Data_Receive < 10)
					printf("0");
			}
		}
		printf("%d", Data_Receive);
		printf("\n");
	}
	Data_Reg_Low++;
	
	return Data_Receive;
}

int Get_High_Score(void){
	int high_score = 0;
	int temp;
	for(int i = 0; i < 5; i++){
		temp = Get_Score(i, false);
		if(temp > high_score)
			high_score = temp;
	}
	return high_score;
}

void Write_Score(int index1, int index2, int value){
	// Replaces lowest score with new score
	uint8_t SlaveAddress = 0xA0;
	uint8_t Data_Reg_High = 0x04;
	uint8_t Data_Reg_Low = 0x00 + 4*index1 + index2;
	uint8_t Data_Send = value & 0xFF;
	uint8_t Write_Data[3];
	Write_Data[0] = Data_Reg_High;
	Write_Data[1] = Data_Reg_Low;
	Write_Data[2] = Data_Send;
	I2C_SendData(I2C1, SlaveAddress, Write_Data, 3);
}

void Write_High_Score(
	){
	int index = 0;
	int low_score = 9999;
	int temp;
	for(int i = 0; i < 5; i++){
		temp = Get_Score(i, false);
		if(temp < low_score){
			low_score = temp;
			index = i;
		}
	}
	Write_Score(index, 0, intials[0]);
	Write_Score(index, 1, intials[1]);
	Write_Score(index, 2, intials[2]);
	Write_Score(index, 3, score);
}

void Reset_Scores(void){
	// Resets all stores scores to AAA 0000
	
	Write_Score(0, 0, 45);
	Write_Score(0, 1, 45);
	Write_Score(0, 2, 45);
	Write_Score(0, 3, 0);
	Write_Score(1, 0, 45);
	Write_Score(1, 1, 45);
	Write_Score(1, 2, 45);
	Write_Score(1, 3, 0);
	Write_Score(2, 0, 45);
	Write_Score(2, 1, 45);
	Write_Score(2, 2, 45);
	Write_Score(2, 3, 0);
	Write_Score(3, 0, 45);
	Write_Score(3, 1, 45);
	Write_Score(3, 2, 45);
	Write_Score(3, 3, 0);
	/*
	uint8_t SlaveAddress = 0xA0;
	uint8_t Data_Reg_High = 0x04;
	uint8_t Data_Reg_Low = 0x00;
	uint8_t Data_Send = 65 & 0xFF;
	uint8_t Write_Data[3];
	Write_Data[0] = Data_Reg_High;
	Write_Data[1] = Data_Reg_Low;
	Write_Data[2] = Data_Send;
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 3; j++){
			//Write_Data[2] = 65 & 0xFF;
			I2C_SendData(I2C1, SlaveAddress, Write_Data, 3);
			Data_Reg_Low++;
				printf("%d", j);
		}
		Write_Data[2] = 0;
		I2C_SendData(I2C1, SlaveAddress, Write_Data, 3);
		Data_Reg_Low++;
	}
	*/
}

void Game_Print(void){
	// Print current game state
	for(int i =0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			printf("%c", display[i][j]);
		}
		if(i == 1){
			printf("   Time: ");
			printf("%d", timer_count);
		}
		if(i == HEIGHT / 2){
			printf("   Hi-Score: ");
			printf("%d", high_score);
			//Print_Score(high_score);
		}
		else if(i == HEIGHT / 2 + 1){
			printf("   Score: ");
			Print_Score(score);
		}
		printf("\n");
	}
}

void Game_Update(void){
	timer_count--;
	// Update game state
	bool temp_aliens[HEIGHT][WIDTH];
	bool temp_projectiles[HEIGHT][WIDTH];
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			temp_aliens[i][j] = false;
			temp_projectiles[i][j] = false;
		}
	}
	
	if(drift > 4 || drift < -4)
		drift_direction = !drift_direction;
	if(drift_direction)
		drift++;
	else
		drift--;
	
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			// Update alien positions
			if(aliens[i][j]){
				// Aliens drift left
				if(!drift_direction){
					temp_aliens[i][j-1] = true;
				}
				// Aliens drift right
				else if(drift_direction){
					temp_aliens[i][j+1] = true;
				}
			}
			// Update projectile positions
			if(projectiles[i][j]){
				temp_projectiles[i-1][j] = true;
			}			
		}		
	}
	
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			// Replace aliens with temp_aliens
			aliens[i][j] = temp_aliens[i][j];
			// Replace projectiles with temp_projectiles
			projectiles[i][j] = temp_projectiles[i][j];
			// Check for collision
			if(aliens[i][j] && projectiles[i][j]){
				aliens[i][j] = false;
				projectiles[i][j] = false;
				num_aliens--;
				score = score + 20;
			}
			
			// Update display
			if(i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
				display[i][j] = WALL;
			else if(aliens[i][j])
				display[i][j] = ALIEN;
			else if(projectiles[i][j])
				display[i][j] = PROJECTILE;
			else if(i == HEIGHT - 2 && j == position)
				display[i][j] = PLAYER;
			else
				display[i][j] = NONE;
		}
	}
	if(timer_count == 0)
		game_status = false;
}

void Game_Update_Projectiles(void){
	// Update game state
	bool temp_projectiles[HEIGHT][WIDTH];
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			temp_projectiles[i][j] = false;
		}
	}

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			// Update projectile positions
			if(projectiles[i][j]){
				temp_projectiles[i-1][j] = true;
			}			
		}		
	}
	
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			// Replace projectiles with temp_projectiles
			projectiles[i][j] = temp_projectiles[i][j];
			// Check for collision
			if(aliens[i][j] && projectiles[i][j]){
				aliens[i][j] = false;
				projectiles[i][j] = false;
				num_aliens--;
				score = score + 20;
			}
			
			// Update display
			if(i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
				display[i][j] = WALL;
			else if(aliens[i][j])
				display[i][j] = ALIEN;
			else if(projectiles[i][j])
				display[i][j] = PROJECTILE;
			else if(i == HEIGHT - 2 && j == position)
				display[i][j] = PLAYER;
			else
				display[i][j] = NONE;
		}
	}
}

void Game_Init(void){
	// Initialize game state
	position = WIDTH/2 + 1;
	timer_count = 20;
	high_score = Get_High_Score();
	score = 0;
	num_aliens = 0;
	drift = 0;
	drift_direction = false;
	game_status = true;

	for(int i =0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			// Initialze alien positions
			if(i == 2 || i == 4){
				if(j % 2 == 0 && j > 5 && j < WIDTH - 6){
					aliens[i][j] = true;
					num_aliens++;
				}
			}
			else
				aliens[i][j] = false;
			
			// Initialize projectile positions
			projectiles[i][j] = false;
			
			// Initialize display
			if(i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
				display[i][j] = WALL;
			else if(aliens[i][j])
				display[i][j] = ALIEN;
			else if(i == HEIGHT - 2 && j == position)
				display[i][j] = PLAYER;
			else
				display[i][j] = NONE;
			
		}
	}
}

void Game_Start_Menu(void){
	char rxByte;
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 2){
			for(int j = 0; j < WIDTH/2 - 7; j++)
				printf(" ");
			printf("SPACE INVADERS\n");
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 3; j++)
				printf(" ");
			printf("(sorta)\n");
		}
		else if(i == HEIGHT/2 + 2){
			for(int j = 0; j < WIDTH/2 - 17; j++)
				printf(" ");
			printf("Type \"h\" to view the leaderboards\n");
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 15; j++)
				printf(" ");
			printf("Type any other letter to start\n");
		}
		else
			printf("\n");
	}
	scanf("%c",&rxByte);
	show_leaderboard = false;
	if(rxByte == 'h')
		show_leaderboard = true;
}

void Game_Over(void){
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			printf("GAME OVER\n");
		}
		else if(i == HEIGHT/2 + 1){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Score: ");
			Print_Score(score);
			printf("\n");
		}
		else
			printf("\n");
	}
}

void Game_Input_Initials(void){
	// Input first inital
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 3){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Score: ");
			Print_Score(score);
			printf("\n");
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 19; j++)
				printf(" ");
			printf("Input your intials to save your score!\n");
		}
		else if(i == HEIGHT/2 + 1){
			for(int j = 0; j < WIDTH/2 - 11; j++)
				printf(" ");
			printf("Input your first intial:\n");
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 2; j++)
				printf(" ");
			printf("_ _ _\n");
		}
		else
			printf("\n");
	}
	scanf("%c",&intials[0]);
	
	// Input second inital
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 3){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Score: ");
			Print_Score(score);
			printf("\n");
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 19; j++)
				printf(" ");
			printf("Input your intials to save your score!\n");
		}
		else if(i == HEIGHT/2 + 1){
			for(int j = 0; j < WIDTH/2 - 11; j++)
				printf(" ");
			printf("Input your first intial:\n");
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 2; j++)
				printf(" ");
			printf("%c", intials[0]);
			printf(" _ _\n");
		}
		else
			printf("\n");
	}
	scanf("%c",&intials[1]);
	
	// Input third inital
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 3){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Score: ");
			Print_Score(score);
			printf("\n");
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 19; j++)
				printf(" ");
			printf("Input your intials to save your score!\n");
		}
		else if(i == HEIGHT/2 + 1){
			for(int j = 0; j < WIDTH/2 - 11; j++)
				printf(" ");
			printf("Input your first intial:\n");
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 2; j++)
				printf(" ");
			printf("%c", intials[0]);
			printf(" ");
			printf("%c", intials[1]);
			printf(" _\n");
		}
		else
			printf("\n");
	}
	scanf("%c",&intials[2]);
}

void Game_Show_Initials(void){
	// Show score and intials
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 3){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Score: ");
			Print_Score(score);
			printf("\n");
		}
		else if(i == HEIGHT/2){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("Nice score!\n");
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 2; j++)
				printf(" ");
			printf("%c", intials[0]);
			printf(" ");
			printf("%c", intials[1]);
			printf(" ");
			printf("%c", intials[2]);
			printf("\n");
		}
		else
			printf("\n");
	}
	
	// Write high score to FRAM
	/*
	Write_Score(0, 0, intials[0]);
	Write_Score(0, 1, intials[1]);
	Write_Score(0, 2, intials[2]);
	Write_Score(0, 3, score);
	*/
	Write_High_Score();
}

void Display_Scores(void){
	// Displays scores, allows player to restart game
	for(int i = 0; i < HEIGHT; i++){
		if(i == 0 || i == HEIGHT - 1){
			for(int j = 0; j < WIDTH; j++)
				printf("#");
			printf("\n");
		}
		else if(i == HEIGHT/2 - 4){
			for(int j = 0; j < WIDTH/2 - 5; j++)
				printf(" ");
			printf("HIGH SCORES\n");
		}
		else if(i == HEIGHT/2 - 2){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			Get_Score(0, true);
		}
		else if(i == HEIGHT/2 - 1){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			Get_Score(1, true);
		}
		else if(i == HEIGHT/2){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			Get_Score(2, true);
		}
		else if(i == HEIGHT/2 + 1){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			Get_Score(3, true);
		}
		else if(i == HEIGHT/2){
			for(int j = 0; j < WIDTH/2 - 4; j++)
				printf(" ");
			Get_Score(4, true);
		}
		else if(i == HEIGHT/2 + 3){
			for(int j = 0; j < WIDTH/2 - 22; j++)
				printf(" ");
			printf("Type any letter to return to the title screen\n");
		}
		else
			printf("\n");
	}
	scanf("%c",&intials[1]);
}