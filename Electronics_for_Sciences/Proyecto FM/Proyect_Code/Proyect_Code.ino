#include <LiquidMenu.h>
#include <Wire.h>
#include <TEA5767N.h>
#include <DHT.h>
#include <RTClib.h>

// Definiciones del Encoder:
#define outputA 13
#define outputB 12
#define sw 11

int actualstate;
int laststate;

#define DHTPIN 7
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);

LiquidLine line00(0, 0, "Operational Menu");
LiquidLine line01(1, 1, "            >>");
LiquidLine line1(1, 0, "1. Radio FM.");
LiquidLine line2(1, 1, "2. Time.");
LiquidLine line3(1, 0, "3. Temperature.");
LiquidLine line4(1, 1, "4. Bluethoot.");
LiquidLine line5(0, 0, "Frequency:");
LiquidLine line7(1, 1, "Return to menu.");
LiquidLine line8(0, 0, "Use the buttons:");
LiquidScreen screen0;
LiquidScreen screen1;
LiquidScreen screen2;
LiquidScreen screen3;
LiquidScreen screen4;
LiquidScreen screen5;
LiquidScreen screen6;
LiquidScreen screen7;
LiquidMenu menu(lcd);

int UP = 2;
int DOWN = 3;
float station = 90.9;
float before = station;
TEA5767N radio = TEA5767N();
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

LiquidLine line6(1, 1, station);

void setup() {
  Serial.begin(9600);
  pinMode(UP,INPUT);
  pinMode(DOWN,INPUT);

  
  // Configuraciones de pantalla y menú:
  lcd.begin();
  lcd.backlight();
  
  screen0.add_line(line00);
  screen0.add_line(line01);
  screen1.add_line(line1);
  screen1.add_line(line2);
  screen1.add_line(line3);
  screen1.add_line(line4);
  screen2.add_line(line5);
  screen2.add_line(line6);
  screen2.add_line(line7);

  line1.set_focusPosition(Position::LEFT);
  line2.set_focusPosition(Position::LEFT);
  line3.set_focusPosition(Position::LEFT);
  line4.set_focusPosition(Position::LEFT);
  line6.set_focusPosition(Position::LEFT);
  line7.set_focusPosition(Position::LEFT);

  line00.attach_function(1, Next);
  line01.attach_function(1, Next);
  line1.attach_function(1, Radio);
  line2.attach_function(1, Time);
  line3.attach_function(1, Temperature);
  line4.attach_function(1, Bluethooth);
  line5.attach_function(1, voided);
  line6.attach_function(1, voided);
  line7.attach_function(1, Back);
  line8.attach_function(1, Back);
  
  menu.add_screen(screen0);
  menu.add_screen(screen1);
  menu.add_screen(screen2);
  menu.add_screen(screen3);
  menu.add_screen(screen4);
  menu.add_screen(screen5);
  menu.add_screen(screen6);
  menu.add_screen(screen7);
  screen0.set_displayLineCount(2);
  screen1.set_displayLineCount(2);
  screen2.set_displayLineCount(2);
  screen3.set_displayLineCount(2);
  screen4.set_displayLineCount(2);
  screen5.set_displayLineCount(2);
  screen6.set_displayLineCount(2);  
  screen7.set_displayLineCount(2);  

  menu.init();
  menu.set_focusedLine(0);
  menu.update();

  // Configuración inicial de la radio:

  // Configuración del módulo de temperatura:
  dht.begin();

  // Configuración del módulo de tiempo:
  if (! rtc.begin()){
  Serial.println("Módulo RTC no encontrado. Por favor, revisar conexiones.");
  while(1);
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));

}

void loop(){
  // ESTRUCTURA DEL MENÚ:
  selectOption();
  actualstate = digitalRead(outputA);
  if (actualstate != laststate) {
    if(digitalRead(outputB) != actualstate){
      menu.switch_focus(false);
    } else {
      menu.switch_focus(true);
    }
    menu.update();
    laststate = actualstate;
  }
} 

void selectOption() {
  if (digitalRead(sw) == LOW) {
   menu.call_function(1);
   delay(500);
  }
}

void voided(){

}

void Radio(bool ON){
  ON = true;
  radio.turnTheSoundBackOn();
  
  menu.change_screen(7);
  lcd.print("Frequency:");
  lcd.setCursor(0, 1);
  lcd.print(station);
  radio.selectFrequency(station);

  while(ON == true){


  if (digitalRead(UP) == HIGH){
    station = station + 5;
  }

  if (digitalRead(DOWN) == HIGH){
      station = station - 5;
  }

  if (before != station){
    radio.selectFrequency(station);
    before = station;
    lcd.clear();

    lcd.print("Frequency:");
    lcd.setCursor(0, 1);
    lcd.print(station);
  }

  if (digitalRead(sw) == LOW) {
   ON = false;
   radio.mute();
  }
  }
menu.change_screen(2);
}

void Back(){
  menu.change_screen(2);
}

void Temperature(bool ON){
  ON = true;
  while(ON == true){
  menu.change_screen(5);
  float h = dht.readHumidity(); // Medición de la humedad relativa.
  float t = dht.readTemperature(); // Medición de la temperatura en grados Celsius.
  float f = dht.readTemperature(true); // Medición de la temperatura en grados Fahrenheit (No sirve es escala :p).

  lcd.print("Temp.: "); 
  lcd.print(t);
  lcd.print("*C.");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%.");
  delay(1000);

  if (digitalRead(sw) == LOW) {
   ON = false;
  }

}
menu.change_screen(2);
}

void Time(bool ON){
  ON = true;
  while(ON == true){
  menu.change_screen(6);
  DateTime fecha = rtc.now();

  lcd.print("Fecha: ");
  lcd.print(fecha.day());
  lcd.print("/");
  lcd.print(fecha.month());
  lcd.print("/");
  lcd.print(fecha.year());
  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  lcd.print(fecha.hour());
  lcd.print(":");
  lcd.print(fecha.minute());
  lcd.print(":");
  lcd.print(fecha.second());
  delay(1000);
  if (digitalRead(sw) == LOW) {
   ON = false;
  }
  }

  menu.change_screen(2);
}

void Bluethooth(){

}

void Next(){
  menu.next_screen();
}
