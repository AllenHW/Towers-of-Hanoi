#include<stdio.h>                                                                     
#include<iostream.h>                       
#include<string>                                                                                  
#include<fstream.h>
#include<vector.h>
#include<dirent.h>
#include<iomanip> 
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<allegro.h> 

#include "TOWERS.h"

 
using namespace std;

//counters
long move_counter;
long recursive_counter;

//parameters controling the status of the game 
bool run; 
bool solving;
bool pause;
bool finished;

//timer function used to adjust the speed of the animation 
volatile long speed_counter = 0;
void increment_speed_counter() 
{
	speed_counter++; 
}
END_OF_FUNCTION(increment_speed_counter);

//recursion function that solves and moves the towers of hanoi
void move(int n, int t1, int t2, int t3, STACK peg[], int &square_x_pos, bool &grab);  
void display(STACK peg[], int &square_x_pos, bool &grab);   //controlls the displaying of bitmaps 
void display_pegs(STACK peg[], BITMAP *buffer);     //displays the pegs
void run_buttons(STACK peg[], int &square_x_pos, bool &grab, BITMAP *buffer);  //function that controlls button functions 
//checks for button clicking
bool click_buttons (BITMAP *buffer, BITMAP *button1, BITMAP *button2, int button_x, int button_y, int button_width, int button_height);   
void pause_game(STACK peg[], int &square_x_pos, bool &grab, BITMAP *buffer);  //pauses the program while leaving some functions still available
void reset_game(STACK peg[], int size);  //initializes the game
int tower_size(STACK peg[]);  //returns the size of the towers
void set_speed(int parameter);   //controlls the speed of animation


int main() {

    //initializing allegro and keyboard/mouse system 
    allegro_init();  
    install_keyboard();
    install_mouse();
    //setting up the timer function
    LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
    //setting graphics 
    set_color_depth(desktop_color_depth()); 
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); 
	
    STACK peg[3]; //declaring the an array of the stack class = pegs
  //  set_speed(150);  //setting the speed with which the program starts

    int square_x_pos = 300; //stores the x position of the speed button, which also affects the speed of animation
    bool grab;  //determines whether the speed buttton is grabbed by the mouse or not - essential for smooth animation
    
    reset_game(peg, 5);   //size of the towers start from 5
    
    //program runs within this loop
    run = true;
    while (run == true)
    {      
           //if not solving or solution finished, just display things
           if (solving == false || finished == true)
                   display(peg, square_x_pos, grab);
           
           //if still solving, run the recursion function
           if (solving == true && finished == false)
           {
                move(tower_size(peg), 0, 1, 2, peg, square_x_pos, grab);
                finished = true;  //once the recursion is done, it's "finished"
           }    
    }
    
}
END_OF_MAIN()

//square_x_pos and grab are not used in the recursion itself but are ESSENTIAL for functins called within the recursion
void move(int n, int source, int path, int target, STACK peg[], int &square_x_pos, bool &grab)  

{
     //if the user decides to abort the recursion, don't do anything and just return
     if (run == false || solving == false)  return;
     
     recursive_counter++;   //count the number of recursive calls
     
     //base case
     if (n==1) 
     {
        if (run == true && solving == true)  // only make moves if the program is supposed to keep running
        {
           peg[target].push(peg[source].pop());   //push what's the in source peg to the target peg
           move_counter++;               //count the number of moves
           display(peg, square_x_pos, grab);   //display after each move
        }  
        return;        //base case return
     }
     
     move(n-1, source, target, path, peg, square_x_pos, grab);   //moves every block but one to the intermediate peg
     //then move the last block to the target peg
     if (run == true && solving == true)     //only make the move if the program is supposed to be going on. without this statement there would be a glitch 
     {
        peg[target].push(peg[source].pop());     //push what's the in source peg to the target peg
        move_counter++;                          //count the number of moves
        display(peg, square_x_pos, grab);      //display after each move
     }
     //finally, move eveything else from the intermediate peg to the target peg
     move(n-1, path, source, target, peg, square_x_pos, grab);
     
}    
END_OF_FUNCTION(move)

