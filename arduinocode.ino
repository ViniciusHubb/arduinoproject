#include <WiFi.h>

#include <HTTPClient.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <SPI.h>

#include <MFRC522.h>

#include <ESP32Servo.h>

 

// ===== WIFI =====

const char* ssid = "wifi";

const char* password = "senha";

const char* apiUrl = "url para envio de dados";

 

 

unsigned long ultimoWifiCheck = 0;

const unsigned long intervaloWifi = 10000;

 

// ===== LCD =====

LiquidCrystal_I2C lcd(0x27, 16, 2);

 

// ===== ULTRASSÔNICO =====

#define TRIG_PIN 0

#define ECHO_PIN 10

 

// ===== RFID =====

#define SS_PIN 7

#define RST_PIN 2

MFRC522 mfrc522(SS_PIN, RST_PIN);

 

// ===== SERVO =====

#define SERVO_PIN 1

Servo porta;

 

// ===== CARTÕES =====

String cartao1 = "29a08159";

String cartao2 = "d9be345a";

 

// ===== VARIÁVEIS =====

float distancia;

int contadorPresenca = 0;

const int leiturasNecessarias = 6;

 

bool sistemaAtivo = false;

 

bool exibindoResultado = false;

unsigned long tempoResultado = 0;

const unsigned long duracaoResultado = 5000;

 

// ===== WIFI AUTO =====

void verificarWiFi() {

 

  if (WiFi.status() == WL_CONNECTED) return;

 

  if (millis() - ultimoWifiCheck >= intervaloWifi) {

 

    Serial.println("Reconectando WiFi...");

    WiFi.begin(ssid, password);

 

    ultimoWifiCheck = millis();

  }

}

 

// ===== SETUP =====

void setup() {

  Serial.begin(115200);

  Serial.println("=== INICIANDO ===");

 

  pinMode(TRIG_PIN, OUTPUT);

  pinMode(ECHO_PIN, INPUT);

 

  // LCD

  Wire.begin(8, 9);

  lcd.init();

  lcd.backlight();

  lcd.noBacklight();

  Serial.println("LCD OK");

 

  // RFID

  SPI.begin(6, 4, 5);

  mfrc522.PCD_Init();

  Serial.println("RFID OK");

 

  // Servo

  porta.setPeriodHertz(50);

  porta.attach(SERVO_PIN, 500, 2400);

  porta.write(0);

  Serial.println("SERVO OK");

 

  // WiFi inicial

  WiFi.begin(ssid, password);

  Serial.print("WiFi");

 

  int tent = 0;

  while (WiFi.status() != WL_CONNECTED && tent < 10) {

    delay(500);

    Serial.print(".");

    tent++;

  }

 

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println(" conectado!");

  } else {

    Serial.println(" offline");

  }

 

  Serial.println("=== PRONTO ===");

}

 

// ===== LOOP =====

void loop() {

 

  verificarWiFi();

 

  // trava durante resultado

  if (exibindoResultado) {

    if (millis() - tempoResultado >= duracaoResultado) {

 

      Serial.println("Fim resultado");

 

      exibindoResultado = false;

 

      porta.write(0);

 

      lcd.clear();

      lcd.setCursor(0,0);

      lcd.print("Aproxime o");

      lcd.setCursor(0,1);

      lcd.print("cartao");

    }

    return;

  }

 

  // ===== ULTRASSÔNICO =====

  distancia = medirDistancia();

 

  Serial.print("Dist: ");

  Serial.println(distancia);

 

  if (distancia >= 8 && distancia <= 15) {

    contadorPresenca++;

  } else {

    contadorPresenca = 0;

  }

 

  // ===== PRESENÇA =====

  if (contadorPresenca >= leiturasNecessarias) {

 

    if (!sistemaAtivo) {

      sistemaAtivo = true;

 

      Serial.println(">>> PRESENCA <<<");

 

      lcd.backlight();

      lcd.clear();

      lcd.print("Aproxime o");

      lcd.setCursor(0,1);

      lcd.print("cartao");

    }

 

    // 🔥 CHAMAR RFID SEM POLUIR SERIAL

    tentarLerCartao();

 

  } else {

 

    sistemaAtivo = false;

    lcd.clear();

    lcd.noBacklight();

  }

 

  delay(300);

}

 

// ===== FUNÇÃO RFID OTIMIZADA =====

void tentarLerCartao() {

 

  if (!mfrc522.PICC_IsNewCardPresent()) return;

  if (!mfrc522.PICC_ReadCardSerial()) return;

 

  Serial.println("Cartao detectado!");

 

  String uid = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {

    uid += String(mfrc522.uid.uidByte[i], HEX);

  }

 

  Serial.print("UID: ");

  Serial.println(uid);

 

  if (uid == cartao1 || uid == cartao2) {

    acessoLiberado(uid);

  } else {

    acessoNegado(uid);

  }

 

  exibindoResultado = true;

  tempoResultado = millis();

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

 

// ===== HTTP =====

void enviarLog(String uid, String status) {

 

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi off -> sem log");

    return;

  }

 

  HTTPClient http;

  http.begin(apiUrl);

  http.addHeader("Content-Type", "application/json");

 

  String json = "{";

  json += "\"uid\":\"" + uid + "\",";

  json += "\"status\":\"" + status + "\"";

  json += "}";

 

  int code = http.POST(json);

 

  Serial.print("HTTP: ");

  Serial.println(code);

 

  http.end();

}

 

// ===== LIBERADO =====

void acessoLiberado(String uid) {

 

  Serial.println("LIBERADO");

 

  lcd.clear();

  lcd.print("Acesso liberado");

  lcd.setCursor(0,1);

  lcd.print("Bem-vindo!");

 

  porta.write(90);

 

  enviarLog(uid, "LIBERADO");

}

 

// ===== NEGADO =====

void acessoNegado(String uid) {

 

  Serial.println("NEGADO");

 

  lcd.clear();

  lcd.print("Acesso negado");

  lcd.setCursor(0,1);

  lcd.print("Invalido");

 

  enviarLog(uid, "NEGADO");

}