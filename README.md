# Modified Space Invaders
Link to Demo Video
------------------
https://drive.google.com/file/d/1ntdhyYIeoqYiXOmBLCjAyKm-eCet7k1G/view?usp=sharing

Explanation of Project Functionality
====================================

The Modified Space Invaders project has four basic interactions: communicating with the computer terminal through UART, communicating to the Volatile FRAM using I2C, commuincating with the OLED Display using SPI, and reading in user button presses using external interupts. All computation for the game is done in the microcontroller.

The game itself is meant to be displayed on the OLED Display. Unfortunately, the code for activating the OLED SPI through SPI is non-oeprable. While the SPI operation itself works, I could not get the OLED Display to turn on. Thus, the responsibility of displaying the game is given to the computer instead using the Termite RS232 terminal. Due to using UART to transfer data, the game has low framerate and stutters a bit. The game is also displayed using ASCII character. However, this setup still works.

The game opens up on a basic menu screen. Pressing the "h" key will take the user to the leaderboards screen. Pressing any key will take the user back to the menu screen. From the menu screen, the game can be activated by pressing key. Once the game starts, the screen will display several rows of aliens shifting left and right. The player will be located at the bottom. The two blue buttons on the breadboard connected to the microcontroller control left and right movement. The red button allows the player to shoot projectiles at the aliens. The aliens move once every second. The projectiles move three times as fast. The timing of these movements is determined by SysTick interupts. The movement of the player is determined by button interrupts, and is thus independent of the speed of the rest of the game.

In normal Space Invaders, the game ends once the aliens reach the bottom of the screen or if the player is hit by an enemy projectile. However in this case, the aliens do not fire back and do not move down the screen. Thus, a 20 second timer is put in place that will end the game once it reaches zero. The player thus attempts to get as many points as possible in the time frame. Points are won when a projectile hits an enemy. For each succesful hit, the player gets 20 points and the alien hit dissapears.

After the game is over, the player's score is displayed and the player is asked to input their intials using the keyboard. Once inputted, the player is shown the leaderboards. All data about highscores is stored in the Volatile FRAM. Each entry consists of 4 bytes: 1 byte for each  of 3 initials, and 1 byte for the score itself. The Space Invaders program only stores 5 entries in the FRAM at a time. Once a game is won, the game attempts to insert the players score into the FRAM. If all entries are full, the game replaces the smallest score with the players score (if the players score is high enough). Then, the player can hit any key to return to the start menu. From there, the player can choose to begin the game again and try for a new high score.

Weekly Progress Updates
=======================

Project Update - 3/11/22
------------------------

I have nearly fully implemented the SPI protocol for the OLED display. Perfecting the code for the OLED display has proven to be difficult, as it is hard to find where errors in my code are originating from. However, once the display is up and running, it is very simple to write to it, as one only needs to input the x and y coordinates of each pixel on the display to draw to that pixel. Since all game elements are stored in a matrix, simply iterating over the matrix and using the DrawPixel() command each time is enough to completely update the entire OLED display each cycle.

The game code is nearly complete as well, and I am waiting for the OLED display to fully work so that I can test its playability. Usually a game only ends when the player gets hit by an enemy projectile. However, to test the "game over" functionality, I have added a simple timer instead. Once the timer reaches zero, the game will end and the score reached will be displayed.

I have been keeping each of the code modules separate up to this point so that I can verify that each of them is functioning correctly before cobbling the code all together. One of my last steps is to implement the I2C functionality to send and receive data from the volatile FRAM. I also would like to see if I can increase the functionality of the UART interaction with the computer terminal by adding the ability for the player to start the game from the terminal (instead of the game immediately starting upon activating the STM32 microcontroller) among other game options.

Project Update - 3/4/22
------------------------

I haven't made as much progress as I would have liked this past week, but I have gotten a majority of the code layout planned out. All of the code is organized based on their purpose.

The SPI protocol is partially implemented, but getting it to work with the OLED display correctly was a little complicated, so I decided to focus on working on the other aspects of the project.

I added the basis for the game logic to the code. Player movement is done through external button interrupts. Player movement is faster than enemy movement and enemy projectile movement. Enemy and enemy projectile movement is controlled via a delay function that uses the SysTick_Handler function. This delay function controls the flow of the game. Enemy informaiton is stored in a custom struct array. Each element in the array stores information about the enemy's position and whether or not it has been eliminated by the player. Information about the player is also stored in a custom struct that stores the player's position. Eventually I will add game logic that allows the enemy to drop bombs on the player.

The UART protocol has already been implemented, and the button interrupts are nearly implemented as well. Finishing the SPI protocol interaction with the OLED display is the next priority, followed by finishing the game logic and interacting with the volatile FRAM using I2C.

Project Update - 2/25/22
------------------------

There are five main functionalities that I must implement in my project: interfacing with the voltaile FRAM unit using I2C; interfacing with the OLED display using SPI; interfacing with a connected computer using UART; reading input from user button presses; and the game logic itself. Integrating all the parts of the project together will be another task.

The UART functionality is one of the easiest to implement, since it only requires minor modifications to the code in Part A of Lab 4. Interfacing with the external buttons will also be easy to implement, as it only requires some initialization of GPIO ports and a quick setup of some interrupt protocols. These two elements are so simple that I have cosen to focus on the other elements of the project first.

Implementing the game logic is not inherently difficult, but will take time to make sure that the game works correctly. Howver, my biggest worries are with interacting with the volatile FRAM and OLED display elements. I believed that implementing the SPI protocol for interacting with the OLED display would be most difficult, thus I chose to focus on implementing a functional SPI protocol first.

This week I have been researching documentation about my OLED display module and how to interface with it using SPI. I have copied over some of the files from previous labs to use as starter code for implementing the rest of the project. I have wired the OLED display to the Nucleo-64 microcontroller. I am learning how the microcontroller interacts with the product, such as what SPI specifications are necessary to allow the display and microcontrolelr to communicate, and what data I need to write in order to get the right LEDS to light up.

Proposal
========

Overview
--------
A basic version of the popular arcade game “Space Invaders” will be implemented using the STM32L476RG microcontroller. In this game, swarms of enemy “invaders” move in from the top of the screen and slowly move towards the bottom. A player sprite exists at the bottom of the screen. A user can control a sprite in the game using a two buttons, one for leftward movement and one for rightward movement. A third button will allow the sprite to “fire” projectiles at enemies. If enemies are shot, they dissapear from the screen. The game ends once the 20 second timer runs out. After the game ends, the player’s score will be displayed, and the user will be asked to input their initials using a keyboard. The keyboard is also used to start the game from the start menu.

Peripherals
-----------
- 128x64 OLED Graphic Display
- Push Buttons
- Non-Volatile FRAM
- Computer Terminal + Keyboard (Optional)

Serial Interface Protocols
--------------------------
- UART
- I2C
- SPI

Software Structure
------------------
The logic used to run the game “Space Invaders” will be coded in C and run inside the STM32 microcontroller. A 2D matrix will be used to represent the position of the enemies in the game. A main loop will periodically move the enemies across and down the screen. User input will be detected using polling. Player movement speed and shooting speed should be independent of enemy movement speed. The buttons will be connected through two of the microntroller’s I/O ports. Interrupts will be used to allow the buttons to control player movement. The game will communicate with the OLED display using SPI protocols, and will store data in the FRAM using I2C protocols. When a computer terminal is connected, the game will interact with the Termite program using UART protocols.
