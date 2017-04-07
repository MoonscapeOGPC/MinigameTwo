int green = 7;
int blue = 6;
int red = 5;
int rb = 12;
int gb = 11;
int bb = 8;

int pattern[7] = {};

enum state {
  WAITING,
  PLAYING
};

state currentState;
int runGames;

int addPattern() {
  for (int x = 0; x < sizeof(pattern) / sizeof(int); x++) {
    if (pattern[x] == 0) {
      pattern[x] = random(1, 4);
      break;
    }
  }
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  randomSeed(analogRead(0));
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(rb, INPUT);
  pinMode(gb, INPUT);
  pinMode(bb, INPUT);

  Serial.begin(9600);
  Serial.write('b');

}

int waitButton() {
  while (digitalRead(rb) == HIGH || digitalRead(gb) == HIGH ||
    digitalRead(bb) == HIGH) delay(25);

  while (digitalRead(rb) == LOW && digitalRead(gb) == LOW &&
    digitalRead(bb) == LOW) delay(25);

  if (digitalRead(rb) == HIGH) {
    while (digitalRead(rb) == HIGH) delay(25);
    return 1;
  } else if (digitalRead(gb) == HIGH) {
    while (digitalRead(gb) == HIGH) delay(25);
    return 2;
  } else if (digitalRead(bb) == HIGH) {
    while (digitalRead(bb) == HIGH) delay(25);
    return 3;
  }

  return 0;
}

void displayPattern() {
  static int writePins[] = {
    red,
    green,
    blue
  };
  for (int x = 0; x < sizeof(pattern) / sizeof(int) && pattern[x] != 0; x++) {
    int cur = pattern[x];
    if (cur) digitalWrite(writePins[cur - 1], LOW);
    delay(500);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
    delay(500);
  }
}

bool readPattern() {
  for (int x = 0; x < sizeof(pattern) / sizeof(int); x++) {
    int cur = pattern[x];
    if (cur == 0) {
      return true;
    }
    int picked = waitButton();
    Serial.println(picked);
    if (cur != picked) {
      delay(250);
      return false;
    }
  }
}

void clearPattern(){
  for(int x = 0; x < sizeof(pattern) / sizeof(int); x++){
    pattern[x] = 0;
  }
}

void loop() {
  if(currentState == WAITING){
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
  } else if(currentState == PLAYING){
    if(runGames > 0){
      runGames--;
      clearPattern();
      for(int i = 0; i < 4; i++) addPattern();
      displayPattern();
      if(readPattern()){
        Serial.write('s');
      } else {
        Serial.write('f');
      }
      delay(500);
    } else {
      Serial.write('d');
    }
  }

  while(Serial.available()){
    int newByte = Serial.read();
    if(newByte == 'r') {
      currentState = WAITING;
      runGames = 0;
      clearPattern();
      Serial.write('o');
    } else {
      runGames += newByte;
      currentState = PLAYING;
    }
  }
}
