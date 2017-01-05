/*
 *  Modify from Row-Column Scanning an 8x8 LED matrix tutorial
 *  http://android-er.blogspot.com
 *  http://arduino-er.blogspot.com/
 */


// 2-dimensional array of row pin numbers:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

// 2-dimensional array of pixels:
int pixels[8][8];

const int ST_0 = 0;       //waiting Sync word
const int ST_1_CMD = 1;   //Waiting CMD
const int ST_2_DATA= 2;  //Receiving Data
const int DATA_LENGTH = 16;  //length of data = 16
const byte SYNC_WORD = 0xFF;
const byte CMD_01 = 0x01;  //currently only 01;
int cmdState;
int dataIndex;

byte data[DATA_LENGTH];

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
  
  for(int y=0; y<8; y++){
    for(int x=0; x<8; x++){
      pixels[x][y] = HIGH;
    }
  }
  
  cmdState = ST_0;
}

void loop() {
  refreshScreen();
  
  if(Serial.available()){
    cmdHandle(Serial.read());
  }

}

void cmdHandle(int incomingByte){
  switch(cmdState){
    case ST_0:
        if(incomingByte == SYNC_WORD){
          cmdState = ST_1_CMD;
        }
        break;
    case ST_1_CMD:
        if(incomingByte == CMD_01){
          cmdState = ST_2_DATA;
          dataIndex = 0;
        }else{
          cmdState = ST_0;
        }
        break;
    case ST_2_DATA:
        data[dataIndex] = incomingByte;
        dataIndex++;
        
        int iData = 0;
        if(dataIndex==DATA_LENGTH){
          cmdState = ST_0;

          for(int i=0; i<8; i++){
            byte mask = 0x01;
            
            for(int j=0; j<4; j++){
                if (data[iData] & mask){
                  pixels[i][j] = LOW;
                }else{
                  pixels[i][j] = HIGH;
                }
                mask = mask << 1;
            }
            
            iData++;
            for(int j=4; j<8; j++){
                if (data[iData] & mask){
                  pixels[i][j] = LOW;
                }else{
                  pixels[i][j] = HIGH;
                }
                mask = mask << 1;
            }
            
            iData++;
          }
        }
        break;
  }
  
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

