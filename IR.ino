void Line_Notify1(String message1) ;

//#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

#define WIFI_SSID "WaRi025308_2.4G"        // ใส่ชื่อ Wifi
#define WIFI_PASSWORD "3730601025"     // ใส่รหัส Wifi

#define LINE_TOKEN_PIR "LyPVexeGO3vkqNG3VTSdHyH8ordfEMd5UNVnSuioNIh"    // ใส่ token ของตนเอง

#define PirPin 4
#define PirPin1 6
#define LED 17

String message1 = "มีจดหมายมาส่งครับผม";        // ใส่ข้อความแจ้งเตือน
uint32_t time1, time2;

void setup() {

  Serial.begin(115200);
  Serial.println();

  pinMode(PirPin, INPUT);
  pinMode(PirPin1, INPUT);
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


  if ((digitalRead(PirPin) == HIGH) && (WiFi.status() == WL_CONNECTED)) {
    while (digitalRead(PirPin) == HIGH) delay(100);
    Serial.println("Detect !");
    digitalWrite(LED, HIGH);
    Line_Notify1(message1);
  }
  if ((digitalRead(PirPin1) == HIGH) && (WiFi.status() == WL_CONNECTED)) {
    while (digitalRead(PirPin1) == HIGH) delay(10);
    Serial.println("Detect !");
    digitalWrite(LED, HIGH);
    Line_Notify1(message1);
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