void display(STACK peg[], int &square_x_pos, bool &grab)
{    
     //creating bitmaps
     static BITMAP *buffer = create_bitmap(640,480); 
     static BITMAP *mouse = load_bitmap("images/mouse.bmp", NULL);
     static BITMAP *speed_bar = load_bitmap("images/speed_bar.bmp", NULL);

     clear(buffer);
     
     //displaying differet parts on the buffer
     display_pegs(peg, buffer);     
     run_buttons(peg, square_x_pos, grab, buffer);   
     stretch_sprite(buffer, speed_bar, 200, 20, 434, 45);
     textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "# of:");
     textprintf_ex(buffer, font, 10, 30, makecol(255, 255, 255), -1, "recursive calls = %d", recursive_counter);
     textprintf_ex(buffer, font, 10, 45, makecol(255, 255, 255), -1, "moves           = %d", move_counter);
     stretch_sprite(buffer, mouse, mouse_x, mouse_y, 15, 19);   //display mouse

     if (square_x_pos != 558)  //unless the speed is set to maximum, 
     {
        speed_counter = 0;        //pause the program for a short duration - wait for the speed_counter to increase from 0 to 1
        while (speed_counter < 1 && run == true && solving == true)  //break the loop if the user wants to abort the recursion itself  
        {
              //the program should be printing everything even within the loop, so that the buttons work at all moment
              clear(buffer);               
     
              display_pegs(peg, buffer);
              run_buttons(peg, square_x_pos, grab, buffer);
              stretch_sprite(buffer, speed_bar, 200, 20, 434, 45);
              textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "# of:");
              textprintf_ex(buffer, font, 10, 30, makecol(255, 255, 255), -1, "recursive calls = %d", recursive_counter);
              textprintf_ex(buffer, font, 10, 45, makecol(255, 255, 255), -1, "moves           = %d", move_counter);
              stretch_sprite(buffer, mouse, mouse_x, mouse_y, 15, 19);    
        
              blit(buffer, screen,0,0,0,0,640,480);
        }
     }
     
     blit(buffer, screen,0,0,0,0,640,480); //print on screen
     
}
END_OF_FUNCTION(display)   

void display_pegs(STACK peg[], BITMAP *buffer) 
{
     //creating bitmaps
     static BITMAP *block1 = load_bitmap("images/block1.bmp", NULL);
     static BITMAP *block2 = load_bitmap("images/block2.bmp", NULL);
     static BITMAP *peg_pic = load_bitmap("images/peg.bmp", NULL);
     
     //display the peg picture three times, at the same y-coordinate but different x-coordinates
     for (int i=0; i<3; i++)
            stretch_sprite(buffer, peg_pic, 40+200*i, 103, 160, 260);
     
     //display the blocks in all three pegs
     for (int i=0; i<3; i++)
            peg[i].display_blocks(tower_size(peg), i, buffer, block1, block2 );   
}
END_OF_FUNCTION(display_pegs);

void run_buttons(STACK peg[], int &square_x_pos, bool &grab, BITMAP *buffer)
{
     //bitmaps
     static BITMAP *pause1 = load_bitmap("images/pause1.bmp", NULL);
     static BITMAP *pause2 = load_bitmap("images/pause2.bmp", NULL);
     static BITMAP *resume1 = load_bitmap("images/resume1.bmp", NULL);
     static BITMAP *resume2 = load_bitmap("images/resume2.bmp", NULL);
     static BITMAP *start1 = load_bitmap("images/start1.bmp", NULL);
     static BITMAP *start2 = load_bitmap("images/start2.bmp", NULL);
     static BITMAP *reset1 = load_bitmap("images/reset1.bmp", NULL);
     static BITMAP *reset2 = load_bitmap("images/reset2.bmp", NULL);
     static BITMAP *exit1 = load_bitmap("images/exit1.bmp", NULL);
     static BITMAP *exit2 = load_bitmap("images/exit2.bmp", NULL);
     static BITMAP *up1 = load_bitmap("images/up1.bmp", NULL);
     static BITMAP *up2 = load_bitmap("images/up2.bmp", NULL);
     static BITMAP *down1 = load_bitmap("images/down1.bmp", NULL);
     static BITMAP *down2 = load_bitmap("images/down2.bmp", NULL);
     static BITMAP *square1 = load_bitmap("images/square1.bmp", NULL);
     static BITMAP *square2 = load_bitmap("images/square2.bmp", NULL);
     
     //if the recursion isn't running
     if (solving == false)
     {
        textprintf_ex(buffer, font, 63, 419, makecol(255, 255, 255), -1, "current size: %d", tower_size(peg));
        if (click_buttons(buffer, start1, start2, 210, 400, 100, 50) == true)  //user can click start
        {
           reset_game(peg, tower_size(peg));       
           solving = true;            //and the solving starts - recursion starts
           //this small while loop prevents the program from counting one click as many clicks
           while (click_buttons(buffer, start1, start2, 210, 400, 100, 50) == true);  
        }
        if (click_buttons(buffer, up1, up2, 105, 375, 35, 35) == true) //user can increase the size of the tower
        {
           if (tower_size(peg) < 10)    //maximum size = 10
              reset_game(peg, tower_size(peg)+1);    //reset the pegs with a number one larger
           while (click_buttons(buffer, up1, up2, 105, 380, 35, 35) == true);   //the same while loop that prevents clicking issue
        }
        if (click_buttons(buffer, down1, down2, 105, 435, 35, 35) == true)  //use can decrease the size of the tower
        {
           if (tower_size(peg) > 1)    //minimum size = 1
              reset_game(peg, tower_size(peg)-1);    //reset the pegs with a number one smaller.
           while (click_buttons(buffer, down1, down2, 105, 430, 35, 35) == true);
        }
     }
     
     //if the recursion is running
     if (solving == true)
     {
        //only if the recursion isn't finished 
        if (finished == false)
        {          
           if (pause == false)    //if the game isn't paused 
           {
              if (click_buttons(buffer, pause1, pause2, 355, 400, 100, 50) == true)  //user can pause the game
              {
                 pause = true;    
                 while (click_buttons(buffer, pause1, pause2, 355, 400, 100, 50) == true);
                 pause_game(peg, square_x_pos, grab, buffer);   //run the pause function 
              }
              
           } 
           if (pause == true)   //if the game is paused
           {
              if (click_buttons(buffer, resume1, resume2, 355, 400, 100, 50) == true)  //user can "resume" the game
              {
                 pause = false;
                 while (click_buttons(buffer, resume1, resume2, 355, 400, 100, 50) == true);
              }
           }      
        }
        
        if (click_buttons(buffer, reset1, reset2, 210, 400, 100, 50) == true)  //user can reset the game while the recursion is running                                                  
        {                                                                      //regardless of whether it's "finished" or not
           reset_game(peg, tower_size(peg));
           while (click_buttons(buffer, reset1, reset2, 210, 400, 100, 50) == true);
        }
     }
     
     //once the user clicks on the speed_button, it's "grab"bed
     if (click_buttons(buffer, square1, square2, square_x_pos, 32, 18, 24) == true)
        grab = true;    
     //the "grabing" lasts until the mouse is released or if the mouse gets too far from the button
     if (mouse_y < 1 || mouse_y > 150 || !(mouse_b & 1) )
        grab = false;
     
     /*this is why we had to pass down this "grab" varible - we cannot declare it each time we dislay things  
     because want to keep track of its status at all moment, inside or outside of the recursion */
    
     //when the button is "grabbed"
     if (grab == true)
     {
        stretch_sprite(buffer, square2, square_x_pos, 32, 18, 24);  //change the picture 
        square_x_pos = mouse_x - 9;             //its x_coordinate follows the mouse
     }
     //limiting the displacement of the button
     if (square_x_pos < 258)        
        square_x_pos = 258;
     if (square_x_pos > 558)
        square_x_pos = 558;
     
     //use the position of the button to control the speed;
     set_speed(square_x_pos - 258);
     
     //exit button - clickable all the time
     if (click_buttons(buffer, exit1, exit2, 500, 400, 100, 50) == true)
        run = false;
}
END_OF_FUNCTION(run_buttons)

