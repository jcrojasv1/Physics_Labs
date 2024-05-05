// Librerías que se estarán utilizando:
#include <Wire.h>
#include <TEA5767N.h>
#include <DHT.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4 // Este pequeño apartado regula el comportamiento del
#define DHTTYPE DHT11 // sensor de temperatura y humedad.

// Definición de las variables:
int UP = 2;
int DOWN = 3;
float station = 105.9;
float before = station;
TEA5767N radio = TEA5767N();
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  // Establecimiento de los pines y su comportamiento.
  pinMode(UP, INPUT); // Estos dos pines son los que regulan la frecuencia de la
  pinMode(DOWN, INPUT); // radio con los pulsadores.
  radio.selectFrequency(105.9); // Esta es la frecuencia inicial por defecto.
  dht.begin(); // Inicialización del sensor de temperatura y humedad.

  if (! rtc.begin()){
  Serial.println("Módulo RTC no encontrado. Por favor, revisar conexiones.");
  while(1);
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));

  lcd.begin();
  lcd.backlight();
  //lcd.print("Hello World");
}
 
void loop() {

  // RELOJ INTERNO:
  // Acá se encuentra la implementación del módulo RTC DS3231:

  DateTime fecha = rtc.now();
  Serial.print(fecha.day());
  Serial.print("/");
  Serial.print(fecha.month());
  Serial.print("/");
  Serial.print(fecha.year());
  Serial.print("  ");
  Serial.print(fecha.hour());
  Serial.print(":");
  Serial.print(fecha.minute());
  Serial.print(":");
  Serial.println(fecha.second());
  delay(1000);

  lcd.clear();
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

  // RADIO:
// Esta primera parte del código regula el comportamiento base del módulo de radio.

  if (digitalRead(2) == HIGH){
      station = station + 0.1;
  } // Acá se establece el incremento en frecuencia.

  if (digitalRead(3) == HIGH){
      station = station - 0.1;
  } // Acá el decrecimiento en frecuencia.

  //lcd.clear();
  //lcd.print(station);

  if (before != station){
    radio.selectFrequency(station);
    Serial.println(station);
    before = station;
  } // Designamos la lógica que permite cambiar de emisora.

// SENSOR DE TEMPERATURA:
// El siguiente código es la lógica subsecuente con el sensor de temperatura.
  
  float h = dht.readHumidity(); // Medición de la humedad relativa.
  float t = dht.readTemperature(); // Medición de la temperatura en grados Celsius.
  float f = dht.readTemperature(true); // Medición de la temperatura en grados Fahrenheit (No sirve es escala :p).

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return; // Verificación de errores en las medidas.
  }

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temp.:");
  Serial.print(t);
  Serial.println("°C.");

  //lcd.clear();
  //lcd.backlight();
  //lcd.print("Temp.: "); 
  //lcd.print(t);
  //lcd.print("*C.");
  //lcd.setCursor(0, 1);
  //lcd.print("Humedad: ");
  //lcd.print(h);
}