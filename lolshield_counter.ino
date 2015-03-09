/*

LOLShield Counter

Receives a 4 digit number over serial and displays it on the LOLShield.
Intended to display a live count recieved from an external system, such as the number of devices on a network.

Original Author: kamikazejoe

*/

#include "Charliplexing.h"      // initializes the LoL Sheild library

// define variables
int col = 14;
int row = 9;
int brightness = 7;
int numeral[4] = {0,0,0,0}; // Displayed number split into single digits.
int digit_place = 0;

char serial_input[4];

boolean numeral_bitmap[11][3][9] =  // bitmap of all the numerical digits that will be displayed.
{
{
// bitmap a blank
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0}
},

{
// bitmap a numeral 0  
{0,1,1,1,1,1,1,1,0},
{1,0,0,0,0,0,0,0,1},
{0,1,1,1,1,1,1,1,0}
},

{
// bitmap a numeral 1
{0,1,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1},
{0,0,0,0,0,0,0,0,1}
},

{
// bitmap a numeral 2
{0,1,1,0,0,0,1,1,1},
{1,0,0,0,0,1,0,0,1},
{0,1,1,1,1,0,0,0,1}
},

{
// bitmap a numeral 3
{0,1,0,0,0,0,0,1,0},
{1,0,0,0,1,0,0,0,1},
{0,1,1,1,0,1,1,1,0}
},

{
// bitmap a numeral 4
{0,1,1,1,1,0,0,0,0},
{0,0,0,0,1,0,0,0,0},
{1,1,1,1,1,1,1,1,1}
},

{
// bitmap a numeral 5
{1,1,1,1,1,0,0,0,1},
{1,0,0,0,1,0,0,0,1},
{1,0,0,0,0,1,1,1,0}
},

{
// bitmap a numeral 6
{0,1,1,1,1,1,1,1,0},
{1,0,0,0,1,0,0,0,1},
{0,1,0,0,0,1,1,1,0}
},

{
// bitmap a numeral 7
{1,0,0,0,0,0,1,1,1},
{1,0,0,1,1,1,0,0,0},
{1,1,1,0,0,0,0,0,0}
},

{
// bitmap a numeral 8
{0,1,1,1,0,1,1,1,0},
{1,0,0,0,1,0,0,0,1},
{0,1,1,1,0,1,1,1,0}
},

{
// bitmap a numeral 9
{0,1,1,1,0,0,0,1,0},
{1,0,0,0,1,0,0,0,1},
{0,1,1,1,1,1,1,1,0}
}
};




// This function is called once, when the sketch starts.
void setup() {
  LedSign::Init(DOUBLE_BUFFER | GRAYSCALE); // Initializes a grayscale frame buffer.
  led_test(); // Flashes the display to ensure the lolshield is functional
  
  // Initialize serial.
  Serial.begin(9600);
}

void loop() {

  //led_test();
  //count_demo();
  serial_mode();
}





serial_mode() {
  while (Serial.available() > 0) { // If the serial is available, read it.
    if (Serial.peek() != 0) {
      for (int i=3; i >= 0; i--) {
        serial_input[i] = Serial.read();
        
      }
      for (int i=0; i <= 3; i++) {
        numeral[i]=serial_input[3-i]-'0'; //converts character, not ascii value, to integer
        numeral[i]=numeral[i] + 1;
      }
      for (int i=3; i >= 0; i--) {
        Serial.print(serial_input[i]);
      }
      Serial.println("");
      for (int i=3; i >= 0; i--) {
        Serial.print(numeral[i]);
      }
      Serial.println("");
      update_lol();
    }
  }
}




void count_demo() {
  for (int d=1; d <= 10; d++) {
    numeral[0] = d;
    for (int c=1; c <= 10; c++) {
      numeral[1] = c;
      for (int b=1; b <= 10; b++) {
        numeral[2] = b;
        for (int a=1; a <= 10; a++) {
          numeral[3] = a;
          update_lol();
          delay(100);
        }
      }
    }
  }
}





void led_test() { // Fades all LEDs in and out.  Add a delay to slow down the effect.
  for (int brightness=0; brightness <= 7; brightness++) { // Increments brightness value to fade in.
    for (int col=0; col < 14; col++) {  // Increments each column of LEDs
      for (int row=0; row < 9; row++) { // Increments each row of LEDs
        LedSign::Set( col, row, brightness ); // Enables column and row location of LED at brightness level.
      }
    }
    LedSign::Flip(true); // Updates the lolshield display.
  }
  for (int brightness=7; brightness >= 0; brightness--) { // Decriments brightness value to fade out.
    for (int col=0; col < 14; col++) {
      for (int row=0; row < 9; row++) {
        LedSign::Set( col, row, brightness );
      }
    }
    LedSign::Flip(true);
  }  
}





void update_lol() { // Goes through ledon array.  Any LEDs marked as true, is set in the lolsheild
  LedSign::Clear();
  for (int col=0; col < 3; col++) {
    for (int row=0; row < 9; row++) {
      
      digit_place = 11;
      if (numeral_bitmap[numeral[3]][col][row] == true) {
        LedSign::Set( col + digit_place, row, brightness );
      }
      
      digit_place = 7;
      if (numeral[2] == 1 && numeral[1] < 2 && numeral[0] < 2) {
        numeral[2] = 0;
      }
      if (numeral_bitmap[numeral[2]][col][row] == true) {
        LedSign::Set( col + digit_place, row, brightness );
      }
      
      digit_place = 3;
      if (numeral[1] == 1 && numeral[0] < 2) {
        numeral[1] = 0;
      }
      if (numeral_bitmap[numeral[1]][col][row] == true) {
        LedSign::Set( col + digit_place, row, brightness );
      }
      
      digit_place = 0;
      if (numeral[0] == 1) {
        numeral[0] = 0;
      }
      if (numeral_bitmap[numeral[0]][col][row] == true) {
        LedSign::Set( col + digit_place, row, brightness );
      }
    }
  }
  LedSign::Flip(true);
}


// Reference information: 

/* 
  Numerical Bitmap

 "1" "2" "3" "4" "5" "6" "7" "8" "9" "0" 
 010 010 010 001 111 010 111 010 010 010 
 110 101 101 101 100 101 001 101 101 101
 010 101 001 101 100 100 001 101 101 101
 010 001 001 101 100 100 010 101 101 101
 010 001 010 111 110 110 010 010 011 101
 010 010 001 001 001 101 010 101 001 101
 010 100 001 001 001 101 100 101 001 101
 010 100 101 001 001 101 100 101 101 101
 111 111 010 001 110 010 100 010 010 010

*/

/*
  LED Grid Reference
  
           11111
  12345678901234
        X   X 
1 00000000000000
2 00000000000000
3 00000000000000
4 00000000000000
5 00000000000000
6 00000000000000
7 00000000000000
8 00000000000000
9 00000000000000

1s position is +11
10s position is +7
100s position is +3
1000s position is +0

*/

/*

LOLShield functions:

LedSign::Set( col, row, Brightness );  // Set individual LEDs by x and y location.  Brightness == 0 is off.  Max bright == 7.
LedSign::Flip(true);  // Updates the lolshield with current settings.
LedSign::Clear(); // Clear the non-active screen. 

*/


