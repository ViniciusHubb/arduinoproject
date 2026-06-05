# 🔐 TOI — Sistema de Controle de Acesso IoT

## 📌 Identificação

**Projeto:** TOI (Controle de Acesso IoT)  
**Curso:** CST em Análise e Desenvolvimento de Sistemas — 4º Período  
**Componente:** Projeto Integrador — IoT  

**Equipe:**
- Vinicius Oliveira  
- Diogo Nascimento  
- Maira Lourenço  
- Pedro Juan  
- Carlos Machado  
- Wslany Lima  
- Gustavo Henrique  
- Vinicius dos Santos  
- Miguel Veloso  

---

## 🔗 Links do Projeto

- 🔹 Repositório Backend: *([adicionar link](https://github.com/diogocoding/testiot-development-backc-))*
- 🔹 Repositório Frontend: *([adicionar link](https://github.com/diogocoding/testiot-development-front))*
- 🔹 Apresentação (slides + PDF): *(em breve)*
- 🔹 Sistema em funcionamento / vídeo demo: *(em breve)*
- 🔹 Dashboard / Backend: ocultado

---

## 📌 Descrição

Sistema de controle de acesso embarcado utilizando ESP32‑C3, RFID (RC522) e sensor ultrassônico para detecção de presença.  

O sistema autentica usuários através de um backend remoto via HTTP, controla o acesso físico por servo motor, fornece feedback visual com LEDs e display LCD e mantém comunicação contínua com a nuvem.

O sistema é resiliente, com reconexão automática de Wi‑Fi e comportamento seguro quando offline.

---

## 🚀 Funcionalidades

- ✅ Detecção inteligente de presença (ultrassônico com filtro e estabilidade)
- ✅ Autenticação via RFID com validação no backend
- ✅ Controle de acesso físico (servo motor)
- ✅ Interface com usuário (LCD 1602)
- ✅ Feedback visual com LEDs RGB (NeoPixel)
- ✅ Envio de dados via HTTP (JSON)
- ✅ Heartbeat para monitoramento do dispositivo
- ✅ Reconexão automática de Wi‑Fi
- ✅ Bloqueio de acesso sem conexão

---

## 📋 Documento de Requisitos Simplificado

### 🔹 Problema
Controle de acesso ineficiente e não automatizado em ambientes institucionais.

### 🔹 Escopo (MVP)
Sistema IoT capaz de autenticar usuários via RFID, validar acesso em backend e controlar uma entrada física.

### 🔹 Requisitos Funcionais (RF)
- RF01: Detectar presença com sensor ultrassônico
- RF02: Ler cartão RFID
- RF03: Enviar UID para o backend
- RF04: Receber autorização
- RF05: Acionar servo para liberar acesso
- RF06: Exibir status no LCD
- RF07: Enviar heartbeat para monitoramento

### 🔹 Requisitos Não Funcionais (RNF)
- RNF01: Tempo de resposta ≤ 2 segundos
- RNF02: Reconexão automática de Wi‑Fi
- RNF03: Operação segura offline (bloqueio)
- RNF04: Intervalo de leitura do sensor ≈ 200ms
- RNF05: Comunicação HTTP estruturada em JSON

---

## 🧠 Arquitetura

```
ESP32 → HTTP POST → Backend → Autorização
       ↘ Heartbeat → Monitoramento
```

---

## 🔌 Hardware Utilizado

- ESP32‑C3 Mini  
- RFID RC522  
- HC-SR04  
- Servo SG90  
- LCD 1602 I2C  
- NeoPixel (8 LEDs)  
- Fonte 5V  

---

## 🔌 Conexões

| Componente | Pino |
|-----------|------|
| TRIG | GPIO0 |
| ECHO | GPIO10 |
| RFID SS | GPIO7 |
| RFID SCK | GPIO6 |
| RFID MOSI | GPIO5 |
| RFID MISO | GPIO4 |
| RFID RST | GPIO2 |
| LCD SDA | GPIO8 |
| LCD SCL | GPIO9 |
| Servo | GPIO1 |
| LED | GPIO3 |

---

## 🌐 Comunicação com Cloud

### Validação

```json
POST /api/access/validate
{
  "rfidTag": "ID_tag",
  "deviceToken": "device_id"
}
```

### Heartbeat

```json
POST /api/aparelhos/ping
{
  "deviceToken": "device_id"
}
```

---

## 📈 Fluxo do Sistema

1. Detecta presença  
2. Ativa LCD  
3. Solicita cartão  
4. Lê RFID  
5. Envia para backend  
6. Valida acesso  
7. Executa ação  
8. Envia heartbeat  

---

## 🧠 Inteligência do Sistema

### Ultrassônico
- Filtro por média  
- Leitura controlada (~200ms)  
- Histerese (15cm / 20cm)  
- Tolerância (~1.5s)  

### RFID
- UID padronizado  
- Validação no backend  

### Wi‑Fi
- Reconexão automática  
- Controle de estado  

---

## 📊 Interface (LCD)

```
Sem conexao / Reconectando
Aproxime o cartao
Lendo cartao / Aguarde
Acesso liberado
Acesso negado
```

---

## 💡 Feedback (LED)

| Estado | Cor |
|------|----|
| Aguardando | OFF |
| Liberado | Verde |
| Negado | Vermelho |

---

## 🛠 Tecnologias

- ESP32 (C++)
- HTTP / REST
- JSON
- Node.js
- Docker
- I2C / SPI / PWM

---

## 🔗 Mapeamento de UCs

| Conceito | UC | Evidência |
|--------|----|--------|
| Firmware embarcado | IoT | Código ESP32 |
| APIs REST | Engenharia de Software | Comunicação HTTP |
| Cloud | Cloud Computing | Backend |
| Segurança | Segurança da Informação | Controle de acesso |

---

## 🔒 Segurança

- Credenciais isoladas
- Comunicação controlada
- Bloqueio offline

---

## 📸 Dossiê de Evidências (em breve)

- Fotos do circuito
- Logs do Serial Monitor
- Prints do dashboard
- Demonstração do sistema

---

## 👨‍💻 Autor

Projeto desenvolvido como parte do Projeto Integrador em IoT — SENAC PE.
