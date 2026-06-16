# 🔐 TOI — Sistema de Controle de Acesso IoT

## 📌 Identificação

**Projeto:** TOI (Controle de Acesso IoT)
**Curso:** CST em Análise e Desenvolvimento de Sistemas — 4º Período
**Componente:** Projeto Integrador — IoT

### 👥 Equipe

* Vinicius Oliveira
* Diogo Nascimento
* Maira Lourenço
* Pedro Juan
* Carlos Machado
* Wslany Lima
* Gustavo Henrique
* Miguel Veloso

---

# 🔗 Links do Projeto

* 🔹 Repositório Backend: ([link Github](https://github.com/diogocoding/testiot-development-backc-))
* 🔹 Repositório Frontend: ([link Github](https://github.com/diogocoding/testiot-development-front))
* 🔹 Apresentação (Slides + PDF): ([link Drive](https://drive.google.com/file/d/1W699Z8z-e3yq0zQTNpNe1JE-uyFwcGhd/view?usp=drivesdk))
* 🔹 Sistema em funcionamento / Vídeo Demo: ([link Youtube](https://youtu.be/LUzZ4fO3L3g?is=I-gzdclqA_iyzZeY))
* 🔹 Dashboard / Backend: Ocultado

---

# 📌 Descrição

O TOI é um sistema de controle de acesso baseado em Internet das Coisas (IoT), desenvolvido utilizando ESP32-C3, RFID RC522 e sensor ultrassônico para monitoramento e autenticação de usuários.

O sistema realiza a leitura de cartões RFID, valida as credenciais através de um backend remoto via API REST e controla fisicamente a liberação de acesso por meio de um servo motor.

Além disso, oferece feedback visual através de LEDs RGB e display LCD, monitoramento contínuo do dispositivo por heartbeat e reconexão automática em caso de falhas de rede.

O sistema foi projetado para operar de forma segura, bloqueando acessos quando não houver comunicação com o servidor.

---

# 🚀 Funcionalidades

* ✅ Detecção inteligente de presença (sensor ultrassônico)
* ✅ Autenticação via RFID
* ✅ Validação de acesso através de API REST
* ✅ Controle de acesso físico por servo motor
* ✅ Interface de usuário via LCD 1602 I2C
* ✅ Feedback visual com LEDs NeoPixel
* ✅ Comunicação HTTP utilizando JSON
* ✅ Heartbeat para monitoramento do dispositivo
* ✅ Reconexão automática de Wi-Fi
* ✅ Bloqueio de acesso quando offline
* ✅ Proteção contra leituras RFID duplicadas (Anti-Releitura)

---

# 📋 Documento de Requisitos Simplificado

## 🔹 Problema

Controle de acesso ineficiente e não automatizado em ambientes institucionais.

## 🔹 Escopo (MVP)

Sistema IoT capaz de autenticar usuários via RFID, validar permissões em backend e controlar uma entrada física.

## 🔹 Requisitos Funcionais (RF)

* RF01: Detectar presença com sensor ultrassônico
* RF02: Ler cartão RFID
* RF03: Enviar UID para o backend
* RF04: Receber autorização
* RF05: Acionar servo motor para liberação
* RF06: Exibir informações no LCD
* RF07: Enviar heartbeat periódico

## 🔹 Requisitos Não Funcionais (RNF)

* RNF01: Tempo de resposta ≤ 2 segundos
* RNF02: Reconexão automática de Wi-Fi
* RNF03: Operação segura em modo offline
* RNF04: Intervalo de leitura do sensor ≈ 200ms
* RNF05: Comunicação HTTP utilizando JSON

---

# 🧠 Arquitetura

```text
ESP32
  │
  ├── RFID RC522
  ├── Sensor Ultrassônico
  ├── LCD I2C
  ├── NeoPixel
  └── Servo Motor
          │
          ▼
      HTTP POST
          │
          ▼
       Backend
          │
          ▼
     Autorização
```

Heartbeat:

```text
ESP32
  │
  └── HTTP POST
          │
          ▼
    Monitoramento
```

---

# 🔌 Hardware Utilizado

* ESP32-C3 Mini
* RFID RC522
* HC-SR04
* Servo SG90
* LCD 1602 I2C
* NeoPixel (8 LEDs)
* Fonte 5V

---

# 🔌 Conexões

| Componente | GPIO   |
| ---------- | ------ |
| TRIG       | GPIO0  |
| ECHO       | GPIO10 |
| RFID SS    | GPIO7  |
| RFID SCK   | GPIO6  |
| RFID MOSI  | GPIO5  |
| RFID MISO  | GPIO4  |
| RFID RST   | GPIO2  |
| LCD SDA    | GPIO8  |
| LCD SCL    | GPIO9  |
| Servo      | GPIO1  |
| LED Strip  | GPIO3  |

---

# 🌐 Comunicação com Cloud

## Validação de Acesso

```json
POST /api/access/validate
{
  "rfidTag": "ID_tag",
  "deviceToken": "device_id"
}
```

## Heartbeat

```json
POST /api/aparelhos/ping
{
  "deviceToken": "device_id"
}
```

---

# 📈 Fluxo do Sistema

1. Detecta presença utilizando sensor ultrassônico
2. Ativa LCD e interface do usuário
3. Solicita aproximação do cartão
4. Realiza leitura RFID
5. Envia UID para o backend
6. Recebe resposta de autorização
7. Executa ação correspondente
8. Exibe resultado ao usuário
9. Envia heartbeat periodicamente

---

# 🧠 Inteligência do Sistema

## Sensor Ultrassônico

O sistema utiliza processamento local para melhorar a confiabilidade das leituras.

### Recursos Implementados

* Filtro por média móvel
* Leitura periódica (~200ms)
* Histerese (15cm / 20cm)
* Tolerância de permanência (~1,5s)

### Benefícios

* Menor sensibilidade a ruídos
* Maior estabilidade
* Menor ocorrência de falsos positivos

---

## RFID

### Recursos

* Conversão padronizada do UID
* Validação em backend
* Controle de autenticação

---

## Wi-Fi

### Recursos

* Reconexão automática
* Verificação periódica de status
* Operação segura quando offline

---

# 🚀 Melhorias Implementadas

## 🔹 Controle Anti-Releitura RFID

Durante os testes foi identificado que um mesmo cartão RFID poderia ser processado diversas vezes caso permanecesse próximo ao leitor.

Para evitar registros duplicados e chamadas repetidas para o backend, foi implementado um mecanismo de bloqueio temporário de leitura.

### Implementação

```cpp
String ultimoUID = "";
unsigned long ultimaLeituraRFID = 0;
const unsigned long intervaloBloqueioRFID = 5000;
```

Validação realizada:

```cpp
if (uid == ultimoUID &&
    millis() - ultimaLeituraRFID < intervaloBloqueioRFID)
{
    return;
}
```

### Benefícios

* Evita registros duplicados
* Reduz chamadas desnecessárias para API
* Melhora a confiabilidade do sistema
* Reduz tráfego de rede
* Aproxima o sistema de soluções profissionais de controle de acesso

---

## 🔹 Padronização do Device Token

O sistema passou a utilizar a variável global `deviceToken` em todas as requisições HTTP.

### Antes

```json
{
  "deviceToken": "ESP32-001"
}
```

### Depois

```json
{
  "deviceToken": "deviceToken"
}
```

### Benefícios

* Configuração centralizada
* Maior facilidade de manutenção
* Reutilização do firmware em múltiplos dispositivos

---

## 🔹 Encerramento Seguro da Comunicação RFID

Após cada leitura foram adicionadas as funções:

```cpp
mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1();
```

### Benefícios

* Encerramento correto da comunicação RFID
* Maior estabilidade do leitor RC522
* Redução de leituras indesejadas
* Liberação adequada dos recursos internos do módulo

---

# 📊 Interface LCD

Mensagens apresentadas ao usuário:

```text
Sem conexao
Reconectando

Aproxime o cartao

Lendo cartao
Aguarde

Acesso liberado

Acesso negado

Cartao ja lido
Aguarde...
```

---

# 💡 Feedback Visual

| Estado          | Cor           |
| --------------- | ------------- |
| Aguardando      | LEDs apagados |
| Acesso liberado | Verde         |
| Acesso negado   | Vermelho      |

---

# 🛠 Tecnologias Utilizadas

## Firmware

* C++
* ESP32
* Arduino Framework

## Comunicação

* HTTP
* REST
* JSON

## Backend

* Node.js
* Docker

## Protocolos e Interfaces

* SPI
* I2C
* PWM

---

# 🔗 Mapeamento Acadêmico

| Conceito            | Disciplina              | Evidência               |
| ------------------- | ----------------------- | ----------------------- |
| Sistemas Embarcados | IoT                     | Firmware ESP32          |
| APIs REST           | Engenharia de Software  | Comunicação HTTP        |
| Computação em Nuvem | Cloud Computing         | Backend                 |
| Segurança           | Segurança da Informação | Controle de acesso      |
| Redes               | Redes de Computadores   | Comunicação ESP32 ↔ API |

---

# 🔒 Segurança

* Credenciais isoladas
* Controle de acesso centralizado
* Bloqueio quando offline
* Prevenção de leituras RFID duplicadas
* Identificação individual do dispositivo por token
* Comunicação controlada com backend

---

# 📸 Dossiê de Evidências

* Fotos da montagem física
* Circuito eletrônico
* Logs do Serial Monitor
* Prints do Dashboard
* Testes de autenticação
* Demonstração em vídeo

---

# 📊 Resultados Obtidos

O sistema demonstrou capacidade de:

* Identificar usuários através de RFID
* Comunicar-se com backend remoto
* Liberar ou negar acessos automaticamente
* Operar com monitoramento contínuo
* Recuperar conexão Wi-Fi automaticamente
* Evitar leituras RFID duplicadas
* Garantir operação segura em ambientes institucionais

---

# 👨‍💻 Autor

Projeto desenvolvido como parte do Projeto Integrador em Internet das Coisas (IoT) do SENAC Pernambuco.

**Curso:** CST em Análise e Desenvolvimento de Sistemas
**Período:** 4º Período
**Instituição:** SENAC PE
