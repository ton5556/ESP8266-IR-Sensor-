void Line_Notify1(String message1) ;

//#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

#define WIFI_SSID "Pixel_Phone"        // ใส่ชื่อ Wifi
#define WIFI_PASSWORD "0929201064"     // ใส่รหัส Wifi

#define LINE_TOKEN_PIR "ShWRXWjB9BsiUd6FWrhGe0d0eLa1Kwqb9eGMtw8Zmy4"    // ใส่ token ของตนเอง

#define PirPin 16
#define LED 17

String message1 = "%E0%B8%A1%E0%B8%B5%E0%B8%9C%E0%B8%B9%E0%B9%89%E0%B8%9A%E0%B8%B8%E0%B8%81%E0%B8%A3%E0%B8%B8%E0%B8%81!";        // ใส่ข้อความแจ้งเตือน
uint32_t time1, time2;

void setup() {

  Serial.begin(115200);
  Serial.println();

  pinMode(PirPin, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);


  Serial.println("connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  delay(5000);
  Serial.println("Pir Ready!!");

  time1 = time2 = millis();
}

void loop() {

  time1 = millis();

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }


  if ((digitalRead(PirPin) == HIGH) && (WiFi.status() == WL_CONNECTED)) {
    while (digitalRead(PirPin) == HIGH) delay(100);
    Serial.println("Detect !");
    digitalWrite(LED, HIGH);
    Line_Notify1(message1);
  }
  delay(10);

}

void Line_Notify1(String message) {
  WiFiClientSecure client;
  //client.setInsecure();           
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    delay(2000);
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN_PIR) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message1).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message1;
  client.print(req);

  delay(20);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}
