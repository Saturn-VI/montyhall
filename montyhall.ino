int LED1 = D0;
int LED2 = D1;
int LED3 = D2;

int button1 = D3;
int button2 = D4;
int button3 = D5;
int button4 = D6; // start button

int buttons[] = {button1, button2, button3, button4};

int leds[] = {LED1, LED2, LED3};

int prizeDoor = 0;
int revealedDoor = 0;

bool gameStarted = false;
bool firstDoorPicked = false;

bool wasPressed = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  randomSeed(analogRead(A0));
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!gameStarted && digitalRead(button4) == LOW) {
    // start
    gameStarted = true;
    wasPressed = true;
    prizeDoor = random(3);
    blinkEverythingTwice();
    return;
  }

  if (!gameStarted) {
    return;
  }

  if (getFirstPressed() == -1 && digitalRead(button4) == HIGH) {
    wasPressed = false;
    return;
  }

  if (wasPressed) {
    // don't want to spam actions
    return;
  }

  wasPressed = true;

  int pressedButton = getFirstPressed();

  if (!firstDoorPicked) {
    firstDoorPicked = true;
    revealedDoor = getRevealDoor(pressedButton, prizeDoor);
    setLed(leds[revealedDoor], HIGH);
  } else {
    if (pressedButton == revealedDoor) {
      return;
    }
    // no matter what just blink the correct door
    blinkLed(leds[prizeDoor]);
    gameStarted = false;
  }
}

// assumes at least one button is pressed
int getFirstPressed() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttons[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

void blinkEverythingTwice() {
  for (int i = 0; i < 2; i++) {
    setAllLeds(HIGH);
    delay(500);
    setAllLeds(LOW);
    delay(500);
  }
}

void blinkLed(int ledid) {
    setAllLeds(LOW);
    setLed(ledid, HIGH);
    delay(500);
    setLed(ledid, LOW);
    delay(500);
}

void setAllLeds(int state) {
  for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
    digitalWrite(leds[i], state);
  }
}

void setLed(int id, int state) {
  digitalWrite(id, state);
}

int getRevealDoor(int selectedDoor, int prizeDoor) {
  int revealDoor;
  if (selectedDoor == prizeDoor) {
    revealDoor = random(2);
    if (revealDoor >= selectedDoor) {
      revealDoor++;
    }
  } else {
    revealDoor = 0;
    while (revealDoor == selectedDoor || revealDoor == prizeDoor) {
      revealDoor++;
    }
  }
  return revealDoor;
}
