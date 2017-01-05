byte col = 0;
byte leds[8][8];
// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
 
int pins[17]= {-1, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};
// col[xx] of leds = pin yy on led matrix
 
int cols[8] = {pins[13], pins[10], pins[15], pins[9], pins[4], pins[16], pins[6], pins[1]};
 
// row[xx] of leds = pin yy on led matrix
int rows[8] = {pins[8], pins[7], pins[3], pins[14], pins[2], pins[12], pins[11], pins[5]};
 
void setup() {
// sets the pins as output
 
for (int i = 1; i &lt;= 16; i++) {
      pinMode(pins[i], OUTPUT);
}
 
// set up cols and rows
 
for (int i = 1; i &lt;= 8; i++) {
      digitalWrite(cols[i - 1], LOW);
}
 
for (int i = 1; i &lt;= 8; i++) {
      digitalWrite(rows[i - 1], LOW);
}
 
}
 
void loop() {
 
for (int i = 1; i &lt;= 8; i++) {
      digitalWrite(rows[i - 1], HIGH);
      delay(100);
}
 
for (int i = 1; i &lt;= 8; i++) {
      digitalWrite(rows[i - 1], LOW);
      delay(100);
}
 
}
 
