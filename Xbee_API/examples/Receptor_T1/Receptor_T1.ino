#include <Xbee_API.h>

/** Setup dos dados de comunicação Ethernet
 * @myADD64: endereçamento do servidor da placa XBee
 * @clientADD64: endereçamento do client da placa XBee
*/
#define myADD64 0x0013A200, 0x40BB0344
#define clientADD64 0x0013A200, 0x40C1B1F0

/** Setup do pino de saída do Relé
 * @pinRELE: pino de saida do sinal do relé
*/
#define pinRELE 2

/** Setup da numeração para acionamento de valvula
 * @avanco: setup do avanço da valvula
*/
#define avanco 1

/** Inicializa o buffer
 * Como setup inicial do buffer foi alocado para 32 bytes, podendo o usuário armazenar as leituras
 * no formato de 1 byte (deslocamento de bits) ou em n bytes.
*/
uint8_t payload[] = {0};

void setup() {
  /* Inicialização da comunicação Serial */
  Serial.begin(9600); // Inicialização da porta Serial com 9600 bits/s

  /** Inicialização da comunicação XBee (Xbee_begin(Modo, Ack_print))
   *  @Modo: Pode ser definido como comunicação RX/TX de valores padrões (Serial, Serial1 e Serial2) ou diferentes (variavel definida
   *  pela biblioteca SoftwareSerial)
   *    Obs.: A comunicação Serial foi setado como valor default, não necessitando que o usuário passe parametros para ela
   *  @Ack_print: variavel de permissão de print
   *    Obs.1: Nos Arduinos Uno e Mega a permissão só pode ser consedida quando for setado um valor diferente da porta Serial, pois a
   *    comunicação entre o Arduino e o Computador é a mesma porta de comunicação com o XBee, podendo gerar envio de lixo na
   *    comunicação XBee
   *    Obs.2: Foi setado como valor default igual a falso
   *         PLACA        |        Serial        |    Serial1     |        Serial2         |    Serial3
   *    Uno, Nano, Mini   |     0(RX), 1(TX)     |                |                        |
   *         Mega         |     0(RX), 1(TX)     | 19(RX), 18(TX) |     17(RX), 16(TX)     | 15(RX), 14(TX)
   * Leonardo, Micro, Yún |     0(RX), 1(TX)     |                |                        |
   *    Uno WiFi Rev.2    |    Conectado a USB   |  0(RX), 1(TX)  |    Conectado a NINA    |
   *      Família MKR     |                      | 13(RX), 14(TX) |                        |
   *         Zero         | Conect a Porta Grava |  0(RX), 1(TX)  |                        |
   *         Due          |     0(RX), 1(TX)     | 19(RX), 18(TX) |     17(RX), 16(TX)     | 15(RX), 14(TX)
   *         101          |                      |  0(RX), 1(TX)  |                        |
   *    ESP32 DEVKIT V1   | GPIO3(RX), GPIO1(TX) |                | GPIO16(RX), GPIO17(TX) |
  */
  Xbee_begin(); // Inicialização da comunicação XBee, com setup inicial comunicação Serial e sem print
  // Xbee_begin(Serial1); // Inicialização da comunicação XBee, com setup inicial comunicação Serial1 e sem print
  // Xbee_begin(sSerial, true); // Inicialização da comunicação XBee, com setup inicial comunicação sSerial e com print

  /** Setup do modo de utilização dos pinos
   * @INPUT: Configuração de entrada de dados (Sensor NA  0 | Sensor NF 1). O sensor deve ser alimentado com 5V
   * @INPUT_PULLUP: Configuração de entrada de dados no formato PULLUP (Sensor NA  1 | Sensor NF 0). O sensor deve ser alimentado com GND
   * @OUTPUT: Configuração da saida de dados. O atuador/indicador deve ser atendido a tensão de funcionamento do mesmo.
  */
  pinMode(pinRELE, OUTPUT); //Definição da pinagem do botão como entrada

  /* Define pino de utilização do led para printar os valor recebido */
  if (!Ack_print) {
    pinMode(11, OUTPUT);
    digitalWrite(11, 1);
    delay(500);
    digitalWrite(11, 0);
  }
}

void loop() {
  /* Leitura da mensagem recebida no buffer e aloca na variável payload */
  Xbee_read(&payload[0], sizeof(payload));

  /* Acionamento da valvula de acordo com o valor armazenado no payload */
  digitalWrite(pinRELE, payload[0]);
  
  /* Impressão do valor recebido do payload no led ou Serial Monitor  */
  if (!Ack_print)  digitalWrite(11, payload[0]);
  else if (Ack_print) printf("Ação: %s", payload[0] == avanco ? "Avanço":"Recuo");
  delay(100);
}