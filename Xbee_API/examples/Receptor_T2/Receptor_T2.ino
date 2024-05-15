#include <Xbee_API.h>

/** Setup dos dados de comunicação XBee
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
 * @recuo: setup do recuo da valvula
*/
#define avanco 1
#define recuo 0

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

  /* Define pinos de utilização dos leds para printar os valores recebidos */
  if (!Ack_print) {
    for (int i = 11; i<14; i++) pinMode(i, OUTPUT);
    for (int i = 11; i<14; i++) digitalWrite(i, 1);
    delay(500);
    for (int i = 11; i<14; i++) digitalWrite(i, 0);
  }
}

void loop() {
  /* Leitura da mensagem recebida no buffer e aloca na variável payload */
  Xbee_read(&payload[0], sizeof(payload));

  /* Lógica de acionamento da valvula */
  if (payload[0] == 20)
    digitalWrite(pinRELE, avanco);
  if (payload[0] == 1)
    digitalWrite(pinRELE, recuo);

  /* Impressão do valor recebido do payload no led ou Serial Monitor */
  if (!Ack_print) {
    ledprint(leitura, DESLOCAMENTO_BIT);
    // ledprint(leitura, MULTI_VARIAVEL);
  } else if (Ack_print) {
    dprint(leitura, DESLOCAMENTO_BIT);
    // dprint(leitura, MULTI_VARIAVEL);
  }
  delay(100);
}

/** Função de print da mensagem armazenada no buffer em leds
 * @valor: mensagem do buffer
 * @mode: setup do modo de print
 *  @DESLOCAMENTO_BIT
 *  @MULTI_VARIAVEL
*/
void ledprint(uint8_t valor[], uint8_t mode) {
  if (mode == DESLOCAMENTO_BIT)
    for (int i = 0; i < 3; i++)
      digitalWrite(11 - i, (valor[0]>>i*2)&3);
  else if (mode == MULTI_VARIAVEL)
    for (int i = 0; i < 3; i++)
      digitalWrite(i + 9, valor[i]);
}

/** Função de print da mensagem armazenada no buffer no Serial Monitor
 * @valor: mensagem do buffer
 * @mode: setup do modo de print
 *  @DESLOCAMENTO_BIT
 *  @MULTI_VARIAVEL
*/
void dprint(uint8_t valor[], uint8_t mode) {
  if (mode == DESLOCAMENTO_BIT) {
    Serial.print("botao: ");
    Serial.print(((valor[0]>>4)&3), DEC);
    Serial.print("\tA0: ");
    Serial.print(((valor[0]>>2)&3), DEC);
    Serial.print("\tA1: ");
    Serial.println((valor[0]&3), DEC);
  } else if (mode == MULTI_VARIAVEL) {
    Serial.print("botao: ");
    Serial.print(valor[0], DEC);
    Serial.print("\tA0: ");
    Serial.print(valor[1], DEC);
    Serial.print("\tA1: ");
    Serial.println(valor[2], DEC);
  }
}