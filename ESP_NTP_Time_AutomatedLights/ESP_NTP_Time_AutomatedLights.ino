#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

#define light 2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "MADT";
const char *password = "luckydumpy";
WiFiUDP ntpUDP;

// NTPClient timeClient(UDP& udp, const char* poolServerName, int timeOffset (5hr 30 mins -> 19800 sec), unsigned long updateInterval);
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 300000);

String intro[] = {"H", "E", "L", "L", "O", " ", "W", "O", "R", "L", "D"};
String arr_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String date_time;

void setup() {
  pinMode(light, OUTPUT);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i <= 10; i++) {
      lcd.setCursor(i + 2, 0);
      lcd.print(intro[i]);
      delay(250);
      Serial.print(".");
    }
  }

  Serial.println("Connected....");
  timeClient.begin();
  lcd.clear();
}

void loop() {
  timeClient.update();

//  Serial.println(timeClient.getFormattedTime());
  lcd.setCursor(2, 0);

  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

  if (hh < 10) {
    lcd.print(0);
  }
  lcd.print(hh);
  lcd.print(":");

  if (mm < 10) {
    lcd.print(0);
  }
  lcd.print(mm);
  lcd.print(":");

  if (sec < 10) {
    lcd.print(0);
  }
  lcd.print(sec);

  if (hh == 16) {
    digitalWrite(light, LOW);
  }
  else {
    digitalWrite(light, HIGH);
  }

  int day = timeClient.getDay();
  lcd.print(" ");
  lcd.print(arr_days[day]);

}
