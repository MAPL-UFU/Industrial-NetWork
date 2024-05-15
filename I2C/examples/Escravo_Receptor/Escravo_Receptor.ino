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
  I2C_begin(SLAVE); // Inicialização da comunicação I2C, com setup do escravo com endereço 1

  /** Setup do modo de utilização dos pinos
   * @INPUT: Configuração de entrada de dados (Sensor NA  0 | Sensor NF 1). O sensor deve ser alimentado com 5V
   * @INPUT_PULLUP: Configuração de entrada de dados no formato PULLUP (Sensor NA  1 | Sensor NF 0). O sensor deve ser alimentado com GND
   * @OUTPUT: Configuração da saida de dados. O atuador/indicador deve ser atendido a tensão de funcionamento do mesmo.
  */
  pinMode(pinRELE, OUTPUT); // Definição da pinagem de acionamento do relé da valvula
}

void loop() {
  /* Armazenamento da mensagem recebida nas variáveis dos sensores */
  uint8_t BT = ((leitura[0]>>4)&3);
  uint8_t A0 = ((leitura[0]>>2)&3);
  uint8_t A1 = (leitura[0]&3);
  // uint8_t BT = leitura[0];
  // uint8_t A0 = leitura[1];
  // uint8_t A1 = leitura[2];

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
  dprint(leitura, DESLOCAMENTO_BIT);
  // dprint(leitura, MULTI_VARIAVEL);
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