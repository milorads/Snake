#include "LedControl.h" //LedControl biblioteka: http://www.wayoda.org/arduino/downloads/LedControl.zip
//Koristi se za MAX7219 driver pomocu kog se mogu kontrolisati led trake i displeji

LedControl lc = LedControl(12, 11, 10, 1); //(DIN, CLK, CS, broj prikacenih MAX7219)

const int SW_pin = 8; // Switch varijabla (vertikalno dugme) na joystick-u
// Analogni X i Y pinovi sa joystick-a
int joyPin1 = A0; 
int joyPin2 = A1; 
int xAxis = 0; //Varijabla koja cita analogne vrijednosti sa x ose
int yAxis = 0; //Varijabla koja cita analogne vrijednosti sa y ose

String direction; //Varijabla koja cuva u kom se smjeru krece zmija

int snakeX[36]; //Varijabla za svih 36 pozicija na ekranu po X-u
int snakeY[36]; //Varijabla za svih 36 pozicija na ekranu po Y-u

int speed = 300; //Pauza koja oznacava brzinu kretanja (300ms izmedju pokreta sa jedne tacke na drugu)

int snakeSize; //Velicina zmije

int foodX; //Varijabla za cuvanje X ose hrane
int foodY; //Varijabla za cuvanje Y ose hrane

boolean inGame = false; //Variable that tells us if the game is running.

//Setup metoda koja se poziva pri pokretanju
void setup() {
  lc.shutdown(0, false); //Funkcija za pokretanje moda za prikazivanje na displeju
  lc.setIntensity(0, 8); //Jacina osvjetljenja diode na ekranu (0-15)
  lc.clearDisplay(0); //Gasenje svih dioda
  Serial.begin(9600); // Zapocinjanje serijske komunikacije sa racunarom

  pinMode(SW_pin, INPUT); // Postavljanje vertikalnog dugmeta kao ulaz
  digitalWrite(SW_pin, HIGH); // Default vrijednost dugmeta - 1 (nakon pritiska prelazi na 0)

  newGame(); //Pokretanje nove igre.
}

//Loop metoda koja je beskonacni while loop na arduinu
void loop() {
  if (inGame) { //Provjera da li je u modu "u igri".
    lc.clearDisplay(0); //Gasenje svih dioda 

    xAxis = analogRead(joyPin1); //Uzimanje x ose sa joystick-a
    yAxis = analogRead(joyPin2); //Uzimanje y ose sa joystick-a

    Serial.println("Reading from joystick");
    Serial.println(xAxis); //Ispis x ose na serijski port
    Serial.println(yAxis); //Ispis x ose na serijski port

    if (yAxis > 900 && direction != "up") { //Ako je input na gore i zmija ne ide na dolje podesavamo kao smjer kretanja
      direction = "down";
    }
    if (yAxis < 500 && direction != "down") { //Ako je input na dolje i zmija ne ide na gore podesavamo kao smjer kretanja
      direction = "up";
    }
    if (xAxis > 900 && direction != "left") {  //Ako je input na desno i zmija ne ide na lijevo podesavamo kao smjer kretanja
      direction = "right";
    }
    if (xAxis < 500 && direction != "right") { //Ako je input na lijevo i zmija ne ide na desno podesavamo kao smjer kretanja
      direction = "left";
    }

    move(direction); //Postavljamo novi smer kretanja zmije

    checkIfHitFood(); //Provjera da li je zmija pojela hranu na ekranu
    checkIfHitSelf(); //Checks if the snake hits itself.

    drawSnake(); //Draws the snake on the MAX7219.
    drawFood(); //Draws the food on the MAX7219.

    delay(speed); //Delay with the configured time from above.
  }
}

//Ovo je metoda koja usmjerava zmiju da ide u pravcu koji je odabran putem joystick-a
void move(String dir) {
  for (int i = snakeSize - 1; i > 0; i--) { //Idemo kroz sve tacke zmijice i postavljamo ih kao prethodnu poziciju
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (dir == "up") {
    if (snakeY[0] == 0) { //Ako zmija ide van granica, prelazi na drugu stranu ekrana
      snakeY[0] = 7;
    } else {
      snakeY[0]--;
    }
  } else if (dir == "down") {
    if (snakeY[0] == 7) { //Ako zmija ide van granica, prelazi na drugu stranu ekrana
      snakeY[0] = 0;
    } else {
      snakeY[0]++;
    }
  } else if (dir == "left") {
    if (snakeX[0] == 0) { //Ako zmija ide van granica, prelazi na drugu stranu ekrana
      snakeX[0] = 7;
    } else {
      snakeX[0]--;
    }
  } else if (dir == "right") {
    if (snakeX[0] == 7) { //Ako zmija ide van granica, prelazi na drugu stranu ekrana
      snakeX[0] = 0;
    } else {
      snakeX[0]++;
    }
  }
}

//This method loops through all the snake parts and draws them on the MAX7219.
void drawSnake() {
  for (int i = 0; i < snakeSize; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }
}

//This method draws the food on the MAX7219.
void drawFood() {
  lc.setLed(0, foodY, foodX, true);
  delay(50); //Have a short delay here to make the food blink and differentiate from the snake parts.
  lc.setLed(0, foodY, foodX, false);
}

//Generisanje hrane na nasumicnoj lokaciji na ekranu
void newFood() {
  int newFoodX = random(0, 8);
  int newFoodY = random(0, 8);
  while (isSnake(newFoodX, newFoodY)) { //Ako su koordinate na lokaciji zmijice, pokusava se generacija nove dok ne bude van pozicije zmije
    newFoodX = random(0, 8);
    newFoodY = random(0, 8);
  }
  foodX = newFoodX; // postavljanje hrane na ekran
  foodY = newFoodY; // postavljanje hrane na ekran
}

//This method checks if the snake moves onto the food.
void checkIfHitFood() {
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeSize++; //Add a snake body part.
    newFood(); //Create a new food at a new location.
  }
}

//This method checks if the snake moves onto itself.
void checkIfHitSelf() {
  for (int i = 1; i < snakeSize - 1; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver(); //Call the gameOver() method.
    }
  }
}

//Checks if the given coordinates are part of the snake.
//Used when checking where to generate new food.
boolean isSnake(int x, int y) {
  for (int i = 0; i < snakeSize - 1; i++) {
    if ((x == snakeX[i]) && (y == snakeY[i])) {
      return true;
    }
  }
  return false;
}

//Podesavanje nove igre
void newGame() {
  Serial.println("Started game."); // Ispis na serijski port
  for (int i = 0; i < 36; i++) { //Podesavanje svih varijabli da budu van ekrana, u slucaju da su ostale u memoriji, kada se igra ponovo
    snakeX[i] = -1;
    snakeY[i] = -1;
  }

  snakeX[0] = 4; //Pocetak na x->4.
  snakeY[0] = 8; //Pocetak na y->8.
  direction = "up"; //Prvo se zmija krece ka gore
  snakeSize = 1; //Velicina je na pocetku 1 tacka
  newFood(); //Generisanje hrane na ekranu
  inGame = true; //Podesavanje varijable koja oznacava da je mod "u igri" i pokrece kod i void loop()-a
}

//This method is called when the snake hits itself.
//It loops through all the coordinates of the MAX7219, making a fancy pattern.
void gameOver() {
  inGame = false; //Makes sure the game is stopped.
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      lc.setLed(0, y, x, true);
      delay(20);
      lc.setLed(0, y, x, false);
    }
  }
  bool switchButton = true;
  while(switchButton)
  {
    if(digitalRead(SW_pin)==0)
    {
            switchButton = false;
    }
  }
  newGame();
}
