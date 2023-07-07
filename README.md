# ESP8266-NTP-Smart-Alarm-Clock

## About Project

In my ESP8266-based project, I have utilized the Async Webserver to create a user-friendly web page interface. The web page allows users to set an alarm time in hours and minutes, control a smart light, and display a clock on a 16x2 LCD. To ensure accurate timekeeping, the system is synchronized with an NTP server, requiring a constant internet connection. Additionally, I have implemented a unique feature: when an alarm is set, the smart light automatically turns on and remains illuminated for 30 minutes. During this period, the light cannot be manually turned off, serving as an effective wake-up mechanism. To prevent interruptions caused by power or Wi-Fi outages, I have incorporated an EEPROM to store the ongoing alarm and smart light status, ensuring their persistence even after disruptions. This project combines the versatility of the ESP8266 microcontroller, the convenience of web-based control, and smart automation features to enhance the alarm and lighting experience for users.

### More About NTP
The Network Time Protocol (NTP) is a networking protocol for clock synchronization between computer systems over packet-switched, variable-latency data networks. In operation since before 1985, NTP is one of the oldest Internet protocols in current use. NTP was designed by David L. Mills of the University of Delaware.
NTP is intended to synchronize all participating computers to within a few milliseconds of Coordinated Universal Time (UTC)

### More About Async Webserver
The Async Webserver is designed to work with the ESP8266's non-blocking event-driven architecture, which means that it can handle multiple client requests simultaneously without blocking the execution of other tasks. This is especially useful in IoT applications where the microcontroller needs to perform various tasks concurrently, such as reading sensor data, controlling actuators, and responding to web requests.

The Async Webserver library provides a simple and flexible API for creating routes and handling HTTP requests. You can define routes for different URLs and associate callback functions to handle the requests. These callback functions are executed asynchronously, allowing you to perform tasks in the background while serving web requests.



## Tools and Technologies Used:

### Arduino/ESP8266 Libraries used:
- NTPClient.h
- ESP8266WiFi.h
- WiFiUdp.h
- LiquidCrystal_I2C.h
- EEPROM.h
- ESPAsyncTCP.h
- ESPAsyncWebServer.h

### Hardware Info: 
- ESP8266
- Programmed in C++/Lua

### Softwares Used 
- Git Bash
- Arduino IDE

### OS Used:
- Windows 10 

## Features  
- Smart Light
- Digital Alarm Clock
- Life Hack and Automation

  # Code for Project
  [here](https://github.com/Ddhruv-IOT/ESP8266-NTP/blob/main/ESP_NTP_Time_ServerControl_EPROM/ESP_NTP_Time_ServerControl_EPROM.ino)

# Thank you
- Thank you all for using my app.
- All suggestions are warmly welcomed.
