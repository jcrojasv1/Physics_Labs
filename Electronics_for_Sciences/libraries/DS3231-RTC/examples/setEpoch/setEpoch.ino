#include <Wire.h>
#include <DS3231-RTC.h>

void showTimeFormated(time_t t) {
#if defined (__AVR__)
    t -= 946684800UL;
#endif
  char buffer[50];
  struct tm *ptm;
  ptm = gmtime (&t);
  const char * timeformat {"%a %F %X - weekday %w; CW %W"};
  strftime(buffer, sizeof(buffer), timeformat, ptm);
  Serial.print(buffer);
  Serial.print("\n");
}

// unix timestamp of: Tue Aug 16 2022 10:00:00 GMT+0000
constexpr time_t tstmp {1660644000UL};

RTClib myRTC;
DS3231 Clock;

void setup () {
    Serial.begin(115200);
    Wire.begin();
    delay(500);
    Serial.println("\n\n\nTest of DS3231 - setEpoch()\n\n\n");

#if defined (__AVR__)
#warning using AVR platform
    Serial.println("\n\nAVR Microcontroller Ready!\n\n");
    Wire.begin();

#elif defined (__SAMD21G18A__)
#warning using SAMD21 platform
    Serial.println("\n\nSAMD21 Microcontroller Ready!\n\n");
    Wire.begin();

#elif defined (ESP8266)
#warning using espressif platform
    Serial.println("\n\nESP8266 Microcontroller Ready!\n\n");
    // SDA, SCL
    Wire.begin(SDA, SCL);
#endif

    // set the DS3231 with a specific UnixTimestamp
    // ==>  	Tue Aug 16 2022 10:00:00 GMT+0000 - weekday 2 (0 = Sunday); CW 33
    // ==>    1660644000
    
    Serial.println("Tue Aug 16 2022 10:00:00 GMT+0000 - weekday 2 (0 = Sunday); CW 33");
    Serial.println("UnixTimestamp - 1660644000");


    // feed UnixTimeStamp and don' t use localtime
    Clock.setEpoch(tstmp, false);
    // set to 24h
    Clock.setClockMode(false);

    // Just for verification of DS3231 Data
    // check now the data from ESP8266 and DS3231
    // get year
    bool century = false;
    bool h12Flag;
    bool pmFlag;

    // read directly from DS3231 module
    Serial.print("\n\n");
    Serial.print(" DateTime of DS3231:     ");
    Serial.print(Clock.getYear(), DEC);
    Serial.print("-");
    Serial.print(Clock.getMonth(century), DEC);
    Serial.print("-");
    Serial.print(Clock.getDate(), DEC);
    Serial.print(" ");
    Serial.print(Clock.getHour(h12Flag, pmFlag), DEC);
    Serial.print(":");
    Serial.print(Clock.getMinute(), DEC);
    Serial.print(":");
    Serial.print(Clock.getSecond(), DEC);
    Serial.print("  -  weekday ");
    Serial.print(Clock.getDoW(), DEC);
    Serial.println();

    // Read now from DateTime class
    DateTime datetime = myRTC.now();
    Serial.print("\n\nData of Struct tm\n");
    Serial.print(" DateTime of RTC:        ");
    Serial.print(datetime.getYear(), DEC);
    Serial.print("-");
    Serial.print(datetime.getMonth(), DEC);
    Serial.print("-");
    Serial.print(datetime.getDay(), DEC);
    Serial.print(" ");
    Serial.print(datetime.getHour(), DEC);
    Serial.print(":");
    Serial.print(datetime.getMinute(), DEC);
    Serial.print(":");
    Serial.print(datetime.getSecond(), DEC);
    Serial.print("  -  weekday ");
    Serial.print(datetime.getWeekDay(), DEC);
    Serial.println();
    Serial.print(" Unixtime: ");
    Serial.println(datetime.getUnixTime());
    Serial.print(" Y2k-Time: ");
    Serial.println(datetime.getY2kTime());

    Serial.print("\n\n Output of Struct tm:  ");
    // Use above defined function
    showTimeFormated(tstmp);

    // Use smart print function from DateTime class
    Serial.print("\nUse show_DateTime function:\n ");
    // provide a buffer
    char buffer[80];
    datetime.show_DateTime(buffer, sizeof(buffer));
    Serial.println(buffer);
}

void loop () {
}
