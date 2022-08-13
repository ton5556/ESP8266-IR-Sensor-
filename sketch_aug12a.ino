void Line_Notify1(String message1) ;
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

#define WIFI_SSID "//ใส่ในนี้"        // ใส่ชื่อ Wifi
#define WIFI_PASSWORD "//ใส่ในนี้"     // ใส่รหัส Wifi

#define LINE_TOKEN_PIR "//https://notify-bot.line.me/th/"    // ใส่ token ของตนเอง https://notify-bot.line.me/th/

#define SENSOR 2 // define pint 2 for sensor
#define ACTION 9
#define LED 17

String message1 = "มีจดหมายมาส่งครับผม";        // ใส่ข้อความแจ้งเตือน
uint32_t time1, time2;

void setup() {

  Serial.begin(115200);
  Serial.println();
  
  pinMode(SENSOR, INPUT);
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
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }


  int L = digitalRead(SENSOR);
  if (L > 30) {
    Serial.println(" Obstacle detected");
    digitalWrite(ACTION, HIGH); // send singal
    Line_Notify1(message1);
  } else {
    digitalWrite(ACTION, LOW);
  }
  delay(10);
}
void Line_Notify1(String message) {
  WiFiClientSecure client;
  client.setInsecure();
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    delay(200);
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
