#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <wire.h>

// --- Setup Wi-Fi ---
const char* ssid = "SSID"; //PASTE NAME OF YOUR WIFI IN ""
const char* password = "PASSWORD"; // PASTE PASSWORD OF YOUR WIFI  IN ""

// --- Yandex region ID ---
String regionID = "XXXXXX";

/*PASTE YANDEX RIGION ID. Type in browser yendex.com 
and search anything then take link 
https://yandex.com/search/?text=XXXXXX&lr=xxxxxx
&search_source=ya_desktop_common after Ir= is your region ID 
it consist only numbers */

unsigned long lastTime = 0;
unsigned long timerDelay = 20000;

StaticJsonDocument<2000> doc;

String tempStr = "";
String sr = "";
String ss = "";
String location = "";

// --- NTP ---
int lastMinute = -1;
WiFiUDP ntpUDP;
NTPClient* timeClient = nullptr;

// --- LCD 16x2 ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Button ---
const int buttonPin = D3;
int page = 0;
bool lastButtonState = LOW;

byte L[8] = 
{
  B01011,
  B01101,
  B01110,
  B01011,
  B01011,
  B01110,
  B01101,
  B01011
};
byte R[8] =
{
  B11010,
  B10110,
  B01110,
  B11010,
  B11010,
  B01110,
  B10110,
  B11010
};
byte T[8] =
{
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte D[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte TR[8] =
{
  B00011,
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte DL[8] =
{
  B11000,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B11000
};
byte DR[8] =
{
  B00011,
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001,
  B00011
};
byte TD[8] =
{
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};

long parseOffset(String offsetStr) {
  int sign = (offsetStr.indexOf('+') != -1) ? 1 : -1;
  int colon = offsetStr.indexOf(':');
  int hours = offsetStr.substring(4, colon).toInt();
  int minutes = offsetStr.substring(colon + 1).toInt();
  return sign * (hours * 3600 + minutes * 60);
}

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  Wire.begin();

  lcd.createChar(0,L);
  lcd.createChar(1,R);
  lcd.createChar(2,T);
  lcd.createChar(3,D);
  lcd.createChar(4,TD);
  lcd.createChar(5,DL);
  lcd.createChar(6,DR);
  lcd.createChar(7,TR);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Abdosh's");
  lcd.setCursor(0,1);
  lcd.print("Technologies");
  delay(5000);
  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.print("Connecting to ");
  lcd.setCursor(0,1);
  lcd.print("WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(4,1);
    lcd.print(".");
    delay(1500);
    lcd.setCursor(5,1);
    lcd.print(".");
    delay(1500);
    lcd.setCursor(6,1);
    lcd.print(".");
    delay(1500);
  }
  lcd.clear();
  lcd.print("WiFi connected");
  lcd.setCursor(0,1);
  lcd.print("IP " + WiFi.localIP().toString());
  delay(2000);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Just a");
  lcd.setCursor(5, 1);
  lcd.print("moment");
}

void custom0(int x)
{

  lcd.setCursor(x,0); 
  lcd.write(0);  
  lcd.write(2);  
  lcd.write(1);  
  lcd.setCursor(x, 1); 
  lcd.write(0);  
  lcd.write(3);  
  lcd.write(1);  
}

void custom1(int x)
{
  lcd.setCursor(x,0);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(1);
  lcd.setCursor(x,1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(1);
}

void custom2(int x)
{
  lcd.setCursor(x,0);
  lcd.write(6);
  lcd.write(4);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(0);
  lcd.write(4);
  lcd.write(5);
}

void custom3(int x)
{
  lcd.setCursor(x,0);
  lcd.write(6);
  lcd.write(4);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(6);
  lcd.write(4);
  lcd.write(1); 
}

void custom4(int x)
{
  lcd.setCursor(x,0);
  lcd.write(0);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(7);
  lcd.write(2);
  lcd.write(1);
}

void custom5(int x)
{
  lcd.setCursor(x,0);
  lcd.write(0);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(x, 1);
  lcd.write(6);
  lcd.write(4);
  lcd.write(1);
}

void custom6(int x)
{
  lcd.setCursor(x,0);
  lcd.write(0);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(x, 1);
  lcd.write(0);
  lcd.write(4);
  lcd.write(1);
}

void custom7(int x)
{
  lcd.setCursor(x,0);
  lcd.write(7);
  lcd.write(2);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(1);
}

void custom8(int x)
{
  lcd.setCursor(x,0);
  lcd.write(0);
  lcd.write(4);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(0);
  lcd.write(4);
  lcd.write(1);
}

void custom9(int x)
{
  lcd.setCursor(x,0);
  lcd.write(0);
  lcd.write(4);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(6);
  lcd.write(4);
  lcd.write(1);
}

void printDigits(int digits, int x){
  
  switch (digits) {
  case 0:  
    custom0(x);
    break;
  case 1:  
    custom1(x);
    break;
  case 2:  
    custom2(x);
    break;
  case 3:  
    custom3(x);
    break;
  case 4:  
    custom4(x);
    break;
  case 5:  
    custom5(x);
    break;
  case 6:  
    custom6(x);
    break;
  case 7:  
    custom7(x);
    break;
  case 8:  
    custom8(x);
    break;
  case 9:  
    custom9(x);
    break;

  }
}

void updateData() {
  if (WiFi.status() != WL_CONNECTED) return;

  String serverPath = "https://yandex.com/time/sync.json?geo=" + regionID;
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  https.begin(client, serverPath);
  https.setTimeout(10000);

  int httpCode = https.GET();
  if (httpCode > 0) {
    String response = https.getString();
    DeserializationError error = deserializeJson(doc, response);
    if (!error && doc.containsKey("clocks") && doc["clocks"].containsKey(regionID)) {
      JsonObject clocks_region = doc["clocks"][regionID];

      String tz = clocks_region["offsetString"].as<const char*>();
      long offsetSeconds = parseOffset(tz);
      if (!timeClient) {
        timeClient = new NTPClient(ntpUDP, "pool.ntp.org", offsetSeconds, 60000);
        timeClient->begin();
      } else {
        timeClient->setTimeOffset(offsetSeconds);
      }

      sr = clocks_region.containsKey("sunrise") ? clocks_region["sunrise"].as<const char*>() : "";
      ss = clocks_region.containsKey("sunset")  ? clocks_region["sunset"].as<const char*>()  : "";

      if (clocks_region.containsKey("weather")) {
        JsonObject weather = clocks_region["weather"];
        if (weather.containsKey("temp")) {
          float temp = weather["temp"];
          tempStr = String(temp,2);
          if(tempStr.endsWith(".00")) tempStr = tempStr.substring(0,tempStr.length()-3);
        }
      }

      location = clocks_region["name"].as<const char*>();
      if(clocks_region.containsKey("parents")) {
        for(JsonObject parent : clocks_region["parents"].as<JsonArray>()) {
          location += ", ";
          location += parent["name"].as<const char*>();
        }
      }
    }
  }
  https.end();
}

void displayPage() {
  lcd.clear();
  if(page == 0) { // Часы
    if(timeClient) {
      timeClient->update();
      int mm = timeClient->getMinutes();
      int hh = timeClient->getHours();
      int h1 = hh / 10; 
      int h2 = hh % 10;
      int m1 = mm / 10;
      int m2 = mm % 10;
      printDigits(h1,0);
      printDigits(h2,3);
      lcd.setCursor(7,0); lcd.print("."); lcd.setCursor(7, 1); lcd.print(".");
      printDigits(m1,9);
      printDigits(m2,12);
    }
    
  }else if(page == 1) {
      lcd.setCursor(0,0);
      lcd.print("SR " + sr);
      lcd.setCursor(0,1);
      lcd.print("SS " + ss);
      lcd.setCursor(9,1);
      lcd.print("T:" + tempStr + (char)223 + "C");

  }else if(page == 2) {
      lcd.setCursor(0,0);
    if(location.length() > 16) {
      lcd.print(location.substring(0,16));
      lcd.setCursor(0,1);
      lcd.print(location.substring(16));
    } else {
      lcd.print(location);
    }
  }
}

void loop() {
  if((millis() - lastTime) > timerDelay) {
    updateData();
    lastTime = millis();
  }

  bool buttonState = digitalRead(buttonPin);
  if(buttonState == LOW && lastButtonState == HIGH) {
    page++;
    if(page > 2) page = 0;
    displayPage();
    delay(300);
  }
  lastButtonState = buttonState;

  if(page == 0 && timeClient) {
    int currentMinute = timeClient->getMinutes();
    if(currentMinute != lastMinute) {
      lastMinute = currentMinute;
      displayPage();
    }
  }

  delay(100);
}
