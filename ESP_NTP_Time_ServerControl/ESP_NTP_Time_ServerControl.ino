#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define light 2
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
String d = " ";

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
          input1: <input type="text" name="input1">
          <input type="submit" value="Submit">
        </form>
        <br>
        <form action="/get">
          input2: <input type="text" name="input2">
          <input type="submit" value="Submit">
        </form>
        <br>
        <form action="/get">
          input3: <input type="text" name="input3">
          <input type="submit" value="Submit">
        </form>
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
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    lcd.clear();
    d = inputMessage;
    request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
}


void setup() {

  Serial.begin(115200);
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
  //lcd.print(clean_date);
  lcd.print(d);

  // condition to turn on/off the light (automated)
  if (hh == d.toInt()) {
    digitalWrite(light, LOW);
  }
  // lights will turn off after 1hr from the given time
  else {
    digitalWrite(light, HIGH);
  }
}
