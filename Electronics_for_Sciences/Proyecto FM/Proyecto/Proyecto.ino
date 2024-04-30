#include <Wire.h>
#include <TEA5767N.h>

int UP = 2;
int DOWN = 3;
float station = 105.9;
float before = station;
TEA5767N radio = TEA5767N();

void setup() {
  pinMode(UP,INPUT);
  pinMode(DOWN,INPUT);
  radio.selectFrequency(105.9);
  Serial.begin(9600);
  
}
 
void loop() {


if (digitalRead(UP) == HIGH){
    station = station + 0.05;
}

if (digitalRead(DOWN) == HIGH){
    station = station - 0.05;
}

delay(20);

if (before != station){
  radio.selectFrequency(station);
  Serial.println(station);
  before = station;
}

}