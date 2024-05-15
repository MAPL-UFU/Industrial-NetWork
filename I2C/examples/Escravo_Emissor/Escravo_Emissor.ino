#include <I2C.h>

/** Setup dos dados de comunicação I2C
 * @SLAVE: endereçamento do escravo [0 127]
*/
#define SLAVE 1

/** Setup dos pinos dos Sensores de Entrada
 * @pinBT: pino de entrada do sinal do botão
 * @pinA0: pino de entrada do sinal do sensor de valvula recuada (A0 ou 1S1)
 * @pinA1: pino de entrada do sinal do sensor de valvula avançada (A1 ou 1S2)
*/
#define pinBT 6
#define pinA0 2
#define pinA1 4

void setup() {
  /* Inicialização da comunicação Serial e I2C */
  Serial.begin(9600); // Inicialização da porta Serial com 9600 bits/s
  I2C_begin(SLAVE); // Inicialização da comunicação I2C, com setup do escravo com endereço 1

  /** Setup do modo de utilização dos pinos
   * @INPUT: Configuração de entrada de dados (Sensor NA  0 | Sensor NF 1). O sensor deve ser alimentado com 5V
   * @INPUT_PULLUP: Configuração de entrada de dados no formato PULLUP (Sensor NA  1 | Sensor NF 0). O sensor deve ser alimentado com GND
   * @OUTPUT: Configuração da saida de dados. O atuador/indicador deve ser atendido a tensão de funcionamento do mesmo.
  */
  pinMode(pinBT, INPUT_PULLUP); // Definição da pinagem do botão como entrada do tipo INPUT_PULLUP
  pinMode(pinA0, INPUT_PULLUP); // Definição da pinagem da valvula de recuo como entrada do tipo INPUT_PULLUP
  pinMode(pinA1, INPUT_PULLUP); // Definição da pinagem do valvula de avanço como entrada do tipo INPUT_PULLUP
}

void loop() {
  /** Armazena no buffer informações de leitura dos sensores entrada
   * Como setup inicial do buffer foi alocado para 32 bytes, podendo o usuário armazenar as leituras
   * no formato de 1 byte (deslocamento de bits) ou em n bytes.
  */
  leitura[0] = (((digitalRead(pinBT)&3)<<4)|((digitalRead(pinA0)&3)<<2)|(digitalRead(pinA1)&3));
  //leitura[0] = digitalRead(pinBT);
  //leitura[1] = digitalRead(pinA0);
  //leitura[2] = digitalRead(pinA1);

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