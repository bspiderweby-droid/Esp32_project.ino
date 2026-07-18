#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>

const char* ssid = "vivo 1726";
const char* password = "33264da71e71";

WebServer server(80);

const char* loginIndex =
"<form method='POST' action='/update' enctype='multipart/form-data'>"
"<input type='file' name='update'>"
"<input type='submit' value='Update'>"
"</form>";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", loginIndex);
  });

  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain",
                Update.hasError() ? "Update Failed" : "Update Success! Rebooting...");
    delay(1000);
    ESP.restart();
  }, []() {

    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Updating: %s\n", upload.filename.c_str());

      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }

    } else if (upload.status == UPLOAD_FILE_WRITE) {

      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }

    } else if (upload.status == UPLOAD_FILE_END) {

      if (Update.end(true)) {
        Serial.println("Update Complete");
      } else {
      #include <Wire.h>

#define PCA9685_ADDR 0x40

#define MODE1      0x00
#define PRESCALE   0xFE
#define LED0_ON_L  0x06

#define SERVO_MIN 150
#define SERVO_MAX 600

void write8(uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(PCA9685_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void setPWM(uint8_t channel, uint16_t on, uint16_t off)
{
  Wire.beginTransmission(PCA9685_ADDR);
  Wire.write(LED0_ON_L + 4 * channel);
  Wire.write(on & 0xFF);
  Wire.write(on >> 8);
  Wire.write(off & 0xFF);
  Wire.write(off >> 8);
  Wire.endTransmission();
}

void setServoAngle(uint8_t channel, int angle)
{
  angle = constrain(angle, 0, 180);

  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);

  setPWM(channel, 0, pulse);
}

void setup()
{
  Wire.begin(21, 22);

  // Reset
  write8(MODE1, 0x00);
  delay(10);

  // Sleep
  write8(MODE1, 0x10);

  // 50Hz prescale
  write8(PRESCALE, 121);

  // Wake up
  write8(MODE1, 0x00);
  delay(5);

  // Auto Increment
  write8(MODE1, 0xA1);

  delay(500);
}

void loop()
{
  setServoAngle(0, 0);
  delay(1000);

  setServoAngle(0, 90);
  delay(1000);

  setServoAngle(0, 180);
  delay(1000);
}
