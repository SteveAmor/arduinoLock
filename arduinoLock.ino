#include <EEPROM.h>

#define relayPin 6
#define buzzerPin 11
#define configCodePin 53

#define firstLedStripPin 31
#define numleds 10
#define ledPinStep 2 // set to 1 if led pins are sequential (2,3,4,5 etc) or 2 for odd or even numbered pins (31,33,35 etc)
#define persistanceDelay 500  // Multiplexed LED persistance delay to increase the brightness.  Set as high as possible without LEDs flickering

#define deadbandADC 30  // shouldn't be larger than 1024 / numleds

#define dial1LedEnablePin  30
#define dial2LedEnablePin  32
#define dial3LedEnablePin  34
#define dial4LedEnablePin  36
#define dial5LedEnablePin  38
#define dial6LedEnablePin  40
#define dial7LedEnablePin  42
#define dial8LedEnablePin  44
#define dial9LedEnablePin  46

#define dial1PotPin  A0
#define dial2PotPin  A1
#define dial3PotPin  A2
#define dial4PotPin  A3
#define dial5PotPin  A4
#define dial6PotPin  A5
#define dial7PotPin  A6
#define dial8PotPin  A7
#define dial9PotPin  A8

int dial1Code = 0;   // code saved and retrieved to and from EEPROM
int dial2Code = 0;
int dial3Code = 0;
int dial4Code = 0;
int dial5Code = 0;
int dial6Code = 0;
int dial7Code = 0;
int dial8Code = 0;
int dial9Code = 0; 

int dial1Value = 0;   // value compared with code
int dial2Value = 0;
int dial3Value = 0;
int dial4Value = 0;
int dial5Value = 0;
int dial6Value = 0;
int dial7Value = 0;
int dial8Value = 0;
int dial9Value = 0;

int previousDial1ADCValue = 0;
int previousDial2ADCValue = 0;
int previousDial3ADCValue = 0;
int previousDial4ADCValue = 0;
int previousDial5ADCValue = 0;
int previousDial6ADCValue = 0;
int previousDial7ADCValue = 0;
int previousDial8ADCValue = 0;
int previousDial9ADCValue = 0;

int currentDial1ADCValue = 0;
int currentDial2ADCValue = 0;
int currentDial3ADCValue = 0;
int currentDial4ADCValue = 0;
int currentDial5ADCValue = 0;
int currentDial6ADCValue = 0;
int currentDial7ADCValue = 0;
int currentDial8ADCValue = 0;
int currentDial9ADCValue = 0;

bool configCode = false;
word ledFlashCounter = 0;

void updateLed(int enablePin, int lights) {

  ledFlashCounter ++;
  if ((ledFlashCounter >= 250) && (configCode == true)) {
    lights = 0;
    if (ledFlashCounter == 500) {
      ledFlashCounter = 0;
    }
  }

  digitalWrite(enablePin, HIGH);
  for (int i = firstLedStripPin; i < firstLedStripPin + (lights * ledPinStep); i += ledPinStep) {
    digitalWrite(i, HIGH);  // turn on required number of leds
  }
  for (int i = firstLedStripPin + (lights * ledPinStep); i < firstLedStripPin + (numleds * ledPinStep); i += ledPinStep) {
    digitalWrite(i, LOW);  // turn off the remaining leds.  We need this so that the brightness is constant for 1 to numleds
  }
  delayMicroseconds(persistanceDelay);
    for (int i = firstLedStripPin; i < firstLedStripPin + (numleds * ledPinStep); i += ledPinStep) {
    digitalWrite(i, LOW);  // turn off all leds
  }
  digitalWrite(enablePin, LOW);
  
}

bool codeCracked() {
  
  if((configCode == false) && (dial1Value == dial1Code) && (dial2Value == dial2Code) && (dial3Value == dial3Code) && (dial4Value == dial4Code) && (dial5Value == dial5Code) && (dial6Value == dial6Code) && (dial7Value == dial7Code) && (dial8Value == dial8Code) && (dial9Value == dial9Code)) {
    return true;
  }
  return false;

}

void getDialCode() {

  dial1Code = EEPROM.read(1);
  dial2Code = EEPROM.read(2);
  dial3Code = EEPROM.read(3);
  dial4Code = EEPROM.read(4);
  dial5Code = EEPROM.read(5);
  dial6Code = EEPROM.read(6);
  dial7Code = EEPROM.read(7);
  dial8Code = EEPROM.read(8);
  dial9Code = EEPROM.read(9);
  
  return;
  
}

void saveDialCode() {

  EEPROM.write(1,dial1Value);
  EEPROM.write(2,dial2Value);
  EEPROM.write(3,dial3Value);
  EEPROM.write(4,dial4Value);
  EEPROM.write(5,dial5Value);
  EEPROM.write(6,dial6Value);
  EEPROM.write(7,dial7Value);
  EEPROM.write(8,dial8Value);
  EEPROM.write(9,dial9Value);

  return;
  
}

