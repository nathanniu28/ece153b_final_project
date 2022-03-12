# ece153b_final_project

Project Update - 2/25/22
------------------------

There are five main functionalities that I must implement in my project: interfacing with the voltaile FRAM unit using I2C; interfacing with the OLED display using SPI; interfacing with a connected computer using UART; reading input from user button presses; and the game logic itself. Integrating all the parts of the project together will be another task.

The UART functionality is one of the easiest to implement, since it only requires minor modifications to the code in Part A of Lab 4. Interfacing with the external buttons will also be easy to implement, as it only requires some initialization of GPIO ports and a quick setup of some interrupt protocols. These two elements are so simple that I have cosen to focus on the other elements of the project first.

Implementing the game logic is not inherently difficult, but will take time to make sure that the game works correctly. Howver, my biggest worries are with interacting with the volatile FRAM and OLED display elements. I believed that implementing the SPI protocol for interacting with the OLED display would be most difficult, thus I chose to focus on implementing a functional SPI protocol first.

This week I have been researching documentation about my OLED display module and how to interface with it using SPI. I have copied over some of the files from previous labs to use as starter code for implementing the rest of the project. I have wired the OLED display to the Nucleo-64 microcontroller. I am learning how the microcontroller interacts with the product, such as what SPI specifications are necessary to allow the display and microcontrolelr to communicate, and what data I need to write in order to get the right LEDS to light up.

Project Update - 3/4/22
------------------------

I haven't made as much progress as I would have liked this past week, but I have gotten a majority of the code layout planned out. All of the code is organized based on their purpose.

The SPI protocol is partially implemented, but getting it to work with the OLED display correctly was a little complicated, so I decided to focus on working on the other aspects of the project.

I added the basis for the game logic to the code. Player movement is done through external button interrupts. Player movement is faster than enemy movement and enemy projectile movement. Enemy and enemy projectile movement is controlled via a delay function that uses the SysTick_Handler function. This delay function controls the flow of the game. Enemy informaiton is stored in a custom struct array. Each element in the array stores information about the enemy's position and whether or not it has been eliminated by the player. Information about the player is also stored in a custom struct that stores the player's position. Eventually I will add game logic that allows the enemy to drop bombs on the player.

The UART protocol has already been implemented, and the button interrupts are nearly implemented as well. Finishing the SPI protocol interaction with the OLED display is the next priority, followed by finishing the game logic and interacting with the volatile FRAM using I2C.

Project Update - 3/11/22
------------------------

I have nearly fully implemented the SPI protocol for the OLED display. Perfecting the code for the OLED display has proven to be difficult, as it is hard to find where errors in my code are originating from. However, once the display is up and running, it is very simple to write to it, as one only needs to input the x and y coordinates of each pixel on the display to draw to that pixel. Since all game elements are stored in a matrix, simply iterating over the matrix and using the DrawPixel() command each time is enough to completely update the entire OLED display each cycle.

The game code is nearly complete as well, and I am waiting for the OLED display to fully work so that I can test its playability. Usually a game only ends when the player gets hit by an enemy projectile. However, to test the "game over" functionality, I have added a simple timer instead. Once the timer reaches zero, the game will end and the score reached will be displayed.

I have been keeping each of the code modules separate up to this point so that I can verify that each of them is functioning correctly before cobbling the code all together. One of my last steps is to implement the I2C functionality to send and receive data from the volatile FRAM. I also would like to see if I can increase the functionality of the UART interaction with the computer terminal by adding the ability for the player to start the game from the terminal (instead of the game immediately starting upon activating the STM32 microcontroller) among other game options.
