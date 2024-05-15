#include <Ethernet_UDP.h>

/** Setup dos dados de comunicação Ethernet
 * @MACADD: endereçamento da placa de rede
 * @SERVERADD: endereço de IP do servidor ou da placa de rede
 * @CLIENTADD: endereço de IP do client ou da placa de rede do client
*/
#define MACADD 0x90, 0xA2, 0xDA, 0x00, 0x64, 0x50
#define SERVERADD 192, 168, 1, 50
#define CLIENTADD 192, 168, 1, 44

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
  /* Inicialização da comunicação Serial e Ethernet */
  Serial.begin(9600); // Inicialização da porta Serial com 9600 bits/s
  Ethernet_begin(uint8_t(MACADD), IPAddress(SERVERADD));  // Inicialização da comunicação Ethernet (@MACADD, @SERVERADD)

  /** Setup do modo de utilização dos pinos
   * @INPUT: Configuração de entrada de dados (Sensor NA  0 | Sensor NF 1). O sensor deve ser alimentado com 5V
   * @INPUT_PULLUP: Configuração de entrada de dados no formato PULLUP (Sensor NA  1 | Sensor NF 0). O sensor deve ser alimentado com GND
   * @OUTPUT: Configuração da saida de dados. O atuador/indicador deve ser atendido a tensão de funcionamento do mesmo.
  */
  pinMode(pinRELE, OUTPUT); //Definição da pinagem do botão como entrada
}

void loop() {
  /** Leitura da mensagem armazenada no buffer
   * Como setup inicial do buffer foi alocado para 24 bytes, podendo o usuário armazenar as leituras
   * no formato de 1 byte (deslocamento de bits) ou em n bytes.
  */
  EthernetRX();

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