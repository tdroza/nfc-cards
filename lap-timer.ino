// include the library
#include <LiquidCrystal.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 7, 6);
const int LANE1PIN = 2;
const int LANE2PIN=3;

typedef struct {
  int currentSwitchState;
  int lastSwitchState;
  int laps=-1;
  unsigned long startTime;
  unsigned long endTime;
  unsigned long lastLap;
  unsigned long bestLap;
}lane;

lane lanes[2];


void LANE1_ISR(void)
{

  lanes[0].currentSwitchState = digitalRead(LANE1PIN);
  
  //if (lanes[0].currentSwitchState != lanes[0].lastSwitchState && lanes[0].currentSwitchState == LOW) {
    Serial.println("ISR Press");
    unsigned long tempMillis = millis();

    if (tempMillis - lanes[0].startTime > 1000) {
      // ignore laps that are too short  
      lanes[0].laps++;
      lanes[0].endTime = tempMillis;
      if (lanes[0].laps > 0) {
        
  
        lanes[0].lastLap = lanes[0].endTime - lanes[0].startTime;
        if (lanes[0].laps == 1) {
          lanes[0].bestLap = lanes[0].lastLap;
        }
        lanes[0].bestLap = min(lanes[0].lastLap, lanes[0].bestLap);
      }
      lanes[0].startTime = lanes[0].endTime;
      
    }
  //}
lanes[0].lastSwitchState = lanes[0].currentSwitchState;
  
}

void setup() {
  Serial.begin(9600);
  
  // setup lane switches
  pinMode(LANE1PIN, INPUT_PULLUP);
  pinMode(LANE2PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LANE1PIN), LANE1_ISR, RISING);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
  lcd.print("** Lap Timer **");
  delay(2000);
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("!!! GO GO GO !!!");
}

void loop() {;
  
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  // Print a message to the LCD.
  char buffer[17];
  char lastStr[6], bestStr[6];
  dtostrf(lanes[0].lastLap/1000.0, -5, 2, lastStr);
  dtostrf(lanes[0].bestLap/1000.0, -5, 2, bestStr);
  sprintf(buffer, "%02d B%s L%s", max(0, lanes[0].laps),bestStr, lastStr );
  if (lanes[0].laps >= 0) {
    lcd.print(buffer);
  }
  
  delay(3);
}
