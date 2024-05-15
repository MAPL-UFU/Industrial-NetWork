#include <Ethernet_UDP.h>

/** Setup dos dados de comunicação Ethernet
 * @MACADD: endereçamento da placa de rede
 * @SERVERADD: endereço de IP do servidor ou da placa de rede
 * @CLIENTADD: endereço de IP do client ou da placa de rede do client
*/
#define MACADD 0x90, 0xA2, 0xDA, 0x00, 0x64, 0x44
#define SERVERADD 192, 168, 1, 44
#define CLIENTADD 192, 168, 1, 50

/** Setup dos pinos dos Sensores de Entrada
 * @pinBT: pino de entrada do sinal do botão
 * @pinA0: pino de entrada do sinal do sensor de valvula recuada (A0 ou 1S1)
 * @pinA1: pino de entrada do sinal do sensor de valvula avançada (A1 ou 1S2)
*/
#define pinBT 2
#define pinA0 4
#define pinA1 6

void setup() {
  /* Inicialização da comunicação Serial e Ethernet */
  Serial.begin(9600); // Inicialização da porta Serial com 9600 bits/s
  Ethernet_begin(uint8_t(MACADD), IPAddress(SERVERADD));  // Inicialização da comunicação Ethernet (@MACADD, @SERVERADD)

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
   * Como setup inicial do buffer foi alocado para 24 bytes, podendo o usuário armazenar as leituras
   * no formato de 1 byte (deslocamento de bits) ou em n bytes.
  */
  packetBuffer[0] = (((digitalRead(pinBT)&3)<<4)|((digitalRead(pinA0)&3)<<2)|(digitalRead(pinA1)&3));
  //packetBuffer[0] = digitalRead(pinBT);
  //packetBuffer[1] = digitalRead(pinA0);
  //packetBuffer[2] = digitalRead(pinA1);

  /* Envio da mensagem armazenada no buffer do IP do client */
  EthernetTX(IPAddress(CLIENTADD));

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