#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define light 13
LiquidCrystal_I2C lcd(0x27, 16, 2);
AsyncWebServer server(80);

const char *ssid = "MADT";
const char *password = "luckydumpy";
WiFiUDP ntpUDP;

String date_formatted;
String month_formatted;
String clean_date;

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

String h_set = " ";
String m_set = " ";
int flag = 0; // to track if light is on or off (Led Bulb)
int flag_lcd = 1; // to track lcd backlight

int t_flag = 0;

// NTPClient timeClient(UDP& udp, const char* poolServerName, int timeOffset (5hr 30 mins -> 19800 sec), unsigned long updateInterval);
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 300000);

String intro[] = {"H", "E", "L", "L", "O", " ", "W", "O", "R", "L", "D"};
String arr_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
    <html>
      <head>
        <title>ESP Input Form</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
      </head>
      <body>
        <form action="/get">
          Enter hours: <input type="text" name="input1">
          <br/>
          <br/>
          Enter mins: &nbsp &nbsp<input type="text" name="input2">
          <br/>
          <br/>
          <input type="submit" value="Submit">
        </form>
        <br>
        <p> lcd backlight </p>
        <button onclick="window.location='http://'+location.hostname+'/dis/on'"> ON / OFF </button>
        <p> Toggle Led Bulb </p>
        <button onclick="window.location='http://'+location.hostname+'/lamp/on'"> ON / OFF </button>
    </body>
</html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void serverCstm() {
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {


    String input1;
    String input2;
    String inputParam;
    int eeAddress = 0;
    if (t_flag == 0) {
      t_flag = 1;
      if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {



        input1 = request->getParam(PARAM_INPUT_1)->value();
        input2 = request->getParam(PARAM_INPUT_2)->value();

        EEPROM.put(eeAddress, input1);
        EEPROM.commit();
        Serial.print("Written");
      }

      else {
        input1 = "No Alarm Set";
        inputParam = "none";
      }

      Serial.println("Time: " + input1 + " " + input2);
      lcd.clear();

      h_set = input1;
      m_set = input2;
    }
    request->send(200, "text/html", String(index_html) + "<br> Alarm Set for: " + input1 + ":" + input2 + "Hrs <br>"  );
  });


  server.on("/dis/on", HTTP_GET, [] (AsyncWebServerRequest * request) {

    if (flag_lcd == 0) {
      flag_lcd = 1;
      lcd.backlight();
    }
    else if (flag_lcd == 1) {
      flag_lcd = 0;
      lcd.noBacklight();
    }
    else {
      ;
    }
    request->send(200, "text/html", String(index_html) + "<br> Alarm Set for: " +  h_set + ":" + m_set + "Hrs <br>"  );
  });

  server.on("/lamp/on", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if (flag == 0) {
      flag = 1;
      digitalWrite(light, HIGH);
    }
    else if (flag == 1) {
      flag = 0;
      digitalWrite(light, LOW);
    }
    else {
      ;
    }
    request->send(200, "text/html", String(index_html) + "<br> Alarm Set for: " +  h_set + ":" + m_set + "Hrs <br>"  );
  });

  server.onNotFound(notFound);
  server.begin();
}


void setup() {
  int eeAddress = 0;

  Serial.begin(115200);
  EEPROM.begin(512);

  while (!Serial) {
    ;
  }
  EEPROM.get(eeAddress, h_set);

  Serial.println(h_set);

  pinMode(light, OUTPUT);

  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
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
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Your IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(5000);

  timeClient.begin();
  lcd.clear();

  serverCstm();
}


void loop() {
  timeClient.update();

  // Get the Unix Epoch Time
  time_t epochTime = timeClient.getEpochTime();

  // Serial.println(timeClient.getFormattedTime());
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

  int day = timeClient.getDay();
  lcd.print(" ");
  lcd.print(arr_days[day]);

  // Get date from Epoch Time
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  // store the date, month and year
  int date = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;

  // Format the date
  if (date < 10) {
    date_formatted = (0 + String(date));
  }
  else {
    date_formatted = String(date);
  }

  // Format the month
  if (month < 10) {
    month_formatted = (0 + String(month));
  }
  else {
    month_formatted = String(month);
  }

  clean_date = (date_formatted + "/" + month_formatted + "/" + String(year));
  // uncomment for debug only
  // Serial.println(clean_date);

  lcd.setCursor(3, 1);
  lcd.print(clean_date);

  if (hh == h_set.toInt()) {
    lcd.setCursor(15, 1);
    lcd.print("A");
    lcd.backlight();
    flag_lcd = 3;
  }
  else {
    lcd.setCursor(15, 1);
    lcd.print(" ");
    if (flag_lcd == 3) {
      flag_lcd = 1;
    }
  }

  //  lcd.setCursor(2, 1);
  //  lcd.print("Alarm: " + h_set + ":" + m_set);

  // condition to turn on/off the light (automated)
  if (hh == h_set.toInt()) {
    flag = 3;

    digitalWrite(light, HIGH);
  }
  // lights will turn off after 1hr from the given time
  else {
    t_flag = 0;
    if (flag != 1) {
      flag = 0;
      digitalWrite(light, LOW);
    }
  }
}