void setup() {

  for(int ledStripPin = firstLedStripPin; ledStripPin < (firstLedStripPin + (numleds * ledPinStep)); ledStripPin += ledPinStep) {
    pinMode(ledStripPin, OUTPUT);
  }
  pinMode(dial1LedEnablePin, OUTPUT);
  pinMode(dial2LedEnablePin, OUTPUT);
  pinMode(dial3LedEnablePin, OUTPUT);
  pinMode(dial4LedEnablePin, OUTPUT);
  pinMode(dial5LedEnablePin, OUTPUT);
  pinMode(dial6LedEnablePin, OUTPUT);
  pinMode(dial7LedEnablePin, OUTPUT);
  pinMode(dial8LedEnablePin, OUTPUT);
  pinMode(dial9LedEnablePin, OUTPUT);
 
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(configCodePin, INPUT_PULLUP);

  getDialCode();

  if((dial1Code + dial2Code + dial3Code + dial4Code + dial5Code + dial6Code + dial7Code + dial8Code + dial9Code) > 90) {
    configCode = true;
  }

  // Check if configCode button pressed on boot up.  
  // If it is, sound buzzer and wait for the button to be released.
  
  if(!digitalRead(configCodePin) && configCode == false) {
    configCode = true;
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    while(!digitalRead(configCodePin)){
    }
    delay(200);
    digitalWrite(buzzerPin, LOW);
  }

}

void loop() {

  if((!digitalRead(configCodePin)) && configCode == true) {
    saveDialCode();
    configCode = false;
    getDialCode();   
  }

  currentDial1ADCValue = analogRead(dial1PotPin);
  if(abs(currentDial1ADCValue - previousDial1ADCValue) > deadbandADC) {
    dial1Value = currentDial1ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial1ADCValue = currentDial1ADCValue;
  }
  updateLed(dial1LedEnablePin, dial1Value);
  
  currentDial2ADCValue = analogRead(dial2PotPin);
  if(abs(currentDial2ADCValue - previousDial2ADCValue) > deadbandADC) {
    dial2Value = currentDial2ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial2ADCValue = currentDial2ADCValue;
  }
  updateLed(dial2LedEnablePin, dial2Value);

  currentDial3ADCValue = analogRead(dial3PotPin);
  if(abs(currentDial3ADCValue - previousDial3ADCValue) > deadbandADC) {
    dial3Value = currentDial3ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial3ADCValue = currentDial3ADCValue;
  }
  updateLed(dial3LedEnablePin, dial3Value);

  currentDial4ADCValue = analogRead(dial4PotPin);
  if(abs(currentDial4ADCValue - previousDial4ADCValue) > deadbandADC) {
    dial4Value = currentDial4ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial4ADCValue = currentDial4ADCValue;
  }
  updateLed(dial4LedEnablePin, dial4Value);

  currentDial5ADCValue = analogRead(dial5PotPin);
  if(abs(currentDial5ADCValue - previousDial5ADCValue) > deadbandADC) {
    dial5Value = currentDial5ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial5ADCValue = currentDial5ADCValue;
  }
  updateLed(dial5LedEnablePin, dial5Value);

  currentDial6ADCValue = analogRead(dial6PotPin);
  if(abs(currentDial6ADCValue - previousDial6ADCValue) > deadbandADC) {
    dial6Value = currentDial6ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial6ADCValue = currentDial6ADCValue;
  }
  updateLed(dial6LedEnablePin, dial6Value);

  currentDial7ADCValue = analogRead(dial7PotPin);
  if(abs(currentDial7ADCValue - previousDial7ADCValue) > deadbandADC) {
    dial7Value = currentDial7ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial7ADCValue = currentDial7ADCValue;
  }
  updateLed(dial7LedEnablePin, dial7Value);

  currentDial8ADCValue = analogRead(dial8PotPin);
  if(abs(currentDial8ADCValue - previousDial8ADCValue) > deadbandADC) {
    dial8Value = currentDial8ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial8ADCValue = currentDial8ADCValue;
  }
  updateLed(dial8LedEnablePin, dial8Value);

  currentDial9ADCValue = analogRead(dial9PotPin);
  if(abs(currentDial9ADCValue - previousDial9ADCValue) > deadbandADC) {
    dial9Value = currentDial9ADCValue / ((1023 - deadbandADC) / numleds);
    previousDial9ADCValue = currentDial9ADCValue;
  }
  updateLed(dial9LedEnablePin, dial9Value);
  
  if(codeCracked()) {
    digitalWrite(relayPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
 
}

