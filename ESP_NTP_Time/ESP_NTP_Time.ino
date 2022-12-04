#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                               // create I2C object

const char *ssid = "MADT";                                        // WIFI Name
const char *password = "luckydumpy";                              // WIFI Password
WiFiUDP ntpUDP;                                                   // NTP Object

// NTPClient timeClient(UDP& udp, const char* poolServerName, int timeOffset (5hr 30 mins -> 19800 sec), unsigned long updateInterval); 
// NTP Client Object Parameters
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);       

String intro[] = {"H", "E", "L", "L", "O", " ", "W", "O", "R", "L", "D"};
String arr_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {
  Serial.begin(115200);                                           // Set the correct baud rate for ESP8266
  lcd.init();                                                     
  lcd.backlight();                                                
  WiFi.begin(ssid, password);                                     // Connect NodeMCU to WIFI

  // Wait for WIFI to get connected
  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i <= 10; i++) {
      lcd.setCursor(i + 2, 0);
      lcd.print(intro[i]);
      delay(250);
      Serial.print(".");
    }
  }
  
  Serial.println("Connected....");                                // WIFI Connected
  timeClient.begin();                                             // Start the NTP 
  lcd.clear();
}

void loop() {
  timeClient.update();                                            // Start the update procedure

  Serial.println(timeClient.getFormattedTime());
  lcd.setCursor(2, 0);

  // Get Hours, Mins and Seconds
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

  // Format the Output
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

  // Translate the day and print it
  int day = timeClient.getDay();
  lcd.print(" ");
  lcd.print(arr_days[day]);

}
