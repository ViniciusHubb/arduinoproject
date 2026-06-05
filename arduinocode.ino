#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// ===== WIFI =====
const char* ssid = "Wifi_name";
const char* password = "Wifi_password";

unsigned long ultimoWifiCheck = 0;
const unsigned long intervaloWifi = 10000;

// ===== ENDPOINTS =====
const char* validateUrl = "https://URL_your_api/api/access/validate";
const char* pingUrl     = "https://URL_your_api/api/aparelhos/ping";
const char* deviceToken = "Arduino_acess_name";

// ===== HEARTBEAT =====
unsigned long ultimoPing = 0;
const unsigned long intervaloPing = 60000;

// ===== LCD =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== ULTRASSÔNICO =====
#define TRIG_PIN 0
#define ECHO_PIN 10

#define NUM_LEITURAS 5
float leituras[NUM_LEITURAS];
int indiceLeitura = 0;

float distanciaFiltrada = 0;

unsigned long ultimoUltrassonico = 0;
const unsigned long intervaloUltra = 200;

bool dentroZona = false;
unsigned long ultimaPresenca = 0;
const unsigned long tempoPresenca = 1500;

// ===== RFID =====
#define SS_PIN 7
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ===== SERVO =====
#define SERVO_PIN 1
Servo porta;

// ===== LED STRIP =====
#define LED_PIN 3
#define NUM_LEDS 8
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ===== CONTROLE =====
bool sistemaAtivo = false;
bool exibindoResultado = false;

unsigned long tempoResultado = 0;
const unsigned long duracaoResultado = 5000;

// ===== LED FUNÇÕES =====
void ledsVerde() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
}

void ledsVermelho() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}

void ledsOff() {
  strip.clear();
  strip.show();
}

// ===== WIFI =====
void verificarWiFi() {

  wl_status_t status = WiFi.status();

  if (status == WL_CONNECTED) return;

  if (status == WL_CONNECT_FAILED || status == WL_DISCONNECTED) {

    if (millis() - ultimoWifiCheck >= intervaloWifi) {

      Serial.println("Reconectando WiFi...");

      WiFi.disconnect(true);
      delay(200);

      WiFi.begin(ssid, password);

      ultimoWifiCheck = millis();
    }
  }
}

// ===== HEARTBEAT =====
void enviarHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(pingUrl);
  http.addHeader("Content-Type", "application/json");

  String body = "{\"deviceToken\":\"ESP32-001\"}";
  http.POST(body);
  http.end();
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Wire.begin(8, 9);
  lcd.init();
  lcd.backlight();
  lcd.noBacklight();

  SPI.begin(6, 4, 5);
  mfrc522.PCD_Init();

  porta.setPeriodHertz(50);
  porta.attach(SERVO_PIN, 500, 2400);
  porta.write(0);

  // LED
  strip.begin();
  strip.show();
  ledsOff();

  WiFi.begin(ssid, password);

  Serial.println("Sistema iniciado");
}

// ===== LOOP =====
void loop() {

  verificarWiFi();

  if (millis() - ultimoPing >= intervaloPing) {
    enviarHeartbeat();
    ultimoPing = millis();
  }

  // ===== MOSTRANDO RESULTADO =====
  if (exibindoResultado) {
    if (millis() - tempoResultado >= duracaoResultado) {

      exibindoResultado = false;
      porta.write(0);
      ledsOff();

      lcd.clear();
      lcd.print("Aproxime o");
      lcd.setCursor(0,1);
      lcd.print("cartao");
    }
    return;
  }

  // ===== ULTRASSÔNICO =====
  if (millis() - ultimoUltrassonico >= intervaloUltra) {

    ultimoUltrassonico = millis();

    float leitura = medirDistancia();

    if (leitura > 0 && leitura < 100) {
      leituras[indiceLeitura] = leitura;
      indiceLeitura = (indiceLeitura + 1) % NUM_LEITURAS;
    }

    float soma = 0;
    for (int i = 0; i < NUM_LEITURAS; i++) {
      soma += leituras[i];
    }

    distanciaFiltrada = soma / NUM_LEITURAS;

    Serial.print("Dist filtrada: ");
    Serial.println(distanciaFiltrada);

    // ===== HISTERese =====
    if (!dentroZona && distanciaFiltrada <= 15) {
      dentroZona = true;
      ultimaPresenca = millis();
    }

    if (dentroZona) {
      if (distanciaFiltrada <= 20) {
        ultimaPresenca = millis();
      } else {
        dentroZona = false;
      }
    }
  }

  bool presencaAtiva = (millis() - ultimaPresenca < tempoPresenca);

  // ===== PRESENÇA =====
  if (presencaAtiva) {

    if (!sistemaAtivo) {
      sistemaAtivo = true;

      lcd.backlight();
      lcd.clear();

      if (WiFi.status() != WL_CONNECTED) {
        lcd.print("Sem conexao");
        lcd.setCursor(0,1);
        lcd.print("Reconectando");
      } else {
        lcd.print("Aproxime o");
        lcd.setCursor(0,1);
        lcd.print("cartao");
      }
    }

    // bloqueia RFID sem WiFi
    if (WiFi.status() != WL_CONNECTED) return;

    tentarLerCartao();

  } else {

    if (sistemaAtivo) {
      sistemaAtivo = false;
      lcd.clear();
      lcd.noBacklight();
      ledsOff();
    }
  }

  delay(50);
}

// ===== ULTRASSÔNICO =====
float medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;
}

// ===== BACKEND =====
bool validarBackend(String uid) {

  if (WiFi.status() != WL_CONNECTED) return false;

  lcd.clear();
  lcd.print("Lendo cartao");
  lcd.setCursor(0,1);
  lcd.print("Aguarde");

  HTTPClient http;
  http.begin(validateUrl);
  http.addHeader("Content-Type", "application/json");

  String body = "{\"rfidTag\":\"" + uid + "\",\"deviceToken\":\"ESP32-001\"}";
  int code = http.POST(body);

  if (code <= 0) {
    http.end();
    return false;
  }

  String response = http.getString();

  StaticJsonDocument<200> doc;
  if (deserializeJson(doc, response)) {
    http.end();
    return false;
  }

  bool autorizado = doc["authorized"];

  http.end();
  return autorizado;
}

// ===== RFID =====
void tentarLerCartao() {

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }

  uid.toLowerCase();

  Serial.print("UID: ");
  Serial.println(uid);

  bool autorizado = validarBackend(uid);

  if (autorizado) {
    acessoLiberado();
  } else {
    acessoNegado();
  }

  exibindoResultado = true;
  tempoResultado = millis();
}

// ===== AÇÕES =====
void acessoLiberado() {
  lcd.clear();
  lcd.print("Acesso liberado");
  porta.write(90);
  ledsVerde();
}

void acessoNegado() {
  lcd.clear();
  lcd.print("Acesso negado");
  ledsVermelho();
}
