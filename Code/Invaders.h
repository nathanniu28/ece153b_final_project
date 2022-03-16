/* Space Invaders */

// Basic structs used to store game information

#include <stdbool.h>

#define WIDTH 51		// odd number for convenience
#define HEIGHT 12

#define WALL '#'
#define ALIEN 'O'
#define PROJECTILE '*'
#define PLAYER 'A'
#define NONE ' '

extern char display[HEIGHT][WIDTH];
extern bool aliens[HEIGHT][WIDTH];
extern bool projectiles[HEIGHT][WIDTH];
//extern bool bombs[HEIGHT][WIDTH];
extern int position;
extern int timer_count;
extern int high_score;
extern int num_aliens;
extern int drift;
extern bool drift_direction; // 0 = left, 1 = right
extern bool game_status; // 0 = game over, 1 = game running
extern char intials[3];
extern bool show_leaderboard;

void Print_Score(int value);
int Get_Score(int index, bool print);
int Get_High_Score(void);
void Write_Score(int index1, int index2, int value);
void Write_High_Score();
void Reset_Scores(void);

void Game_Print(void);
void Game_Update(void);
void Game_Update_Projectiles(void);
void Game_Init(void);
void Game_Start_Menu(void);
void Game_Over(void);
void Game_Input_Initials(void);
void Game_Show_Initials(void);
void Display_Scores(void);