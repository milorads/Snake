/*
 *  Modify from Row-Column Scanning an 8x8 LED matrix tutorial
 *  http://arduino-er.blogspot.com/
 */
/*
  Row-Column Scanning an 8x8 LED matrix with X-Y input

 This example controls an 8x8 LED matrix using two analog inputs

 created 27 May 2009
 modified 30 Aug 2011
 by Tom Igoe

 This example works for the Lumex  LDM-24488NI Matrix. See
 http://sigma.octopart.com/140413/datasheet/Lumex-LDM-24488NI.pdf
 for the pin connections

 For other LED cathode column matrixes, you should only need to change
 the pin numbers in the row[] and column[] arrays

 rows are the anodes
 cols are the cathodes
 ---------

 Pin numbers:
 Matrix:
 * Digital pins 2 through 13,
 * analog pins 2 through 5 used as digital 16 through 19
 Potentiometers:
 * center pins are attached to analog pins 0 and 1, respectively
 * side pins attached to +5V and ground, respectively.

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/RowColumnScanning

 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */


// 2-dimensional array of row pin numbers:
const int row[8] = {
  A0, A1, A2, A3, A4, A5, A6, A7
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  2, 3, 4, 5, 6, 7, 8, 9
};

// 2-dimensional array of pixels:
int pixels[8][8];

int posX = 7;
int posY = 7;
int count = 30;
bool bg = false;

void setup() {
  Serial.begin(9600);
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off:
    digitalWrite(col[thisPin], HIGH);
  }

  setupScreen();

}

void loop() {

  // draw the screen:
//  refreshScreen();
//  
//  if(count-- == 0){
//    count = 500;
//    if(posX--==0){
//      posX = 7;
//      if(posY--==0){
//        posY = 7;
//        bg = !bg;
//      }
//    }
//    setupScreen();
//
//  }

test();

}

void test()
{
  for(int i=0; i<8;i++){
      for(int j=0; j<8;j++){
                digitalWrite(row[i], HIGH);
        digitalWrite(col[j], HIGH);
      } 
    }  
}

void setupScreen(){
  Serial.println("Setup.");
  if(bg){
    //ON all others
    Serial.println("Inside bg for/for");
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        pixels[x][y] = LOW;
      }
    }       
            Serial.println("Pos x: "+(String)posX+" Pos y: "+(String)posY);
    //OFF current pos
    pixels[posX][posY] = HIGH;
  }else{
     Serial.println("Inside else for/for");
    //OFF all others
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        pixels[x][y] = HIGH;
      }
    }
    //ON current pos
  }
}

void refreshScreen() {
  Serial.println("Refresh.");
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    Serial.println((String)row[thisRow]);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      Serial.println((String)col[thisCol]+" pixel: "+ thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
        Serial.println((String)col[thisCol]);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
    Serial.println((String)row[thisRow]);
  }
}
