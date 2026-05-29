Sistema de Controle de Acesso IoT
📌 Descrição
Sistema de controle de acesso embarcado utilizando ESP32‑C3, RFID (RC522) e sensor ultrassônico para detecção de presença. O sistema autentica usuários por meio de cartões RFID, controla o acesso físico via servo motor e exibe informações em um display LCD.
Também realiza envio de logs para a nuvem via HTTP, com funcionamento resiliente mesmo em modo offline.

🚀 Funcionalidades

✅ Detecção inteligente de presença (sensor ultrassônico)
✅ Autenticação via RFID (cartões)
✅ Controle de acesso físico (servo motor)
✅ Interface com usuário (display LCD)
✅ Envio de logs via HTTP
✅ Funcionamento offline com reconexão automática de Wi-Fi


🧠 Arquitetura
ESP32 → HTTP POST → Backend (Docker) → Logs

+ Interface visual (LCD)
+ Atuação física (Servo)


🔌 Hardware Utilizado

ESP32‑C3 Mini
Módulo RFID RC522
Sensor ultrassônico HC-SR04
Servo motor SG90
Display LCD 1602 (I2C)
Fonte de alimentação 5V


 🔌 Conexões dos Pinos

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

🌐 Comunicação com Cloud

Envio de dados via HTTP POST
Formato JSON:

JSON{  "uid": "29a08159",  "status": "LIBERADO"}Mostrar mais linhas

🐳 Backend (Docker)
O backend é responsável por:

Receber logs via /log
Exibir logs via /logs

🔹 Executar com Docker
Shelldocker build -t rfid-app .docker run -p 3000:3000 rfid-appMostrar mais linhas

📊 Visualização
Acesse:
http://localhost:3000/logs

Para ver os acessos em tempo real.

⚙️ Configuração do ESP32
No código Arduino, configure:
C++const char* ssid = "SEU_WIFI";const char* password = "SENHA";const char* apiUrl = "http://SEU_SERVIDOR:3000/log";Mostrar mais linhas

📈 Fluxo do Sistema

Sensor detecta presença
LCD exibe "Aproxime o cartão"
RFID lê cartão
Sistema valida acesso
Servo abre/fecha
Evento é enviado para o backend


🛠️ Tecnologias Utilizadas

ESP32 (C++)
Node.js (Express)
Docker
HTTP / REST
I2C, SPI, PWM


💡 Melhorias Futuras

Dashboard com gráficos
Banco de dados persistente (MongoDB/PostgreSQL)
Autenticação de usuários
Integração com AWS (Terraform)


👨‍💻 Autor
Projeto desenvolvido como parte de estudo de sistemas embarcados, IoT e computação em nuvem.
