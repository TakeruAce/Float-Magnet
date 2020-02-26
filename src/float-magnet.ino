#include<Arduino.h>
#include"const.hpp"
#include"manget.hpp"

bool isGenerating = false;
float targetVal = 180.0;
Magnet magnet1(MAGNET_DIR_PIN_1,MAGNET_PWM_PIN_1,MAGNET_SENSOR_PIN_1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  magnet1.setup();
  pinMode(MAGNET_DIR_PIN_1,OUTPUT);
  pinMode(MAGNET_PWM_PIN_1,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  magnet1.update();
  magnet1.handleMagnet(targetVal);
  digitalWrite(MAGNET_DIR_PIN_1,magnet1.getPWM() > 0 ? LOW : HIGH);
  analogWrite(MAGNET_PWM_PIN_1,magnet1.getPWM());
  delayMicroseconds(500);

  if (Serial.available()) {
    handleSerialEvent();
  }
}

void handleSerialEvent() {
  char cmd = Serial.read();
  switch (cmd) {
    case 'g':
      magnet1.setIsGenerating(!magnet1.getIsGenerating());
      Serial.println(magnet1.getIsGenerating() ? "Magnet is active." : "Magnet is deactive.");
      magnet1.reset();
      break;
    case 't':
      targetVal = fetchValue();
    default:
      break;
  }
}


int fetchValue() {
  int i = 0;
  char string[10];

  while (i < (int)sizeof(string)) {
    if (Serial.available()) {
      char c = Serial.read();
      if ((c >= '0' && c <= '9') || c == '-') {
        string[i] = c;
        i++;
      } else {
        string[i] = '\0';
        break;
      }
    }
  }
  return atoi(string);
}

