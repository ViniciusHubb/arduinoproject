# 🔐 Sistema de Controle de Acesso IoT

## 📌 Descrição

Sistema de controle de acesso embarcado utilizando ESP32‑C3, RFID (RC522) e sensor ultrassônico para detecção de presença.  
O sistema autentica usuários através de um backend remoto via HTTP, controla o acesso físico por servo motor, fornece feedback visual com LEDs e display LCD e mantém comunicação contínua com a nuvem.

O sistema é resiliente, com reconexão automática de Wi‑Fi e comportamento seguro quando offline.

---

## 🚀 Funcionalidades

- ✅ Detecção inteligente de presença (ultrassônico com filtro e estabilidade)
- ✅ Autenticação via RFID com validação no backend
- ✅ Controle de acesso físico (servo motor)
- ✅ Interface com usuário (display LCD 1602)
- ✅ Feedback visual com LEDs RGB (NeoPixel)
- ✅ Envio de dados via HTTP (JSON)
- ✅ Heartbeat para monitoramento do dispositivo
- ✅ Reconexão automática de Wi‑Fi
- ✅ Bloqueio de leitura sem conexão

---

## 🧠 Arquitetura

```
ESP32 → HTTP POST → Backend → Autorização
       ↘ Heartbeat → Monitoramento
```

---

## 🔌 Hardware Utilizado

- ESP32‑C3 Mini  
- Módulo RFID RC522  
- Sensor ultrassônico HC-SR04  
- Servo motor SG90  
- Display LCD 1602 (I2C)  
- LED RGB (NeoPixel - 8 LEDs)  
- Fonte de alimentação 5V  

---

## 🔌 Conexões dos Pinos

| Componente | Pino ESP32 |
|-----------|-----------|
| TRIG      | GPIO0     |
| ECHO      | GPIO10    |
| RFID SS   | GPIO7     |
| RFID SCK  | GPIO6     |
| RFID MOSI | GPIO5     |
| RFID MISO | GPIO4     |
| RFID RST  | GPIO2     |
| LCD SDA   | GPIO8     |
| LCD SCL   | GPIO9     |
| Servo     | GPIO1     |
| LED STRIP | GPIO3     |

---

## 🌐 Comunicação com Cloud

### 🔹 Validação de Acesso

**POST** `/api/access/validate`

```json
{
  "rfidTag": "ID_tag",
  "deviceToken": "acess_name_arduino"
}
```

Resposta:

```json
{
  "authorized": true
}
```

---

### 🔹 Heartbeat

**POST** `/api/aparelhos/ping`

```json
{
  "deviceToken": "acess_name_arduino"
}
```

---

## 📈 Fluxo do Sistema

1. Sensor detecta presença  
2. LCD exibe instrução  
3. RFID lê cartão  
4. ESP envia UID ao backend  
5. Backend valida acesso  
6. Sistema executa ação  
7. ESP envia heartbeat  

---

## 🧠 Inteligência do Sistema

### 🔹 Ultrassônico

- Filtro por média de múltiplas leituras
- Leitura em intervalos (~200ms)
- Histerese (entrada ≤ 15cm / saída ≥ 20cm)
- Tolerância temporal (~1.5s)

→ evita ruído e oscilações

---

### 🔹 RFID

- UID padronizado (hex minúsculo com zero)
- Validação feita no backend
- Evita decisões locais

---

### 🔹 Wi‑Fi

- Reconexão automática controlada
- Evita múltiplas tentativas simultâneas
- Reset de estado antes de reconectar

---

## ⚙️ Configuração do ESP32

```cpp
const char* ssid = "SEU_WIFI";
const char* password = "SENHA";

const char* validateUrl = "https://SEU_BACKEND/api/access/validate";
const char* pingUrl = "https://SEU_BACKEND/api/aparelhos/ping";

const char* deviceToken = "acess_name_arduino";
```

---

## 📊 Interface do Usuário (LCD)

```
Sem conexao
Reconectando
```

```
Aproxime o
cartao
```

```
Lendo cartao
Aguarde
```

```
Acesso liberado
```

```
Acesso negado
```

---

## 💡 Feedback Visual (LED)

| Situação | Cor |
|--------|-----|
| Aguardando | OFF |
| Acesso liberado | Verde |
| Acesso negado | Vermelho |

---

## 🛠 Tecnologias Utilizadas

- ESP32 (C++)  
- HTTP / REST  
- JSON  
- Node.js (backend)  
- Docker  
- I2C, SPI e PWM  

---

## 🔒 Segurança

- Validação centralizada no backend  
- Identificação por deviceToken  
- Bloqueio offline  

---

## 💡 Melhorias Futuras

- Dashboard com gráficos  
- Banco de dados persistente  
- Autenticação segura (API Key / JWT)  
- Modo offline inteligente  

---

## 👨‍💻 Autor

Projeto desenvolvido como estudo de sistemas embarcados, IoT e integração com backend.
