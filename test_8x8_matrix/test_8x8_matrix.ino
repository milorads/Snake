
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

// cursor position:
int x = 0;
int y = 0;

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
  }

//  // initialize the pixel matrix:
//  for (int x = 0; x < 8; x++) {
//    for (int y = 0; y < 8; y++) {
//      pixels[x][y] = HIGH;
//    }
//  }
}

void loop() {
  // read input:
//  readSensors();
  // draw the screen:
//  refreshScreen();
for(int i=0;i<8;i++)
  {
    pixels[i][i] = HIGH;
//    row[i] = HIGH;
    delay(500);
  }
}

void readSensors() {
  // turn off the last position:
  pixels[x][y] = HIGH;
  // read the sensors for X and Y values:
  x = 7 - map(analogRead(A0), 0, 1023, 0, 7);
  y = map(analogRead(A1), 0, 1023, 0, 7);
  // set the new pixel position low so that the LED will turn on
  // in the next screen refresh:
  pixels[x][y] = LOW;

}

void refreshScreen() {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}
