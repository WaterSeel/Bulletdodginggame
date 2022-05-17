//Was written for ECE 198 final project, all code below was fully written by Anthony Lazar and Graeme Seelemann
//Any code given was taken out
#ifdef MATRIX_GAME
    
    max_init(); //Initializes the MAX7219 Dot Matrix display.

    uint8_t display[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//Initializes all rows to have no bullets
    
    //Game initialization
    uint8_t player = 0x08;
    int counter = 0; //counter ensures the game's events (bullet spawning, movement) are delayed to make it playable by humans
    int color = 1;
    int speed = 75; //As speed value decreases, so does the delay between loops, causing the game's tick rate to increase
    HAL_GPIO_WritePin(GPIOA, blue_pin, color & 0x01);  // blue  (hex 1 == 0001 binary)
    HAL_GPIO_WritePin(GPIOA, green_pin, color & 0x02);  // green (hex 2 == 0010 binary)
    HAL_GPIO_WritePin(GPIOA, red_pin, color & 0x04);  // red   (hex 4 == 0100 binary)'

    while (1){
        //char mystr[10];  // sprintf(mystr, "%d", player); // SerialPuts(mystr); // SerialPuts("!\n"); (Code for logs, commented out afterwards)    
        
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) && player < 0x80){
            player = player << 1;
            SerialPuts("    TO the left    !\n");//logs
            HAL_Delay(30);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) && player > 0x01){
            player = player >> 1;
            SerialPuts("    TO the right   !\n");//logs
            HAL_Delay(30);
        }
        srand(HAL_GetTick());
        int spawn = rand() % 254; //random number between 0 and 0xFE. In binary form, 0 = gap (LED off), 1 = bullet (LED on)
        //SerialPuts(spawn); //old logs
        if ((counter % 2) == 0){
            spawn = 0x00;
        }
        SerialPuts("!\n");

        if ((counter % 5) == 0){
            display[7] = display[6];
            display[6] = display[5];
            display[5] = display[4];
            display[4] = display[3];
            display[3] = display[2];
            display[2] = display[1];
            display[1] = display[0];
            display[0] = spawn;
            
        }
        for (int i = 0; i<9; i++){
            if (i != 8){
                write_max(i, display[i-1]);
            }
            else{
                write_max(i, display[i-1] | player);
            }
        }
        if ((player & display[7]) > 0){
             SerialPuts("You Lose!");
             break;
        }
        

        if (counter%100 == 0){
            //Final three bits indicate which of the three LEDs should be on (eight possible combinations)
            //Intent is to change a multicolor LED's color as speed increases
            HAL_GPIO_WritePin(GPIOA, blue_pin, color & 0x01);  // blue  (hex 1 == 0001 binary)
            HAL_GPIO_WritePin(GPIOA, green_pin, color & 0x02);  // green (hex 2 == 0010 binary)
            HAL_GPIO_WritePin(GPIOA, red_pin, color & 0x04);  // red   (hex 4 == 0100 binary)

           color++;

           if(color >= 8){
               color=7;
           }

           if(speed > 40){
                speed-=5;
           }
        }

        ++counter;
        HAL_Delay(speed); 

    }    
#endif