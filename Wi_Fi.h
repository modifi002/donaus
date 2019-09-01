#define FS_NO_GLOBALS
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

//-------- Parameter AP -------------
char ssidname[13] = "MeterAP";  //name of SSID for config
char password[20] = "";    //password of SSID for config
//---------------------------------------

//-------- Parameter Sensor -------------
String sensor_1, sensor_2, sensor_3;
char csensor_1[7], csensor_2[7], csensor_3[7];

const int PinConfig = 3; //D0 Pin button for first time config
const int LED = 2;
boolean keypress = false; //flag pin config press
boolean shouldSaveConfig = false;  //flag for saving data
boolean wificonnect = false;

void saveConfigCallback () {
  //Serial.println("Should save config");
  shouldSaveConfig = true;
}

void firstConfig() {  //Config Connecting WiFi ,Nodename, Token, LINE Token and state status=false
  WiFiManager wifiManager;

  wifiManager.resetSettings();
  //wifiManager.autoConnect(ssidname);
  //WiFi.disconnect(true);
  //--Clear config--------------
  shouldSaveConfig = false;

  strcpy(csensor_1, "");
  strcpy(csensor_2, "");
  strcpy(csensor_3, "");

  WiFiManagerParameter custom_sensor_1("csensor_1", "ID Meter 1", csensor_1, 7);
  WiFiManagerParameter custom_sensor_2("csensor_2", "ID Meter 2", csensor_2, 7);
  WiFiManagerParameter custom_sensor_3("csensor_3", "ID Meter 3", csensor_3, 7);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&custom_sensor_1);
  wifiManager.addParameter(&custom_sensor_2);
  wifiManager.addParameter(&custom_sensor_3);

  //reset settings - for testing
  wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  wifiManager.setMinimumSignalQuality();
  wifiManager.startConfigPortal(ssidname);
  //wifiManager.startConfigPortal(ssidname, password);
  delay(2000);
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(csensor_1, custom_sensor_1.getValue());
  strcpy(csensor_2, custom_sensor_2.getValue());
  strcpy(csensor_3, custom_sensor_3.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    json["csensor_1"] = csensor_1;
    json["csensor_2"] = csensor_2;
    json["csensor_3"] = csensor_3;

    if (SPIFFS.begin()) {
      fs::File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println(F("failed to open config file for writing"));
      }

      json.prettyPrintTo(Serial);
      json.printTo(configFile);
      configFile.close();
      //end save
    }
  }
}

void loadWiFiConfig() {   //--Load WiFi parameter and Nodename, token, LINE Token
  //Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    //Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      //Serial.println("reading config file");
      fs::File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        //Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          //Serial.println("\nparsed json");

          strcpy(csensor_1, json["csensor_1"]);
          strcpy(csensor_2, json["csensor_2"]);
          strcpy(csensor_3, json["csensor_3"]);

          sensor_1 = csensor_1;
          sensor_2 = csensor_2;
          sensor_3 = csensor_3;

        } else {
          Serial.println(F("failed to load json config"));
        }
      }
      configFile.close();
    }
  } else {
    Serial.println(F("failed to mount FS"));
  }
}
int count = 0;
void configwifi() {
  pinMode(PinConfig, INPUT_PULLUP);   //Config pin

  //-------------First config-----------------------------
  keypress = digitalRead(PinConfig);  //Clear config for initial new connecting Wifi
  if (keypress == LOW) {
    digitalWrite(LED, LOW);
    firstConfig();
    Serial.print(F("Press key ")); Serial.println(keypress);
    ESP.restart();
  }
  if (SPIFFS.begin()) {           //Check First config for connecting WiFi
    Serial.println(F("mounted file system"));
    if (!SPIFFS.exists("/config.json")) {
      firstConfig();
    }
  }
  //----------if first config is set ------------------------
  loadWiFiConfig();   //Load WiFi config from flash
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count = count + 1;
    if (count >= 30) {
      wificonnect = false;
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    wificonnect = true;
    WiFi.setAutoReconnect(true);
    Serial.println("Ready");
    Serial.print(F("IP address: "));
    digitalWrite(LED, HIGH);
    Serial.println(WiFi.localIP());
  }

}
void Wifi_re() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("Disconnect..");
      wificonnect = false;
      delay(50);
    }
  }
  else {
    wificonnect = false;
    delay(50);
  }
  //Serial.println("connect..");
  return;
}
