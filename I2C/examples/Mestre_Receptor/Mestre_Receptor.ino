#include <I2C.h>

/** Setup dos dados de comunicação I2C
 * @SLAVE: endereçamento do escravo [0 127]
*/
#define SLAVE 1

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

void setup() {
  /* Inicialização da comunicação Serial e I2C */
  Serial.begin(9600); // Inicialização da porta Serial com 9600 bits/s
  I2C_begin(); // Inicialização da comunicação I2C, com setup do mestre

  /** Setup do modo de utilização dos pinos
   * @INPUT: Configuração de entrada de dados (Sensor NA  0 | Sensor NF 1). O sensor deve ser alimentado com 5V
   * @INPUT_PULLUP: Configuração de entrada de dados no formato PULLUP (Sensor NA  1 | Sensor NF 0). O sensor deve ser alimentado com GND
   * @OUTPUT: Configuração da saida de dados. O atuador/indicador deve ser atendido a tensão de funcionamento do mesmo.
  */
  pinMode(pinRELE, OUTPUT); // Definição da pinagem de acionamento do relé da valvula
}

void loop() {
  /** Inicializa o buffer
   * Como setup inicial do buffer foi alocado para 32 bytes, podendo o usuário armazenar as leituras
   * no formato de 1 byte (deslocamento de bits) ou em n bytes.
  */
  uint8_t packetBuffer[I2C_PACKET_MAX_SIZE];

  /** Requisita do endereço do escravo 1 em bytes para alocar no buffer
   * Como setup inicial do buffer foi alocado para 32 bytes, podendo o usuário armazenar as leituras
   * no formato de 1 byte (deslocamento de bits) ou em n bytes.
  */
  requestfrom(SLAVE, packetBuffer, 1);

  /* Armazenamento da mensagem recebida nas variáveis dos sensores */
  uint8_t BT = ((packetBuffer[0]>>4)&3);
  uint8_t A0 = ((packetBuffer[0]>>2)&3);
  uint8_t A1 = (packetBuffer[0]&3);
  // uint8_t BT = packetBuffer[0];
  // uint8_t A0 = packetBuffer[1];
  // uint8_t A1 = packetBuffer[2];

  /* Lógica de acionamento da valvula */
  if (BT && A0 && !A1) {
    digitalWrite(pinRELE, avanco);
    Serial.print("rele: ");
    Serial.println(avanco, DEC);
  } if (!A0 && A1) {
    digitalWrite(pinRELE, recuo);
    Serial.print("rele: ");
    Serial.println(recuo, DEC);
  }

  /* Print da mensagem armazenada no buffer */
  dprint(packetBuffer, DESLOCAMENTO_BIT);
  // dprint(packetBuffer, MULTI_VARIAVEL);
  delay(100);
}

/** Função de print da mensagem armazenada no buffer
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