bool click_buttons (BITMAP *buffer, BITMAP *button1, BITMAP *button2, int button_x, int button_y, int button_width, int button_height)
{
     //if the mouse hovers over the image
     if (mouse_x > button_x && mouse_x < button_x+button_width && mouse_y > button_y && mouse_y < button_y+button_height)
     {  //draw a different image
        stretch_sprite(buffer, button2, button_x, button_y, button_width, button_height);  
        if (mouse_b & 1)    //if right clicked, clicking is true
           return(true);
        else 
             return(false);    //if not clicked, false 
     }
     
     else   //otherwise draw the normal image
     {   
         stretch_sprite(buffer, button1, button_x, button_y, button_width, button_height);
         return(false);      
     }
}       
END_OF_FUNCTION(click_buttons);

void pause_game(STACK peg[], int &square_x_pos, bool &grab, BITMAP *buffer)
{    
     //when the program is paused but running, trap it inside this loop
     while (pause == true && run == true)
     {
           //call the display function to display everything even when paused - especially the buttons. they should at any time
           display(peg, square_x_pos, grab);    
           //this pause funciton is called inside the display function - functions calling each other!
           //but it's only called under a special condition, when the pause button is clicked.  so no worries for inifite loop   
           
           speed_counter = 1;    //sets the speed counter to 1 - prevents glitch  
     }
}
END_OF_FUNCTION(pause)


void reset_game(STACK peg[], int size)
{
     //reset all the parameters
     solving = false;      
     finished = false;
     pause = false;
     speed_counter = 1;   
     move_counter = 0;
     recursive_counter = 0;
     
     //clear all blocks from the peg
     for (int i=0; i<3; i++)
         peg[i].clear();
     
     //pushing new blocks, based on the size of the tower
     for (int i=size; i>0; i--)
        peg[0].push(i);
}
END_OF_FUNCTION(reset_game)

int tower_size(STACK peg[])
{
    int size = 0;
    //basically, it adds up the number of blocks in all three pegs - tells us the total number of blocks = size of the tower
    for (int i=0; i<3; i++)
         size += peg[i].stack_size();
    
    return (size);
}
END_OF_FUNCTION(tower_size)

void set_speed(int parameter)  //parameter is taken from the x_coordinate of the speed button. varies form 1 to 300
{
     float speed_float;
     int speed_int;
     
     //I decided to use an exponential model, because it allows user to control the speed more precisely at lower speed
     speed_float = pow(1.015469, parameter);    
     //EX: when the speed button is at half way, the speed is 10. when it's at maximum, the speed is 100.
     //it means we can change the speed more precisely from 1 to 10, less so at higher speed. 
     //But at higher speed precision matters less so it all makes sense.
     
     //coverting the float value to int.
     speed_int = (int)(speed_float+0.5);                                           
     
     install_int_ex(increment_speed_counter, BPS_TO_TIMER(speed_int));  //change the speed
}
END_OF_FUNCTION(set_speed)
